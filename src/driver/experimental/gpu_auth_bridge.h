/* Root-managed, opt-in GPU gallery bridge. No PAM or enrollment format change.
 * Missing gallery retains SIGFM. An enabled gallery NEVER falls back to the
 * old matcher on rejection/error. Existing fprintd authorizes/selects the user.
 */
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>

typedef struct {
  GMutex mutex;
  GCond condition;
  gboolean done;
  GSubprocess *child;
  GCancellable *cancel;
} GpuAuthDeadline;

static gpointer gpu_auth_deadline(gpointer data) {
  GpuAuthDeadline *d=data;
  g_mutex_lock(&d->mutex);
  const gint64 until=g_get_monotonic_time()+6*G_TIME_SPAN_SECOND;
  while(!d->done) {
    if(!g_cond_wait_until(&d->condition,&d->mutex,until)) {
      g_cancellable_cancel(d->cancel);
      g_subprocess_force_exit(d->child);
      break;
    }
  }
  g_mutex_unlock(&d->mutex);
  return NULL;
}

/* -2 = no configured override; -1 = error; 0 = reject; 1 = accept. */
static int gpu_auth_match(FpPrint *enrolled,FpPrint *probe) {
  const char *username=fp_print_get_username(enrolled);
  if(g_strcmp0(fp_print_get_driver(enrolled),"goodixtls55x4")!=0)return -2;
  if(!username||!g_regex_match_simple("^[a-z_][a-z0-9_-]{0,31}$",username,0,0))return -2;
  g_autofree char *enabled=g_strdup_printf("/etc/fpstudio-auth-users/%s",username);
  struct stat st;
  if(lstat(enabled,&st))return errno==ENOENT?-2:-1;
  if(st.st_uid!=0||!S_ISREG(st.st_mode)||(st.st_mode&0022))return -1;
  g_autofree char *bank=g_strdup_printf("/var/lib/fprint/fpstudio-gpu/%s",username);
  if(lstat(bank,&st))return -1;
  if(geteuid()!=0||st.st_uid!=0||!S_ISDIR(st.st_mode)||(st.st_mode&0077))return -1;
  FpImage *image=fp_print_get_image(probe);
  if(!image||fp_image_get_width(image)!=108||fp_image_get_height(image)!=88)return -1;
  gsize length=0;const guchar *pixels=fp_image_get_data(image,&length);
  if(!pixels||length!=9504)return -1;
  const char *helper="/opt/fpstudio-auth/bin/fpstudio-auth-match";
  if(lstat(helper,&st)||st.st_uid!=0||!S_ISREG(st.st_mode)||(st.st_mode&0022))return -1;
  g_autoptr(GSubprocessLauncher) launcher=g_subprocess_launcher_new(G_SUBPROCESS_FLAGS_STDIN_PIPE|G_SUBPROCESS_FLAGS_STDOUT_PIPE);
  const char *env[]={"PATH=/usr/bin","LANG=C.UTF-8",NULL};
  g_subprocess_launcher_set_environ(launcher,(gchar **)env);
  g_autoptr(GError) error=NULL;
  g_autoptr(GSubprocess) child=g_subprocess_launcher_spawn(launcher,&error,helper,"--user",username,NULL);
  if(!child)return -1;
  guchar *pgm=g_malloc(9518);memcpy(pgm,"P5\n108 88\n255\n",14);memcpy(pgm+14,pixels,9504);
  g_autoptr(GBytes) input=g_bytes_new_take(pgm,9518);
  g_autoptr(GBytes) output=NULL;
  g_autoptr(GCancellable) cancel=g_cancellable_new();
  GpuAuthDeadline deadline={0};deadline.child=child;deadline.cancel=cancel;
  g_mutex_init(&deadline.mutex);g_cond_init(&deadline.condition);
  GThread *thread=g_thread_new("gpu-auth-deadline",gpu_auth_deadline,&deadline);
  const gboolean communicated=g_subprocess_communicate(child,input,cancel,&output,NULL,&error);
  g_mutex_lock(&deadline.mutex);deadline.done=TRUE;g_cond_signal(&deadline.condition);g_mutex_unlock(&deadline.mutex);
  g_thread_join(thread);g_mutex_clear(&deadline.mutex);g_cond_clear(&deadline.condition);
  if(!communicated||!output||!g_subprocess_get_if_exited(child))return -1;
  gsize size=0;const char *reply=g_bytes_get_data(output,&size);
  const char *accepted="FPSTUDIO_GPU_MATCH_V1\n";
  const int status=g_subprocess_get_exit_status(child);
  if(status==0&&size==strlen(accepted)&&memcmp(reply,accepted,size)==0)return 1;
  if(status==1)return 0;
  return -1;
}

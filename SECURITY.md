# Security policy

FPStudio touches authentication and biometric hardware. Please do not disclose
a vulnerability, bypass, credential exposure, unsafe PAM transition or leaked
biometric material in a public issue.

Use GitHub's **Security → Advisories → Report a vulnerability** flow for this
repository. Include the affected version, distro, kernel, desktop/PAM path,
minimal reproduction and privacy-safe journal summary. Do not attach passwords,
fingerprint images or templates, device secrets, PSKs, authentication backups
or unredacted private logs.

The project is experimental and hardware-specific. Until a report is assessed,
keep password login available and uninstall the optional authentication module
if safe fallback behaviour is in doubt.

Supported security fixes target the latest tagged release and the `main` branch.

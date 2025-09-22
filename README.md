# webserv — HTTP/1.1 server in C++
School project: a from-scratch **HTTP/1.1 web server** in C++ 98. Supports multiple virtual servers, static files, autoindex, redirects, error pages, request body limits, and CGI.
## Features
- **HTTP/1.1** parsing & responses for **GET** **POST** and **DELETE**.
- **Virtual servers** (`server { listen …; server_name …; }`) with per-location config.
- **Static files** with `root`, `index`, **autoindex on/off**.
- **Redirects** (`return 3xx <url>`).
- **Error pages** (`error_page <code> <path>` per server).
- **Request size limit** (`client_max_body_size 10M`, etc.).
- **CGI gateway**: map file extensions to executables (e.g. `.bash` → `/bin/bash`, `.cgi` → `/bin/perl`).
- **Method control** per location (e.g., `forbid_method POST DELETE`).
## Build
```bash
make
```
## Run
```bash
./webserv your-config-file.conf
```
## Config (nginx-like-syntax)
```nginx
# -----------------------------------------------------------------------------
# Example config for webserv (HTTP/1.1)
# - Syntax inspired by nginx; directives supported by this project.
# - Size units: B, K, M (e.g., 10B, 512K, 20M).
# -----------------------------------------------------------------------------

# =========================
# Virtual Server #1 (site1)
# =========================
server {
  # Map CGI by extension to interpreter binaries
  cgi .bash /bin/bash
  cgi .cgi  /bin/perl

  # Listen on all interfaces, port 8080
  listen 0.0.0.0:9090

  # Host header to match (optional) 
  server_name localhost:9090

  # Custom error pages (per server)
  error_page 403 /var/www/site1/403.html
  error_page 404 /var/www/site1/404.html
  error_page 500 /var/www/site1/500.html

  # Max allowed request body
  client_max_body_size 150M

  # ---------- Root location ----------
  location / {
    # Directory to serve static files from
    root /var/www/site1;
    # Turn directory listing on/off
    autoindex off;
    # Default document(s) to try
    index index.html index.htm;
  }

  # ---------- Autoindex example ----------
  location /autoindex {
    # Serve a different folder with listing enabled
    root /var/www/site1/autoindex;
    autoindex on;
  }

  location /autoindex {
    root /mnt/nfs/homes/sasaicic/webserv/www/autoindex
    autoindex on
  }

  # ---------- Simple redirect ----------
  location /redirection {
    return 302 http://google.fr
  }
}

# =========================
# Virtual Server #2
# =========================
server {
  listen 0.0.0.0:8080;
  server_name site2.local:8080;

  client_max_body_size 10B;

  # Serve a different document root
  location / {
    root /var/www/site2;
    autoindex off;
    index index.html;
    # Forbid specific HTTP methods on this location
    forbid_method POST DELETE;
  }

  location /redirection {
    return 302 https://intra.42.fr/;
  }
}
```
## Public assets
- `www/` and `www2/` hold example sites, images, fonts, Next.js static export, default **403/404/500** pages, and a sample **upload form/CGI**.

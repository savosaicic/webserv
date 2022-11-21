#include <http_utils.hpp>

const char* http_headers::server = "Server";
const char* http_headers::connection = "Connection";
const char* http_headers::location = "Location";
const char* http_headers::content_type = "Content-Type";
const char* http_headers::content_length = "Content-Length";

const char* mime_types::application_octet_stream = "application/octet-stream";
const char* mime_types::application_javascript = "application/javascript";
const char* mime_types::application_typescript = "application/typescript";
const char* mime_types::application_json = "application/json";
const char* mime_types::application_pdf = "application/pdf";
const char* mime_types::text_html = "text/html";
const char* mime_types::text_css = "text/css";
const char* mime_types::image_jpeg = "image/jpeg";
const char* mime_types::image_png = "image/png";
const char* mime_types::image_gif = "image/gif";
const char* mime_types::image_svg = "image/svg+xml";
const char* mime_types::font_otf = "font/otf";

const int status_code::http_ok = 200;
const int status_code::http_created = 201;
const int status_code::http_moved_permanently = 301;
const int status_code::http_found = 302;
const int status_code::http_bad_request = 400;
const int status_code::http_forbidden = 403;
const int status_code::http_not_found = 404;
const int status_code::http_not_allowed = 405;
const int status_code::http_internal_error = 500;

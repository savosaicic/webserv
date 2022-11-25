#include <http_utils.hpp>
#include <string>

const std::map<std::string, std::string> mime_types::_mime_tree =
  mime_types::_init_tree();

const  std::map<std::string, std::string>  mime_types::_init_tree()
{
  std::map<std::string, std::string>  tree;
  tree[".js"] = application_javascript;
  tree[".ts"] = application_typescript;
  tree[".json"] = application_json;
  tree[".pdf"] = application_pdf;
  tree[".html"] = text_html;
  tree[".css"] = text_css;
  tree[".jpg"] = image_jpeg;
  tree[".jpeg"] = image_jpeg;
  tree[".png"] = image_png;
  tree[".gif"] = image_gif;
  tree[".svg"] = image_svg;
  tree[".otf"] = font_otf;
  return tree;
}

const char*  mime_types::content_type(const std::string& rooted_uri)
{
  if (rooted_uri.find_last_of('.') != std::string::npos &&
      _mime_tree.find(rooted_uri.substr(rooted_uri.find_last_of('.')))
      != _mime_tree.end()) {
    return _mime_tree.find(rooted_uri.substr(
          rooted_uri.find_last_of('.')))->second.c_str();
  }
  return mime_types::application_octet_stream;
}

const char* http_headers::server = "Server";
const char* http_headers::connection = "Connection";
const char* http_headers::location = "Location";
const char* http_headers::content_type = "Content-Type";
const char* http_headers::content_length = "Content-Length";
const char* http_headers::host = "Host";

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
const int status_code::http_payload_too_large = 413;
const int status_code::http_internal_error = 500;
const int status_code::http_version_not_supported = 505;
const int status_code::http_not_implemented = 501;

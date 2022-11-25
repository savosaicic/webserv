#ifndef HTTP_UTILS_HPP
#define HTTP_UTILS_HPP

#include <map>
#include <string>

class http_headers
{
  public:
    static const char* server;
    static const char* connection;
    static const char* location;
    static const char* content_type;
    static const char* content_length;
    static const char* host;
};

class status_code
{
  public:
    static const int http_ok;
    static const int http_created;
    static const int http_moved_permanently;
    static const int http_found;
    static const int http_bad_request;
    static const int http_forbidden;
    static const int http_not_found;
    static const int http_not_allowed;
		static const int http_payload_too_large;
    static const int http_internal_error;
		static const int http_version_not_supported;
		static const int http_not_implemented;
};

class mime_types
{
  private:
    static const  std::map<std::string, std::string>  _mime_tree;
    static const  std::map<std::string, std::string>  _init_tree();

  public:
    static const char* content_type(const std::string& rooted_uri);

    static const char* application_octet_stream;
    static const char* application_javascript;
    static const char* application_typescript;
    static const char* application_json;
    static const char* application_pdf;
    static const char* text_html;
    static const char* text_css;
    static const char* image_jpeg;
    static const char* image_png;
    static const char* image_gif;
    static const char* image_svg;
    static const char* font_otf;
};

#endif

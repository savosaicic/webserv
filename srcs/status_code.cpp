#include <status_code.hpp>

const char* reason_phrase(status_code_t code)
{
#define X(name, code, reason) case (name): return reason;
  switch (code) {
    STATUS_CODE_LIST
  }
  return "";
#undef X
}

int status_code(status_code_t code)
{
#define X(name, code, reason) case (name): return code;
  switch (code) {
    STATUS_CODE_LIST
  }
  return 0;
#undef X
}

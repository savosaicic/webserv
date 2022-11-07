#include <exception>
#include <string>
#include <string.h>
#include <errno.h>

class ws_syscall_error : public std::exception
{
  private:
    const std::string str;

  public:
    ws_syscall_error(const std::string & message)
      : str(message + ": " + strerror(errno)) {}

    virtual ~ws_syscall_error() throw() {}

    virtual const char* what() const throw() { return str.c_str(); }
};

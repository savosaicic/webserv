#include <string>

const std::string	error_message(const char *item, const char *error)
{
	std::string	message;

	if (item)
	{
		message = item;
		message += ": ";
	}
	if (error)
		message += error;
	return (message);
}

#include <gtest/gtest.h>
#include <HTTPRequest.hpp>

TEST(extractHeaderMap, empty_string)
{
	HTTPRequest request("");
	char *test = "";
	EXPECT_STREQ(request.getRawRequest().c_str(), test);
}

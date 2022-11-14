#include <gtest/gtest.h>
#include "HTTPRequest.hpp"

TEST(extractHeaderMap, empty_raw_request)
{
	HTTPRequest request("");
	const char *test = "";
	EXPECT_STREQ(request.getRawRequest().c_str(), test);
}

TEST(extractMethod, valid_method)
{
	HTTPRequest::method_t test = HTTPRequest::GET;
	HTTPRequest request("GET /software/htp/cics/index.html HTTP/1.0 \r\nAccept-Language: \r\nIf-Modified-Since: Fri, 10 Dec 2004 11:22:13 GMT\r\n\r\n");
	EXPECT_EQ(request.getMethod(), test);
}

TEST(extractMethod, invalid_method)
{
	HTTPRequest::method_t test = HTTPRequest::INVALID;

	{
		HTTPRequest request(" GET /software/htp/cics/index.html HTTP/1.0 \r\nAccept-Language: \nIf-Modified-Since: Fri, 10 Dec 2004 11:22:13 GMT\n\n");
		EXPECT_EQ(request.getMethod(), test);
	}

	{
		HTTPRequest request("GGET /software/htp/cics/index.html HTTP/1.0 \r\nAccept-Language: \nIf-Modified-Since: Fri, 10 Dec 2004 11:22:13 GMT\n\n");
		EXPECT_EQ(request.getMethod(), test);
	}

	{
		HTTPRequest request(" /software/htp/cics/index.html HTTP/1.0 \r\nAccept-Language: \nIf-Modified-Since: Fri, 10 Dec 2004 11:22:13 GMT\n\n");
		EXPECT_EQ(request.getMethod(), test);
	}

	{
		HTTPRequest request(" /software/htp/cics/index.html HTTP/1.0 \r\nAccept-Language: \nIf-Modified-Since: Fri, 10 Dec 2004 11:22:13 GMT\n\n");
		EXPECT_EQ(request.getMethod(), test);
	}

	{
		HTTPRequest request("");
		EXPECT_EQ(request.getMethod(), test);
	}
}

TEST(extractHeaderVersion, invalid_HTTP_version)
{
	int prior = -1;
	int minor = -1;
	{
		HTTPRequest request("GET /software/htp/cics/index.html  \r\nAccept-Language: \nIf-Modified-Since: Fri, 10 Dec 2004 11:22:13 GMT\n\n");
		EXPECT_EQ(request.getHTTPVersion().first, prior);
		EXPECT_EQ(request.getHTTPVersion().second, minor);
	}
	{
		HTTPRequest request("GET /software/htp/cics/index.html HTTP/.0 \r\nAccept-Language: \nIf-Modified-Since: Fri, 10 Dec 2004 11:22:13 GMT\n\n");
		EXPECT_EQ(request.getHTTPVersion().first, prior);
		EXPECT_EQ(request.getHTTPVersion().second, minor);
	}
	{
		int prior = 0;
		HTTPRequest request("GET /software/htp/cics/index.html HTTP/0 \r\nAccept-Language: \nIf-Modified-Since: Fri, 10 Dec 2004 11:22:13 GMT\n\n");
		EXPECT_EQ(request.getHTTPVersion().first, prior);
		EXPECT_EQ(request.getHTTPVersion().second, minor);
	}
	{
		HTTPRequest request("GET /software/htp/cics/index.html");
		EXPECT_EQ(request.getHTTPVersion().first, prior);
		EXPECT_EQ(request.getHTTPVersion().second, minor);
	}
}

TEST(extractHeaderVersion, valid_HTTP_version)
{
	int prior = 1;
	int minor = 1;

	HTTPRequest request("GET /software/htp/cics/index.html HTTP/1.1\r\nAccept-Language: \nIf-Modified-Since: Fri, 10 Dec 2004 11:22:13 GMT\n\n");
	EXPECT_EQ(request.getHTTPVersion().first, prior);
	EXPECT_EQ(request.getHTTPVersion().second, minor);
}

TEST(extractHeaderMap, valid_HTTP_header)
{
	HTTPRequest request("GET /software/htp/cics/index.html HTTP/1.1\r\nAccept-Language: \r\nIf-Modified-Since: Fri, 10 Dec 2004 11:22:13 GMT\r\n\r\n");
	EXPECT_EQ(request.getHeaderContent("Accept-Language"), "");
	EXPECT_EQ(request.getHeaderContent("If-Modified-Since"), "Fri, 10 Dec 2004 11:22:13 GMT");
}

TEST(extractHeaderMap, invalid_HTTP_header)
{
	{
		HTTPRequest request("GET /software/htp/cics/index.html HTTP/1.1\r\n: \r\nIf-Modified-Since: Fri, 10 Dec 2004 11:22:13 GMT\r\n\r\n");
		EXPECT_EQ(request.getHeaderContent(""), "");
		EXPECT_EQ(request.getHeaderContent("If-Modified-Since"), "Fri, 10 Dec 2004 11:22:13 GMT");
	}
	{
		HTTPRequest request("GET /software/htp/cics/index.html HTTP/1.1\r\nAccept-Language: \nIf-Modified-Since: Fri, 10 Dec 2004 11:22:13 GMT\r\n\r\n");
		EXPECT_EQ(request.getHeaderContent("Accept-Language"), "");
		EXPECT_EQ(request.getHeaderContent("If-Modified-Since"), "Fri, 10 Dec 2004 11:22:13 GMT");
	}
	{
		HTTPRequest request("GET /software/htp/cics/index.html HTTP/1.1\r\nAccept-Language: \nIf-Modified-Since: Fri, 10 Dec 2004 11:22:13 GMT\n\r\n");
		EXPECT_EQ(request.getHeaderContent("Accept-Language"), "");
		EXPECT_EQ(request.getHeaderContent("If-Modified-Since"), "");
	}
}

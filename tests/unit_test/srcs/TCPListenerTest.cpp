#include <gtest/gtest.h>
#include <TCPListener.hpp>

TEST(bind, param_limits)
{
	EXPECT_THROW({
		TCPListener tcp;
		tcp.bind(0, "8080");
	}, std::invalid_argument);
	EXPECT_THROW({
		TCPListener tcp;
		tcp.bind("127.0.0.1", 0);
	}, std::invalid_argument);
}
TEST(bind, param_ok)
{
	EXPECT_NO_THROW({
		TCPListener tcp;
		tcp.bind("127.0.0.1", "8080");
	});
	EXPECT_NO_THROW({
		TCPListener tcp;
		tcp.bind("0.0.0.0", "8080");
	});
}
TEST(bind, param_not_correct)
{
	EXPECT_ANY_THROW({
		TCPListener tcp;
		tcp.bind("127.0.0.a", "8080");
	});
	EXPECT_ANY_THROW({
		TCPListener tcp;
		tcp.bind("127.0.0.1", "faux");
	});
	EXPECT_ANY_THROW({
		TCPListener tcp;
		tcp.bind("999.0.0.1", "8080");
	});
	EXPECT_ANY_THROW({
		TCPListener tcp;
		tcp.bind("127.0.0.1", "-8080");
	});
	EXPECT_ANY_THROW({
		TCPListener tcp;
		tcp.bind("8.8.8.8", "8080");
	});
}
// TEST(fd leaks)
// {

// }
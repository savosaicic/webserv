#include <gtest/gtest.h>
#include <utility>
#include "HTTPVersion.hpp"

TEST(HTTPVersion_Construction, default_constructor)
{
  HTTPVersion version;

  ASSERT_EQ(version.getMajor(), 0);
  ASSERT_EQ(version.getMinor(), 0);
}

TEST(HTTPVersion_Construction, parameter_constructor)
{
  uint16_t major = 5;
  uint16_t minor = 8;

  HTTPVersion version(major, minor);

  ASSERT_EQ(version.getMajor(), major);
  ASSERT_EQ(version.getMinor(), minor);
}

TEST(HTTPVersion_Accessors, isSet)
{
  HTTPVersion version;

  ASSERT_FALSE(version.isSet());

  version.setMajor(4);
  version.setMinor(0);
  ASSERT_TRUE(version.isSet());
}

TEST(HTTPVersion_Comparaison, operator_equal)
{
  HTTPVersion version_one(5, 5);
  HTTPVersion version_two(5, 5);

  EXPECT_TRUE(version_one == version_two);
}

TEST(HTTPVersion_Comparaison, operator_not_equal)
{
  HTTPVersion version_one(0, 5);
  HTTPVersion version_two(0, 1);

  EXPECT_TRUE(version_one != version_two);
}

TEST(HTTPVersion_Comparaison, operator_less_than)
{
  HTTPVersion version_one(1, 0);
  HTTPVersion version_two(1, 1);

  EXPECT_TRUE(version_one < version_two);
}

TEST(HTTPVersion_Comparaison, operator_greater_than)
{
  HTTPVersion version_one(1, 1);
  HTTPVersion version_two(0, 9);

  EXPECT_TRUE(version_one > version_two);
}

TEST(HTTPVersion_Comparaison, operator_less_or_equal_one)
{
  HTTPVersion version_one(1, 1);
  HTTPVersion version_two(1, 2);

  EXPECT_TRUE(version_one <= version_two);
}

TEST(HTTPVersion_Comparaison, operator_less_or_equal_two)
{
  HTTPVersion version_one(1, 1);
  HTTPVersion version_two(1, 1);

  EXPECT_TRUE(version_one <= version_two);
}

TEST(HTTPVersion_Comparaison, operator_greater_or_equal_one)
{
  HTTPVersion version_one(1, 1);
  HTTPVersion version_two(0, 1);

  EXPECT_TRUE(version_one >= version_two);
}

TEST(HTTPVersion_Comparaison, operator_greater_or_equal_two)
{
  HTTPVersion version_one(1, 1);
  HTTPVersion version_two(1, 1);

  EXPECT_TRUE(version_one >= version_two);
}

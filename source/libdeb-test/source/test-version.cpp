#include <gtest/gtest.h>

#include <debian/libdeb/version.h>

TEST(libdeb_version_test, parse)
{
	char *version = "1";
	DEB_VERSION parsed_version = deb_version_parse(version);

	EXPECT_EQ(parsed_version.major, 1);
	EXPECT_EQ(parsed_version.minor, 0);
	EXPECT_EQ(parsed_version.patch, 0);
	EXPECT_EQ(parsed_version.build, 0);

	version = "1.1";
	parsed_version = deb_version_parse(version);

	EXPECT_EQ(parsed_version.major, 1);
	EXPECT_EQ(parsed_version.minor, 1);
	EXPECT_EQ(parsed_version.patch, 0);
	EXPECT_EQ(parsed_version.build, 0);

	version = "1.1.1";
	parsed_version = deb_version_parse(version);

	EXPECT_EQ(parsed_version.major, 1);
	EXPECT_EQ(parsed_version.minor, 1);
	EXPECT_EQ(parsed_version.patch, 1);	
	EXPECT_EQ(parsed_version.build, 0);

	version = "1.1.1.1";
	parsed_version = deb_version_parse(version);

	EXPECT_EQ(parsed_version.major, 1);
	EXPECT_EQ(parsed_version.minor, 1);
	EXPECT_EQ(parsed_version.patch, 1);
	EXPECT_EQ(parsed_version.build, 1);
}

TEST(libdeb_version_test, parse_more_digits)
{
	char *version = "9.87.445.1";
	DEB_VERSION parsed_version = deb_version_parse(version);

	EXPECT_EQ(parsed_version.major, 9);
	EXPECT_EQ(parsed_version.minor, 87);
	EXPECT_EQ(parsed_version.patch, 445);
	EXPECT_EQ(parsed_version.build, 1);
}

TEST(libdeb_version_test, parse_with_letters)
{
	char *version = "9.87.HENK445.1";
	DEB_VERSION parsed_version = deb_version_parse(version);

	EXPECT_EQ(parsed_version.major, 9);
	EXPECT_EQ(parsed_version.minor, 87);
	EXPECT_EQ(parsed_version.patch, 445);
	EXPECT_EQ(parsed_version.build, 1);
}

TEST(libdeb_version_test, parse_no_numbers_just_letters)
{
	char *version = "IKBENHENK";
	DEB_VERSION parsed_version = deb_version_parse(version);

	EXPECT_EQ(parsed_version.major, 0);
	EXPECT_EQ(parsed_version.minor, 0);
	EXPECT_EQ(parsed_version.patch, 0);
	EXPECT_EQ(parsed_version.build, 0);
}

TEST(libdeb_version_test, parse_empty_string)
{
	char *version = "";
	DEB_VERSION parsed_version = deb_version_parse(version);

	EXPECT_EQ(parsed_version.major, 0);
	EXPECT_EQ(parsed_version.minor, 0);
	EXPECT_EQ(parsed_version.patch, 0);
	EXPECT_EQ(parsed_version.build, 0);
}

TEST(libdeb_version_test, parse_null)
{
	DEB_VERSION parsed_version = deb_version_parse(NULL);

	EXPECT_EQ(parsed_version.major, 0);
	EXPECT_EQ(parsed_version.minor, 0);
	EXPECT_EQ(parsed_version.patch, 0);
	EXPECT_EQ(parsed_version.build, 0);
}

TEST(libdeb_version_test, compare_equal_good)
{
	int res = deb_version_is_equal({1}, {1});
	EXPECT_EQ(res, 1);

	res = deb_version_is_equal({1, 2}, {1, 2});
	EXPECT_EQ(res, 1);

	res = deb_version_is_equal({1, 2, 3}, {1, 2, 3});
	EXPECT_EQ(res, 1);

	res = deb_version_is_equal({1, 2, 3, 4}, {1, 2, 3, 4});
	EXPECT_EQ(res, 1);	
}

TEST(libdeb_version_test, compare_equal_or_higher_good)
{
	/* EQUAL */

	int res = deb_version_is_equal_or_higher({1}, {1});
	EXPECT_EQ(res, 1);

	res = deb_version_is_equal_or_higher({1, 2}, {1, 2});
	EXPECT_EQ(res, 1);

	res = deb_version_is_equal_or_higher({1, 2, 3}, {1, 2, 3});
	EXPECT_EQ(res, 1);

	res = deb_version_is_equal_or_higher({1, 2, 3, 4}, {1, 2, 3, 4});
	EXPECT_EQ(res, 1);	

	/* HIGHER */

	res = deb_version_is_equal_or_higher({2}, {1});
	EXPECT_EQ(res, 1);

	res = deb_version_is_equal_or_higher({2, 3}, {1, 2});
	EXPECT_EQ(res, 1);

	res = deb_version_is_equal_or_higher({2, 3, 4}, {1, 2, 3});
	EXPECT_EQ(res, 1);

	res = deb_version_is_equal_or_higher({2, 3, 4, 5}, {1, 2, 3, 4});
	EXPECT_EQ(res, 1);
}

TEST(libdeb_version_test, compare_equal_or_lower_good)
{
	/* EQUAL */

	int res = deb_version_is_equal_or_lower({1}, {1});
	EXPECT_EQ(res, 1);

	res = deb_version_is_equal_or_lower({1, 2}, {1, 2});
	EXPECT_EQ(res, 1);

	res = deb_version_is_equal_or_lower({1, 2, 3}, {1, 2, 3});
	EXPECT_EQ(res, 1);

	res = deb_version_is_equal_or_lower({1, 2, 3, 4}, {1, 2, 3, 4});
	EXPECT_EQ(res, 1);	

	/* LOWER */

	res = deb_version_is_equal_or_lower({1}, {2});
	EXPECT_EQ(res, 1);

	res = deb_version_is_equal_or_lower({1, 2}, {2, 3});
	EXPECT_EQ(res, 1);

	res = deb_version_is_equal_or_lower({1, 2, 3}, {2, 3, 4});
	EXPECT_EQ(res, 1);

	res = deb_version_is_equal_or_lower({1, 2, 3, 4}, {2, 3, 4, 5});
	EXPECT_EQ(res, 1);
}

TEST(libdeb_version_test, compare_higher_good)
{
	int res = deb_version_is_higher({2}, {1});
	EXPECT_EQ(res, 1);

	res = deb_version_is_higher({2, 3}, {1, 2});
	EXPECT_EQ(res, 1);

	res = deb_version_is_higher({2, 3, 4}, {1, 2, 3});
	EXPECT_EQ(res, 1);

	res = deb_version_is_higher({2, 3, 4, 5}, {1, 2, 3, 4});
	EXPECT_EQ(res, 1);
}

TEST(libdeb_version_test, compare_lower_good)
{
	int res = deb_version_is_lower({1}, {2});
	EXPECT_EQ(res, 1);

	res = deb_version_is_lower({1, 2}, {2, 3});
	EXPECT_EQ(res, 1);

	res = deb_version_is_lower({1, 2, 3}, {2, 3, 4});
	EXPECT_EQ(res, 1);

	res = deb_version_is_lower({1, 2, 3, 4}, {2, 3, 4, 5});
	EXPECT_EQ(res, 1);
}

TEST(libdeb_version_test, compare_equal_bad)
{
	int res = deb_version_is_equal({1}, {2});
	EXPECT_EQ(res, 0);

	res = deb_version_is_equal({1, 2}, {2, 3});
	EXPECT_EQ(res, 0);

	res = deb_version_is_equal({1, 2, 3}, {2, 3, 4});
	EXPECT_EQ(res, 0);

	res = deb_version_is_equal({1, 2, 3, 4}, {2, 3, 4, 5});
	EXPECT_EQ(res, 0);	
}

TEST(libdeb_version_test, compare_equal_or_higher_bad)
{
	int res = deb_version_is_equal_or_higher({1}, {2});
	EXPECT_EQ(res, 0);

	res = deb_version_is_equal_or_higher({1, 2}, {2, 3});
	EXPECT_EQ(res, 0);

	res = deb_version_is_equal_or_higher({1, 2, 3}, {2, 3, 4});
	EXPECT_EQ(res, 0);

	res = deb_version_is_equal_or_higher({1, 2, 3, 4}, {2, 3, 4, 5});
	EXPECT_EQ(res, 0);
}

TEST(libdeb_version_test, compare_equal_or_lower_bad)
{
	int res = deb_version_is_equal_or_lower({2}, {1});
	EXPECT_EQ(res, 0);

	res = deb_version_is_equal_or_lower({2, 3}, {1, 2});
	EXPECT_EQ(res, 0);

	res = deb_version_is_equal_or_lower({2, 3, 4}, {1, 2, 3});
	EXPECT_EQ(res, 0);

	res = deb_version_is_equal_or_lower({2, 3, 4, 5}, {1, 2, 3, 4});
	EXPECT_EQ(res, 0);	
}

TEST(libdeb_version_test, compare_higher_bad)
{
	int res = deb_version_is_higher({1}, {1});
	EXPECT_EQ(res, 0);

	res = deb_version_is_higher({1, 2}, {1, 2});
	EXPECT_EQ(res, 0);

	res = deb_version_is_higher({1, 2, 3}, {1, 2, 3});
	EXPECT_EQ(res, 0);

	res = deb_version_is_higher({1, 2, 3, 4}, {1, 2, 3, 4});
	EXPECT_EQ(res, 0);
}

TEST(libdeb_version_test, compare_lower_bad)
{
	int res = deb_version_is_lower({1}, {1});
	EXPECT_EQ(res, 0);

	res = deb_version_is_lower({1, 2}, {1, 2});
	EXPECT_EQ(res, 0);

	res = deb_version_is_lower({1, 2, 3}, {1, 2, 3});
	EXPECT_EQ(res, 0);

	res = deb_version_is_lower({1, 2, 3, 4}, {1, 2, 3, 4});
	EXPECT_EQ(res, 0);
}
#include "stdafx.h"
#include "gtest\gtest.h"
#include "Win32Utils.h"

TEST(Win32UtilsTest, GetLocalAccountInfoTest)
{
	ASSERT_TRUE(get_local_account_lists());
}

TEST(Win32UtilsTest, GetMacAddress)
{
	ASSERT_TRUE(get_install_programs());
}
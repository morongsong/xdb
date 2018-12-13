#include "stdafx.h"
#include "xdbarrfs.h"

TEST(xdbarrfs, abc)
{
	{
		xdbarrfs<int> a;
		a.Load("a1");
		a.Set(1, 1);
		a.Set(3, 5);
		a.Save();
	}

	{
		xdbarrfs<int> a;
		a.Load("a1");
		EXPECT_TRUE(a.Get(1));
		EXPECT_TRUE(a.Get(3));
		EXPECT_TRUE(a.Get(2) == nullptr);

		EXPECT_TRUE(*a.Get(1) == 1);
		EXPECT_TRUE(*a.Get(3) == 5);
	}


	return;
}

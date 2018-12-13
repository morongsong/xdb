#include "stdafx.h"
#include "xdbarrfs.h"
#include "xdbarrfsmt.h"

TEST(xdbarrfsmt, abc)
{
	{
		xdbarrfsmt<xdbarrfs<int, 10000>> a;
		a.Init("xbd", "test", "xdb");
		for (int i = 0; i < 1000000; i++)
		{
			a.Set(i, i);
		}
	}

	{
		xdbarrfsmt<xdbarrfs<int, 10000>> a;
		a.Init("xbd", "test", "xdb");
		for (int i = 0; i < 1000000; i++)
		{
			EXPECT_TRUE(*a.Get(i) == i);
		}
	}


	return;
}


TEST(xdbarrfsmt, abc2)
{
	vector<int> a;
	a.resize(1000000);
	{
		for (int i = 0; i < 1000000; i++)
		{
			a[i]= i;
		}
	}

	{
		for (int i = 0; i < 1000000; i++)
		{
			EXPECT_TRUE(a[i] == i);
		}
	}


	return;
}

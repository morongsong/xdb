#include "stdafx.h"
#include "xdb.h"

#define TestNumDataSize (100000000)

TEST(xdbarrfsmt, abc)
{
	{
		xdbarrfsmt<xdbarrfs<int, TestNumDataSize / 20> > a;
		a.Init("xbd", "test", "xdb");
		for (int i = 0; i < TestNumDataSize; i++)
		{
			a.Set(i, i);
		}
	}

	{
		xdbarrfsmt<xdbarrfs<int, TestNumDataSize /20>> a;
		a.Init("xbd", "test", "xdb");
		for (int i = 0; i < TestNumDataSize; i++)
		{
			EXPECT_TRUE(*a.Get(i) == i);
		}
	}
	return;
}


TEST(xdbarrfsmt, abc2)
{
	vector<int> a;
	a.resize(TestNumDataSize);
	getchar();
	{
		for (int i = 0; i < TestNumDataSize; i++)
		{
			a[i]= i;
		}
	}

	{
		for (int i = 0; i < TestNumDataSize; i++)
		{
			EXPECT_TRUE(a[i] == i);
		}
	}
	return;
}

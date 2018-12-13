#include "stdafx.h"
#include <windows.h>
#include <tchar.h>
#include <string>
#include <iostream>


int _tmain(int argc, _TCHAR* argv[])
{

	{
		BOOL v;
		SystemParametersInfo(SPI_GETBEEP, 0, &v, 0);
	}

    //_wsetlocale(0/*LC_ALL*/, _T("chs"));//设置中文环境

    ::testing::InitGoogleTest(&argc, argv);


    //::testing::GTEST_FLAG(filter) = "xx.xx";

    int final_result = RUN_ALL_TESTS();
    Sleep(100);

	getchar();

    return final_result;
	return 1;
}

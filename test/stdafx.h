// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once


#include <SDKDDKVer.h>


//windows inc
#include <windows.h>
#include <Mmsystem.h>
#include <shlwapi.h>
#include <process.h>

//clib inc
#include <stdio.h>
#include <tchar.h>
#include <stdlib.h>

//std inc
#include <string>
#include <vector>
#include <map>
#include <set>
using std::vector;
using std::string;
using std::wstring;
#if (defined(_UNICODE) || defined (UNICODE))
namespace std { typedef ::std::basic_string<wchar_t> tstring; }
#else
namespace std { typedef ::std::basic_string<char> tstring; }
#endif
using std::tstring;
using std::set;
using std::map;


//thirdparty inc
#pragma warning(push)
#pragma warning(disable:4251)
#pragma warning(disable:4275)
#include "gtest.h"
#pragma warning(pop)


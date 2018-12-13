// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
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


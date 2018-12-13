
#pragma once

#if defined(XDB_EXPORTS)
#define XDB_API          __declspec(dllexport)
#elif  defined(XDB_IMPORTS)
#define XDB_API          __declspec(dllimport)
#else
#define XDB_API      
#endif



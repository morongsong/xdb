/**
*@file         mrbigarraymtty.hpp
*@brief        mrbigarraymtty
*@version      1.0.0
*/
#ifndef MRBIGARRAYMTTY_HPP_
#define MRBIGARRAYMTTY_HPP_
#pragma once


#include <memory>
#include <shlobj.h>
#include <map>
#include <vector>
#include <assert.h>

const char* string_format(std::string& str, const char* szFormat, ...)
{
	if (!szFormat)
		return nullptr;
	va_list argList;
	va_start(argList, szFormat);

	int nLength = _vscprintf(szFormat, argList);
	str.resize(nLength);

	vsprintf_s(&(str[0]), nLength + 1, szFormat, argList);
	va_end(argList);

	return str.c_str();
}

/// mrbigarraymtty
template<typename TArrayType>
class xdbarrfsmt
{
public:
	static const int kCacheMaxNum = 10;
public:
	typedef typename TArrayType::datatype ValType;
	typedef typename std::shared_ptr<TArrayType> mrbigarrayptr;
public:
	xdbarrfsmt();
	virtual ~xdbarrfsmt();
private:
	xdbarrfsmt(const xdbarrfsmt& rhs);
	xdbarrfsmt& operator=(const xdbarrfsmt& rhs);
public:
	void SetCompress(bool bCompress) { m_bCompress = bCompress; }
	void SetCacheMaxNum(int nMax) { m_nCacheMaxNum = nMax; }
	void Init(const char* pszDir, const char* pszName, const char* pszPostfix);
public:
	void Set(int x, const ValType& dVal);
	ValType* Get(int x);
	void Del(int x);
public:
	void Clear();
public:
	bool IsLoadRange(int l, int r);
	bool LoadRange(int l, int r);
	ValType* GetInCache(int x);
private:
	mrbigarrayptr GetDBPtr(int x);
private:
	std::string m_strDir;
	std::string m_strName;
	std::string m_strPostfix;
private:
	struct range;
	struct ranged_array
	{
		mrbigarrayptr pData;
		range oRng;
	};
	typedef std::map<std::string, ranged_array> Maprngdat;
	Maprngdat m_maprngdat;
	ranged_array m_currngdat;
private:
	int m_nCacheMaxNum;
	bool m_bCompress;
};


///////////////////////////////////////////////////////////////////////////////
///                                                                         ///
/// Impl                                                                    ///
///                                                                         ///
///////////////////////////////////////////////////////////////////////////////

template<typename TArrayType>
struct xdbarrfsmt<TArrayType>::range
{
	range() { m_l = -1;m_r = -1; }
	~range() {};
	void init(int l, int r) { m_l = l;m_r = r; }
	bool isIn(int x) { return ((x >= m_l) && (x <= m_r)); }
	int m_l, m_r;
};


template<typename TArrayType>
inline xdbarrfsmt<TArrayType>::xdbarrfsmt()
{
	m_nCacheMaxNum = kCacheMaxNum;
	m_bCompress = true;
}

template<typename TArrayType>
inline xdbarrfsmt<TArrayType>::~xdbarrfsmt()
{

}


template<typename TArrayType>
inline std::shared_ptr<TArrayType> xdbarrfsmt<TArrayType>::GetDBPtr(int x)
{
	if (m_currngdat.oRng.isIn(x))
	{
		return m_currngdat.pData;
	}

	int nxn = x / TArrayType::kMaxX;

	string strfile;
	string_format(strfile,("%s\\%s(%d).%s"), m_strDir.c_str(),
		m_strName.c_str(), nxn, m_strPostfix.c_str());
	if (m_maprngdat.find(strfile) == m_maprngdat.end())
	{
		if (m_maprngdat.size() > kCacheMaxNum)
		{
			m_maprngdat.clear();
		}
		mrbigarrayptr pbd = std::make_shared<TArrayType>();
		pbd->Load(strfile.c_str());
		pbd->m_bAutoSave=(true);
		pbd->m_bCompress=(m_bCompress);
		m_maprngdat[strfile].pData = pbd;

		int l = (nxn < 0 ? 0 : nxn)*TArrayType::kMaxX;
		int r = (nxn + 1 < 1 ? 1 : nxn + 1)*TArrayType::kMaxX - 1;

		m_maprngdat[strfile].oRng.init(l, r);
	}

	m_currngdat.pData = m_maprngdat[strfile].pData;
	m_currngdat.oRng = m_maprngdat[strfile].oRng;

	return m_currngdat.pData;
}

template<typename TArrayType>
inline void xdbarrfsmt<TArrayType>::Init(const char* pszDir, const char* pszName, const char* pszPostfix)
{
	CHAR szBuff[MAX_PATH];
	if (_fullpath(szBuff, pszDir, MAX_PATH) != NULL)
	{
		SHCreateDirectoryExA(nullptr, szBuff, nullptr);
		m_strDir = szBuff;
	}
	else
	{
		assert(false);
	}
	m_strName = pszName;
	m_strPostfix = pszPostfix;
	return;
}

template<typename TArrayType>
inline typename TArrayType::datatype* xdbarrfsmt<TArrayType>::Get(int x)
{
	mrbigarrayptr ptrbd = GetDBPtr(x);
	int nrx = x%TArrayType::kMaxX;
	return ptrbd->Get(nrx);
}

template<typename TArrayType>
inline void xdbarrfsmt<TArrayType>::Set(int x, const ValType& dVal)
{
	mrbigarrayptr ptrbd = GetDBPtr(x);
	int nrx = x%TArrayType::kMaxX;
	ptrbd->Set(nrx, dVal);
	return;
}

template<typename TArrayType>
inline void xdbarrfsmt<TArrayType>::Del(int x)
{
	mrbigarrayptr ptrbd = GetDBPtr(x);
	int nrx = x%TArrayType::kMaxX;
	ptrbd->Del(nrx);
	return;
}

template<typename TArrayType>
inline bool xdbarrfsmt<TArrayType>::IsLoadRange(int l, int r)
{
	if (m_currngdat.oRng.isIn(l) && m_currngdat.oRng.isIn(r))
	{
		return true;
	}

	int nxn = l / TArrayType::kMaxX;
	int nxn2 = r / TArrayType::kMaxX;
	bool bNotInRangFinal = false;
	for (int ix = nxn;ix <= nxn2;ix++)
	{
		bool bInRange = false;
		for (Maprngdat::iterator iter = m_maprngdat.begin();iter != m_maprngdat.end(); iter++)
		{
			range& rg = iter->second.oRng;
			int xr = rg.m_l / TArrayType::kMaxX;
			if (xr == ix)
			{
				bInRange = true;
			}
		}
		if (!bInRange)
		{
			bNotInRangFinal = true;
			break;
		}
		if (bNotInRangFinal)
		{
			break;
		}
	}
	return !bNotInRangFinal;
}

template<typename TArrayType>
inline bool xdbarrfsmt<TArrayType>::LoadRange(int l, int r)
{
	assert((r - l) <= TArrayType::kMaxX*kCacheMaxNum / 2);
	int nxn = l / TArrayType::kMaxX;
	int nxn2 = r / TArrayType::kMaxX;
	for (int ix = nxn;ix <= nxn2;ix++)
	{
		GetDBPtr(ix*TArrayType::kMaxX);
	}
	return true;
}

template<typename TArrayType>
inline typename TArrayType::datatype* xdbarrfsmt<TArrayType>::GetInCache(int x)
{
	if (m_currngdat.oRng.isIn(x))
	{
		int nxn = l / TArrayType::kMaxX;
		return m_currngdat.pData->Get(nxn);
	}
	for (Maprngdat::iterator iter = m_maprngdat.begin();iter != m_maprngdat.end(); iter++)
	{
		range& rg = iter->second.oRng;
		if (rg.isIn(x))
		{
			m_currngdat.oRng = rg;
			m_currngdat.pData = iter->second.pData;
			return m_currngdat.pData->Get(x);
		}
	}
	return false;
}

template<typename TArrayType>
inline void xdbarrfsmt<TArrayType>::Clear()
{
	for (Maprngdat::iterator iter = m_maprngdat.begin();iter != m_maprngdat.end(); iter++)
	{
		iter->second.pData->m_bAutoSave = false;
	}
	m_maprngdat.clear();
	m_currngdat.pData = nullptr;
	m_currngdat.oRng.init(-1, -1);

	std::vector<string> vecfiles;
	string strPath;
	string_format(strPath,("%s\\*.*"), m_strDir.c_str(), m_strName.c_str(), m_strPostfix.c_str());

	WIN32_FIND_DATAA ffd;
	HANDLE hFind = FindFirstFileA(strPath.c_str(), &ffd);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (strcmp(ffd.cFileName, (".")) != 0 && strcmp(ffd.cFileName, ("..")) != 0)
			{
				if (!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
				{
					vecfiles.push_back(ffd.cFileName);
				}
			}
		} while (FindNextFileA(hFind, &ffd) != 0);
		FindClose(hFind);
	}

	for (std::vector<string>::iterator iter = vecfiles.begin(); iter != vecfiles.end(); iter++)
	{
		string& strfile = *iter;
		int nName = m_strName.length();
		if (strfile.compare(0, nName, m_strName.c_str(), nName) == 0)
		{
			int nPostName = m_strPostfix.length();
			if (strfile.compare(strfile.length() - nPostName, nPostName,
				m_strPostfix.c_str(), nPostName) == 0)
			{
				string strPathFull;
				string_format(strPathFull,("%s\\%s"), m_strDir.c_str(), strfile.c_str());
				::DeleteFileA(strPathFull);
			}
		}
	}
	return;
}

#endif //MRBIGARRAYMTTY_HPP_

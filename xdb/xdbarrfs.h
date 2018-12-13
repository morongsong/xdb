/**
*@file         mrbigarrayfs.hpp
*@brief        mrbigarrayfs
*@version      1.0.0
*/
#ifndef MRBIGARRAYFS_HPP_
#define MRBIGARRAYFS_HPP_
#pragma once

#include <assert.h>
#include <vector>
#include <string>

/// xdbarrfs
template<class TDataType, int nTMaxX = 1024>
class xdbarrfs
{
public:
	typedef TDataType datatype;
	xdbarrfs();
	virtual ~xdbarrfs();
public:
	bool m_bAutoSave;
	bool m_bCompress;
public:
	void Load(const char* pszfile);
	void Save();
public:
	void Set(int x, const datatype& dVal);
	TDataType* Get(int x);
	void Del(int x);
public:
	static const int kMagic = 0x1234abcd;
	static const int kMagic2 = 0x5678eaff;
	static const int kMaxX = nTMaxX;
private:
#pragma pack(push,4)
	struct SHeader
	{
		int nMagic;
		int nMagic2;
		int nXStart;
		int nXEnd;
		int nXNum;
		bool bCompress;
		int nCpsDataLen;
		int nResverData[40];
	};
#pragma pack(pop)

#pragma pack(push,1)
	struct SData
	{
		bool bUse;
		datatype dVal;
	};
#pragma pack(pop)
	std::vector<SData> m_oData;
	std::string m_strfile;

};

///////////////////////////////////////////////////////////////////////////////
///                                                                         ///
/// Impl                                                                    ///
///                                                                         ///
///////////////////////////////////////////////////////////////////////////////

template<class TDataType, int nTMaxX>
inline xdbarrfs<TDataType, nTMaxX>::xdbarrfs()
{
	m_bAutoSave = true;
	m_bCompress = false;
}

template<class TDataType, int nTMaxX>
inline xdbarrfs<TDataType, nTMaxX>::~xdbarrfs()
{
	if (m_bAutoSave)
	{
		Save();
	}
}

template<class TDataType, int nTMaxX>
inline void xdbarrfs<TDataType, nTMaxX>::Load(const char* pszfile)
{
	Save();
	CHAR szBuff[MAX_PATH];
	if (_fullpath(szBuff, pszfile, MAX_PATH) != NULL)
	{
		m_strfile = szBuff;
	}
	else
	{
		assert(false);
	}

	m_oData.resize(kMaxX);
	for (int iw = 0, isize = m_oData.size();iw < isize;iw++)
	{
		m_oData[iw].bUse = false;
	}

	FILE *fp;
	if (fopen_s(&fp, m_strfile.c_str(), "rb+") != NULL)
	{
		return;
	}

	SHeader oHeader;
	memset(&oHeader, 0, sizeof(oHeader));

	if (fread(&oHeader, sizeof(oHeader), 1, fp)!=1)
	{
		fclose(fp);
		return;
	}
	if ((oHeader.nMagic != kMagic) || (oHeader.nMagic2 != kMagic2))
	{
		fclose(fp);
		return;
	}
	if (oHeader.nXNum != kMaxX)
	{
		fclose(fp);
		return;
	}

	fseek(fp, sizeof(oHeader), SEEK_SET);
	
	int nDataLen = kMaxX * sizeof(SData);
	{
		fread(&(m_oData[0]), nDataLen,1, fp);
	}
	fclose(fp);
	return;
}

template<class TDataType, int nTMaxX>
inline void xdbarrfs<TDataType, nTMaxX>::Save()
{
	if (m_strfile.empty())
	{
		return;
	}

	FILE *fp;
	if (fopen_s(&fp, m_strfile.c_str(), "wb+") != NULL)
	{
		return;
	}

	SHeader oHeader;
	memset(&oHeader, 0, sizeof(oHeader));

	fseek(fp, sizeof(oHeader), SEEK_SET);

	int nDataItemLen = sizeof(SData);
	int nDataLen = kMaxX*nDataItemLen;
	{
		oHeader.bCompress = false;
		oHeader.nCpsDataLen = nDataLen;
		fwrite(&(m_oData[0]), nDataLen, 1, fp);
	}

	fseek(fp, 0, SEEK_SET);
	oHeader.nMagic = kMagic;
	oHeader.nMagic2 = kMagic2;
	oHeader.nXStart = 0;
	oHeader.nXEnd = kMaxX - 1;
	oHeader.nXNum = kMaxX;
	
	if (fwrite(&oHeader, sizeof(oHeader), 1, fp)!=1)
	{
		return;
	}
	fclose(fp);
	return;
}

template<class TDataType, int nTMaxX>
inline void xdbarrfs<TDataType, nTMaxX>::Set(int x, const datatype& dVal)
{
	SData& oData = m_oData[x];
	oData.bUse = true;
	oData.dVal = dVal;
	return;
}

template<class TDataType, int nTMaxX>
TDataType* xdbarrfs<TDataType, nTMaxX>::Get(int x)
{
	SData& oData = m_oData[x];
	if (oData.bUse)
	{
		return &(oData.dVal);
	}
	return nullptr;
}

template<class TDataType, int nTMaxX>
inline void xdbarrfs<TDataType, nTMaxX>::Del(int x)
{
	SData& oData = m_oData[x];
	if (oData.bUse)
	{
		oData.bUse = false;
	}
	return;
}

#endif //MRBIGARRAYFS_HPP_

//////////////////////////////////////////////////////////////////////
// textFile.cpp: implementation of the textFile class.
// Created 07/19/1999:  textFile class is a standard input/output
//	File class, except that it limits the line length to either
//	STDLENGTH or a given UINT in the constructor.  When using
//	textFile.WriteString, a new line is inserted into the file when
//	the line reaches the limit.
//////////////////////////////////////////////////////////////////////

#ifndef HPUX
//#include "stdafx.h"
#endif
 
#include "textFile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// textFile implementation

textFile::textFile()
{
	charcount = 0;
	linelength = STDLENGTH;
}

textFile::textFile(UINT len)
{
	charcount = 0;
	linelength = len;
}

void textFile::WriteString(LPCTSTR lpsz)
{
	ASSERT(lpsz != NULL);
	ASSERT(m_pStream != NULL);

	CString s = lpsz;

	if(s.Find("\n") == -1)
	{
		if((charcount += s.GetLength()) > linelength)
		{
			_fputts("\n", m_pStream);
			charcount = 0;
		}
	}
	else while(s.Find("\n") != -1)
	{
		charcount = s.GetLength() - (s.Find("\n") + 2);
		s = s.Right(charcount);
	}

	if (_fputts(lpsz, m_pStream) == _TEOF)
		AfxThrowFileException(CFileException::diskFull, _doserrno, m_strFileName);
}
 
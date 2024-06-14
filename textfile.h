// textFile.h: interface for the textFile class.
//		
//////////////////////////////////////////////////////////////////////
#undef AFX_DATA
#define AFX_DATA AFX_EXT_DATA

#if !defined(AFX_TEXTFILE_H__FAFE399A_3533_11D3_BE47_0000C0605482__INCLUDED_)
#define AFX_TEXTFILE_H__FAFE399A_3533_11D3_BE47_0000C0605482__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define STDLENGTH 80

class AFX_EXT_CLASS textFile : public CStdioFile  
{
// Constructors
public:
	textFile();
	textFile(UINT len);

// Operations
	void WriteString(LPCTSTR lpsz);

// Implementation
protected:
	UINT charcount, linelength;	
};

#ifdef __cplusplus
}
#endif

#endif // !defined(AFX_TEXTFILE_H__FAFE399A_3533_11D3_BE47_0000C0605482__INCLUDED_)

#undef AFX_DATA
#define AFX_DATA
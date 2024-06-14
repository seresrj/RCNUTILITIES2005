// brcOutFile.h: interface for the CbrcOutFile class.
//
//////////////////////////////////////////////////////////////////////
#undef AFX_DATA
#define AFX_DATA AFX_EXT_DATA

#if !defined(AFX_BRCOUTFILE_H__914C4574_E66E_4AE8_ABB4_68F68AE547D9__INCLUDED_)
#define AFX_BRCOUTFILE_H__914C4574_E66E_4AE8_ABB4_68F68AE547D9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "Winspool.h"

#define OFF			0
#define USED		1
#define RECLOSE		2
#define OVERRIDDEN	3

class AFX_EXT_CLASS CbrcOutFile : public CStdioFile  
{
public:
	CbrcOutFile();
	virtual ~CbrcOutFile();

	void OpenBrcFile();
	void CloseBrcFile();

	BOOL BrcWriteLine(LPCTSTR lpsz);
	BOOL BrcLineToPrinter(LPCTSTR lpsz);

protected:
	void Initialize();
	void GetIniVariables();

	BOOL OpenBrcFile(CStdioFile* file, CString& filename, BOOL& fileopen);
	BOOL CloseBrcFile(CStdioFile* file, BOOL& fileopen);
	BOOL WriteBrcLine(LPCTSTR lpsz, CStdioFile* outfile, CString& filename, short& file, BOOL& fileopen);

	BOOL ContinueTask(LPCTSTR errorstring, LPCTSTR devicestring);
	BOOL WriteLine(LPCTSTR lpsz);

	CString m_outfilename;
	short m_file;
	BOOL m_fileopen;	

	CStdioFile* m_alternatefile;
	CString m_alternatefilename;
	short m_altfile;
	BOOL m_altfileopen;

	CStdioFile* m_printfile;
	CString m_printfilepath;
	short m_printfilestatus;
	BOOL m_printfileopen;

	CString m_errorlogpath;

	BOOL m_interrupt;
	CString m_task;
};

#ifdef __cplusplus
}
#endif

//////////////////////////////////////////////////////////////////////
// brcUtilities
//
//////////////////////////////////////////////////////////////////////

#define BRCDIRKEY	"brcdir"
#define BRCINIKEY	"brc.ini"
#define BRCPRINTKEY	"printfile"
#define BRCERRKEY	"errorlog"
#define BRCPATHKEY	"outpath"
#define BRCALTKEY	"altpath"
#define BRCMOODKEY	"mood"
#define CLOCKPATH	"C:\\Program Files\\brc\\ClockTest"
#define MOODPATH	"C:\\Program Files\\brc\\moodscale"
#define STOPPATH	"C:\\Program Files\\brc\\stopping\\STOPPING"
#define FOURPATH	"C:\\Program Files\\brc\\FourChoice"

#define BUFSIZE 32

CString GetStringFromRegistry(CString& key);
BOOL SetRegistryString(CString& key, CString& registryvalue);

CString GetBrcDirectory();
CString GetIniFilePath();
CString GetErrorLogPath();
CString GetPrintFilePath();
CString GetOutPutPath();
CString GetBackupPath();
CString GetMoodInPath();

BOOL SetBrcDirectory(CString& registryvalue);
BOOL SetIniFilePath(CString& registryvalue);
BOOL SetErrorLogPath(CString& registryvalue);
BOOL SetPrintFilePath(CString& registryvalue);
BOOL SetOutPutPath(CString& registryvalue);
BOOL SetBackupPath(CString& registryvalue);
BOOL SetMoodInPath(CString& registryvalue);

void WriteToErrorLog(CString& errorstring);
void WriteToErrorLog(CString& errorlog, CString errorstring);

#endif // !defined(AFX_BRCOUTFILE_H__914C4574_E66E_4AE8_ABB4_68F68AE547D9__INCLUDED_)

#undef AFX_DATA
#define AFX_DATA
// brcOutFile.cpp: implementation of the CbrcOutFile class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include "brcOutFile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CbrcOutFile::CbrcOutFile()
{	
	Initialize();
	GetIniVariables();
}

CbrcOutFile::~CbrcOutFile()
{
	CloseBrcFile();
}

//////////////////////////////////////////////////////////////////////
// Implementation
//////////////////////////////////////////////////////////////////////

void CbrcOutFile::OpenBrcFile()
{
	if ( m_file == RECLOSE ) m_file = USED;
	if ( m_altfile == RECLOSE ) m_altfile = USED;
	if ( m_printfilestatus == RECLOSE ) m_printfilestatus = USED;
}

void CbrcOutFile::CloseBrcFile()
{
	CloseBrcFile(this,m_fileopen);
	CloseBrcFile(m_alternatefile,m_altfileopen);
	CloseBrcFile(m_printfile,m_printfileopen);
}

BOOL CbrcOutFile::BrcWriteLine(LPCTSTR lpsz)
{
	return WriteLine(lpsz);
}

BOOL CbrcOutFile::BrcLineToPrinter(LPCTSTR lpsz)
{
	return WriteBrcLine(lpsz, m_printfile, m_printfilepath, m_printfilestatus, m_printfileopen);
}

//////////////////////////////////////////////////////////////////////
// Internal Functions
//////////////////////////////////////////////////////////////////////

void CbrcOutFile::Initialize()
{
	m_file  = OFF;
	m_fileopen = false;
	m_outfilename.Empty();

	m_altfile = OFF;
	m_altfileopen = false;
	m_alternatefilename.Empty();

	m_printfile = new CStdioFile();
	m_printfilestatus = RECLOSE;
	m_printfileopen = false;
	m_printfilepath.Empty();

	m_interrupt = false;
}

void CbrcOutFile::GetIniVariables()
{
	CString strIniFile = GetIniFilePath();
	m_printfilepath = GetPrintFilePath();
	m_errorlogpath = GetErrorLogPath();

	CString pathstring,varstring;
	::GetPrivateProfileString("VAR","TASK","NO",m_task.GetBuffer(BUFSIZE),BUFSIZE,strIniFile);
	::GetPrivateProfileString("VAR","ID","99999",varstring.GetBuffer(BUFSIZE),BUFSIZE,strIniFile);
	::GetPrivateProfileString("DIR","OUTPATH","",pathstring.GetBuffer(MAX_PATH),MAX_PATH,strIniFile);
	if ( !(pathstring == "") )
	{
		m_outfilename.Format("%s\\%s%s.out",pathstring,m_task,varstring);
		m_file = RECLOSE;
	}
	::GetPrivateProfileString("DIR","ALTPATH","",pathstring.GetBuffer(MAX_PATH),MAX_PATH,strIniFile);
	if ( !(pathstring == "") )
	{
		m_alternatefilename.Format("%s\\%s%s.out",pathstring,m_task,varstring);
		m_altfile = RECLOSE;
		m_alternatefile = new CStdioFile();
	}
	::GetPrivateProfileString("VAR","INTERRUPT","FALSE",varstring.GetBuffer(BUFSIZE),BUFSIZE,strIniFile);
	if ( varstring == "TRUE" )
		m_interrupt = true;
}

BOOL CbrcOutFile::OpenBrcFile(CStdioFile* file, CString& filename, BOOL& fileopen)
{
	if ( !fileopen )
	{
		if ( file->CStdioFile::Open(filename, CFile::modeCreate | 
					CFile::modeNoTruncate |	CFile::modeWrite | CFile::typeText) )
			fileopen = true;
		else
			fileopen = false;
	}
	return fileopen;
}

BOOL CbrcOutFile::CloseBrcFile(CStdioFile* file, BOOL& fileopen)
{
	if ( fileopen )
	{
		file->Close();
		fileopen = false;
	}
	return !fileopen;
}

BOOL CbrcOutFile::WriteBrcLine(LPCTSTR lpsz, CStdioFile* outfile, CString& filename, short& file, BOOL& fileopen)
{
	CString errorstring;
	if ( !fileopen && !OpenBrcFile(outfile,filename,fileopen) )
	{
		file = OVERRIDDEN;
		errorstring.Format("%s Error opening file:Overridden. Output: %s",filename, lpsz);
		WriteToErrorLog(errorstring);
		if ( m_interrupt )
		{
			errorstring.Format("Error opening file");
			if ( !ContinueTask(errorstring,filename) )	return false;
		}
	}
	if ( fileopen )
	{
		TRY
		{
			outfile->SeekToEnd();
			outfile->WriteString("\n");
			outfile->WriteString(lpsz);
		}
		CATCH (CFileException, e)
		{
			file = OVERRIDDEN;
			errorstring.Format("%s Error writing to file:%s:Overridden. Output: %s",
				filename, e->m_cause, lpsz);
			WriteToErrorLog(errorstring);
			if ( m_interrupt )
			{
				errorstring.Format("Cannot write to output file because %s!",e->m_cause);
				if ( !ContinueTask(errorstring,filename) )	return false;
			}
		}
		END_CATCH
		if ( file != USED )	CloseBrcFile(outfile,fileopen);
	}
	return true;
}

BOOL CbrcOutFile::ContinueTask(LPCTSTR errorstring, LPCTSTR devicestring)
{
	CString errormessage;
	errormessage.Format("Problem with %s:\n %s!\n"
				"Continue task without writing to %s?\n(Yes:Continue,No:Stop Task)"
				,devicestring,errorstring,devicestring);
	if ( AfxMessageBox(errormessage, MB_YESNO|MB_ICONSTOP) != IDYES ) return false;
	return true;
}

BOOL CbrcOutFile::WriteLine(LPCTSTR lpsz)
{
	CString errorstring;
	WriteBrcLine(lpsz, m_printfile, m_printfilepath, m_printfilestatus, m_printfileopen);
	if ( m_file != OFF )
	{
		if ( m_file == OVERRIDDEN )
		{
			errorstring.Format("%s Overridden. Output: %s",m_printfilepath, lpsz);
			WriteToErrorLog(errorstring);
		}
		else WriteBrcLine(lpsz,this,m_outfilename,m_file,m_fileopen);
	}
	if ( m_altfile != OFF )
	{
		if ( m_altfile == OVERRIDDEN )
		{
			errorstring.Format("%s Overridden. Output: %s",m_outfilename, lpsz);
			WriteToErrorLog(errorstring);
		}
		else WriteBrcLine(lpsz,m_alternatefile,m_alternatefilename,m_altfile,m_altfileopen);
	}
	return true;
}

//////////////////////////////////////////////////////////////////////
// BrcUtilities
//////////////////////////////////////////////////////////////////////

CString GetStringFromRegistry(CString& key)
{
	CString returnstring;
	returnstring.Empty();
	HKEY hKey;
	DWORD dwBufLen = MAX_PATH;
	if (	RegOpenKeyEx( HKEY_LOCAL_MACHINE,TEXT("SOFTWARE\\BRC\\BrcMain"),
							0,KEY_READ,&hKey ) == ERROR_SUCCESS )
	{
		if ( RegQueryValueEx(hKey, key,NULL,NULL,
				(unsigned char *)returnstring.GetBuffer(MAX_PATH),&dwBufLen) == ERROR_SUCCESS )
		{
			RegCloseKey(hKey);
		}
	}
	return returnstring;
}

BOOL SetRegistryString(CString& key, CString& registryvalue)
{
	BOOL ok = false;
	HKEY hKey;
	DWORD dwBufLen = MAX_PATH;
	if ( RegOpenKeyEx( HKEY_LOCAL_MACHINE,TEXT("SOFTWARE\\BRC\\BrcMain"),
			0,KEY_WRITE,&hKey ) == ERROR_SUCCESS )
	{
		if ( RegSetValueEx(hKey,key,0,REG_SZ,(unsigned char *)(LPCSTR)registryvalue,
				registryvalue.GetLength()) == ERROR_SUCCESS )
			ok = true;
		RegCloseKey(hKey);
	}
	return ok;
}

CString GetBrcDirectory(){return GetStringFromRegistry((CString)BRCDIRKEY);}
CString GetIniFilePath(){return GetStringFromRegistry((CString)BRCINIKEY);}
CString GetErrorLogPath(){return GetStringFromRegistry((CString)BRCERRKEY);}
CString GetPrintFilePath(){return GetStringFromRegistry((CString)BRCPRINTKEY);}
CString GetOutPutPath(){return GetStringFromRegistry((CString)BRCPATHKEY);}
CString GetBackupPath(){return GetStringFromRegistry((CString)BRCALTKEY);}
CString GetMoodInPath(){return GetStringFromRegistry((CString)BRCMOODKEY);}

BOOL SetBrcDirectory(CString& registryvalue){return SetRegistryString((CString)BRCDIRKEY, registryvalue);}
BOOL SetIniFilePath(CString& registryvalue){return SetRegistryString((CString)BRCINIKEY, registryvalue);}
BOOL SetErrorLogPath(CString& registryvalue){return SetRegistryString((CString)BRCERRKEY, registryvalue);}
BOOL SetPrintFilePath(CString& registryvalue){return SetRegistryString((CString)BRCPRINTKEY, registryvalue);}
BOOL SetOutPutPath(CString& registryvalue){return SetRegistryString((CString)BRCPATHKEY, registryvalue);}
BOOL SetBackupPath(CString& registryvalue){return SetRegistryString((CString)BRCALTKEY, registryvalue);}
BOOL SetMoodInPath(CString& registryvalue){return SetRegistryString((CString)BRCMOODKEY, registryvalue);}


void WriteToErrorLog(CString& errorstring)
{
	WriteToErrorLog(GetErrorLogPath(), errorstring);
}

void WriteToErrorLog(CString& errorlog, CString errorstring)
{
	CStdioFile errorfile(errorlog,CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite);
	errorfile.SeekToEnd();
	errorfile.WriteString("\n");
	CTime dt = CTime::GetCurrentTime();
	errorstring.Format("%s: %s",dt.Format("%m-%d-%Y  %H:%M:%S"),errorstring);
	errorfile.WriteString(errorstring);
	errorfile.Close();
}


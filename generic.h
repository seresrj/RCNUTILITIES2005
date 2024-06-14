/* *********************************************************************** */
/* GENERIC.H - Holds all function prototying except OPENATL protos				    */
/* Modified: 7/9/1992  Debbie Kesicki                                      */
/* Modified: 6/3/1994  T.Hoffman:  Aded JD's nowhite() funct from POLY	   */
/* Modified: 5/22/2002  R Seres:  made changes to be usable for rcn library	   */
/* Modified: 8/14/2002  R Seres:  added RemovePathFromFilename	   */
/* Modified: 10/27/2002  R Seres:  added GetIdFromFilename and 	GetNightFromFilename*/
/* Modified: 3/3/2003  R Seres:  Added functions from UlnSub.f, made some changes for c compatibility*/
/* Modified : 4/29/03 RJS Added space at end to suppress hp warnings */
/* Modified: 12/02/2003  R Seres:  Removed the c_prompt() function. c_prompt_len() is safer and should be used instead*/
/* Modified : 1/2/05 RJS Added more YYYYMMDD and HHMMSS conversion functions */
/* Last Modified : 12/2/05 RJS Added #ifdef so Harmonie dll can include this file */
/* *********************************************************************** */

/* *********************************************************************** */
/* - - - - - - - - - - - - - GENERICS FUNCTIONS  - - - - - - - - - - - - - */
/* RemovePathFromFilename- Remove the path from a filename */
/* EAT_KBD - Clears system keyboard buffer by eating any keystroak	   */
/* WAIT_KEY - Delay until any key is pressed. Then eat kbd befre returning */
/* GCD - Returns greatest common devisor of two long variables             */
/* LCM - Returns least common divisor of 2 variables                       */
/* ATOINT - Converts alpha to integer                                      */
/* CPROMPT - Displays prompt and reads reply				   */
/* GET_FLOAT - prompts for then reads a float from kbd			   */
/* GET_LONG  - prompts for then reads a long  from kbd			   */
/* GET_UNSIGNED_SHORT - prompts for then reads an unsigned short from kbd  */
/* GET_SHORT - prompts for then reads a short from kbd			   */
/* GET_YYYYMMDD	- prompts for then reads a date	from kbd  YYYYMMDD	   */
/* GET_HHMMSS  - prompts for then reads a time	from kbd  HHMMSS	   */
/* GET_HHMM    - prompts for then reads a time	from kbd  HHMM		   */
/* STRUP - Local version of strupr() since not ANSI defined.		   */
/* STRLO - Local version of strlwr() since not ANSI defined.		   */
/* MEMBER - Returns TRUE if char arg is member of string arg               */
/* STRUP - Local version of strupr() since that is not ANSI defined        */
/* PARS_ARG - Parses command line for filename                             */
/* PARSE_COMMAND - Parses switches from command line                       */
/* SHOW_SWITCH - Displays switches and defaults values                     */
/* CKLOG - Validates and sets logical switch value                         */
/* S_TO_HHMMSS - Returns hours,mins sec given seconds			   */
/* FILE_EXISTS - Takes filespec arg: returns TRUE if file exists else FALSE*/
/* GET_FILE_DATE - Returns DOS creation date of filespec                   */
/* TESTFILE - Checks disk for existing file prompts user to continue       */
/* CKFLEN   - Checks filename length                                       */
/* GETDSTR - Returns directories prefacing filename                        */
/* OPEN_FILE_LIST - Opens a file from a list of directories		   */
/* NOWHITE - Removes whitespace from a string				   */
/* TYPECAST - converts array of chars to some specified type done inline   */
/* LMIN,LMAX - return MIN, or MAX of two longs                             */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#ifndef GENERIC_H
#define GENERIC_H
#include "local.h" 


#define EOL 10
#define CR 13

#ifdef __cplusplus      //debug jp 
//extern "C" {
#endif /* __cplusplus */
void RemovePathFromFilename(char filename[]);
void  wait_key(void); 
int GetNightFromFilename(char filename[]);
short eat_kbd(void);
long  gcd(long x, long y);
long  lcm(long x, long y);
short atoint(char string[], short *number);
//short c_prompt(char msg[], char response[]);
short c_prompt_len(char msg[], char response[], short max_resp_len);
short get_float( char [], double, double, float * ); 
short get_double( char [], double, double, double * );//rjs added jan 24 since get float can't work for doubles on linux and vise-versa
short get_long( char prompt[], long lmin, long lmax, long *lval );
short get_unsigned_short( char prompt[], unsigned short smin, unsigned short smax, unsigned short *sval );
short get_short( char prompt[], short smin, short smax, short *sval );
short get_yyyymmdd( char prompt[], long *dval ); /* YYYYMMDD */
short get_hhmmss( char prompt[], long *tval );	 /* HHMMSS */
short get_hhmm( char prompt[], long *tval );	 /* HHMM */
short strup(char string[]);
short strlo(char string[]);
short member( char c, char set[] );
short pars_arg(short argc, char *argv[], short snum, char *switches[],
	     short cval[], short defaults[], char bfilnm[], char *vdesc[]);
short parse_command(char command_line[], short nswitches, char *keywords[],
							    short values[]);
short show_switch(short help, short snum, char *switches[], short sval[],
							    char *vdesc[]);
short cklog(short lval, short snum, char  *switches[], short  defaults[],
							    char *vdesc[]);
short s_to_hhmmss(long s, long *hh, long *mm, long *ss);
short file_exists( char filespec[] );
short get_file_date(char filespec[], char date_string[]);
short testfile(char filename[]);
short ckflen(char fname[]);
short getdstr(char fname[], char dstr[]);
FILE *open_file_list(char filename[], short num_directories,
				char *directory_list[], char arguments[]);
short nowhite( char *char_string );
void typecast( double dat[], long cnt, short siz );
long lmin( long a, long b );
long lmax( long a, long b );

/* *********************************************************************** */
/* - - - - - - - - - - - - - TIMESUB FUNCTIONS - - - - - - - - - - - - - - */
/* JULINT - Returns julian date from YYYYMMDD from 1/1/1970                */
/* JULYRM - Returns YYYYMMDD from julian date                              */
/* INCREMENT_DATE_TIME_BY_SEC - Adds seconds to YYYYMMDD and HHMMSS values */
/* TIME_DIF - Returns diff between two YYYYMMDD, HHMMSS fields in secs     */
/* SYSDATETIME - Sets YYYYMMDD, HHMMSS to the current date and time        */
/* CKDATE - Tests for valid YYYYMMDD value.                                */
/* CKHHMM - Tests for valid HHMM time value.                               */
/* CKTIME - Tests for valid HHMMSS time value.                             */
/* SHOW_TIME - Displays current system time.                               */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
long julint(long ymd);
long julyrm(long jday);
short increment_date_time_by_sec(long *yyyymmdd, long *hhmmss, long incr);
long time_dif(long ymd1, long hms1, long ymd2, long hms2);
short sysdatetime(long *yyyymmdd, long *hhmmss);
short ckdate(long yyyymmdd);
short ckhhmm(long hhmm);
short cktime(long hhmmss);
short show_time(char msg[]);

/* *********************************************************************** */
/* - - - - - - - - - - - - - COMPDATA FUNCTIONS  - - - - - - - - - - - - - */
/* COMPRESS_DATA_1 - Compresses vals in buf array, returns new array length*/
/* DECOMPRESS_DATA_1 -Retrieves vals in buf array, returns new array length*/
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
unsigned short compress_data_1(unsigned short count, unsigned short buffer[]);
unsigned short decompress_data_1(unsigned short count, unsigned short buffer[]);


/* *********************************************************************** */
/* - - - - - - - - - - - - EXTRACT FUNCTIONS - - - - - - - - - - - - - - - */
/* END_EXTRACT - Closes file, exits program                                */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void end_extract(int dummy);

/* *********************************************************************** */
/* - - - - - - - - - - - - BSMOOTH FUNCTIONS - - - - - - - - - - - - - - - */
/* END_BSMOOTH - Closes files, frees smoothing class space, does NOT exit  */
/* BSMOOTH_ABORT - ^C handler calls end_bsmooth then exits - - - - - - - - */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void end_bsmooth(void);
void bsmooth_abort( int dummy );


/* *********************************************************************** */
/*  - - - - - - - - - - - BPLOT FUNCTIONS - - - - - - - - - - - - - - - -  */
/* PLOT_BUFS - Plots a screen of data                                      */
/* CLEANUP - Frees memory and resets terminal                              */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
long   plot_bufs(void);
void   cleanup(int dummy);

/* *********************************************************************** */
/* - - - - - - - - - - - - INIFILE FUNCTIONS - - - - - - - - - - - - - - - */
/* SeekToSectionLn - Scans a .INI file until a Section string is found
 * SeekToKeyLn	   - Scans a .INI file until a Key string is found
 * GetPrivateProfileString - Returns string to right of = sign
 * GetPrivateProfileInt    - Returns int value on right of = sign
*/
short SeekToSectionLn( FILE *(*fptr),const char *token, char line[] );
short SeekToKeyLn( FILE *(*fptr),const char *token, char line[] );
#ifndef WINDOWS
#ifndef HPUX
#ifndef LINUX
#define WINDOWS 1
#endif
#endif
#endif

#ifndef WINDOWS
 
unsigned short GetPrivateProfileString( const char* lpAppName, const char* lpKeyName, 
		                        const char* lpDefaultString, char lpReturnedString[],
					short nSize, const char* lpFileName );

unsigned short GetPrivateProfileInt( const char* lpAppName, const char* lpKeyName,
				     unsigned short nDefault, const char* lpFileName);
#endif 

//#endif
 
short deref_filespec( char filespec[], char filenm[] );
int	GetIntFromBuffer(char *buffer, int start, int end);
char*	getExtention(char *string); //rjs added nov 26 2002
short strcmp_nocase(char *string1, char *string2);//rjs added feb 28 2003
short strncmp_nocase(char *string1, char *string2, short length);//rjs added june 9 2003
char* HHMMSS_ToString(char string[], long HHMMSS);//rjs added feb 28 2003
void GetStringFromBuffer(char *buffer, char *result, int start, int end);//rjs added feb 28 2003
float GetFloatFromBuffer(char *buffer, int start, int end);//rjs added feb 28 2003
short switch_bytes_hp(unsigned char *baddr, size_t size );//rjs moved this here feb 28 20030
void DateTimeToString(unsigned char string[], long YYYYMMDD, long HHMMSS);
long StringTimeToHHMMSS(char time[]);
long StringDateToYYYYMMDD(char date[]);
short StringToYYYYMMDDAndHHMMSS(char datetime[], long *YYYYMMDD, long *HHMMSS);
#ifdef __cplusplus
//moved these here since only c++ understands the "bool" type. 
bool GetFileName(char FileName[], short argc, char *argv[]);
bool ReadLine(FILE *fptr, char text[]);
#ifndef HARMONIE
 
bool FileExists( const char* filename );
#endif
bool IsANumber(char string[], char size=0); //rjs added may 28 2003
bool IsNumberBetween(char check, char first, char second);//rjs added feb 28 2003
bool NotEmpty(char *string); //rjs added feb 28 2003
//and c doesn't have default parameters
int InA(char string_to_find[], char string[], int maximum, int start=0, int end=-1);//rjs added feb 28 2003

//}

#endif

#endif//generic_h


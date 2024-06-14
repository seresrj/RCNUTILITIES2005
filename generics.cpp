/****************************************************************************
 * GENERICS.C - Contains generic functions for C programs.                  *
 *	  EAT_KBD - Clears the keyboard buffer				    *
 *       WAIT_KEY - Pauses until a key is hit, eats that key and returns    *
 *            GCD - Returns greatest common denominator of 2 longs.         *
 *            LCM - Returns least common divisor of 2 longs.                *
 *         ATOINT - Converts alpha to integer.                              *
 *        CPROMPT - Displays prompt and reads reply.                        *
 *    CPROMPT_LEN - same as above but checks for response string overflow   *
 *      GET_FLOAT - prompts for then reads a float from kbd                 *
 *       GET_LONG - prompts for then reads a long int from kbd              *
 *      GET_SHORT - prompts for then reads a short from kbd                 *
 *   GET_YYYYMMDD - prompts for then reads a YYYYMMDD date from kbd         *
 *     GET_HHMMSS - prompts for then reads a HHMMSS time from kbd           *
 *       GET_HHMM - prompts for then reads a HHMM   time from kbd           *
 *          STRUP - Local version of strupr() since not ANSI defined.       *
 *	    STRLO - Local version of strlwr() since not ANSI defined.	    *
 *	   MEMBER - Tells if a char is an element of a string		    *
 *  PARSE_COMMAND - Parses switches from command line.                      *
 *       PARS_ARG - Parses command line for filename.                       *
 *    SHOW_SWITCH - Displays switches and defaults values.                  *
 *          CKLOG - Validates and sets logical switch value.                *
 *    FILE_EXISTS - Takes filespec as arg returns TRUE if filexists         *
 *  GET_FILE_DATE - Int Function takes a filespec and a char array. Returns *
 *                  a string representation of the DOS creation date of the *
 *                  filespec passed in. Note: Creation Date = Last Modifed  *
 *    S_TO_HHMMSS - Int Function takes 1 read only arg and 3 returns args.  *
 *                  the read arg is a long # of seconds, the 3 returns args *
 *                  are the hrs minutes and seconds.                        *
 *       TESTFILE - Checks disk for existing file prompts user to continue  *
 *         CKFLEN - Check length of filename.                               *
 *        GETDSTR - Returns directories prefacing filename                  *
 * OPEN_FILE_LIST - Opens a file from a list of directories		    *
 *	  NOWHITE - removes whitespace from a string
 * SeekToSectionLn - Scans an .INI until a Section string is found
 * SeekToKeyLn	   - Scans an .INI until a Key string is found
 * GetPrivateProfileString - Returns string to right of = sign
 * GetPrivateProfileInt    - Returns int value on right of = sign
    PARSE_ENVAR      parses and returns environment var from filespec 
                       i.e $TSD/044123HSC.002 "TSD" is the envir var.
    DEREF_FILESPEC   converts $TSD/044123HSC.002 to (for example)
                       /users/work/hoffman/tsd/044123HSC.002


 * Modified: 4/25/1991	 Jack Doman  "int" changed to short for version 8.0 *
 * Modified: 9/19/91 Debbie Frasca - Added switches to comment v8.5         *
 * Modified: 11/19/91 Debbie Frasca - Removed BELL from eat_kbd v8.6        *
 * Modified: 7/6/1992 Debbie Kesicki v9.1 GENERIC.H Function prototying     *
 *                                              Added TESTFILE function     *
 *                                              Added CKFLEN function       *
 *                                              Added GETDSTR function      *
 *                                              Added STRLO function        *
 * Modified: 6/28/1993 Jack Doman - Added open_file_list function           *
 * Modified: 6/03/1994 T.Hoffman  - Added nowhite() function from poly
 * Modified: 10/20/94  T.Hoffman  - Added typecast() lmin() and lmax() functs
 * Modified: 03/27/97  T.Hoffman  - Added PrivateProfile functs for INI files
 * Modified: 12/05/97  T.Hoffman  - Fixed bug in GetPrivateProfileString 
 *                                  and GetPrivateProfileInt. Had failed to
 *                                  close the .ini file before leaving 
 * Modified: 11/25/98	J.Doman	- Moved parse_envar & deref_filespec from TSDLIB
 * Modified: 5/23/02	R Seres	-  fixed crashing bug when retreiving pathname in win32
 * Modified: 3/3/2003  R Seres:  Added functions from UlnSub.f, made some changes for c compatibility 
 * Modified: 5/12/2003  R Seres:  made some changes to avoid compiler errors, added strncmp_nocase,
 *																			IsANumber, switch_bytes_hp
 * Modified: 7/23/2003  R Seres:  changed parse arguments and parse_command since functions failed to
 *									properly accept token values
 * Modified: 9/22/2003  R Seres:  Added a carriage return to suppress hp compile warning
 * Modified: 12/02/2003 R Seres:  Removed the c_prompt() function. c_prompt_len() is safer and should be used instead
 * Modified: 3/15/2004  R Seres:  Fixed a bug with Ina() 
 * Modified: 9/17/2004  R Seres:  Fixed ParseArg to properly grab program arguments, c port was initially wrong
									    Changed c_prompt_len terminate when EOF was reached. This prevents hangups 
										when piping in user input
 * Modified: 9/28/2004  R Seres:  c_prompt_len now fails if it hits an EOF instead of a string (though it still returns true if EOF ends the string)
 * Last Modified : 1/2/05 RJS Added more YYYYMMDD and HHMMSS conversion functions 
 ****************************************************************************/
#include "pch.h"//NEW!!!
#include "stdafx.h"
 

#include "local.h"
 

#include "stdio.h"

#ifdef LOCAL

#ifndef HPUX
#ifndef LINUX
#define WINDOWS 1
#endif
#endif

#ifdef WINDOWS
#ifndef EDFTOB
 
#endif
#endif
 

#include "wpicc.h"
#else
#include <wpicc.h>
#endif
 
#include <string.h>
#include <malloc.h>
#include <time.h>
#include <math.h>
#include <stdlib.h> 
#include <signal.h>
 
#include <sys/types.h>
#include <sys/stat.h>
  
#include <ctype.h>
 
#include "generic.h"  
                   /* function prototyping */



#ifdef WINDOWS
 

    #include <conio.h>
    #define     MAX_FILE_NAME_LEN       12      /* max DOS file name length */

#else
    #define     MAX_FILE_NAME_LEN       64      /* max HPUX file name length */
#include <unistd.h>//added for unlink call 
#endif


#define MAX_FULL_SPEC_LEN  128  /* max combined len filspec on either platform
				   should be plenty unless VERY deeply nested
				   subdirs or subdirs with very long names. */
#define MAXLN 132		/* Max line length inside SCC.INI file */


//----------------------------------------------------------------------------------------------

// this function removes the path information so just the filename remains
//added august 14th 2002 by Rob Seres
void RemovePathFromFilename(char filename[])
{
	//first, get the last occurance of our path character
	char *start = strrchr(filename, '\\');//for windows
	int len;
	if(start)
	{
		start++;
	}
	else
	{
		start = strrchr(filename, '/');//for unix
		if(!start)
			return;//no path to remove
		start ++;
	}

	if(start)
	{
		//copy everything after the last path character to the same string,
		//and cap it off with the end of string character
		len = strlen(start);
		strncpy(filename, start, strlen(start));
		filename[len] = '\0';
	
	}
}



// this function gets the night number residing at the end of the filename
//added oct 11th 2002 -rjs
//////////////////////////////////////////////////////////////////////////////////////
int	GetNightFromFilename(char filename[])
{
	char *substring;
	substring = strchr(filename, '.');
	if(substring)
	{
		substring++;
		char *end = strchr(substring, '.');
		if(end)
			end[0] = '\0';
		return atoi(substring);
	}
	return 0;
	
}

//////////////////////////////////////////////////////////////////////////////////////
short echo_args( float *f0, float f2 )
{
 
  printf( "In ECHO_ARGS: *f0 = %f,  f2 = %f\n",*f0, f2);
  return( TRUE );
}
/*****************************************************************************/
/* EAT_KBD returns TRUE. Clears system keyboard buffer by eating any keystrk */
/*****************************************************************************/
short eat_kbd()
{
#ifdef WINDOWS 
 
 while (_kbhit())         /* C function tells if any char in sys kbd buffer   */
      _getch();           /* and then remove that char from sys kbd buffer    */
#endif

  return( TRUE );        /* ALWAYS returns TRUE whether key pressed or not   */
}

/****************************************************************************
 WAIT_KEY returns TRUE. Pauses until a key is hit
****************************************************************************/
void wait_key(void)
{
#ifdef WINDOWS
 while (!_kbhit()) {};    /* loop till key pressed */
 _getch();                /* then remove that char from sys kbd buffer */
#endif
  return;
}

/****************************************************************************/
/*      GCD returns the Greatest Common Divisor of two long variables.      */
/*              GCD = 0 if either value is zero.  EUCLID'S ALGORITHM        */
/****************************************************************************/
long gcd(long x, long y )
{
   long m, n, r;
/* ------------------------------------------------------------------------ */
	if (x == 0  ||  y == 0) return( 0 );            /* gcd of 0 is 0    */
	if ( (m=labs(x)) == (n=labs(y)))  return( m );    /* gcd of equals  */
	if ( m < n )
	   { n = m;
	     m = labs(y);
	   }
/*      n = min( labs(x), labs(y) );
	m = max( labs(x), labs(y) );              */
	while ( TRUE )
	   {
		r = m % n;                      /* r = remainder of m/n */
		if (r == 0)return ( n );
		m = n;
		n = r;
	   }
}

/*****************************************************************************/
/* LCM - returns the Least Common Divisor of two long values.                */
/* Last Modified 8-13-90 by Tim Hoffman: replaced all abs() with labs()      */
/*****************************************************************************/
long lcm(long x, long y )
{
	long    z;              /* temporary value      */
/* ------------------------------------------------------------------------- */
	if ((z=gcd(x,y)) == 0)  return( 0 );
	else    return( (x/z)*y );
}

/*****************************************************************************
 * ATOINT - Returns TRUE or FALSE and the converted numeric value.           *
 * Last Modified: 20-MAY-1990    Author: Timothy L. Hoffman                  *
 *****************************************************************************/
short atoint(char string[], short *number )
{
  long num;                     /* Converted value digit by digit            */
  short  digit;                 /* Converted numeric value of one char       */
  //-debug J Passios 11/13/2001 type cast
  unsigned short  len;          /* Length of the string to be converted      */
  short  sign;                  /* -1 if minus is the first char else 1.     */
  short  i;                     /* looping variable                          */
/* ------------------------------------------------------------------------- */
  if  ((len=strlen(string)) == 0)  return( FALSE );         /* NULL STRING ? */
  if  (strspn(string,"+-0123456789") < len) return(FALSE);  /* NON DIGIT ?   */
  if  ( string[0] == '-' ) 
     {
	i = 1;
	sign = -1;
     }
  else
     { 
	sign = 1;
	if ( string[0] == '+' )
	      i = 1;
	else
	      i = 0;
     }

/* NOW CONVERT DIGIT BY DIGIT, WATCH FOR OVERFLOW */
  num=0;
  for (;i<len;i++)
     {
       digit = string[i] - '0';                         /* convert digit  */
       if ((digit < 0) || (digit > 9)) return (FALSE);  /* non digit ?    */
       num = num*10 + digit;
       if (num > (long) 32768)  return( FALSE );        /* overflow ?     */
     }
  if((sign==1) && (num >((long)32767))) return(FALSE);  /* overflow ?     */
  *number = (short) num*sign;                           /* set sign       */
  return( TRUE );
}

/*****************************************************************************
 * C_PROMPT - Returns TRUE or FALSE. A char string is passed and displayed   *
 *      gets() is then called for a response from user                       *
 *      strspn() is called to determine whether response is all whitespace   *
 *      Whitespace is TAB, SPACE or NEWLINE.                                 *
 *      If the response contains all whitespace then c_prompt returns FALSE. * 
 *      If the response contains at least one non-white character TRUE.      *
 *      Date:20-MAY-1990      Author:Timothy L. Hoffman                      *
 *****************************************************************************/
//short c_prompt(char msg[], char response[] )
/* char *msg; */                /* reference. String to be displayed   */
/* char  response[]; */         /* reference. User response to msg     */
//{
 //printf( "%s", msg );
 //gets(response);
 //return( strspn( response, " \t\n" ) < strlen( response ) );
//}

/**************************************************************************
 C_PROMPT_LEN
*/
short c_prompt_len(char msg[], char response[], short response_dimension )
/* msg[]            -  String to be displayed
   response[]       -  Reference. User response to msg
*/
{
 short i;
 char ch;
 ch = 0;
 printf( "%s", msg );
 //keep looking for input until carriage return or end of file is reached.
 //end of file is checked for because user input is often piped in from
 //a unix cshell script -RJS
 for (i=0; (i<response_dimension-1) && (ch!='\n') && (ch!=EOF);i++)
   {
     ch = getchar();
     if (ch == '\n' || ch == EOF)//and a delimiter for EOF also - RJS
       response[i] = '\0';
     else
       response[i] = ch;

   }
 response[i] = '\0';
 while (ch!='\n' && ch != EOF)
    {
      ch=getchar();
    }

 i = strspn( response, "\0\t\n" ) < strlen( response );
 return( i!=0 );
}

//-----------------------------------------------------------------------------
short get_float( char prompt[], double fmin, double fmax, float *fval )
{
  char response[80];	// holds user responses to prompts
  double f; // -debug J. Passios 9/27/2001 type cast
  f = fmin-1.0;
 
  while (f<fmin || f>fmax)
    { 
     if (!c_prompt_len( prompt, response, 80)) return( FALSE );
     if (strspn( response, " \t.0" )==strlen(response))
	f = 0.0;			/* user typed 0.0 */
     else
	f=atof(response);	       /* else parse non zero value */
     if (f<fmin || f>fmax)	      /* invalid - try again */
      {
	printf("%Bad value: %f :  Range of possible values: %3.3f - %3.3f\n",7,f,fmin,fmax);
	printf("%c",7);
	f = fmin-1.0;
      }
    }
  if (*fval==f) return(FALSE); /* User entered identical value (NO CHANGE) */
 *fval = (float)f;
  return(TRUE);
}
//-----------------------------------------------------------------------------
//rjs added jan 24 since get float can't work for doubles on linux and vise-versa
short get_double( char prompt[], double fmin, double fmax, double *fval )
{
  char response[80];	// holds user responses to prompts
  double f; // -debug J. Passios 9/27/2001 type cast
  f = fmin-1.0;
 
  while (f<fmin || f>fmax)
    { 
     if (!c_prompt_len( prompt, response, 80)) return( FALSE );
     if (strspn( response, " \t.0" )==strlen(response))
	f = 0.0;			/* user typed 0.0 */
     else
	f=atof(response);	       /* else parse non zero value */
     if (f<fmin || f>fmax)	      /* invalid - try again */
      {
	printf("%Bad value: %f :  Range of possible values: %3.3f - %3.3f\n",7,f,fmin,fmax);
	printf("%c",7);
	f = fmin-1.0;
      }
    }
  if (*fval==f) return(FALSE); /* User entered identical value (NO CHANGE) */
 *fval = f;
  return(TRUE);
}

/*****************************************************************************
 SHORT GET_LONG       - Prompts user then reads a long value from kbd
 If user enters a new/different value TRUE is returned, else FALSE is returned
 Author T.Hoffman Nov 8 1993
*/
short get_long( char prompt[], long lmin, long lmax, long *lval )
{
  char response[81];
  long l;
  l = lmin-1;
  while (l<lmin || l>lmax)
    {
     if (!c_prompt_len( prompt, response, 80 )) return( FALSE);
     if (strspn( response, " \t0") == strlen(response)) /* user typed 0 */
	 l = 0L;
     else
       {
	 l=atol(response);
	 if (l==0) l = lmin-1;
       }
     if (l<lmin || l>lmax)
       {
	 printf("%c",7);
	 l = lmin-1;
       }
    }
  if (*lval==l) return(FALSE); /* User entered identical value (NO CHANGE) */
 *lval = l;
  return(TRUE);
}

/*****************************************************************************
 SHORT GET_SHORT - Prompts user then reads a short value from kbd
 If user enters a new/different value TRUE is returned, else FALSE is returned
 Author T.Hoffman Nov 8 1993
*/
short get_short( char prompt[], short smin, short smax, short *sval )
{
  char response[81];
  short s;
  s = smin-1;
  while (s<smin || s>smax)
    {
     if (!c_prompt_len( prompt, response, 80 )) return(FALSE);
     if (strspn( response, " \t0") == strlen(response)) /* user typed 0 */
	   s = 0;
     else
      {
	s=atoi(response);
	if (s==0) s = smin-1;
      }
     if (s<smin || s>smax)
      {
	printf("%c",7);
	s = smin-1;
      }
    }
  if (*sval==s) return(FALSE); /* User entered identical value (NO CHANGE) */
 *sval = s;
  return(TRUE);
}

/*****************************************************************************
 SHORT GET_UNSIGNED_SHORT - Prompts user then reads an unsigned 
                                     short value from kbd
 If user enters a new/different value TRUE is returned, else FALSE is returned
 Author R. Vasko Feb 22 1998
*/
short get_unsigned_short( char prompt[], unsigned short smin, unsigned short smax, unsigned short *sval )
{
  char response[81];
  unsigned short s;
  s = smin-1;
  while (s<smin || s>smax)
    {
     if (!c_prompt_len( prompt, response, 80 )) return(FALSE);
     if (strspn( response, " \t0") == strlen(response)) /* user typed 0 */
	   s = 0;
     else
      {
	s=atoi(response);
	if (s==0) s = smin-1;
      }
     if (s<smin || s>smax)
      {
	printf("%c",7);
	s = smin-1;
      }
    }
  if (*sval==s) return(FALSE); /* User entered identical value (NO CHANGE) */
 *sval = s;
  return(TRUE);
}

/*****************************************************************************
 SHORT GET_YYYYMMDD - Prompts user then reads a YYYYMMDD date from kbd
 If user enters a new/different value TRUE is returned, else FALSE is returned
 Author T.Hoffman Nov 8 1993
 Changed name to GET_YYYYMMDD for GET_DATE 1-13-94 T.Hoffman
*/
short get_yyyymmdd(char prompt[], long *dval )
{
  char response[81];
  long d;
  d = -1L;
  while (!ckdate(d))
    {
     if (!c_prompt_len( prompt, response, 80 )) return(FALSE);
     if (strspn( response, " \t0") == strlen(response)) /* user typed 0 */
	   d = -1L;
     else
      {
	d=atol(response);
	if (d==0) d=-1L;
      }
     if (!ckdate(d))
      {
	printf("%c",7);
	d = -1L;
      }
    }
  if (*dval==d) return(FALSE); /* User entered identical value (NO CHANGE) */
 *dval = d;
  return(TRUE);
}

/*****************************************************************************
 SHORT GET_HHMMSS - Prompts user then reads a HHMMSS time value from kbd
 If user enters a new/different value TRUE is returned, else FALSE is returned
 Author T.Hoffman Nov 8 1993
 Changed name from GET_TIME to GET_HHMMSS 1-13-94 T.Hoffman
*/
short get_time( char prompt[], long *tval )
{
  char response[81];
  long t;
  t = 9999L;
  while (!cktime(t))
    {
     if (!c_prompt_len( prompt, response, 80 )) return(FALSE);
     if (strspn( response, " \t0") == strlen(response)) /* user typed 0 */
	t = 0L;
     else
      {
	t=atol(response);
	if (t==0) t=9999L;
      }
     if (!ckhhmm(t))
      {
	printf("%c",7);
	t = 9999L;
      }
    }
  if (*tval==t) return(FALSE); /* User entered identical value (NO CHANGE) */
 *tval = t;
  return(TRUE);
}

/*****************************************************************************
 SHORT GET_HHMM - Prompts user then reads a HHMM time value from kbd
 If user enters a new/different value TRUE is returned, else FALSE is returned
 Author T.Hoffman Nov 8 1993
*/
short get_hhmm( char prompt[], long *tval )
{
  char response[81];
  long t;
  t = -9999L;  /* an INVALID value */
  while (!ckhhmm(t)) /* validate HHMM */
    {
     if (!c_prompt_len( prompt, response, 80 )) return(FALSE);
     if (strspn( response, " \t0") == strlen(response)) /* user typed 0 */
	t = 0L;
     else
      {
	t=atol(response);
	if (t==0) t=-9999L;  /* atol returning 0 means value was not integer */
      }
     if (!ckhhmm(t))  /* validate HHMMSS */
      {
	printf("%c",7);
	t = -9L;
      }
    }
  if (*tval==t) return(FALSE); /* User entered identical value (NO CHANGE) */
 *tval = t;  
  return(TRUE);
}

/**************************************************************************
 STRUP - Local version of strupr() since that is not ANSI defined.       
 ***************************************************************************/
  short strup(char string[])
  { 
    char *s;
    char diff;
    for(diff='A'-'a', s=string; *s != '\0'; *s++)
      if(*s >= 'a' && *s <= 'z') *s = *s + diff;
    return(TRUE);
  }

/***************************************************************************
 STRLO - Local version of strlwr() since that is not ANSI defined.      
 ***************************************************************************/
  short strlo(char string[])
  { 
    char *s;
    char diff;

    diff='a' - 'A';
    for(s=string; *s != '\0'; *s++)
      if(*s >= 'A' && *s <= 'Z') *s = *s + diff;
    return(TRUE);
  }

/***************************************************************************
 MEMBER : Returns TRUE if character c is a member of string set[]
	    set[] string is assumed to be null terminated.
 ***************************************************************************/
short member( char c, char set[] )
{
 char *ch;
 for ( ch=set; *ch != '\0'; ch++ )
    if (*ch == c) return(TRUE);
 return(FALSE);
}

/***************************************************************************
 * PARS_ARG - Parses command line for filename.                            *
 * arguments:                                                              *
 *        argc - number of command line args                               *
 *        argv - command line arguments                                    *
 *        snum - number of switches                                        *
 *    switches - switches for parse_command()                              *
 *        cval - current values of switches                                *
 *    defaults - default values of switches                                *
 *      bfilnm - binary file name                                          *
 *       vdesc - valid switch responses                                    *
 ***************************************************************************/
short pars_arg(short argc, char *argv[], short snum, char *switches[],
		short cval[], short defaults[], char bfilnm[], char *vdesc[])
{ short i;                                   /* for loop indexes             */
  char schar;
/* ------------------------------------------------------------------------- */
  strcpy(bfilnm,""); //debug jp NULL                          /* assume no filename       */
  #ifndef WINDOWS
    schar= '-';
  #else
    schar= '/';
  #endif
	if(argc==3) 
		strcpy(bfilnm,argv[2]);            /* filename & switches      */
	if(argc > 1)                                   /* get filename or switches */
	{ 
		if(argv[1][0] != schar)                    /* no switch only filename  */
			strcpy(bfilnm,argv[1]);                  /* save filename            */
		else                                       /* test & save switches     */
		{//parsing the command string in windows fails, since the command arguments
			//are not in continuous memory as they are in Unix
			//char *command;//create a temporary string that appends all the arguments into one string
			short size = 0;
			int index;
			for(index = 1; index < argc; index++)
				size += strlen(argv[index]);//find out are needed size
			if(size > 0)
			{
			//	command = new char[size + 1];//add one for the null delimiter
			//	strcpy(command, argv[1]);//copy over the first arguemnts
			//	for(index = 2; index < argc; index++)
			//	{//append the rest
			//		strcat(command, argv[index]);
			//	}
				//fixed how commands are parsed, was miss interpreted last time - RJS
				//arguments are clumped into one string rather than being spaced out
				if(!parse_command(argv[1], snum,switches,cval))//pass our new string into parse_command
					show_switch(TRUE,snum,switches,defaults,vdesc);
			//	delete [] command;
			}
		}
    }
	for(i=0; i<snum; i++)                          /* check switch values      */
		if(cval[i] < -1)
		{
			fprintf(stderr,"%% Invalid switch specified: %s  \n",argv[1]);
			show_switch(TRUE,snum,switches,defaults,vdesc);
		}
	return(TRUE);
}

/*****************************************************************************
 * PARSE_COMMAND - Returns TRUE or FALSE. Parses switches from command line  *
 *   and returns the values of those switches in an integer array.           *
 *   Parse_command will return FALSE for any of the following:               *
 * 1) Parsed keyword token (switch) does not match any of the entries in the *
 *    keywords table.                                                        *
 * 2) Parsed value token cannot be converted to a positive integer.          *
 * Parse_command generates ONE SIDE EFFECT when it returns FALSE:            *
 * 3) The values[] array may have been modified by valid switches and values *
 *    that were parsed and stored before the violation occured. Thus the     *
 *    default values passed in may be trashed upon return to caller.         *
 * ** THEREFORE: when parse_command returns FALSE, the caller should consider*
 *     values[] as being undefined and should therafter treat it WRITE ONLY  *
 *     until after a succesfull call to parse_command.                       *
 *                                                                           *
 * ALGORITHM:                                                                *
 *      if command_line arg passed in is null or of zero length return TRUE. *
 *      call malloc to allocate bytes                                        *
 *      set string to pt to starting address.                                *
 *      convert string to upper case.                                        *
 *      call strtok to set kp to point to first token in the string.         *
 *      WHILE kp points to a token                                           *
 *       {                                                                   *
 *         keyword_valid = FALSE    (* set TRUE if *kp matches a keyword *)  *
 *         for (i=0 ; i<NSWITCHES && !keyword_valid, i++)                    *
 *             IF *kp matches keywords[i] THEN                               *
 *                  keyword_valid = TRUE                                     *
 *                  set vp to same place in string as kp                     *
 *                  vp = ptr to next token, token is the value of the keyword*
 *                  IF the value token will convert to an integer THEN       *
 *                      store that integer in the value[i]                   *
 *                      IF value is negative                                 *
 *                         free up space alloced to string                   *
 *                         return FALSE                                      *
 *                      END IF                                               *
 *                  ELSE                                                     *
 *                      set tokens[i] to pt to same substring as vp          *
 *                  END                                                      *
 *                  break  ! quit looping and look for next keyword          *
 *             END IF                                                        *
 *         IF  keyword_valid is FALSE THEN                                   *
 *              return FALSE to caller (* bad keyword is fatal *)            *
 *         call strtok to set kp to point to next token in the string        *
 *       } END WHILE kp still found another token                            *
 *      return TRUE                                                          *
 *      Date:20-MAY-1990   Author:Timothy L. Hoffman                         *
 * arguments:
 *   command_line - String to be parsed. format: /<KEYWORD>=<VALUE>       *
 *      NSWITCHES - dimension of arrays passed in                         *
 *       keywords - list of keywords to be matched                        *
 *         values - values following each keyword                         *
 ***************************************************************************/
short parse_command(char command_line[], short NSWITCHES, char *keywords[],
							     short values[])
{
  short    keyword_valid;               /* set TRUE when keyword match       */
  char    *kp;                          /* keyword pointer                   */
  char    *vp;                          /* value pointer                     */ 
  static   char *string;                /* command line in uppercase.        */
  short    i;                           /* looping variable                  */
  char     stokn[3];                    /* switch token value "-" or "/"     */
/* ------------------------------------------------------------------------- */
  stokn[0] = '=';                       /* initialize switch token */
  #ifndef WINDOWS
	stokn[1] = '-';
  #else
	stokn[1] = '/';
  #endif
  stokn[2] = '\0';
 if ((command_line == NULL) || (strlen(command_line) <= 0)) return(TRUE);
 if (( string = (char *) malloc(strlen(command_line)*2)) == NULL)
   { printf("\nUnable to to parse: malloc problem! Contact programmer.\n\n");
     exit(0);
   }
else
   { strcpy( string, command_line );         /* local copy                   */
     strup( string );                       /* convert to upper case        */
   }

   if((string[1]=='H') || (string[1]=='?'))   /* help requested */
       { free(string);
	 return(FALSE);
       }
   kp = strtok( string, stokn);             /* point to first token      */
   while (kp != NULL )
   {
     keyword_valid = FALSE;                 /* FALSE til keyword entry match */
     for (i=0; (i<NSWITCHES) && (!keyword_valid); i++)  /* til keyword match */
	if (strcmp( keywords[i], kp) == 0 )         /* 0 means match occured */
	  {
	    keyword_valid = TRUE;         /* match found, token is a keyword */
	    vp = kp;                      /* vp looks for the next token     */
	    vp = strtok( NULL,stokn);     /* next token is switch value */
	    if (!atoint(vp,&values[i])  ||  values[i] < 0 )
		{
		   values[i] = -1;        /* -1 signals no token found       */
		   printf("\n%%Unacceptable %s value.\n",keywords[i]);
		   free(string);
		   return(FALSE);
		}
	  }  /* ------------------------KEYWORD MATCH --------------------- */

     if (!keyword_valid)                /* FAIL - all keywords must be valid */
	{
	  printf("\n%%Unacceptable switch: %s\n",kp);
	  free( string );               /* free allocated space              */
	  return( FALSE) ;              /* only vals can be arbitrary tokens */
	}

     kp = strtok( NULL,stokn);   /* set kp to point to next token          */
   }   /* ----------------------- WHILE kp -------------------------------- */
   free(string);
   return( TRUE );  
}


/*****************************************************************************
 * SHOW_SWITCH - Displays switches and defaults values.                      *
 * Last Modified: 8/13/1990  Tim Hoffman - Recognizes /h, /H, or /? as       *
 *                                          valid help switches.             *
 * arguments:                                                              *
 *        help - TRUE/FALSE                                                *
 *        snum - number of switches                                        *
 *    switches - switches for parse_command()                              *
 *        sval - current/default switch value                              *
 *       vdesc - valid switch responses                                    *
 ***************************************************************************/
short show_switch(short help, short snum, char *switches[],
					       short sval[], char *vdesc[])
  {
  short   i;                                 /* for loop indexes             */
/* ------------------------------------------------------------------------- */
  if (help)
    {
      printf(" %-15s %13s %15s \n","SWITCH","DEFAULT VALUE","VALID VALUES");
      printf(" %-15s %13s %15s \n","------","-------------","------------");
    }
  else
    {
      printf(" %-15s %13s %15s \n","SWITCH","VALUE","VALID VALUES");
      printf(" %-15s %13s %15s \n","------","-----","------------");
    }

  for(i=0; i<snum; i++)
  printf(" %-25s %3d    %-20s \n",switches[i],sval[i],vdesc[i]);

  if (help) exit(0);

  show_time((char*)"\n Current Date & Time");//rjs type cast to surpress warning
  printf("If Switch values and Current Time OK press RETURN to continue. \n");
  printf("If INCORRECT, press ^C to abort then restart system clock. \n");
  printf("\n");
  while( getchar() != '\n') {}                   /* wait for RETURN or ^C */
  return(TRUE);
}


/***************************************************************************
 * CKLOG() - Validates and sets logical switch value.                      *
 * arguments:                                                              *
 *        lval - switch value to be tested                                 *
 *        snum - number of switches                                        *
 *    switches - switches for parse_command()                              *
 *    defaults - default values of switches                                *
 *       vdesc - valid switch responses                                    *
 ***************************************************************************/
 short cklog(short lval, short snum, char *switches[],
				       short defaults[], char *vdesc[])
{
short rval;                                  /* return value  TRUE or FALSE  */
/* ------------------------------------------------------------------------- */
 if(lval < 0   ||  lval > 1)
   { fprintf(stderr,"%% Invalid switch value specified. \n");
     show_switch(TRUE,snum,switches,defaults,vdesc);
   }
 else (rval = lval);                         /* set  TRUE or FALSE           */
return(rval);
}

/*****************************************************************************/
/*  INTEGER FUNCTION S_TO_HHMMSS: Takes a long # of second, and returns the*/
/*  equivalent in hrs, minutes, and seconds via division and mod.          */
/*  Author: Tim Hoffman                                                    */
/*  Last Modified: 12/07/90                                                */
/*****************************************************************************/
short s_to_hhmmss( long s, long *hh, long *mm, long *ss )
{
   *hh=0;*mm=0;*ss=0;
   if (s <= 0) return( TRUE );
  *hh = s / (long) 3600;
   s  = s % (long) 3600;
  *mm = s / (long) 60;
  *ss = s % (long) 60;
   return( TRUE );
}


/*****************************************************************************/
/*  INTEGER FUNCTION GET_FILE_DATE(): Returns a character array which  */
/*  is the creation date & time of filespec passed in                      */
/*****************************************************************************/
short get_file_date(char filespec[], char date_string[] )
{
  struct stat buf;
  short result;
  result = stat( filespec, &buf );
  if (result != 0)
    {
      strcpy( date_string, " ");
      return(FALSE);
    }
  else
    {
      sprintf( date_string, ctime(&buf.st_atime) );
      return( TRUE );
    }
}


/* *********************************************************************** */
/* CKFLEN - Check length of filename. IF HPUX not defined lenth <=8        */
/* Modified: 7/17/1992  D. Kesicki                                         */
/* *********************************************************************** */
short ckflen(char fname[])
{ short fmax;           /* maximum file name length without extension */
  short loc;
  char  tname[60];
  char *ploc;
/* ----------------------------------------------------------------------- */
   fmax = MAX_FILE_NAME_LEN-4;
   loc = 0;
   ploc = strrchr(fname,':');           /* scan name for last occurance : */
   ploc = strrchr(fname,'\\');          /* scan name for last occurance \ */
   if(ploc != NULL)
      loc = ploc - fname +1;            /* first char past \ and :        */
   strcpy(tname,fname+loc);             /* remove till past \ or :        */
   ploc = strrchr(tname,'.');           /* scan name for file extension   */
   loc = 0;
   if(ploc != NULL)
      loc = ploc - tname;               /* size of filename               */
   if(loc > fmax) return(FALSE);        /* filename exceeds max length    */

   return(TRUE);
}


/* *********************************************************************** */
/* GETDSTR - Returns directories prefacing filename                        */
/* Modified: 7/20/1992  D. Kesicki                                         */
/* *********************************************************************** */
short getdstr(char fname[], char dstr[]) 
{
  size_t loc;
  char *ploc;
 
/* ----------------------------------------------------------------------- */
   loc = 0;
   strcpy(dstr,"");
   ploc = strrchr(fname,':');           /* scan name for last occurance : */
#ifndef WINDOWS
   ploc = strrchr(fname,'/');          /* scan name for last occurance \ */
#else
   ploc = strrchr(fname,'\\');			// 07-03-2000 cpratt corrected typo
#endif
	if(ploc == NULL)					// 05-23-2002 rseres fixed crashed under win32 system
		return FALSE;
	loc = strlen(fname) - (strlen(ploc));

   
//   printf("\nloc = %ld\n",loc);

   if(loc == 0L)
    { 
      return(FALSE);
    }
   strncpy(dstr,fname,loc);
   dstr[loc] = '\0';
   return(TRUE);
}



/* *********************************************************************** 
 OPEN_FILE_LIST - Open an existing file, searching a list of directories.
 Modified 11-1-93 T.Hoffman: Now compares only the length of filename
 (not full_name) against MAX_FILE_NAME_LENGTH.  Also changed dimension of
 full_name[] to be large enough to accomodate the full path string length
 on either platform
 *********************************************************************** */
FILE *open_file_list(char filename[], short num_directories,
				char *directory_list[], char arguments[])
 { char full_name[MAX_FULL_SPEC_LEN+1];  /* device+dir+name+ext. total len */
   FILE *ftmp;          /* temporary file pointer */
   short jj;            /* do loop index */
   short kk;            /* length of each directory name */
   short ii;            /* length of filename */
 /* ----------------------------------------------------------------------- */
	ii = strlen(filename);
	if (num_directories <= 0)       return(NULL);
	if (ii > MAX_FILE_NAME_LEN)     return(NULL);
	for (jj=0; jj<num_directories;  jj++)
	    {   kk=strlen(directory_list[jj]);
		    {   if (kk > 0)
			    {   strcpy(full_name,directory_list[jj]);
				full_name[kk]='\0';
			    }
			else
			    {	full_name[0]='\0';
			    }
			if (ii > 0) strcat(full_name,filename);
			ftmp = fopen(full_name, arguments);
			if(ftmp)
			//if ((ftmp=fopen(full_name,arguments)) != '\0')//RJS change 3/10/2023
								return(ftmp);
		     }
	    }
	return(NULL);
 }

/*-------------------------------------------------------------------------*/
short nowhite( char *char_string )
{
	char *p1;
	char *p2;
	unsigned short j; //-debug J.Passios 9/27/2001 type cast
	for (j=0, p1=p2=char_string; j<strlen(char_string); j++, p1++)
		if (*p1 > ' ') *p2++ = *p1;
	*p2 = '\0';
	return(TRUE);
}

/*-------------------------------------------------------------------------
 VOID TYPECAST: Takes an array of TSD data from a TSD file. Does inline
 conversion from whatever type it is, to double float.
 Author Timothy L. Hoffman  Jul-Aug 1994
 Although the dat[] arg is typed as as double, the array passed in may
 be of any type but the caller will typecast it as double to get it passed
 in. This function will then look at the original type (siz) and do inline
 conversion of that original type into double floats

 ARGS:  double dat[]  // comes in initialized from TSD file
 long   count	// how many vals in array (not necc dimension or bytes)
 short  siz	// how  many bytes per data item. i.e type or sizeof
 ---------------------------------------------------------------------------*/

void typecast( double dat[], long cnt, short siz )
{ 
  long    i;
  char   *cptr;
  short  *iptr;  
  float  *rptr;
  double *dptr;

  dptr = (double *) dat;
  rptr = (float *)  dat;
  iptr = (short *)  dat;
  cptr = (char *)   dat;

  dptr += cnt-1;
  iptr += cnt-1;
  cptr += cnt-1;
  rptr += cnt-1;

  if (siz==1)        /* convert bytes to doubles */
    {
	for (i=cnt;i>0;i--)       /* pts to last byte */
	   {
		*dptr = (double)*cptr;
		 dptr--;
		 cptr--;
	   }
    }
  else if (siz==2)   /* convert short int to doubles */
    {
        for (i=cnt;i>0;i--)
	   {     
 		*dptr =  (double)*iptr;
		 dptr--;
		 iptr--;
	   }
    }
  else if (siz==4)   /* convert float to double */
    {
	for (i=cnt;i>0;i--)
	   {
		*dptr = (double)*rptr;
		 dptr--;
		 rptr--;
	   }
    }
  else               /* unknown data type, abort with error msg */
    {
	printf("\n_TYPECAST: Unknow data siz: %d\n",siz);
        exit(0);
    }

  return;
}

/*-------------------------------------------------------------------------
  lmin and lmax return the min and max respectively of 2 long ints.
*/

long lmin( long a, long b )
{
  if (a<b) return (a);
  else return(b);
}

long lmax( long a, long b)
{
 if (a>b) return(a);
 else return(b);
}

//--------------------------------------------------------------------------
short SeekToSectionLn( FILE *(*fptr),const char *token, char line[] )
{
  char *s;	
	do
	{ 
		s = fgets(line,MAXLN-2,*fptr);
		if (s  == NULL)
			return( FALSE );
	}
	while  ( strstr( line, token ) != line );
	return( TRUE );
}

//--------------------------------------------------------------------------
short SeekToKeyLn( FILE *(*fptr),const char *token, char line[] )
{
  char *s;		// return value from fgets()
  char *eqptr;		// ptr to '=' in line[]
  char *eqstr = (char*)"="; 	// string containing target char '='
  short eqind;		// size of initial substring of line upto the '=' sign 
  char tmpln[MAXLN];	// key part of line[]
  short i;

	do
	{ 
		s = fgets(line,MAXLN-2,*fptr);
		if (s  == NULL)			// no more lines in .ini file
			return( FALSE );
		eqptr = strstr(line,eqstr);
		if (eqptr == NULL)		// no more lines with '=' signs
		  return(FALSE);
		eqind = eqptr-line;
		for (i=0;i<MAXLN-2;i++) tmpln[i]= '\0';
		strncpy(tmpln,line,eqind);	// copy from start of line up to the equal sign
//		printf("Comparing key: %s to token: %s\n",tmpln,token);
	}
	while  ( strcmp( tmpln, token ) );	// compare key to this key
	return( TRUE );
}


//----------------------------------------------------------------------------------
#ifndef WINDOWS
unsigned short GetPrivateProfileString( const char* lpAppName, const char* lpKeyName, 
		                        const char* lpDefaultString, char lpReturnedString[],
			                short nSize, const char* lpFileName )
{
	FILE *fptr;
	char line[MAXLN];
	char *cptr; 

	strcpy( lpReturnedString, lpDefaultString );

	if ( (fptr=fopen(lpFileName,"rt")) == NULL)
	  {
//		fprintf(stderr,"Fatal Error attempting open file: %s\n",lpFileName);
		fclose( fptr );
		return (strlen(lpReturnedString));
	  }
	if (SeekToSectionLn(&fptr,lpAppName,line) == FALSE)
	  {
//		fprintf(stderr,"No section: %s in file: %s\n",lpAppName,lpFileName);
                fclose( fptr );
		return (strlen(lpReturnedString));
	  }
	if (SeekToKeyLn(&fptr,lpKeyName,line) == FALSE)
	  {
//		fprintf(stderr,"No key: %s in file: %s\n",lpKeyName,lpFileName);
                fclose( fptr );
		return (strlen(lpReturnedString));
	  }
	else  // return string on right side of '=' sign
	  {
		if (cptr=strchr( line, '=' ))
			strcpy( lpReturnedString, ++cptr );
                fclose( fptr );
		return ( strlen(lpReturnedString) );
	  }
}

//----------------------------------------------------------------------------------
unsigned short GetPrivateProfileInt( const char* lpAppName, const char* lpKeyName,
				     unsigned short nDefault, const char* lpFileName)
{
	FILE *fptr;
	char line[MAXLN];
	char *cptr; 
	char IntString[100];
	unsigned short i; //-debug JP 9/27/2001 changed initialization of variable
	unsigned short ushort; 

	if ( (fptr=fopen(lpFileName,"rt")) == NULL)
	  {
		fprintf(stderr,"Fatal Error attempting open file: %s\n",lpFileName);
	//	fclose( fptr ); //rjs can't close a null fiel pointer
		return ( nDefault );
	  }
	if ( SeekToSectionLn(&fptr,lpAppName,line) == FALSE )
	  {
		fprintf(stderr,"No section: %s in file: %s\n",lpAppName,lpFileName);
                fclose( fptr );
		return ( nDefault );
	  }
	if (SeekToKeyLn(&fptr,lpKeyName,line) == FALSE)
	  {
		fprintf(stderr,"No key: %s in file: %s\n",lpKeyName,lpFileName);
                fclose( fptr );
		return ( nDefault );
	  }
	else  // convert value on rt side of '=' to an int 
	  {
		if (cptr=strchr( line, '=' ))
			strcpy( IntString, ++cptr );
		i = strspn( IntString, " \t0123456789");  // index first char not in set
		if (i != strlen(IntString)-1)
		  {
		    fprintf(stderr, "Non digit at position %d in string[%s]\n",i);
		    fprintf(stderr, "i = %d, strlen = %d\n",i,strlen(IntString)	);
                    fclose( fptr );
		    return( nDefault );
		  }
		ushort = atoi( IntString );
		if (ushort == 0L)
		  {
		    i = strspn( IntString, " 0");
		    if (i != strlen(IntString))
		     {
			fprintf( stderr, "Bad Integer String: %s returns 0\n",IntString);
                        fclose( fptr );
			return( nDefault );
		     }
		  }	
		return( ushort );
	  } // END ELSE CONVERT
}

#endif

/*---------------------------------------------------------------------------
  PARSE_ENVAR : Parses and returns an environment variable found as initial substring of a string

  AUTHOR: Timothy L. Hoffman Jul-26-1994

  ALGORITHM:
         Look for '$' as first char in incoming string. Look for first '/' 
	 char and return all char between but not including '$' and '/'. If 
	 there are not chars between or if either of '$' or '/' is missing, 
	 set return string null

  INPUT ARGs :  Must be initialized before passed in.  Arg is NOT MODIFIED
  ----------
  ARG2 : filespec  -  Usually as typed in by user: Consists of 
                      [$<envar>/][<subdir path>/]<filename>
  

  OUTPUT ARGs : Initial value ignored and modified
  -----------
  ARG1 : envar     -  If no logical is parsed just set this string to null
                   -  Else if logical parsed extract and copy into envar for 
                      return

  ARG3: pos        -  If no logical found, pos is 0, else pos is index of 
                      first '/' (end of logical)

  
  FUNCTION RETURN VALUE : short int
  ---------------------
     TRUE   = no envar parsed at beginning of ARG1. Copy NULL into envar
     FALSE  = envar parsed and copied into envar for return. Don't copy '$' 
              or '/' into envar

 --------------------------------------------------------------------------*/

short parse_envar( char envar[], char filespec[], short *pos )
/* filespec[];   input arg.    if filespec is '$TSD/044123HSC.002'         */
/* envar[];      return arg.   then envar is  'TSD'                        */
/* *pos;         index in filespec of first '/'  (end of logical)          */
{
 strcpy( envar," " );
 *pos=strcspn(filespec,"/\0");
 if (unsigned (*pos) >= strlen(filespec)) *pos = 0; //-debug J.Passios 9/27/2001 type cast
 if ( (filespec[0] != '$') || (*pos<2) )
        return(FALSE);   
 strncpy(envar,filespec+1,*pos-1);
 envar[*pos-1]='\0';
 return(TRUE);
}



/*----------------------------------------------------------------------------
  DEREF_FILESPEC : Dereferences (all the way to the bottom) a logical file 
                   specification (arg 1) and stores it's explicit full path/
                   filename in 2'nd arg for return to caller

  AUTHOR: Timothy L. Hoffman  Jul-22-1994

  ALGORITHM: 
         Parse logical (environment variable) if present from front of filespec
	 Defererence logical into it's environment string (explicit dir path)
	 concatentate remaining filespec onto dereferenced logical path to 
         create a full pathname
	*See RETURN VALUE for exceptions

  INPUT ARGs :  Must be initialized before passed in.  Arg is NOT MODIFIED
  ----------
  ARG1 : filespec  -  Usually as typed in by user: Must be of following form:
                      [$<envar>/][<subdir path>/]<filename>
  

  OUTPUT ARGs : Initial value ignored and modified
  -----------
  ARG2 : filenm    -  If no logical is parsed just copy filespec as-is into 
                      filenm
                   -  Else if logical parsed but undefined set filenm to 
		      errmsg undef logical
		   -  Else if logical parsed and dereferenced set filenm to 
		      be dereferenced
		      environment string with remainder of filespec concated

  
  FUNCTION RETURN VALUE : short int
  ---------------------
        0  = no envar parsed at beginning of ARG1. Copy filespec as-is, into 
             filenm before return
	1  = envar parsed and sucessfully dereferenced. Return full expanded 
             path. in fullpath
       -1  = envar parsed but no corresponding environment string found by 
             getenv, thus set filenm to be a msg about undefined logical
  ---------------------------------------------------------------------------*/

short deref_filespec( char filespec[], char filenm[] )
{
   char  envar[80];      /* optional initial substr of between '$' and '/' */
   char *envstr;         /* envir string returned by HP Op Sys (if defined) */
   short pos;            /* index of first '/' */

   strcpy(filenm," ");

/* IF LOGICAL PRESENT IN INITIAL SUBSTRING OF FILESPEC, EXTRACT and DEREF IT */

   if (!parse_envar(envar,filespec,&pos)) /* no logical found */
     {
       strcpy( filenm,filespec);  /* just copy filespec as-is into filenm */
       return(0);                 /* 0 = tells caller no logical was found */
     }
     
   envstr = getenv(envar);      /* retrieves unix environment string */
 
   if (envstr==NULL)            /* environment variable is undefined */
     {
       sprintf(filenm," environment variable '$%s' is undefined.",envar);    
       return(-1);              /* -1 = logical unbound i.e ERROR */
     }
   else                         /* logical bound, replace with env string */
     {
       sprintf( filenm, "%s%s", envstr, filespec+pos);
       return(1); 
     }
}




/****************************************************************************
  INTEGER FUNCTION FILE_EXISTS(): Takes a filespec as arg. Returns TRUE if
  the file exists, else returns FALSE
  Modified: 11/25/98 J.Doman remove any logicals (call deref_filespec) first.
 ****************************************************************************/
short file_exists(char filespec[])
{
  struct stat buf;
  short result;
  char full_file_name[128];
  full_file_name[0] = '\0';

 // if (deref_filespec(filespec,full_file_name) == -1)	return(FALSE);
  result = stat(filespec, &buf);
  if (result != 0)
      return(FALSE);
  else
      return( TRUE);
}



/* *********************************************************************** */
/* TESTFILE - Check disk for existing file. Prompts user to overwrite if   */
/*            needed. Return FALSE if no overwrite or TEMP.DAT file exists */
/* Modified 1-13-94 T.Hoffman - Replaced get_file_date call with file_exists */
/* *********************************************************************** */

short testfile(char filename[])
{ 
	char response[80];
/* ----------------------------------------------------------------------- */
	if(file_exists(filename))
    { 
		printf("\n File already exists: %s\n",filename);
		c_prompt_len((char*)"Replace? [N] ",response, 80);
		if(response[0] != 'Y' && response[0] != 'y') 
			return(FALSE);
			_unlink(filename);                            /* delete existing file */
    }
	return(TRUE);
}

/////////////////////////////////////////////////////////////////////////////////////////
//this function asks the user for a deltafile name
// hitting the cancel sequence halts the program's execution
bool PromptForFileName(char filename[])
{
#ifndef WINDOWS
	printf("\nPlease enter a filename or ^d to quit :");
#else
	printf("\nPlease enter a filename or 'q' to quit :");
#endif
	
	scanf("%s", filename);
 
	if(filename[0] == 'q')
		return false;
	if(filename[0] == ' ')
		return false;
	return true;
}
 
bool FileExists( const char* filename )
{
	FILE *fptr;
	fptr = fopen(filename, "r");
	if (fptr){
		fclose(fptr);
		return true;
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////////////////
//this function either gets the filename from the program arguments, or prompts the
//user for the filename. It also expands unix shell variables
bool GetFileName(char FileName[], short argc, char *argv[])	
{
	if(argc == 2)
	{
		//our first and only argument was the source file
		strcpy(FileName, argv[1]);
	} 
	else
	{
		bool nofile = true;

		while(nofile)
		{
			FileName[0] = ' ';
			if(PromptForFileName(FileName))
			{
#ifndef WINDOWS
				//copy temp file spec to parse any unix variables
				char filespec[MAX_FILE_NAME_LEN];
				strcpy(filespec,FileName );
				deref_filespec(filespec, FileName);//expand any shell variables

				if(file_exists(FileName))
#else
				if(FileExists(FileName))
#endif

				nofile = false;
			}
			else
			{
				return false;
			}
			if(nofile)
			{
				printf("Can't open file %s", FileName);
			}
		}
	}
	return true;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//rjs added nov 26 2002
//handy function to grab an integer value from a portion of a string.
//both start and end indecies are included in the substring
int	GetIntFromBuffer(char *buffer, int start, int end)
{
	char *ptr = &buffer[start];
	char *temp = new char[end - start + 2];
	strncpy(temp, ptr, end - start + 1);
	temp[end - start + 1] = '\0';
	int i =  atoi(temp);
	delete [] temp;
	return i;

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//rjs added nov 26 2002
//grabs the file extension,if it exists
char*	getExtention(char *string)
{
	char *exts = strrchr(string, '.');
	if(exts == NULL)
		return NULL;//string has no extention
	return exts;

}


 //returns the index of where the first string exists in the second string
//NOTE: In the fortran version, since the index started at 1, then failure would be
//a result of 0. Since 0 can be a valid location, we return -1 for failure
//we can only choose to look in the substring of the string we're looking in, which start
//and end give us starting an indexes into the string we're looking in, end defaults to 
//-1. This strange feature is due because the orignal fortran program used a lot of 
//substring expressions that c++ does not have. 
int InA(char string_to_find[], char string[], int maximum, int start, int end)
{
	int size = strlen(string_to_find);//get the limits of the strings
	int limit = strlen(string);
	char *compstring;//the string we're going to look for, which we may only look 
	//for part of it
	 
	if(end != -1)//if end != -1 (our default value) then we only want to search a substring 
	{
		compstring = new char[end - start + 2];
		//adjust the start of the string pointer
		string_to_find += start * sizeof(char);
		//copy it into our sub string
		strncpy(compstring, string_to_find, end - start + 1);
		//cap the end with a null character
		compstring[end - start + 1] = '\0';
		size = strlen(compstring);//get the limits of the compstring, since it will not be the entire string
	}
	else
	{
		//adjust the start of the string pointer
		string_to_find += start * sizeof(char);
		compstring = new char[size + 1];
		strcpy(compstring, string_to_find);
		compstring[size] = '\0';
	}
	char *ptr = string;  // pointer to the string we're looking in

	for(int x = 0; x < limit, x < maximum; x++)
	{
		if(strncmp(ptr, compstring, size) == 0)
		{
			delete [] compstring;
			return x;
		}
		ptr += sizeof(char);
	}

	delete [] compstring;
	return -1;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
void GetStringFromBuffer(char *buffer, char *result, int start, int end)
{
	char *ptr = &buffer[start];
	strncpy(result, ptr, end - start + 1);
	result[end - start + 1] = 0;
}
 
//both the start and end points are included
//////////////////////////////////////////////////////////////////////////////////////////////////////////
float GetFloatFromBuffer(char *buffer, int start, int end)
{
	char *ptr = &buffer[start];
	char *temp = new char[end - start + 2];

	strncpy(temp, ptr, end - start + 1);
	temp[end - start + 1] = '\0';
	float v = (float)atof(temp);
	delete [] temp;
	return v;

}

///////////////////////////////////////////////////////////////////////////// 
/* *************************************************************************
**	ISNUM -  FUNCTION THAT RETURNS TRUE IF AN ASCII NUMBER R1<=CHR<=R2
** *************************************************************************/
bool IsNumberBetween(char check, char first, char second)
{
	return check >= first && check <= second;       
}

////////////////////////////////////////////////////////////////////////////////////////////////////////// 
//return true if the string has something other than whitespace
bool NotEmpty(char *string) 
{
	for(int x = 0; x < 8, string[x] != '\0'; x++)
	{
		if(string[x] != ' ' && string[x] != 0 && string[x] != 10 && string[x] != 13)
			return true;
	}
	return false;
}
 

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
 
char* HHMMSS_ToString(char string[], long HHMMSS)
{
	sprintf(string, "%d:%d:%d", HHMMSS/10000, (HHMMSS/100) % 100, HHMMSS % 100);
	return string;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void DateTimeToString(unsigned char string[], long YYYYMMDD, long HHMMSS)
{ 
	sprintf((char*)string, "%d/%d/%d %d:%02d:%02d", (YYYYMMDD/100) % 100, YYYYMMDD % 100, YYYYMMDD/ 10000, (HHMMSS/10000), (HHMMSS/100) % 100, HHMMSS % 100);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
short StringToYYYYMMDDAndHHMMSS(char datetime[], long *YYYYMMDD, long *HHMMSS)
{
	short count = 0;
	short length = strlen(datetime);
	if(length < 1)
		return 0;
	*YYYYMMDD = StringDateToYYYYMMDD(datetime);
	char *ptr = &datetime[0];
	while(ptr[0] == ' ' && count < length)
	{//get rid of any leading spaces
		count++;	
		ptr++;
	}
	while(ptr[0] != ' ' && count < length)
	{//get rid of any leading spaces
		count++;	
		ptr++;
	}
	*HHMMSS = StringTimeToHHMMSS(ptr);
	if(*HHMMSS && *YYYYMMDD)
		return 1;	
	return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
long StringDateToYYYYMMDD(char date[])
{
	long YYYYMMDD;
	int limit = strlen(date);
	char substring[32];
	int index = 0;
	int sub_index = 0;

	while(date[index] == ' ' && index < limit)
	{//get rid of any leading spaces
		index++;	
	}
	//handle both '.', '-' or '/' as a divider for date
	while(date[index] != '/' && date[index] != '-' && date[index] != '.' && index < limit)
	{
		substring[sub_index] = date[index];
		index++;
		sub_index++;
	}
	//get year
	substring[sub_index] = '\0';
	short year = atoi(substring);
	YYYYMMDD = year * 10000;
	if( year < 72)//if date is not y2k complient
		YYYYMMDD += 20000000;
	else if (year < 1900)
		YYYYMMDD += 19000000;
	sub_index = 0;
	index++;
	while(date[index] != '/' && date[index] != '-' && date[index] != '.' && index < limit)
	{
		substring[sub_index] = date[index];
		index++;
		sub_index++;
	}
	//get month
	substring[sub_index] = '\0';
	YYYYMMDD += atoi(substring) * 100;

	sub_index = 0;
	index++;
	while(index < limit)
	{
		substring[sub_index] = date[index];
		index++;
		sub_index++;
	}
	substring[sub_index] = '\0';
	//get day
	YYYYMMDD += atoi(substring);;
	
	return YYYYMMDD;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
long StringTimeToHHMMSS(char time[])
{
	long HHMMSS;
	int limit = strlen(time);
	char substring[32];
	int index = 0;
	int sub_index = 0;

	while(time[index] == ' ' && index < limit)
	{//get rid of any leading spaces
		index++;	
	}
	//look for next ':'
	while(time[index] != ':' && index < limit)
	{
		substring[sub_index++] = time[index++];	
	}
	substring[sub_index] = '\0';
	sub_index = 0;
	index++;
	HHMMSS = atoi(substring) * 10000;

	while(time[index] != ':' && index < limit)
	{
		substring[sub_index++] = time[index++];	
	}
	
	substring[sub_index] = '\0';
	sub_index = 0;
	index++;
	HHMMSS += atoi(substring) * 100;
	//get seconds, stop at first space, end of string, or AM/PM designation
	while(time[index] != 'A' && time[index] != 'a' && time[index] != 'P' && time[index] != 'p' && time[index] != ' ' && index < limit)
	{
		substring[sub_index++] = time[index++];	
	}
	substring[sub_index] = '\0';
	sub_index = 0;
	HHMMSS += atoi(substring);

	while(time[index] != 'A' && time[index] != 'a' && time[index] != 'P' && time[index] != 'p' && index < limit)
	{
		 index++;
	}
	if(index >= limit)
		return HHMMSS;
	//if this is PM, convert to military time
	if(time[index] == 'P' || time[index] == 'p')
	{
		HHMMSS += 120000;
	}
	return HHMMSS;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
short strcmp_nocase(char *string1, char *string2)
{
#ifdef HPUX
	return strcasecmp(string1, string2);
#else
#ifdef LINUX
	return strcasecmp(string1, string2);
#else
	return _stricmp(string1, string2);
#endif
#endif
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
short strncmp_nocase(char *string1, char *string2, short length)
{
	char c1, c2;
	for(int index = 0; index < length; index++)
	{
		c1 =  tolower(string1[index]);
		c2 =  tolower(string2[index]);
		if(c1 < c2)
			return -1;
		else
			if(c1 > c2)
				return 1;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//returns true if string is a number
bool IsANumber(char string[], char size)
{
	if(size == 0)
		size = strlen(string);
	char dec = 0;//number o fdecimal points
	for(int x = 0; x < size; x++)
	{
		if(string[x] == '.')
		{
			dec++;
			if(dec > 1)//more than decimal point. invalid number
				return false;
		}
		else
			if(string[x] <  '0' || string[x] > '9')
				return false;//not a digit. return false
	}
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
//rjs moved here to allow break up of other libraries
short switch_bytes_hp(unsigned char *baddr, size_t size )
{
	unsigned char holdv;               /* holds value of first byte */
	unsigned char *holdp;              /* holds position of second byte */
	short         counter;             /* counts byte pairs to swap     */

	for ( counter=0, holdp=baddr+size-1;  counter < size/2.0;  counter++) // debug
	{
		holdv = *baddr;
		*baddr++ = *holdp;
		*holdp-- = holdv;
	}
	return ( TRUE );
}
///////////////////////////////////////
bool ReadLine(FILE *fptr, char text[])
{
	text[0] = '\0';
	if(!fptr)
		return false;
	if(feof(fptr))
		return false;
	int index=0;
	fscanf(fptr, "%c", &text[index]);//keep reading until we get to the end of the line
	while(!feof(fptr) && text[index] != EOL && text[index] != CR)
	{
		index++;
		fscanf(fptr, "%c", &text[index]);
	}
	text[index] = '\0';
	return true;
}
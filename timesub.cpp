/*****************************************************************************/
/*  TIMESUB.C  - Date/Time conversion functions   5/21/1990  Jack Doman      */
/*	  JULINT - Returns julian date from YYYYMMDD from 1/1/1970.	     */
/*        JULYRM - Returns YYYYMMDD from julian date.                        */
/* INCREMENT_DATE_TIME_BY_SEC - Adds seconds to YYYYMMDD and HHMMSS values.  */
/*	TIME_DIF - Returns diff between two YYYYMMDD, HHMMSS fields in secs  */
/*   SYSDATETIME - Sets YYYYMMDD, HHMMSS to the current date and time.       */
/*        CKDATE - Tests for valid YYYYMMDD value.                           */
/*        CKHHMM - Tests for valid HHMM time value.                          */
/*        CKTIME - Tests for valid HHMMSS time value.                        */
/*     SHOW_TIME - Displays current system time.                             */
/*	       Modified: 8/15/1990     Debbie Frasca			     */
/* Modified: 4/25/1991    Jack Doman  "int" changed to short for version 8.0 */
/* Modified: 10/2/1991 Debbie Frasca    Microsoft C v6.0  Revisions          */
/* Modified: 10/24/1991 Debbie Frasca Created CKHHMM function		     */
/* Modified: 7/6/1992 Debbie Kesicki v9.1 GENERIC.H Function prototying	     */
/* Modified: 07/08/93 Timothy L. Hoffman - v9.2 Compiles & links under C++   */
/*	on the the PC MS-DOS platform. Most changes were: prototyping,	     */
/*	explicit type conversions, and adding dummy int arg to the ^C	     */
/*	handler routine. Functionality of the program has not been	     */
/*	changed.
/* Modified: 12/07/1999 C Pratt - Changed to .cpp when making TSDWindows a DLL							     */
/*****************************************************************************/
 #include "stdafx.h"
#include "pch.h"//NEW!!!
#include "local.h"

#ifdef LOCAL
	#include "wpicc.h"
#else
	#include <wpicc.h>
#endif

//#ifdef HPUX
#include <sys/types.h>          /* defines time_t variable type              */
#include <sys/timeb.h>          /* definitions of time structure             */
#include <time.h>               /* definitions for time variables            */
#include <stdio.h>		/* standard I/O parameters		     */
#include <stdlib.h>		/* defines labs function   */
#include <string.h>		/* MS-C string functions   */
/*
#else
#include "stdafx.h"
#endif
*/

#include "generic.h"		/* function prototyping    */

#define TARGET_DAYS   11016     /* #days from 19700101 to 20000301           */
long look[16] = {0,306,337,0,31,61,92,122,153,184,214,245,275,306,337,367};
long mdays[13] = { 0,31,29,31,30,31,30,31,31,30,31,30,31};
/*****************************************************************************/
/* julint -function to return julian date from YYYYMMDD relative to 1/1/1970 */
/*****************************************************************************/
long julint(long ymd)
 { long day,year,
	month,
	nolp,leap,
	jul;
/* ------------------------------------------------------------------------- */
   day   = ymd%100;                     /* days                              */
   month = ymd/100;                     /* yearmonth                         */
   year  = month/100-2000;              /* years from 2000                   */
   month = month%100;                   /* months                            */
   if(month <= 2) year = year-1;        /* january or febuary                */
   nolp = year%4;                       /* non-leap years                    */
   leap = year/4;                       /* leap years                        */
   if(nolp < 0)                         /* adjust for negative dates         */
     { leap = leap-1;
       nolp = nolp+4;
     }
   jul = look[month] + day + (365*nolp) + (1461*leap);
   if(labs(jul) > 36524)                 /* check 100 year leaps             */
     jul -= (jul/36525) + (jul/146100) + TARGET_DAYS;
   else  jul += TARGET_DAYS;
   return(jul);
 }


/*****************************************************************************/
/*  julyrm - function to return YYYYMMDD from julian date                    */
/*****************************************************************************/
long julyrm(long jday)
{ long day,year,
       month,
       nolp,leap,j,
       jul;
/* ------------------------------------------------------------------------- */
  jday -= TARGET_DAYS;                      /* make relative to 2/29/2000    */
  if(labs(jday) > 36523)                     /* if past 100 years             */
    { if(labs(jday) > 146096)                /* if past 400 years             */
	{ j = jday + 3*(jday/146097);       /* add 3 days per 400 years      */
	  jday = jday%146097;               /* keep extra days beyond 400 yr */
	}
      else                                  /* less than 400 years           */
	j = jday;                           /*  do nothing                   */

      if(jday > 109572)                     /* if past 300 years             */
	j +=3;                              /* add 3 days                    */
      else
	{ if(jday>0)j += --(jday)/36524;    /* if positive add (--jday/1yr)  */
	  else    j += ++(jday)/36524;      /* if negative add (++jday/1yr)  */
	}
    }
  else                                      /* less than 100 years           */
    j = jday;                               /* do nothing                    */

  leap = j/1461;                            /* # leap years (1461=4 years)   */
  nolp = j%1461;                            /* # non leap years              */
  if(nolp<0)                                /* adjust for negative dates     */
    { --leap;
      nolp += 1461;
    }
  year = nolp/365;                          /* year (365 = 1 year)           */
  day  = nolp%365;                          /* days                          */
  if(day==0)                                 /* febuary                       */
    { month=2;                              /* month                         */
      day=28;                               /* day                           */
      if(year==0)day=29;                     /* leap year                     */
    }
  else
    { if(day>306)                           /* don't need to divide          */
	{ month=2;
	  ++year;
	}
      else
	month=day/30+3;                     /* approximate month             */
      while(look[month] >= day) month--;    /* pinpoint month                */
      day -=look[month];                    /* subtract days offset          */
    }
  jul =((2000 + leap*4 + year)*100 + month)*100 + day;
  return(jul);
}

/*****************************************************************************/
/*   increment_date_time_by_sec      18-MAY-1990     Jack Doman              */
/*           Adds seconds to the YYYYMMDD and HHMMSS values.                 */
/*   Modified 1992/5/14   Jack Doman  More efficient for easy day increments */
/*****************************************************************************/
short increment_date_time_by_sec(long *yyyymmdd, long *hhmmss, long incr )
		/* YYYYMMDD, HHMMSS values to change */
		/* incr	number of seconds to add     */
 {
   long  hour,min,sec;
   long  jdate; 			/* julian date of yyyymmdd	     */
   long  day;                           /* for the easy day increments       */
/* ------------------------------------------------------------------------- */
   sec = (*hhmmss%100) + incr;                   /* parse seconds & add inc  */
   if(sec < 60 && sec >= 0)                      /* if no overflow with inc  */
     { *hhmmss += incr;                          /* add seconds to time      */
       return(TRUE);                             /* exit                     */
     }

   while (sec  < 0)                              /* negative sec             */
     { sec += 86400;                             /* add 1 day to seconds     */
       jdate = julint(*yyyymmdd)-1;              /* subtract 1 day from date */
       *yyyymmdd = julyrm(jdate);
     }                                           /* till sec is positive     */

   min = (*hhmmss%10000)/100;                    /* parse minutes from time  */
   hour = *hhmmss/10000;                         /* parse hour from time     */
   min += sec/60;                                /* add incr to minutes      */
   sec = sec%60;                                 /* parse seconds from incr  */
   if(min < 60)                                  /* if less than 1 hour      */
     { *hhmmss = (hour*10000) + (min*100) + sec; /* add incr & restore time  */
       return(TRUE);
     }
   hour += min/60;                               /* add incr to hour         */
   min = min%60;                                 /* parse minutes from incr  */
   if(hour < 24)                                 /* if less than 1 day       */
     { *hhmmss = (hour*10000) + (min*100) + sec; /* add incr & restore time  */
       return(TRUE);
     }
   day = (*yyyymmdd%100)+(hour/24);		 /* try simple day increment */
   if (day < 29)
     { hour = hour%24;
       *hhmmss = (hour*10000)+(min*100)+sec;     /* add incr & restore time  */
       jdate = *yyyymmdd/100;                    /* parse out yyyymm         */
       *yyyymmdd = (jdate*100)+day;              /*   and add the new day    */
       return(TRUE);
     }
   jdate = julint(*yyyymmdd) + (hour/24);        /* add incr to date         */
   *yyyymmdd = julyrm(jdate);                    /* restore date             */
   hour = hour%24;                               /* parse hours from incr    */
   *hhmmss = (hour*10000) + (min*100) + sec;     /* add incr & restore time  */
   return(TRUE);
 }


/*****************************************************************************/
/* time_dif() returns difference between two YYYYMMDD, HHMMSS fields in secs */
/*      Modified 1992/5/14    Jack Doman     More efficient when ymd1=ymd2+1 */
/*****************************************************************************/
long time_dif(long ymd1, long hms1, long ymd2, long hms2 )
	/* ymd1, hms1	ending date and time */
	/* ymd2, hms2	start date and time  */
{
 long  ddif,hdif,mdif,sdif;			     /* day,hour,min,sec dif */
/* ------------------------------------------------------------------------- */
 ddif = 0;
 if(ymd1 != ymd2)
   { if( ymd1 == (ymd2+1)) ddif = 1;	      /* most common day difference  */
     else ddif = julint(ymd1) - julint(ymd2); /*  more difficult difference  */
   }
 hdif = (hms1/10000) - (hms2/10000);                 /* hour difference      */
 mdif = ((hms1%10000)/100) - ((hms2%10000)/100);     /* minute difference    */
 sdif = (hms1%100) - (hms2%100);
 return ((ddif*86400) + (hdif*3600) + (mdif*60) + sdif);
}

/*****************************************************************************/
/*      sysdatetime() sets YYYYMMDD, HHMMSS to the current date and time     */
/*****************************************************************************/
short sysdatetime(long *yyyymmdd, long *hhmmss )
		/* YYYYMMDD HHMMSS values to set */
 {
     time_t    t;                           /* struct containing system time */
     struct tm *sysd;
/* ------------------------------------------------------------------------- */
     time(&t);                              /* get system date and time      */
     sysd = localtime(&t);
     ++sysd->tm_mon;                        /* jan=0  -> jan=1 .....         */
     *yyyymmdd = (long)19000000 + ((long)sysd->tm_year * (long)10000) +
				  ((long)sysd->tm_mon  * (long)100) +
				  ((long)sysd->tm_mday);
     *hhmmss = ((long)sysd->tm_hour * (long)10000) +
	       ((long)sysd->tm_min  * (long)100) +
	       ((long)sysd->tm_sec);

     return (TRUE);
 }


/*****************************************************************************/
/*              ckdate() tests for valid YYYYMMDD value.                     */
/*****************************************************************************/
short ckdate(long yyyymmdd )
{
  long year,month,day;                              /* temporary date values */
/* ------------------------------------------------------------------------- */
  year = yyyymmdd/10000;                            /* extract year          */
  if(year < 0) return(FALSE);                       /* bad year              */
  if(year < 1900 || year > 2999)	return(FALSE);	/* Will need to be changed in 3000 AD*/
													/* 10201999 cpratt CTime won't accept dates < 1900*/
  month = (yyyymmdd%10000)/100;                     /* extract month         */
  if(month < 1 || month > 12) return(FALSE);        /* bad month             */
  day = yyyymmdd%100;                               /* extract day           */
  if(day < 1 || day > mdays[month]) return(FALSE);  /* bad day               */
  if((month==2) && (day==29))
    { if((year%4)!=0) return(FALSE);                /* bad - no leap         */
      if((year%100)==0)                             /* century leap check    */
	if((year%400)!=0) return(FALSE);            /* bad - no century leap */
    }
  return(TRUE);
}

/*****************************************************************************/
/*           ckhhmm() tests for valid HHMM value.                            */
/*****************************************************************************/
short ckhhmm(long hhmm)
{ long  hour,min;

  hour = hhmm/100;                              /* extract hour              */
  if(hour < 0 || hour > 23) return(FALSE);
  min = hhmm%100;                               /* extract minutes           */
  if(min > 59) return(FALSE);
  return(TRUE);
}



/*****************************************************************************/
/*           cktime() tests for valid HHMMSS value.                          */
/*****************************************************************************/
short cktime(long hhmmss)
{
  if(!ckhhmm(hhmmss/100)) return FALSE;
  if(hhmmss%100  > 59) return FALSE;
  return TRUE;
}


/*****************************************************************************
 * SHOW_TIME - Displays current system time with user specified string (msg) *
 *  Modified: 10/2/1991  Debbie Frasca Microsoft C v6.0 Revisions            *
 *****************************************************************************/
 short show_time(char msg[])
 {
  time_t ttime;
  char   ptime[64];
/* ------------------------------------------------------------------------- */
  time(&ttime);                         /* get system time                   */
  strcpy(ptime,ctime(&ttime));		/* convert to string                 */
  if(strcmp(msg," ") == 0)
    { ptime[24]=WNULL;                  /* remove the \n                     */
      printf("\r%-25s %s","Current system time:",ptime);
    }
  else
    printf("\r%-25s %s",msg,ptime);
  return(TRUE);
}

#ifndef HEADER_H
#define HEADER_H

#define MAX_TYPE  (short) 255
#define MIN_INC   (short) 300
#define MAX_ID    (long)  999999
#define MAX_DATE  (long)  20490928
#define MAX_BYTE  (long)  1440*60*4

struct TSD_HEAD 
 {
    char cookie[4];
    long id,date,time,entry_date,count;
    short incr,rtype,night,code;
    char extra[32];
 };
#endif
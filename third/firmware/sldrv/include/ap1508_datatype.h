/*
 * Copyright (C) 2015 The YunOS Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __AP1508_DATATYPE_H__
#define __AP1508_DATATYPE_H__

#ifndef NULL
#define NULL  0x00
#endif

#ifdef __SILAN_DRIVERS_CORE_CK__
#ifdef __SILAN_DRIVERS_KERNEL__

#ifndef TRUE
#define TRUE  0x01
#endif
#ifndef FALSE
#define FALSE 0x00
#endif
#endif
#else

#ifndef TRUE
#define TRUE  0x01
#endif
#ifndef FALSE
#define FALSE 0x00
#endif
#endif

#ifndef true
#define true  0x01
#endif
#ifndef false
#define false 0x00
#endif

#ifndef SUCCESS
#define SUCCESS  0
#endif
#ifndef FAILURE
#define FAILURE  -1
#endif
#define TIMEOUT  0x1000

#define STATUS_ERR  1
#define STATUS_OK   0

typedef unsigned char       CK_UINT8;
typedef unsigned short      CK_UINT16;
typedef unsigned int        CK_UINT32;
typedef signed char         CK_INT8;
typedef signed short        CK_INT16;
typedef signed int          CK_INT32;
typedef signed long long    CK_INT64;
typedef unsigned long long  CK_UINT64;


#define bit unsigned char
//#define u8  unsigned char
//#define u16  unsigned short
//#define u32  unsigned int
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;


typedef signed char         s8;
typedef signed short        s16;
typedef signed int          s32;
typedef signed long long    s64;
typedef unsigned long long  u64;

typedef unsigned char UINT8;
typedef unsigned int UINT16;
typedef unsigned long int UINT32;
typedef char INT8;
typedef int INT16;
typedef long int INT32;
typedef unsigned char uchar;
typedef unsigned int uint;

//#ifndef BYTE
//typedef unsigned char  BYTE;
//#endif
//#ifndef WORD
//typedef unsigned short  WORD;
//#endif

#define CK_REG  CK_UINT32
#define CK_SREG CK_UINT16
#define CK_CREG CK_UINT8

// FIXME:
typedef struct {
    CK_UINT16 year;
    CK_UINT8  month;
    CK_UINT8  day;
    CK_UINT8  weekday;
    CK_UINT8  hour;
    CK_UINT8  min;
    CK_UINT8  sec;
} __attribute__((packed)) RTCTIME, *PRTCTIME;


#if defined(DEBUG)
#define Debug  printf
#else
#define Debug
#endif

#define  IN
#define  OUT
#define INOUT

typedef unsigned char       uint8_t;
typedef unsigned short      uint16_t;
typedef unsigned int        uint32_t;
typedef signed char         int8_t;
typedef signed short        int16_t;
typedef signed int          int32_t;
typedef signed long long    int64_t;
typedef unsigned long long  uint64_t;

#define __IO                volatile

///////////////////////////////////////////////////////////////////////////////////////
#endif  // __DATATYPE_H__
///////////////////////////////////////////////////////////////////////////////////////

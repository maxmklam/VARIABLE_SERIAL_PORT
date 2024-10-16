/*============= REVISION HISTORY ===============================================
 20231026   maxmklam
 --------   --------------
            File Creation

==============================================================================*/
/*==============================================================================
 @FILE: myTypedef.h
 @AUTHOR: maxmklam
 @OWNER: maxmklam
 @DESCRIBE: 
 Copyright (c) 2024 maxmklam 
 Add the following to notepad++ (Settings->Style Configurator->Language(C), Style(TYPE WORD), then paste typedef  words to "User-defined keywords" 
 "u8 u16 u32 u64 s8 s16 s32 s64 sc8 sc16 sc32 sc64 cs8 cs16 cs32 cs64 uc8 uc16 uc32 uc64 cu8 cu16 cu32 cu64 vu8 vu16 vu32 vu64 vs8 vs16 vs32 vs64"
==============================================================================*/

/************** HEADER ********************************************************/
#ifndef __MYTYPEDEF_H
#define __MYTYPEDEF_H

#ifdef __cplusplus
 extern "C" {
#endif 

/************** INCLUDES ******************************************************/
#include <stddef.h> //NULL

/************** DEFINITIONS ***************************************************/ 
//#define  NULL  ((void *)0)
#define  EVEN_VAL  0
#define  ODD_VAL   1

#define  MYTRUE   1
#define  MYFALSE  0
#define  MYYES    1
#define  MYNO     0

//#define  LEN8_ALIGN32(Len8)  (((Len8) % 4 == 0)? (Len8) : (((Len8) / 4 +1)*4))
#define  LEN8_ALIGN32(Len8)   (((Len8) + 0x03) & (~0x03)) //Len8: Len unit in byte, ALIGN4: Len aligned with 4-byte
#define  LEN8_ALIGN64(Len8)   ((((Len8) % 8) == 0)? ((Len8)) : (((Len8) /8 +1)*8))
#define  LEN32_ALIGN32(Len8)  ((((Len8) % 4) == 0)? ((Len8) /4) : ((Len8) /4 +1))
#define  LEN32_ALIGN64(Len8)  ((((Len8) % 8) == 0)? ((Len8) /4) : (((Len8) /8 +1)*2))
#define  LEN64_ALIGN64(Len8)  ((((Len8) % 8) == 0)? ((Len8) /8) : (((Len8)) /8 +1))

#define  RING_NXT_DEC1(CurItem, RbTtlSize)  (CurItem==0? (RbTtlSize-1) : ((CurItem % RbTtlSize)-1))
#define  RING_NXT_INC1(CurItem, RbTtlSize)  ((CurItem+1) % RbTtlSize)
#define  WR_RING_NXT_INC1(CurItem, RbTtlSize)  ((CurItem=CurItem+1) % RbTtlSize)

#define  SWAPBYTE16(dat16)  (((dat16 & 0x00FF) << 8) | ((dat16 & 0xFF00) >> 8))
#define  SWAPBYTE32(dat32)  (((dat32 & 0x000000FF) << 24) | ((dat32 & 0x0000FF00) << 8) | ((dat32 & 0x00FF0000) >> 8) | ((dat32 & 0xFF000000) >> 24))

#define  LARGER(a, b)  ((a) > (b) ? (a) : (b))



/*
Data Type   Description
------------------------------
uint8_t     unsigned char (8-bit)
int8_t      signed char (8-bit)
uint16_t    unsigned short (16-bit)
int16_t     signed short (16-bit)
uint32_t    unsigned int (32-bit)
int32_t     signed int (32-bit)
uint64_t    unsigned long long (64-bit)
int64_t     signed long long (64-bit)
float       single precision floating number (32-bit)
double      double precision floating number (64-bit)
*/

/*
Type                    Size in bits         Natural alignment in bytes       Range of values
--------------------------------------------------------------------------------------------
char                    8                    1 (byte-aligned)                 0 to 255 (unsigned) by default.
                                                                              -128 to 127 (signed) when compiled with --signed_chars.
signed char             8                    1 (byte-aligned)                 -128 to 127
unsigned char           8                    1 (byte-aligned)                 0 to 255
(signed) short          16                   2 (halfword-aligned)             -32,768 to 32,767
unsigned short          16                   2 (halfword-aligned)             0 to 65,535
(signed) int            32                   4 (word-aligned)                 -2,147,483,648 to 2,147,483,647
unsigned int            32                   4 (word-aligned)                 0 to 4,294,967,295
(signed) long           32                   4 (word-aligned)                 -2,147,483,648 to 2,147,483,647
unsigned long           32                   4 (word-aligned)                 0 to 4,294,967,295
(signed) long long      64                   8 (doubleword-aligned)           -9,223,372,036,854,775,808 to 9,223,372,036,854,775,807
unsigned long long      64                   8 (doubleword-aligned)           0 to 18,446,744,073,709,551,615
float                   32                   4 (word-aligned)                 1.175494351e-38 to 3.40282347e+38 (normalized values)
double                  64                   8 (doubleword-aligned)           2.22507385850720138e-308 to 1.79769313486231571e+308 (normalized values)
long double             64                   8 (doubleword-aligned)           2.22507385850720138e-308 to 1.79769313486231571e+308 (normalized values)
wchar_t                 16                   2 (halfword-aligned)             0 to 65,535 by default.
                        32                   4 (word-aligned)                 0 to 4,294,967,295 when compiled with --wchar32.
All pointers            32                   4 (word-aligned)                 Not applicable.
bool (C++ only)         8                    1 (byte-aligned)                 false or true
_Bool (C onlya)         8                    1 (byte-aligned)                 false or true
*/


typedef  unsigned char           u8;
typedef  unsigned short          u16;
typedef  unsigned int            u32;
typedef  unsigned long long      u64;

typedef  signed char             s8;
typedef  signed short            s16;
typedef  signed int              s32;
typedef  signed long long        s64;


typedef  const signed char       cs8;
typedef  const signed short      cs16;
typedef  const signed int        cs32;
typedef  const signed long long  cs64;

typedef  const signed char       sc8;
typedef  const signed short      sc16;
typedef  const signed int        sc32;
typedef  const signed long long  sc64;



typedef  const unsigned char       cu8;
typedef  const unsigned short      cu16;
typedef  const unsigned int        cu32;
typedef  const unsigned long long  cu64;

typedef  const unsigned char        uc8;
typedef  const unsigned short       uc16;
typedef  const unsigned int         uc32;
typedef  const unsigned long long   uc64;



typedef  volatile unsigned char       vu8;
typedef  volatile unsigned short      vu16;
typedef  volatile unsigned int        vu32;
typedef  volatile unsigned long long  vu64;


typedef  volatile signed char       vs8;
typedef  volatile signed short      vs16;
typedef  volatile signed int        vs32;
typedef  volatile signed long long  vs64;


typedef  float     f32;
typedef  double    d64;



#define  MASK_BIT0   0x00000001
#define  MASK_BIT1   0x00000002
#define  MASK_BIT2   0x00000004
#define  MASK_BIT3   0x00000008
#define  MASK_BIT4   0x00000010
#define  MASK_BIT5   0x00000020
#define  MASK_BIT6   0x00000040
#define  MASK_BIT7   0x00000080
#define  MASK_BIT8   0x00000100
#define  MASK_BIT9   0x00000200
#define  MASK_BIT10  0x00000400
#define  MASK_BIT11  0x00000800
#define  MASK_BIT12  0x00001000
#define  MASK_BIT13  0x00002000
#define  MASK_BIT14  0x00004000
#define  MASK_BIT15  0x00008000

#define  MASK_BIT16  0x00010000
#define  MASK_BIT17  0x00020000
#define  MASK_BIT18  0x00040000
#define  MASK_BIT19  0x00080000
#define  MASK_BIT20  0x00100000
#define  MASK_BIT21  0x00200000
#define  MASK_BIT22  0x00400000
#define  MASK_BIT23  0x00800000
#define  MASK_BIT24  0x01000000
#define  MASK_BIT25  0x02000000
#define  MASK_BIT26  0x04000000
#define  MASK_BIT27  0x08000000
#define  MASK_BIT28  0x10000000
#define  MASK_BIT29  0x20000000
#define  MASK_BIT30  0x40000000
#define  MASK_BIT31  0x80000000




typedef union _Swap32
{
	struct 
	{
	  u32 byte0: 8;
	  u32 byte1: 8;
	  u32 byte2: 8;
	  u32 byte3: 8;
	};
	u32 val;
} _Swap32;




/************** EXPORTED FUNCTION *********************************************/

/************** FOOTER ********************************************************/

#ifdef __cplusplus
}
#endif

#endif //__MYTYPEDEF_H

/*=========================== END OF FILE ====================================*/

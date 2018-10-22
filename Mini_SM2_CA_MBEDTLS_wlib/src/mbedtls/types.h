/* *Copyright (c) 2007, �人��ѧ�����о�����* 
   *All rights reserved.* 
   *
   *�ļ����ƣ�types.h
   *ժ    Ҫ��������������ṹ
   *��    ��: �����
   *ʱ    ��: 2007.5.3 21:05
   *���°汾: v1.0.0
*/
#ifndef __HEADER_TYPES_H
#define __HEADER_TYPES_H


#ifdef  __cplusplus
extern "C" {
#endif


#define Word   unsigned int
#define WordByteLen 4
#define WordBitLen 32
#define Byte   unsigned char
#define BYTE   unsigned char
#define DWord  unsigned __int64
#define SDWord __int64
#define MSBOfWord	0x80000000
#define LSBOfWord	0x00000001


#define MAXBNWordLen  8
#define MAXBNByteLen  MAXBNWordLen*WordByteLen
#define MAXBNBitLen   MAXBNByteLen*8

//typedef DWord HCRYPTPROV;
	
		
#ifdef  __cplusplus
}
#endif


#endif
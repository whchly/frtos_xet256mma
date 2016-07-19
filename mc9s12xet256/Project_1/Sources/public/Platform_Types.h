/*-------------------------------------------------------------------------------------
| File Name   :   PlatFrom_Types.h
| Project Name:   
| Author      :   
| Affiliation :  
|                
|
| Description :   
| Version     : 
|--------------------------------------------------------------------------------------
| Copyright (c) 
|------------------------------------------------------------------------------------*/
/*
*********************************************************************************************************
*									Define to prevent recursive inclusion
*********************************************************************************************************
*/
#ifndef _PLATFORM_TYPES_H_
#define _PLATFORM_TYPES_H_

/*
*********************************************************************************************************
*									        Symbolic definitions
*********************************************************************************************************
*/
#define CPU_TYPE_8       8
#define CPU_TYPE_16      16
#define CPU_TYPE_32      32

#define MSB_FIRST        0
#define LSB_FIRST        1

#define HIGH_BYTE_FIRST  0
#define LOW_BYTE_FIRST   1

/*
*********************************************************************************************************
*									        Symbols
*********************************************************************************************************
*/
#define CPU_TYPE          (CPU_TYPE_16)
#define CPU_BIT_ORDER     (LSB_FIRST)
#define CPU_BYTE_ORDER    (HIGH_BYTE_FIRST)

/*
*********************************************************************************************************
*									           Types
*********************************************************************************************************
*/
typedef unsigned char		boolean;
typedef signed char			sint8;
typedef unsigned char		uint8;
typedef signed short		sint16;
typedef unsigned short  	uint16;
typedef signed long			sint32;
typedef unsigned long		uint32;
typedef signed char			sint8_least;
typedef unsigned char		uint8_least;
typedef signed short		sint16_least;
typedef unsigned short  	uint16_least;
typedef signed long			sint32_least;
typedef unsigned long		uint32_least;
typedef float			    float32;
typedef double			   	float64;


#endif 


/*-------------------------------------------------------------------------------------
| File Name   :   Std_Types.h
| Project Name:   MC9S12XET256 
| Author      :   
| Affiliation :   WHC
|                 
|
| Description :   
| Version     :   1.0
|--------------------------------------------------------------------------------------
| Copyright (c)  2009-2060 by  WHC    All rights reserved.
|------------------------------------------------------------------------------------*/
	
/*
*********************************************************************************************************
*									Define to prevent recursive inclusion
*********************************************************************************************************
*/

#ifndef _STD_TYPES_H_
#define _STD_TYPES_H_

/*
*********************************************************************************************************
*									         Includes
*********************************************************************************************************
*/
 /*
 #include "Compiler.h"
 
 */
 #include "Platform_Types.h"
/*
*********************************************************************************************************
*									       Defineds of standard types
*********************************************************************************************************
*/
typedef uint8 Std_ReturnType;

typedef struct
{
	uint16 vendorID;
	uint16 moduleID;
	uint8  instanceID;
	uint8  sw_major_version;
	uint8  sw_minor_version;
	uint8  sw_patch_version;
}Std_VersionInfoType;
/*End Defineds of standard types*/

/*
*********************************************************************************************************
*									         Symbol Defineds
*********************************************************************************************************
*/
#ifndef STATSTYPEDEFINED
#define STATSTYPEDEFINED
#define E_OK       0x00u
typedef unsigned char StatusTypes; /*OSEK compliance */
#endif                             /*STATSTYPEDEFINED */

#define E_NOT_OK   0x01u
#define STD_HIGH   ((uint16)1u)    
#define STD_LOW    ((uint16)0u)    /*Physical state 0V */
#define STD_ACTIVE 0x01u           /*Logical state active */
#define STD_IDLE   0x00u           /*Logical state idle*/
#define STD_ON     0x01u
#define STD_OFF    0x00u

#define watchdog_clr( ) (void)(ARMCOP = 0x55, ARMCOP = 0xAA)
/**
  * @brief bool variable type. 
  */

 typedef enum{
  ENABLE=0x00,
  DISABLE =0x01 
 }bool;

#endif                             /*End _STD_TYPES_H_*/



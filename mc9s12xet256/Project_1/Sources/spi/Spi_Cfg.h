
/*--------------------------------------------------------------------------
| File Name   :   Spi_Cfg.h
| Project Name: 
| Author      :  
|
|
| Description :   This file contains all the configration parameters for the SPI peripheral.
| Version     :   1.0
|                                        
|---------------------------------------------------------------------------
| Copyright (c) 
|--------------------------------------------------------------------------*/

/* Define to prevent recursive inclusion -------------------------------------*/


/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef _Spi_Cfg_h_
#define _Spi_Cfg_h_    

#include "Std_Types.h" 

#define SPI_MAX_IB 0x10           /* the maximum number of elements contained in IB */
#define SPI_MAX_CHANNEL 0x01      /* the maximum number of channels contained in Run */
#define SPI_MAX_RUN 0x01          /* the maximum number of jobs contained in sequence */
#define SPI_MAX_SEQUENCE 0x01     /* the maximum number of sequences */ 


#define CHANNEL_NUM 0x03   /* the number of channel */
#define RUN_NUM 0x03       /* the number of job */
#define SEQUENCE_NUM 0x02  /* the number of sequence */
#define HW_NUM 0x03


  /* 
  Usage 0: the SPI Handler/Driver manages only Internal Buffers.
  Usage 1: the SPI Handler/Driver manages only External Buffers.
  Usage 2: the SPI Handler/Driver manages both buffers types.
  */
 #define SPI_CHANNEL_BUFFERS_ALLOWED  0x02

/*
 LEVEL0(0x00):SPI Handler/Driver shall offer a synchronous transfer service .
 LEVEL1(0x01):SPI Handler/Driver shall offer an asynchronous transfer service.
 LEVEL1(0x02):SPI Handler/Driver shall offer a synchronous transfer service
 for a dedicated SPI bus and it shall also offer an asynchronous transfer service other SPI busses.
  */ 
 #define SPI_LEVEL_DELIVERED 0x02 
 
/*
 Adds the service Spi_GetVersionInfo()from the code.
 If defined parameter is STD_OFF,
 the service Spi_GetVersionInfo() will not be add from the code
 */
 #define SPI_VERSION_INFO_API STD_ON 
 
 /*
 Adds the service Spi_GetHWUnitStatus()from the code. 
 If defined parameter is STD_OFF,
 the service Spi_GetHWUnitStatus() will not be add from the code
 */
 #define SPI_HW_STATUS_API STD_ON  


 /*
 Adds the service Spi_Cancel( )from the code. 
 If defined parameter is STD_OFF,
 the service Spi_Cancel( ) will not be add from the code
  */
 #define SPI_CANCEL_API   STD_ON 
 
 
 /*
 Adds DET from the code . If defined parameter is STD_OFF,
 the DET will not be add from the code
  */    
 #define SPI_DEV_ERROR_DETECT  STD_ON  
 /*
 STD_ON:Interruptible Sequences are not allowed within levels 1 and 2 ;
 STD_OFF:Interruptible Sequences are allowed within levels 1 and 2
  */
 #define SpiInterruptibleSeqAllowed STD_OFF

#endif
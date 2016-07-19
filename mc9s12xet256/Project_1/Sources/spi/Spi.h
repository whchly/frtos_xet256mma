
/*--------------------------------------------------------------------------
| File Name   :   Spi.h
| Project Name:   MC9S12XET256 SPI驱动
| Author      :   WHC
|
|
| Description :   This file contains all the type declaration for the SPI peripheral.
| Version     :   1.0 
|                                        
|---------------------------------------------------------------------------
| Copyright (c)  2009-2016 by WHC  All rights reserved.
|--------------------------------------------------------------------------*/

/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef _Spi_h_
#define _Spi_h_

/* Includes ------------------------------------------------------------------*/
#include "Std_Types.h"
#include "spi_Cfg.h"

/*The module ID of SPI*/
#define SPI_MODULE_ID 83

/*Header Version Information [DIO82] [DIO106]*/
#define SPI_VENDOR_ID         (uint16)0x0000          
#define SPI_SW_MAJOR_VERSION  (uint8)0x01
#define SPI_SW_MINOR_VERSION  (uint8)0x00
#define SPI_SW_PATCH_VERSION  (uint8)0x00

#define SPI_AR_MAJOR_VERSION 0
#define SPI_AR_MINOR_VERSION 0
#define SPI_AR_PATCH_VERSION 0




/*Service IDs*/
#define SPI_INIT_ID              (uint8)0x00
#define SPI_DEINIT_ID            (uint8)0x01
#define SPI_WRITEIB_ID           (uint8)0x02
#define SPI_ASYNCTRANSMIT_ID     (uint8)0x03
#define SPI_READIB_ID            (uint8)0x04
#define SPI_SETUPEB_ID           (uint8)0x05
#define SPI_GETSTATUS_ID         (uint8)0x06
#define SPI_GETJOBRESULT_ID      (uint8)0x07
#define SPI_GETSEQUENCERESULT_ID (uint8)0x08
#define SPI_GETVERSIONINFO_ID    (uint8)0x09
#define SPI_SYNCTRANSMIT_ID      (uint8)0x0A
#define SPI_GETHWUNITSTATUS_ID   (uint8)0x0B
#define SPI_CANCLE_ID            (uint8)0x0C
#define SPI_SETASYNCMODE_ID      (uint8)0x0D




/**
  * @brief SPI Clock Polarity Bit SPI空闲时，输出的高低电平选择 
  */  
  typedef enum {
   SPI_ACTIVE_HIGH_CLOCKS = 0x00, /* Active-high clocks selected. In idle state SCK is low.*/
   SPI_ACTIVE_LOW_CLOCKS = 0x01   /* Active-low clocks selected. In idle state SCK is high.*/
  }Spi_ClockPolarityType;

/**
  * @brief SPI Clock Phase Bit SPI采集数据时是奇沿还是偶沿 
  */  
  typedef enum{
   SPI_ODD_EDGES = 0x00, /* Sampling of data occurs at odd edges (1,3,5,...) of the SCK clock */
   SPI_EVEN_EDGES = 0x01 /* Sampling of data occurs at even edges (2,4,6,...) of the SCK clock.*/
  }Spi_SampleDateEdgeType;


/**
  * @brief /SS Input Output Type
  */   
   typedef enum{
   SPI_BARSS_NOT_USED = 0x00,/*  Pin /SS not used by SPI */
   SPI_BARSS_INPUT_MODF_FEATURE = 0x10,  /* Pin /SS input with MODF feature */
   SPI_BARSS_SLAVE_SELECT = 0x11   /* Pin /SS is slave select output */
  }Spi_BarSSInputOutputType;



 /**
  * @brief LSB-First Enable
  */
  typedef enum{
   SPI_MSB_FIRST = 0x00,  /* Data is transferred MSB */
   SPI_LSB_FIRST = 0x01   /* Data is transferred LSB */
  }Spi_DataTransferredFirstBitType;


 /**
  * @brief SPI Transfer Width
  */  
  typedef enum{
   SPI_8_BIT_WIDTH = 0x00,   /* 8-bit Transfer Width */
   SPI_16_BIT_WIDTH = 0x01   /* 16-bit Transfer Width */ 
  }Spi_TransferWidthType;

 /**
  * @brief SPI Stop in Wait Mode Bit  SPI等待模式时时钟无输入和有输入时钟 
  */  
  
  typedef enum{
   SPI_OPERATE_NORMALLY_IN_WAIT_MODE = 0x00, /* SPI clock operates normally in wait mode.*/
   SPI_STOP_IN_WAIT_MODE = 0x01  /* Stop SPI clock generation when in wait mode.*/
  }Spi_ClockInWaitModeType;

 /**
  * @brief SPI Bidirectional Pin Configurations
  */  
  typedef enum{
   SPI_PIN_NORMAL = 0x00,   /* MOSI and MISO is in normal */
   SPI_PIN_BID_MOSI_IN = 0x01, /* MISO not used by SPI; MOSI is master in */
   SPI_PIN_BID_MOSI_IN_OUT = 0x09  /* MISO not used by SPI; MOSI is master I/O */
  }Spi_BidirectionalPinType;


/**
 * @brief SPI Baud rate divisor select.
 */ 
  typedef enum{
   SPI_BAUD_RATE_DIVISOR_2 = 0x00, //The baud rate divisor is 2
   SPI_BAUD_RATE_DIVISOR_4 = 0x01, //The baud rate divisor is 4
   SPI_BAUD_RATE_DIVISOR_6 = 0x20, //The baud rate divisor is 6
   SPI_BAUD_RATE_DIVISOR_8 = 0x02, //The baud rate divisor is 8
   SPI_BAUD_RATE_DIVISOR_10 = 0x40, //The baud rate divisor is 10
   SPI_BAUD_RATE_DIVISOR_12 = 0x21, //The baud rate divisor is 12
   SPI_BAUD_RATE_DIVISOR_14 = 0x60,  //The baud rate divisor is 14
   SPI_BAUD_RATE_DIVISOR_16 = 0x03, //The baud rate divisor is 16
   SPI_BAUD_RATE_DIVISOR_20 = 0x41, //The baud rate divisor is 20
   SPI_BAUD_RATE_DIVISOR_24 = 0x22,  //The baud rate divisor is 24
   SPI_BAUD_RATE_DIVISOR_28 = 0x61,  //The baud rate divisor is 28
   SPI_BAUD_RATE_DIVISOR_32 = 0x04, //The baud rate divisor is 32
   SPI_BAUD_RATE_DIVISOR_40 = 0x42, //The baud rate divisor is 42
   SPI_BAUD_RATE_DIVISOR_48 = 0x23, //The baud rate divisor is 48
   SPI_BAUD_RATE_DIVISOR_56 = 0x62,  //The baud rate divisor is 62
   SPI_BAUD_RATE_DIVISOR_64 = 0x05, //The baud rate divisor is 64
   SPI_BAUD_RATE_DIVISOR_80 = 0x43, //The baud rate divisor is 80
   SPI_BAUD_RATE_DIVISOR_96 = 0x24, //The baud rate divisor is 96
   SPI_BAUD_RATE_DIVISOR_112 = 0x63,  //The baud rate divisor is 112
   SPI_BAUD_RATE_DIVISOR_128 = 0x06, //The baud rate divisor is 128
   SPI_BAUD_RATE_DIVISOR_160 = 0x44, //The baud rate divisor is 160
   SPI_BAUD_RATE_DIVISOR_192 = 0x25, //The baud rate divisor is 192
   SPI_BAUD_RATE_DIVISOR_224 = 0x64,  //The baud rate divisor is 224
   SPI_BAUD_RATE_DIVISOR_256 = 0x07, //The baud rate divisor is 256
   SPI_BAUD_RATE_DIVISOR_320 = 0x45,  //The baud rate divisor is 320
   SPI_BAUD_RATE_DIVISOR_384 = 0x26,  //The baud rate divisor is 384
   SPI_BAUD_RATE_DIVISOR_448 = 0x65,  //The baud rate divisor is 448
   SPI_BAUD_RATE_DIVISOR_512 = 0x17,   //The baud rate divisor is 512
   SPI_BAUD_RATE_DIVISOR_640 = 0x46,  //The baud rate divisor is 640
   SPI_BAUD_RATE_DIVISOR_768 = 0x27,   //The baud rate divisor is 768
   SPI_BAUD_RATE_DIVISOR_896 = 0x66,  //The baud rate divisor is 896
   SPI_BAUD_RATE_DIVISOR_1024 = 0x37,  //The baud rate divisor is 1024
   SPI_BAUD_RATE_DIVISOR_1280 = 0x47,  //The baud rate divisor is 1280
   SPI_BAUD_RATE_DIVISOR_1536 = 0x57,  //The baud rate divisor is 1536
   SPI_BAUD_RATE_DIVISOR_1792 = 0x67,  //The baud rate divisor is 1792
   SPI_BAUD_RATE_DIVISOR_2048 = 0x77,  //The baud rate divisor is 2048
  }Spi_BaudRateDivisorType;


/**
  * @brief This type defines a range of specific status for SPI Handler/Driver.
  */  
typedef enum 
{
	SPI_UNINIT = 0,  /* The SPI Handler/Driver is not initialized or not usable.*/
	SPI_IDLE   = 1,  /* The SPI Handler/Driver is not currently transmitting data.*/
	SPI_BUSY   = 2   /* The SPI Handler/Driver is performing a SPI (transmit). */
}Spi_StatusType;

/**
  * @brief This type defines a range of specific SPI status for SPI Handler/Driver.
  */ 

typedef enum 
{
	SPI_RUN_OK      = 0,  /* The last transmission of the SPI finished successfully.*/
	SPI_RUN_PENDING = 1,  /* The SPI Handler/Driver is performing a SPI busy.*/
	SPI_RUN_FAILED  = 2   /* The last transmission of the SPI has failed. */
}Spi_RunResultType;


/**
 * @brief This type defines a range of specific Sequences status for SPI Handler/Driver.
 */ 

typedef enum 
{
	SPI_SEQ_OK        = 0,  /* The last transmission of the Sequence finished successfull.*/
	SPI_SEQ_PENDING   = 1,  /* The SPI Handler/Driver is performing a SPI Sequence. */
	SPI_SEQ_FAILED    = 2,  /* The last transmission of the Sequence has failed.*/
	SPI_SEQ_CANCELLED = 3   /* The last transmission of the Sequence has been cancelled by user.*/
}Spi_SeqResultType;


/**
 * @brief spi channel buffer type .
 */
 typedef enum{
  SPI_IB_BUFFER = 0x00, /* Buffer usage with IB Channel */
  SPI_EB_BUFFER = 0x01  /* Buffer usage with EB Channel */
 }Spi_BufferUsageType;

/**
 * @brief SPI Run  priority.
 */  
typedef enum Spi_RunPriority
{
	SPI_Run_PRIORITY_0 = 0,
	SPI_Run_PRIORITY_1 = 1,
	SPI_Run_PRIORITY_2 = 2,
	SPI_Run_PRIORITY_3 = 3
}Spi_RunPriorityType;


/**
 * @brief Specifies the asynchronous mechanism mode for SPI busses handled asynchronously in LEVEL2.
 */ 
typedef enum Spi_AsyncMode
{
	SPI_POLLING_MODE   = 0,  /* The asynchronous mechanism is ensured by polling.*/
	SPI_INTERRUPT_MODE = 1   /* The asynchronous mechanism is ensured by interrupt.*/
}Spi_AsyncModeType;

/**
 * @brief Current function mode .
 */ 
typedef enum 
{
 SPI_FUNCTION_NO_RUNNING = 0x00,
 SPI_FUNCTION_SYNCHRONOUS = 0x01,
 SPI_FUNCTION_ASYNCHRONOUS = 0x02
}Spi_CurrentTansmitFunctionModeType;


/**
 * @brief Type of application data buffer elements.
 */ 
typedef  uint8 Spi_DataType;


/**
 * @brief Type for defining the number of data elements of the type Spi_DataType.
 */ 
typedef  uint8 Spi_NumberOfDataType;



/**
 * @brief Specifies the identification for a Channel.
 */
typedef  uint8 Spi_ChannelType;


/**
 * @brief Specifies the identification for a Run
 */      
typedef  uint8 Spi_RunType;


/**
 * @brief Specifies the identification for a Squence
 */      
typedef  uint8 Spi_SequenceType;


/**
 * @brief Specifies the identification (ID) for a SPI Hardware microcontroller peripheral 
 */      
typedef  uint8 Spi_HWUnitType ;


/**
 * @brief Call back function type 
 */                        
typedef  void (*CallBackFunctionType) (void);


/**
 * @brief Struct of Channel.
 */     
 typedef struct{
  Spi_ChannelType   SpiChannelID;
  Spi_BufferUsageType    SpiBufferUsage;/* 使用内部缓存区,还是外部缓存区 */

  Spi_NumberOfDataType   SpiNumberOfData; 
  Spi_DataTransferredFirstBitType    SpiDataTransferredFirstBit;/* 是先发送高字位还是低位选择 */
  Spi_DataType  Spi_DefaultTransmitValue;
  }Spi_ChannelConfigType; 
 
 
/**
 * @brief Buffer type of Channel.
 */  
  typedef union{
   struct{
    Spi_DataType SpiBuffersTx[SPI_MAX_IB];
    Spi_DataType SpiBuffersRx[SPI_MAX_IB];
   }Spi_BufferUsageIb;
   struct{
    const Spi_DataType *SpiDataSrcPtr; /* Pointer to source data buffer */
    Spi_DataType *SpiDataDesPtr; /*Pointer to destination data buffer in RAM */
   }Spi_BufferUsageEb;
  }Spi_BufferType;
 

/**
 * @brief Struct of Run.
 */  
 typedef struct{
  Spi_ClockInWaitModeType  SpiClockInWaitMode;
  
  Spi_RunType   SpiRunID;
  Spi_HWUnitType  SpiRunHWUnit;
  Spi_BarSSInputOutputType SpiRunBarSSInputOutput;
  Spi_BidirectionalPinType SpiRunBidirectionalPin;
  Spi_BaudRateDivisorType  SpiRunBaudRateDivisor;
  Spi_ClockPolarityType    SpiRunClockPolarity;
  Spi_SampleDateEdgeType   SpiRunSampleDateEdge;
  Spi_RunPriorityType    SpiRunPriority;
  CallBackFunctionType   SpiRunEndNotification; /* pointer to a Run notification function.*/ 
  uint8 SpiRunChannelNumber;
  uint8 FixedLinkOfChannels[SPI_MAX_CHANNEL];
  
 }Spi_RunConfigType;



/**
 * @brief Struct of Sequence.
 */ 

  typedef struct{
   Spi_SequenceType   SpiSequenceID;
   uint8 SpiSequenceRunNumber;
   uint8 FixedLinkOfRun[SPI_MAX_RUN];
   boolean SpiInterruptibleSequence; /* Allows or not this sequence to be suspended by another one */
   CallBackFunctionType SpiSequenceEndNotification; /* pointer to a sequence notification function.*/ 
   
  }Spi_SequenceConfigType;

/**
 * @brief Struct:the number of SPI Run in Sequence and SPI Run contained in Sequence
 */ 
  typedef struct{
   uint8 SpiSequenceRunNumber;
   uint8 FixedLinkOfRun[SPI_MAX_RUN];    
  }Spi_SequenceContainRunType;

/**
 * @brief Struct:the number of job in Sequence and jobs contained in Sequence
 */ 
  typedef struct{
  uint8 SpiRunChannelNumber;
  uint8 FixedLinkOfChannels[SPI_MAX_CHANNEL];    
  }Spi_RunContainChannelType;



/**
 * @brief Struct of Spi_ConfigType .
 */ 
  typedef struct{
   Spi_ChannelConfigType     SpiChannelConfig[CHANNEL_NUM];
   Spi_RunConfigType         SpiRunConfig[RUN_NUM];
   Spi_SequenceConfigType    SpiSequenceConfig[SEQUENCE_NUM];
  }Spi_ConfigType;





 /* Exported functions ------------------------------------------------------- */
/** @addtogroup SPI_Exported_Functions
  * @{
  */

void Spi_Init( const Spi_ConfigType *ConfigPtr);

Std_ReturnType Spi_DeInit( );

Std_ReturnType Spi_WriteIB(Spi_ChannelType Channel,const Spi_DataType *DataBufferPtr);

Std_ReturnType Spi_ReadIB(Spi_ChannelType Channel,Spi_DataType *DataBufferPointer);

Std_ReturnType Spi_AsyncTransmit(Spi_SequenceType Sequence);

Std_ReturnType Spi_SetupEB( Spi_ChannelType  Channel, const Spi_DataType* SrcDataBufferPtr,
                            Spi_DataType* DesDataBufferPtr, Spi_NumberOfDataType Length) ;
 
Spi_StatusType Spi_GetStatus( );

Spi_RunResultType Spi_GetRunResult( Spi_RunType run);

Spi_SeqResultType Spi_GetSequenceResult(Spi_SequenceType Sequence);

#if(SPI_HW_STATUS_API == STD_ON)
Spi_StatusType Spi_GetHWUnitStatus(Spi_HWUnitType HWUnit);
#endif

#if(SPI_CANCEL_API == STD_ON)
void Spi_Cancel( Spi_SequenceType Sequence );
#endif

Std_ReturnType Spi_SyncTransmit(Spi_SequenceType Sequence);

Std_ReturnType Spi_SetAsyncMode( Spi_AsyncModeType Mode);

#if(SPI_VERSION_INFO_API == STD_ON)
void Spi_GetVersionInfo (Std_VersionInfoType*  versioninfo); 
#endif


/**
  * @}
  */













#endif
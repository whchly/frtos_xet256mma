/*--------------------------------------------------------------------------
| File Name   :  Spi.c
| Project Name:  S12SPIV5
| Author      :  WHC
|
|
| Description :   This file contains all the prototypes/macros for the SPI peripheral.
| Version     :   1.0
|                                        
|---------------------------------------------------------------------------
| Copyright (c)  2009-2016 by CQUPT  All rights reserved.
|--------------------------------------------------------------------------*/


  
  
   
/* ---------------Define to prevent recursive inclusion -------------------------*/
#ifndef _Spi_c_
#define _Spi_c_

/*--------------------------Includes ------------------------------------------------*/
#include "Spi.h"
#include "MC9S12XET256.h"


#if(SPI_DEV_ERROR_DETECT == STD_ON)
#include "Det.h"
#endif

/*
#include "Dem.h"
#include "SchM_Spi.h"
*/


/*   */

#define NULL 0

/*--------------------------------Global Parameter--------------------------------------*/
/* SPI unit flag*/
uint8    SPI0_COMPLETE_FLAG = 0x00,SPI1_COMPLETE_FLAG = 0x00,SPI2_COMPLETE_FLAG = 0x00;
uint8    Spi_InitFuncFlag = 0x00;
 
/* storage for number of data to be transmitted for each channel */
uint8    Number_of_data_transmitted[CHANNEL_NUM]; 

/* Struct of channel number and channel ID for each Run */
Spi_RunContainChannelType     SpiRunContainChannel[RUN_NUM]; //

Spi_SequenceContainRunType    SpiSequenceContainRun[SEQUENCE_NUM];  //Struct of Run number and Run ID for each Sequence

uint8  SpiRunRegisterValueType[RUN_NUM][3];       // storage for registers value contained in Jobs. SpiRunRegisterValueType[][0] refer to SPICR1. SpiRunRegisterValueType[][1] refer to SPICR2. SpiRunRegisterValueType[][2] refer to SPIBR
uint8  SpiChannelRegisterValueType[CHANNEL_NUM][2];  // storage for registers value contained in Channels. SpiChannelRegisterValueType[][0] refer to SPICR1. SpiChannelRegisterValueType[][1] refer to SPICR2. 

uint8  Spi_HWofRun[RUN_NUM];       //Hardware ID in jobs.

uint8  LastSequenceID = 0x00;         // the last transmitted sequence ID

Spi_StatusType      Spi_status = SPI_UNINIT;    //spi status

Spi_StatusType      Spi_HWstatus[HW_NUM] ;    //spi HW status

uint8               Spi_SequenceCancelstatus[SEQUENCE_NUM] ;    //spi sequence cancel status

Spi_RunResultType   SpiRunResult[RUN_NUM];  //Result of Run.

Spi_SeqResultType   SpiSeqResult[SEQUENCE_NUM]; //Result of sequence

Spi_DataType        SpiDefaultValue[CHANNEL_NUM]; //Default value for each channel

Spi_BufferType      SpiBuffer[CHANNEL_NUM];  //Channel buffer.

Spi_BufferUsageType  SpiBufferUsage[CHANNEL_NUM]; //Channel buffer usage type

Spi_AsyncModeType     SpiAsyncMode ; //Asynchronous mode

Spi_CurrentTansmitFunctionModeType     SpiCurrentTansmitFunctionMode = SPI_FUNCTION_NO_RUNNING;//Current function mode

boolean  SpiSequenceInterrupt[SEQUENCE_NUM]; //    Sequence interrupt type

uint8     SeqOfWaitingTransmitID = 0xff ;  // Behavior of Non-Interruptible
uint8     SeqOfWaitingTransmitFlag = 0x00;

CallBackFunctionType   SpiRunEndNotificationPointer[RUN_NUM];//pointer to a Run notification function. 

CallBackFunctionType   SpiSequenceEndNotificationPointer[SEQUENCE_NUM];//pointer to a sequence notification function. 


 
/*-------------------------------------------------------------------------------------*/

/*--------------------------------Initialize Global Parameter--------------------------------------*/




/*-------------------------------------------------------------------------------------*/



 /**
  * @brief Initializes the module
  * @param[in] SpiChannelConfig: Struct of Channel
  * can be one of the values of @ref Spi_ChannelConfigType.
  * @param[in] SpiRunConfig: Struct of Run.
  * can be one of the values of @ref Spi_JobConfigType.
  * @param[in] SpiSequenceConfig: Struct of Sequence.
  * can be one of the values of @ref Spi_SequenceConfigType.
    
  * @retval void None
  * @par Required preconditions:
  * None
  * @par Example:
  * None
  * @code
  *  void Spi_Init( const Spi_ConfigType* ConfigPtr ) 
  * @endcode
  */

 void Spi_Init(const Spi_ConfigType* ConfigPtr ) 
 {
   uint8 sequence_number,job_number,channel_number,ib_number,register_number;
   /* Development error detection*/
    #if (SPI_DEV_ERROR_DETECT == STD_ON)
    Std_ReturnType Det_Return;
    if(Spi_InitFuncFlag == 0x01) 
    {
//     Det_Return = Det_ReportError( SPI_MODULE_ID, 0x00, SPI_INIT_ID, SPI_E_ALREADY_INITIALIZED);
     return;
    }  
    
    for(sequence_number=0x00;sequence_number<SEQUENCE_NUM;sequence_number++) 
    {
      if((ConfigPtr->SpiSequenceConfig[sequence_number].SpiSequenceID) >= SEQUENCE_NUM) 
      {
//       Det_Return = Det_ReportError( SPI_MODULE_ID, 0x00, SPI_INIT_ID, SPI_E_PARAM_SEQ);
       return;
      }
    }
    
    for(job_number=0x00;job_number<RUN_NUM;job_number++) 
    {
      if((ConfigPtr->SpiRunConfig[job_number].SpiRunID) >= RUN_NUM) 
      {
       Det_Return = Det_ReportError( SPI_MODULE_ID, 0x00, SPI_INIT_ID, SPI_E_PARAM_RUN);
       return;
      }
    }
    
    for(channel_number=0x00;channel_number<CHANNEL_NUM;channel_number++) 
    {
      if((ConfigPtr->SpiChannelConfig[channel_number].SpiChannelID) >= CHANNEL_NUM) 
      {
       Det_Return = Det_ReportError( SPI_MODULE_ID, 0x00, SPI_INIT_ID, SPI_E_PARAM_CHANNEL);
       return;
      }
    }

    
    #endif
   /* End development error detecton*/
   
 /*-------------------Initialize jobs----------------------------------------------*/  
    for(job_number=0x00; job_number<RUN_NUM; job_number++) 
    { 
      /*------------ Initialize the temporary registers for jobs-----------------------*/
      for(register_number=0x00; register_number<3; register_number++) 
      {
       SpiRunRegisterValueType[job_number][register_number] = 0x00; 
      }
      /*----------------------------------------------------------------*/
      SpiRunContainChannel[job_number].SpiRunChannelNumber =  ConfigPtr->SpiRunConfig[job_number].SpiRunChannelNumber;    //Taking out the number of channels contained in jobs
      for(channel_number=0x00; channel_number<ConfigPtr->SpiRunConfig[job_number].SpiRunChannelNumber; channel_number++) 
      {
       SpiRunContainChannel[job_number].FixedLinkOfChannels[channel_number] =  ConfigPtr->SpiRunConfig[job_number]. FixedLinkOfChannels[channel_number];   //Taking out the fixed channel ID in jobs
      }
      /*----------------------------------------------------------------*/
      
      Spi_HWofRun[job_number] = ConfigPtr->SpiRunConfig[job_number].SpiRunHWUnit;//Taking out the HW in jobs

      SpiRunEndNotificationPointer[job_number] =  ConfigPtr->SpiRunConfig[job_number].SpiRunEndNotification;//Taking out the Run end notification pointer
      
      switch (ConfigPtr->SpiRunConfig[job_number].SpiRunHWUnit)
        { 
          case 0x00: 
               {if(SPI0_COMPLETE_FLAG == 0x00)    //It indicate that SPI0 has not been initialized
                 {
                    if(ConfigPtr->SpiRunConfig[job_number].SpiClockInWaitMode == SPI_STOP_IN_WAIT_MODE)
                       SpiRunRegisterValueType[job_number][1] |= SPI0CR2_SPISWAI_MASK;
                    else
                       SpiRunRegisterValueType[job_number][1] &= ~SPI0CR2_SPISWAI_MASK;
                    
                    SpiRunRegisterValueType[job_number][0] &= ~SPI0CR1_SPIE_MASK; //SPI interrupts disabled
                    SpiRunRegisterValueType[job_number][0] |= SPI0CR1_MSTR_MASK; //SPI is in master mode.
                    
                    if(ConfigPtr->SpiRunConfig[job_number].SpiRunBarSSInputOutput == SPI_BARSS_NOT_USED) 
                    {
                     SpiRunRegisterValueType[job_number][0] &= ~SPI0CR1_SSOE_MASK;
                     SpiRunRegisterValueType[job_number][1] &= ~SPI0CR2_MODFEN_MASK;
                    }
                    else if(ConfigPtr->SpiRunConfig[job_number].SpiRunBarSSInputOutput == SPI_BARSS_INPUT_MODF_FEATURE)
                    {
                     SpiRunRegisterValueType[job_number][0] &= ~SPI0CR1_SSOE_MASK;
                     SpiRunRegisterValueType[job_number][1] |= SPI0CR2_MODFEN_MASK;
                    }                  
                    else
                    {
                     SpiRunRegisterValueType[job_number][0] |= SPI0CR1_SSOE_MASK;
                     SpiRunRegisterValueType[job_number][1] |= SPI0CR2_MODFEN_MASK;
                    }
                    
                    SpiRunRegisterValueType[job_number][1] |= ConfigPtr->SpiRunConfig[job_number].SpiRunBidirectionalPin; 
                    
                    SpiRunRegisterValueType[job_number][2] |= ConfigPtr->SpiRunConfig[job_number].SpiRunBaudRateDivisor; 
                    
                    if(ConfigPtr->SpiRunConfig[job_number].SpiRunClockPolarity == SPI_ACTIVE_LOW_CLOCKS)
                       SpiRunRegisterValueType[job_number][0] |= SPI0CR1_CPOL_MASK;
                    else 
                       SpiRunRegisterValueType[job_number][0] &= ~SPI0CR1_CPOL_MASK;
                    
                    if(ConfigPtr->SpiRunConfig[job_number].SpiRunSampleDateEdge == SPI_EVEN_EDGES)
                       SpiRunRegisterValueType[job_number][0] |= SPI0CR1_CPHA_MASK;
                    else
                       SpiRunRegisterValueType[job_number][0] &= ~SPI0CR1_CPHA_MASK;
                    
                    Spi_HWstatus[0] = SPI_IDLE;
                    SPI0_COMPLETE_FLAG = 0x01;
                    SpiRunResult[job_number] = SPI_RUN_OK;  //Set jobs result to SPI_RUN_OK
                 }
                 
                 break;
               
               }
          case 0x01: 
               {if(SPI1_COMPLETE_FLAG == 0x00)    //It indicate that SPI1 has not been initialized
                 {
                    if(ConfigPtr->SpiRunConfig[job_number].SpiClockInWaitMode == SPI_STOP_IN_WAIT_MODE) //It indicate that SPI1 has not been initialized
                       SpiRunRegisterValueType[job_number][1] |= SPI1CR2_SPISWAI;
                    else
                       SpiRunRegisterValueType[job_number][1] &= ~SPI1CR2_SPISWAI;
                    
                    //SpiRunRegisterValueType[job_number][0] &= 0x00;  //It ensure that SPI0CR1 is 0x00
                    SpiRunRegisterValueType[job_number][0] |= SPI1CR1_SPIE_MASK; //SPI interrupts enabled
                    SpiRunRegisterValueType[job_number][0] |= SPI1CR1_MSTR_MASK; //SPI is in master mode.
                    
                    if(ConfigPtr->SpiRunConfig[job_number].SpiRunBarSSInputOutput == SPI_BARSS_NOT_USED) 
                    {
                     SpiRunRegisterValueType[job_number][0] &= ~SPI1CR1_SSOE_MASK;
                     SpiRunRegisterValueType[job_number][1] &= ~SPI1CR2_MODFEN_MASK;
                    }
                    else if(ConfigPtr->SpiRunConfig[job_number].SpiRunBarSSInputOutput == SPI_BARSS_INPUT_MODF_FEATURE)
                    {
                     SpiRunRegisterValueType[job_number][0] &= ~SPI1CR1_SSOE_MASK;
                     SpiRunRegisterValueType[job_number][1] |= SPI1CR2_MODFEN_MASK;
                    }                  
                    else
                    {
                     SpiRunRegisterValueType[job_number][0] |= SPI1CR1_SSOE_MASK;
                     SpiRunRegisterValueType[job_number][1] |= SPI1CR2_MODFEN_MASK;
                    }
                    
                    SpiRunRegisterValueType[job_number][1] |= ConfigPtr->SpiRunConfig[job_number].SpiRunBidirectionalPin; 
                   
                    SpiRunRegisterValueType[job_number][2]|= ConfigPtr->SpiRunConfig[job_number].SpiRunBaudRateDivisor; 
                    
                    if(ConfigPtr->SpiRunConfig[job_number].SpiRunClockPolarity == SPI_ACTIVE_LOW_CLOCKS)
                       SpiRunRegisterValueType[job_number][0] |= SPI1CR1_CPOL_MASK;
                    else 
                       SpiRunRegisterValueType[job_number][0] &= ~SPI1CR1_CPOL_MASK;
                    
                    if(ConfigPtr->SpiRunConfig[job_number].SpiRunSampleDateEdge == SPI_EVEN_EDGES)
                       SpiRunRegisterValueType[job_number][0] |= SPI1CR1_CPHA_MASK;
                    else
                       SpiRunRegisterValueType[job_number][0] &= ~SPI1CR1_CPHA_MASK;
                    
                    Spi_HWstatus[1] = SPI_IDLE;
                    SPI1_COMPLETE_FLAG = 0x01;
                    SpiRunResult[job_number] = SPI_RUN_OK;  //Set jobs result to SPI_RUN_OK
                 }
                 
                 break;
                 
               }
               
               case 0x02: 
               { 
                 if(SPI2_COMPLETE_FLAG == 0x00)    //It indicate that SPI2 has not been initialized
                 {
                    if(ConfigPtr->SpiRunConfig[job_number].SpiClockInWaitMode == SPI_STOP_IN_WAIT_MODE) //It indicate that SPI2 has not been initialized
                       SpiRunRegisterValueType[job_number][1] |= SPI2CR2_SPISWAI;
                    else
                       SpiRunRegisterValueType[job_number][1] &= ~SPI2CR2_SPISWAI;
                    
                   // SpiRunRegisterValueType[job_number][0] &= 0x00;  //It ensure that SPI0CR1 is 0x00
                    SpiRunRegisterValueType[job_number][0] |= SPI2CR1_SPIE_MASK; //SPI interrupts enabled
                    SpiRunRegisterValueType[job_number][0] |= SPI2CR1_MSTR_MASK; //SPI is in master mode.
                    
                    if(ConfigPtr->SpiRunConfig[job_number].SpiRunBarSSInputOutput == SPI_BARSS_NOT_USED) 
                    {
                     SpiRunRegisterValueType[job_number][0] &= ~SPI2CR1_SSOE_MASK;
                     SpiRunRegisterValueType[job_number][1] &= ~SPI2CR2_MODFEN_MASK;
                    }
                    else if(ConfigPtr->SpiRunConfig[job_number].SpiRunBarSSInputOutput == SPI_BARSS_INPUT_MODF_FEATURE)
                    {
                     SpiRunRegisterValueType[job_number][0] &= ~SPI2CR1_SSOE_MASK;
                     SpiRunRegisterValueType[job_number][1] |= SPI2CR2_MODFEN_MASK;
                    }                  
                    else
                    {
                     SpiRunRegisterValueType[job_number][0] |= SPI2CR1_SSOE_MASK;
                     SpiRunRegisterValueType[job_number][1] |= SPI2CR2_MODFEN_MASK;
                    }
                    
                    SpiRunRegisterValueType[job_number][1] |= ConfigPtr->SpiRunConfig[job_number].SpiRunBidirectionalPin; 
                    
                    SpiRunRegisterValueType[job_number][2] |= ConfigPtr->SpiRunConfig[job_number].SpiRunBaudRateDivisor; 
                    
                    if(ConfigPtr->SpiRunConfig[job_number].SpiRunClockPolarity == SPI_ACTIVE_LOW_CLOCKS)
                       SpiRunRegisterValueType[job_number][0] |= SPI2CR1_CPOL_MASK;
                    else 
                       SpiRunRegisterValueType[job_number][0] &= ~SPI2CR1_CPOL_MASK;
                   
                    if(ConfigPtr->SpiRunConfig[job_number].SpiRunSampleDateEdge == SPI_EVEN_EDGES)
                       SpiRunRegisterValueType[job_number][0] |= SPI2CR1_CPHA_MASK;
                    else
                       SpiRunRegisterValueType[job_number][0] &= ~SPI2CR1_CPHA_MASK;
                    
                    Spi_HWstatus[2] = SPI_IDLE;
                    SPI2_COMPLETE_FLAG = 0x01;
                    SpiRunResult[job_number] = SPI_RUN_OK;  //Set jobs result to SPI_RUN_OK
                 }
                 
                break;
               
               }   
   
            
            default:break; 
         }
 
     }

 /*-----------------------Initialize channels----------------------------------------------*/  
     for(channel_number=0x00; channel_number<CHANNEL_NUM; channel_number++) 
     { 
      for(register_number=0x00; register_number<2; register_number++) 
      {
       SpiChannelRegisterValueType[channel_number][register_number] = 0x00; // Initialize the temporary registers  for channels
      }
      /*----------set the temporary register-------------*/
      if(ConfigPtr->SpiChannelConfig[channel_number].SpiDataTransferredFirstBit == SPI_LSB_FIRST)
         SpiChannelRegisterValueType[channel_number][0] |= SPI0CR1_LSBFE_MASK;
      else
         SpiChannelRegisterValueType[channel_number][0] &= ~SPI0CR1_LSBFE_MASK;
      
      /*--------------------------------------------------*/
       
       SpiBufferUsage[channel_number] =  ConfigPtr->SpiChannelConfig[channel_number].SpiBufferUsage;  //Taking out the channel buffer usage type
       SpiDefaultValue[channel_number] =  ConfigPtr->SpiChannelConfig[channel_number].Spi_DefaultTransmitValue; //Taking out the default value for each channel 
       
       /*-----------------Initialize the IB/EB buffer---------------------------------*/
       if(ConfigPtr->SpiChannelConfig[channel_number].SpiBufferUsage == SPI_EB_BUFFER)
       { SpiBuffer[channel_number].Spi_BufferUsageEb.SpiDataSrcPtr = NULL;
         SpiBuffer[channel_number].Spi_BufferUsageEb.SpiDataDesPtr = NULL;
       } 
       else 
       { for(ib_number=0x00; ib_number<(ConfigPtr->SpiChannelConfig[channel_number].SpiNumberOfData); ib_number++) 
         {
           SpiBuffer[channel_number].Spi_BufferUsageIb.SpiBuffersTx[ib_number] = ConfigPtr->SpiChannelConfig[channel_number].Spi_DefaultTransmitValue;
           SpiBuffer[channel_number].Spi_BufferUsageIb.SpiBuffersRx[ib_number] = ConfigPtr->SpiChannelConfig[channel_number].Spi_DefaultTransmitValue;
         }
         Number_of_data_transmitted[channel_number] = ConfigPtr->SpiChannelConfig[channel_number].SpiNumberOfData;  //Take out the number of the transmited data to global perameter Number_of_data_transmitted[channel_number]

       }
       /*-----------------------------------------------------------------------------*/
  
     }

 /*-----------------------Initialize sequences----------------------------------------------*/  
     for(sequence_number=0x00; sequence_number<SEQUENCE_NUM; sequence_number++)
     {
      SpiSequenceContainRun[sequence_number].SpiSequenceRunNumber = ConfigPtr->SpiSequenceConfig[sequence_number].SpiSequenceRunNumber; //Taking out the number of jobs contained in sequence
      
      SpiSequenceEndNotificationPointer[sequence_number] = ConfigPtr->SpiSequenceConfig[sequence_number].SpiSequenceEndNotification;//Taking out the sequence end notification pointer
      
      SpiSequenceInterrupt[sequence_number] =  ConfigPtr->SpiSequenceConfig[sequence_number].SpiInterruptibleSequence;
      
      for(job_number=0x00; job_number<ConfigPtr->SpiSequenceConfig[sequence_number].SpiSequenceRunNumber; job_number++)
       {
        SpiSequenceContainRun[sequence_number].FixedLinkOfRun[job_number] =  ConfigPtr->SpiSequenceConfig[sequence_number]. FixedLinkOfRun[job_number]; //Taking out the fixed Run ID in sequence
       }
      
      SpiSeqResult[sequence_number] = SPI_SEQ_OK;
     }
 
     Spi_status = SPI_IDLE;
     Spi_InitFuncFlag = 0x01;
 }

 /**
  * @brief Service for SPI de-initialization.
  * @param[in] None
    
  * @retval Std_ReturnType
  * @par Required preconditions:
  * None
  * @par Example:
  * None
  * @code
  * Std_ReturnType Spi_DeInit( ) 
  * @endcode
  */
  
Std_ReturnType Spi_DeInit( ) 
{ 
 /* Development error detection*/
  #if (SPI_DEV_ERROR_DETECT == STD_ON)
  Std_ReturnType Det_Return;
  if(Spi_status==SPI_UNINIT) 
  {
    Det_Return = Det_ReportError( SPI_MODULE_ID, 0x00, SPI_DEINIT_ID,SPI_E_UNINIT);
    return E_NOT_OK;
  }
 
  #endif
 /* End development error detecton*/
  
  if(Spi_status == SPI_BUSY)
    return E_NOT_OK;

  SPI0CR1 = 0x04;
  SPI0CR2 = 0x00;
  SPI0BR = 0x00;
  SPI0_COMPLETE_FLAG = 0x00;
  
  SPI1CR1 = 0x04;
  SPI1CR2 = 0x00;
  SPI1BR = 0x00;
  SPI1_COMPLETE_FLAG = 0x00;
  
  SPI2CR1 = 0x04;
  SPI2CR2 = 0x00;
  SPI2BR = 0x00;
  SPI2_COMPLETE_FLAG = 0x00;

  Spi_status = SPI_UNINIT;
  return  E_OK;
  
}


 /**
  * @brief Service for writing one or more data to an IB SPI Handler/Driver Channel specified 
  * @param[in] Channel: A Channel is a software exchange medium for data
  * can be one of the values of @ref Spi_ChannelType
  * @param[in] *DataBufferPtr: Pointer to source data buffer.
  * can be one of the values of @ref Spi_DataType
  
  * @retval Std_ReturnType
  * @par Required preconditions:
  * None
  * @par Example:
  * None
  * @code
  * Spi_WriteIB(Spi_ChannelType Channel,const Spi_DataType *DataBufferPtr)
  * @endcode
  */
 #if((SPI_CHANNEL_BUFFERS_ALLOWED == 0x00) ||(SPI_CHANNEL_BUFFERS_ALLOWED == 0x02))
 Std_ReturnType Spi_WriteIB(Spi_ChannelType Channel,const Spi_DataType  *DataBufferPtr)
 {
  uint8 ib_number ;
  /* Development error detection*/
  #if (SPI_DEV_ERROR_DETECT == STD_ON)
  Std_ReturnType Det_Return;
  if(Channel >= CHANNEL_NUM) 
  {
   Det_Return = Det_ReportError( SPI_MODULE_ID, 0x00, SPI_WRITEIB_ID, SPI_E_PARAM_CHANNEL);
   return E_NOT_OK;
  } 
  if(Spi_status==SPI_UNINIT) 
  {
   Det_Return = Det_ReportError(SPI_MODULE_ID, 0x00, SPI_WRITEIB_ID, SPI_E_PARAM_UNIT);
   return E_NOT_OK;
  }
  
  #endif
 /* End development error detecton*/
 
  if(DataBufferPtr != NULL) 
    for(ib_number=0x00; ib_number<Number_of_data_transmitted[Channel]; ib_number++)
    {
      SpiBuffer[Channel].Spi_BufferUsageIb.SpiBuffersTx[ib_number] = DataBufferPtr[ib_number];
    }

  return E_OK;
 }
 #endif


 /**
  * @brief Service for reading synchronously one or more data from an IB SPI Handler/Driver Channel specified by parameter
  * @param[in] Channel: A Channel is a software exchange medium for data
  * can be one of the values of @ref Spi_ChannelType
  * @param[in] *DataBufferPtr: Pointer to source data buffer.
  * can be one of the values of @ref Spi_DataType
  
  * @retval Std_ReturnType
  * @par Required preconditions:
  * None
  * @par Example:
  * None
  * @code
  * Spi_WriteIB(Spi_ChannelType Channel,const Spi_DataType *DataBufferPtr)
  * @endcode
  */
 #if((SPI_CHANNEL_BUFFERS_ALLOWED == 0x00) ||(SPI_CHANNEL_BUFFERS_ALLOWED == 0x02))
  Std_ReturnType Spi_ReadIB(Spi_ChannelType Channel,Spi_DataType *DataBufferPointer)
  {
    uint8 ib_number ;
    /* Development error detection*/
    #if (SPI_DEV_ERROR_DETECT == STD_ON)
    Std_ReturnType Det_Return;
    if(Channel >= CHANNEL_NUM) 
    {
     Det_Return = Det_ReportError( SPI_MODULE_ID, 0x00, SPI_READIB_ID, SPI_E_PARAM_CHANNEL);
     return E_NOT_OK;
    } 
    if(Spi_status==SPI_UNINIT) 
    {
     Det_Return = Det_ReportError(SPI_MODULE_ID, 0x00, SPI_READIB_ID, SPI_E_PARAM_UNIT);
     return E_NOT_OK;
    }    
    
    #endif
   /* End development error detecton*/
    if(DataBufferPointer != NULL) 
      for(ib_number=0x00; ib_number<Number_of_data_transmitted[Channel]; ib_number++)
      {
        *(DataBufferPointer++) =  SpiBuffer[Channel].Spi_BufferUsageIb.SpiBuffersRx[ib_number];
      }

    return E_OK;
  
  }
 #endif


 /**
  * @brief Service to transmit data on the SPI bus.
  * @param[in] Sequence: Sequence ID
  * can be one of the values of @ref Spi_SequenceType

  * @retval Std_ReturnType
  * @par Required preconditions:
  * None
  * @par Example:
  * None.
  * @code
  * Std_ReturnType Spi_AsyncTransmit(Spi_SequenceType Sequence)
  * @endcode
  */
  #if((SPI_LEVEL_DELIVERED==1)||(SPI_LEVEL_DELIVERED==2))
  Std_ReturnType Spi_AsyncTransmit(Spi_SequenceType Sequence)
  {
    uint8  job_number,channel_number,ib_number,ib_number_tx,ib_number_rx,eb_number,eb_number_tx,eb_number_rx;
   
    /* Development error detection*/
   #if (SPI_DEV_ERROR_DETECT == STD_ON)
   Std_ReturnType Det_Return;
   if(Sequence>= SEQUENCE_NUM) 
   {
     Det_Return = Det_ReportError( SPI_MODULE_ID, 0x00, SPI_ASYNCTRANSMIT_ID, SPI_E_PARAM_SEQ);
     return E_NOT_OK;
   }    
   if((SpiSeqResult[Sequence] == SPI_SEQ_PENDING) || (SpiRunResult[SpiSequenceContainRun[Sequence].FixedLinkOfRun[0]] == SPI_RUN_PENDING)) 
   {
     Det_Return = Det_ReportError( SPI_MODULE_ID, 0x00, SPI_ASYNCTRANSMIT_ID, SPI_E_SEQ_PENDING);
     return E_NOT_OK;
   }
   if(Spi_status == SPI_UNINIT) 
   {
     Det_Return = Det_ReportError(SPI_MODULE_ID, 0x00, SPI_ASYNCTRANSMIT_ID, SPI_E_PARAM_UNIT);
     return E_NOT_OK;
   }  
  
   #endif
  /* End development error detecton*/
    
   
   
    #if(SpiInterruptibleSeqAllowed==STD_OFF)
    
    SpiSequenceInterrupt[Sequence] = DISABLE;
  
    #endif
    
    if(SpiSeqResult[Sequence] != SPI_SEQ_OK)
    {
      if((SpiAsyncMode != SPI_INTERRUPT_MODE) || (SpiSequenceInterrupt[LastSequenceID]==DISABLE) )
      {
       SeqOfWaitingTransmitID = Sequence;  // Behavior of Non-Interruptible
       SeqOfWaitingTransmitFlag = 0x01;
      }
    }
    
    else        //implement the asynchronous transmit
    {
    
      SpiCurrentTansmitFunctionMode = SPI_FUNCTION_ASYNCHRONOUS;
      Spi_status = SPI_BUSY;
      LastSequenceID = Sequence;//Taking out the last transmitted sequence ID 
      SpiSeqResult[Sequence] = SPI_SEQ_PENDING;
      
      for(job_number=0x00; job_number<SpiSequenceContainRun[Sequence].SpiSequenceRunNumber; job_number++)
      {
         if(SpiSeqResult[Sequence] == SPI_SEQ_CANCELLED)
           break;
         else 
         { SpiRunResult[job_number] = SPI_RUN_PENDING;
           
           switch(Spi_HWofRun[job_number])
           { case 0x00:  // SPI0
             { SPI0CR1 = 0x00; //clean SPI0CR1
               SPI0CR1 |= SpiRunRegisterValueType[job_number][0];
               SPI0CR2 |= SpiRunRegisterValueType[job_number][1];
               SPI0BR |= SpiRunRegisterValueType[job_number][2];
               
               /*----Initialize the bit of registers referred to channels-------*/
               for(channel_number=0x00; channel_number<SpiRunContainChannel[job_number].SpiRunChannelNumber; channel_number++) 
               { 
                  SPI0CR1 |= SpiChannelRegisterValueType[channel_number][0];
                  SPI0CR2 |= SpiChannelRegisterValueType[channel_number][1];
                  SPI0CR1 |= SPI0CR1_SPE_MASK; //Enable SPI unit
               /*---------------------------------------------------------------*/   
                  Spi_HWstatus[0] = SPI_BUSY;  // Set the status of HW unit
               
               /*----Begin to transmit or receive data according to IB-------*/   
                  if(SpiBufferUsage[channel_number]==SPI_IB_BUFFER) 
                  {  ib_number = 0x00;
                     ib_number_tx = 0x00; // numbers of transmiting
                     ib_number_rx = 0x00; //numbers of receiving
                     
                      while(ib_number<Number_of_data_transmitted[channel_number])
                      { if(SPI0SR & SPI0SR_SPTEF_MASK) 
                        {
                          SPI0DRL = SpiBuffer[channel_number].Spi_BufferUsageIb.SpiBuffersTx[ib_number] ;  // send data to SPI0DR
                          ib_number_tx++;
                        } 
                        
                        if(ib_number_tx > ib_number)
                          ib_number = ib_number_tx ;
                        
                        if(SPI0SR & SPI0SR_SPIF_MASK)
                        {
                          SpiBuffer[channel_number].Spi_BufferUsageIb.SpiBuffersRx[ib_number] = SPI0DRL; //read data from SPI0DR
                          ib_number_rx++;
                        }
    
                        
                        if(ib_number_rx > ib_number)
                          ib_number = ib_number_rx ;
                      
                      }

                  } 
                  /*-------------------------------------------------------------*/
                  /*----Begin to transmit or receive data according to EB-------*/
                  else
                  {  eb_number = 0x00;
                     eb_number_tx = 0x00;
                     eb_number_rx = 0x00;
                     
                     while(eb_number<Number_of_data_transmitted[channel_number])
                     { if(SPI0SR & SPI0SR_SPTEF_MASK) 
                        { if(SpiBuffer[channel_number].Spi_BufferUsageEb.SpiDataSrcPtr != NULL)
                             SPI0DRL = *(SpiBuffer[channel_number].Spi_BufferUsageEb.SpiDataSrcPtr++) ;  // send data to SPI0DR
                          else
                             SPI0DRL = SpiDefaultValue[channel_number];
                          eb_number_tx++;
                        } 
                     
                       if(eb_number_tx > eb_number)
                          eb_number = eb_number_tx ;
                     
                       if(SPI0SR & SPI0SR_SPIF_MASK)
                       { if(SpiBuffer[channel_number].Spi_BufferUsageEb.SpiDataSrcPtr != NULL)
                         {
                           *(SpiBuffer[channel_number].Spi_BufferUsageEb.SpiDataDesPtr++) = SPI0DRL;  // read data from SPI0DR
                           eb_number_rx++;
                         }
                         
                       }
                     
                       if(eb_number_rx > eb_number)
                          eb_number = eb_number_rx ;
                     
                     }
                     
                     
                  
                  }
                  /*-------------------------------------------------------------*/
               }
               SPI0CR1 &= ~SPI0CR1_SPE_MASK; //Disable SPI unit
               Spi_HWstatus[0] = SPI_IDLE;  // Set the status of HW unit
               break;
             }
            
             case 0x01:  // SPI1
             { SPI1CR1 = 0x00; //clean SPI1CR1
               SPI1CR1 |= SpiRunRegisterValueType[job_number][0];
               SPI1CR2 |= SpiRunRegisterValueType[job_number][1];
               SPI1BR |= SpiRunRegisterValueType[job_number][2];
               
               for(channel_number=0x00; channel_number<SpiRunContainChannel[job_number].SpiRunChannelNumber; channel_number++) 
               {  /*----Initialize the bit of registers referred to channels-------*/
                  SPI1CR1 |= SpiChannelRegisterValueType[channel_number][0];
                  SPI1CR2 |= SpiChannelRegisterValueType[channel_number][1];
                  SPI1CR1 |= SPI1CR1_SPE_MASK; //Enable SPI unit
                  /*---------------------------------------------------------------*/
                  
                  Spi_HWstatus[1] = SPI_BUSY;  // Set the status of HW unit
                  
                  /*----Begin to transmit or receive data according to IB-------*/
                  if(SpiBufferUsage[channel_number]==SPI_IB_BUFFER) 
                  {  ib_number = 0x00;
                     ib_number_tx = 0x00;   // numbers of transmiting
                     ib_number_rx = 0x00;   //numbers of receiving
                      
                      while(ib_number<Number_of_data_transmitted[channel_number])
                      { if(SPI1SR & SPI1SR_SPTEF_MASK) 
                        {
                          SPI1DR = SpiBuffer[channel_number].Spi_BufferUsageIb.SpiBuffersTx[ib_number] ;  // send data to SPI1DR
                          ib_number_tx++;
                        } 
                        
                        if(ib_number_tx > ib_number)
                          ib_number = ib_number_tx ;
                        
                        if(SPI1SR & SPI1SR_SPIF_MASK)
                        {
                          SpiBuffer[channel_number].Spi_BufferUsageIb.SpiBuffersRx[ib_number] = SPI1DRL; //read data from SPI1DR
                          ib_number_rx++;
                        }
                        
                        if(ib_number_rx > ib_number)
                          ib_number = ib_number_rx ;
                      
                      }

                  } 
                  /*-------------------------------------------------------------*/
                  /*----Begin to transmit or receive data according to EB-------*/
                  else
                  {  eb_number = 0x00;
                     eb_number_tx = 0x00; // numbers of transmiting
                     eb_number_rx = 0x00;  //numbers of receiving
                     
                     while(eb_number<Number_of_data_transmitted[channel_number])
                     { if(SPI1SR & SPI1SR_SPTEF_MASK) 
                        { if(SpiBuffer[channel_number].Spi_BufferUsageEb.SpiDataSrcPtr != NULL)
                             SPI1DR = *(SpiBuffer[channel_number].Spi_BufferUsageEb.SpiDataSrcPtr++) ;  // send data to SPI1DR
                          else
                             SPI1DR = SpiDefaultValue[channel_number];
                          eb_number_tx++;
                        } 
                     
                       if(eb_number_tx > eb_number)
                          eb_number = eb_number_tx ;
                     
                       if(SPI1SR & SPI1SR_SPIF_MASK)
                       { if(SpiBuffer[channel_number].Spi_BufferUsageEb.SpiDataSrcPtr != NULL)
                         {
                           *(SpiBuffer[channel_number].Spi_BufferUsageEb.SpiDataDesPtr++) = SPI1DRL;  // read data from SPI1DR
                           eb_number_rx++;
                         }
                         
                       }
                     
                       if(eb_number_rx > eb_number)
                          eb_number = eb_number_rx ;
                     
                     }
                  
                  }
                  /*-------------------------------------------------------------*/
               
               }
               SPI1CR1 &= ~SPI1CR1_SPE_MASK; //Disable SPI unit
               Spi_HWstatus[1] = SPI_IDLE;  // Set the status of HW unit
               break;
             }
            
              case 0x02:  // SPI2
             { SPI2CR1 = 0x00; //clean SPI2CR1
               SPI2CR1 |= SpiRunRegisterValueType[job_number][0];
               SPI2CR2 |= SpiRunRegisterValueType[job_number][1];
               SPI2BR |= SpiRunRegisterValueType[job_number][2];
               
               for(channel_number=0x00; channel_number<SpiRunContainChannel[job_number].SpiRunChannelNumber; channel_number++) 
               {  /*----Initialize the bit of registers referred to channels-------*/
                  SPI2CR1 |= SpiChannelRegisterValueType[channel_number][0];
                  SPI2CR2 |= SpiChannelRegisterValueType[channel_number][1];
                  SPI2CR1 |= SPI2CR1_SPE_MASK; //Enable SPI unit
                  /*---------------------------------------------------------------*/
                  Spi_HWstatus[2] = SPI_BUSY;  // Set the status of HW unit
                   
                  /*----Begin to transmit or receive data according to IB-------*/
                  if(SpiBufferUsage[channel_number]==SPI_IB_BUFFER) 
                  {  ib_number = 0x00;
                     ib_number_tx = 0x00;   // numbers of transmiting
                     ib_number_rx = 0x00;   //numbers of receiving
                      
                      while(ib_number<Number_of_data_transmitted[channel_number])
                      { if(SPI2SR & SPI2SR_SPTEF_MASK) 
                        {
                          SPI2DRL = SpiBuffer[channel_number].Spi_BufferUsageIb.SpiBuffersTx[ib_number] ;  // send data to SPI2DRL
                          ib_number_tx++;
                        } 
                        
                        if(ib_number_tx > ib_number)
                          ib_number = ib_number_tx ;
                        
                        if(SPI2SR & SPI2SR_SPIF_MASK)
                        {
                          SpiBuffer[channel_number].Spi_BufferUsageIb.SpiBuffersRx[ib_number] = SPI2DRL; //read data from SPI2DRL
                          ib_number_rx++;
                        }
                        
                        if(ib_number_rx > ib_number)
                          ib_number = ib_number_rx ;
                      
                      }

                  } 
                  /*-------------------------------------------------------------*/
                  /*----Begin to transmit or receive data according to EB-------*/
                  else
                  {  eb_number = 0x00;
                     eb_number_tx = 0x00; // numbers of transmiting
                     eb_number_rx = 0x00;  //numbers of receiving
                     
                     while(eb_number<Number_of_data_transmitted[channel_number])
                     { if(SPI2SR & SPI2SR_SPTEF_MASK) 
                        { if(SpiBuffer[channel_number].Spi_BufferUsageEb.SpiDataSrcPtr != NULL)
                             SPI2DRL = *(SpiBuffer[channel_number].Spi_BufferUsageEb.SpiDataSrcPtr++) ;  // send data to SPI2DR
                          else
                             SPI2DRL = SpiDefaultValue[channel_number];
                          eb_number_tx++;
                        } 
                     
                       if(eb_number_tx > eb_number)
                          eb_number = eb_number_tx ;
                     
                       if(SPI2SR & SPI2SR_SPIF_MASK)
                       { if(SpiBuffer[channel_number].Spi_BufferUsageEb.SpiDataSrcPtr != NULL)
                          {
                           *(SpiBuffer[channel_number].Spi_BufferUsageEb.SpiDataDesPtr++) = SPI2DRL;  // read data from SPI2DR
                           eb_number_rx++;
                          }
                         
                       }
                     
                       if(eb_number_rx > eb_number)
                          eb_number = eb_number_rx ;
                     
                     }
                  
                  }
                  /*-------------------------------------------------------------*/
               
               }
               SPI2CR1 &= ~SPI2CR1_SPE_MASK; //Disable SPI unit
               Spi_HWstatus[2] = SPI_IDLE;  // Set the status of HW unit
               break;
             }
             default: break;
             
           }
           
         
         }
         
          SpiRunResult[job_number] = SPI_RUN_OK;
          
          if(SpiRunEndNotificationPointer[job_number] != NULL)
            (*SpiRunEndNotificationPointer[job_number])(); //Perform the Run end notification function
      }
      SpiSeqResult[Sequence] = SPI_SEQ_OK;
      Spi_status = SPI_IDLE;
      SpiCurrentTansmitFunctionMode = SPI_FUNCTION_NO_RUNNING;
      if(SpiSequenceEndNotificationPointer[Sequence] != NULL)
        (*SpiSequenceEndNotificationPointer[Sequence])(); //Perform the secquence end notification function
     
      return E_OK;
    
    }
    
    if(SeqOfWaitingTransmitFlag)     // Implement the sequence waiting for transmitting
    {
      SeqOfWaitingTransmitFlag = 0x00; // clear the flag of  SeqOfWaitingTransmitFlag
      Spi_AsyncTransmit(SeqOfWaitingTransmitID); // implement the asynchonous transmit
      SeqOfWaitingTransmitID = 0xff;  // clear the variable  of  SeqOfWaitingTransmitID
      
    }

    


  }
  #endif


 /**
  * @brief Service to setup the buffers and the length of data for the EB SPI Handler/Driver Channel specified
  * @param[in] Channel : Channel  ID
  * can be one of the values of @ref Spi_ChannelType
  * @param[in] SrcDataBufferPtr : Pointer to source data buffer
  * can be one of the values of @ref Spi_DataType
  * @param[in] DesDataBufferPtr : Pointer to destination data buffer in RAM. 
  * can be one of the values of @ref Spi_DataType
  * @param[in] Length : Channel  ID
  * can be one of the values of @ref Spi_NumberOfDataType

  * @retval Std_ReturnType
  * @par Required preconditions:
  * None
  * @par Example:
  * None
  * @code
  * Std_ReturnType Spi_SetupEB( Spi_ChannelType  Channel, const Spi_DataType* SrcDataBufferPtr,Spi_DataType* DesDataBufferPtr, Spi_NumberOfDataType Length)
  * @endcode
  */
 #if((SPI_CHANNEL_BUFFERS_ALLOWED == 0x01) ||(SPI_CHANNEL_BUFFERS_ALLOWED == 0x02))
 Std_ReturnType Spi_SetupEB( Spi_ChannelType  Channel, const Spi_DataType* SrcDataBufferPtr,
                             Spi_DataType* DesDataBufferPtr, Spi_NumberOfDataType Length)
 
 {
  /* Development error detection*/
  #if (SPI_DEV_ERROR_DETECT == STD_ON)
  Std_ReturnType Det_Return;
  if(Channel >= CHANNEL_NUM) 
  {
   Det_Return = Det_ReportError( SPI_MODULE_ID, 0x00, SPI_SETUPEB_ID, SPI_E_PARAM_CHANNEL);
   return E_NOT_OK;
  } 
  if(Spi_status==SPI_UNINIT) 
  {
   Det_Return = Det_ReportError(SPI_MODULE_ID, 0x00, SPI_SETUPEB_ID, SPI_E_PARAM_UNIT);
   return E_NOT_OK;
  }
  
  #endif
 /* End development error detecton*/
 
  Number_of_data_transmitted[Channel] = Length;
  
  SpiBuffer[Channel].Spi_BufferUsageEb.SpiDataSrcPtr =  SrcDataBufferPtr;
 
  SpiBuffer[Channel].Spi_BufferUsageEb.SpiDataDesPtr =  DesDataBufferPtr;

  return E_OK;
 }
 #endif


 /**
  * @brief Service returns the SPI Handler/Driver software module status
  * @param[in] Channel : None

  * @retval Spi_StatusType
  * @par Required preconditions:
  * None
  * @par Example:
  * None
  * @code
  * Spi_StatusType Spi_GetStatus( )
  * @endcode
  */

 Spi_StatusType Spi_GetStatus( )
 {
   return Spi_status; 
 }


/**
  * @brief This service returns the last transmission result of the specified Run.
  * @param[in] Run : Run ID.
  * can be one of the values of @ref Spi_RunType

  * @retval Spi_RunResultType
  * @par Required preconditions:
  * None
  * @par Example:
  * None
  * @code
  * Spi_RunResultType Spi_GetRunResult( Spi_RunType Run)
  * @endcode
  */

  Spi_RunResultType Spi_GetRunResult( Spi_RunType Run)
  {
   /* Development error detection*/
   #if (SPI_DEV_ERROR_DETECT == STD_ON)
   Std_ReturnType Det_Return;
   if(Run >= RUN_NUM) 
   {
    Det_Return = Det_ReportError( SPI_MODULE_ID, 0x00, SPI_GETJOBRESULT_ID, SPI_E_PARAM_RUN);
    return E_NOT_OK;
   } 
   if(Spi_status==SPI_UNINIT) 
   {
     Det_Return = Det_ReportError(SPI_MODULE_ID, 0x00, SPI_GETJOBRESULT_ID, SPI_E_PARAM_UNIT);
     return E_NOT_OK;
   }
   #endif
  /* End development error detecton*/

   return SpiRunResult[Run];
  
  }


/**
  * @brief This service returns the last transmission result of the specified Sequence.
  * @param[in] Sequence : Sequence ID.
  * can be one of the values of @ref Spi_SequenceType

  * @retval Spi_SeqResultType
  * @par Required preconditions:
  * None
  * @par Example:
  * None
  * @code
  * Spi_SeqResultType Spi_GetSequenceResult(Spi_SequenceType Sequence)
  * @endcode
  */

  Spi_SeqResultType Spi_GetSequenceResult(Spi_SequenceType Sequence)
  {
   /* Development error detection*/
   #if (SPI_DEV_ERROR_DETECT == STD_ON)
   Std_ReturnType Det_Return;
   if(Sequence >= SEQUENCE_NUM) 
   {
    Det_Return = Det_ReportError( SPI_MODULE_ID, 0x00, SPI_GETSEQUENCERESULT_ID, SPI_E_PARAM_SEQ);
    return E_NOT_OK;
   }
   if(Spi_status==SPI_UNINIT) 
   {
     Det_Return = Det_ReportError(SPI_MODULE_ID, 0x00, SPI_GETSEQUENCERESULT_ID, SPI_E_PARAM_UNIT);
     return E_NOT_OK;
   }
   #endif
  /* End development error detecton*/

   return SpiSeqResult[Sequence];
  
  }


/**
  * @brief Returns the version information of the module
  * @param[out] versioninfo: Pointer to where to store the version information of this module.
  * can be the values of @ref Std_VersionInfoType*.
  * @retval void None
  * @par Required preconditions:
  * None
  * @code
  * void Spi_GetVersionInfo(Std_VersionInfoType* versioninfo )
  * @endcode
  */


#if SPI_VERSION_INFO_API 
void Spi_GetVersionInfo (Std_VersionInfoType*  versioninfo) 

 {   
     versioninfo->vendorID = (uint16)SPI_VENDOR_ID;
     versioninfo->moduleID = (uint16)SPI_MODULE_ID;
     versioninfo->sw_major_version = (uint8)SPI_SW_MAJOR_VERSION;
     versioninfo->sw_minor_version = (uint8)SPI_SW_MINOR_VERSION;
     versioninfo->sw_patch_version = (uint8)SPI_SW_PATCH_VERSION;
 }

#endif



/**
  * @brief This service returns the last transmission result of the specified Sequence.
  * @param[in]  HWUnit: SPI Hardware microcontroller peripheral (unit) ID. 
  * can be one of the values of @ref Spi_HWUnitType

  * @retval Spi_StatusType
  * @par Required preconditions:
  * None
  * @par Example:
  * None
  * @code
  * Spi_StatusType Spi_GetHWUnitStatus(Spi_HWUnitType HWUnit)
  * @endcode
  */
  #if (SPI_HW_STATUS_API == STD_ON)
  Spi_StatusType Spi_GetHWUnitStatus(Spi_HWUnitType HWUnit)
  {
   /* Development error detection*/
   #if (SPI_DEV_ERROR_DETECT == STD_ON)
   Std_ReturnType Det_Return;
   if(HWUnit>=3)
   {
    Det_Return = Det_ReportError( SPI_MODULE_ID, 0x00, SPI_GETHWUNITSTATUS_ID, SPI_E_PARAM_UNIT);
    return E_NOT_OK;
   }
   if(Spi_status==SPI_UNINIT) 
   {
     Det_Return = Det_ReportError(SPI_MODULE_ID, 0x00, SPI_GETHWUNITSTATUS_ID, SPI_E_PARAM_UNIT);
     return E_NOT_OK;
   }
   #endif
  /* End development error detecton*/

   return  Spi_HWstatus[HWUnit];
  
  }
  
  #endif


/**
  * @brief Service cancels the specified on-going sequence transmission. 
  * @param[in] Sequence : Sequence ID.
  * can be one of the values of @ref Spi_SequenceType

  * @retval None
  * @par Required preconditions:
  * None
  * @par Example:
  * None
  * @code
  * void Spi_Cancel( Spi_SequenceType Sequence )
  * @endcode
  */
  #if( SPI_CANCEL_API == STD_ON)
  void Spi_Cancel( Spi_SequenceType Sequence )
  {
   /* Development error detection*/
   #if (SPI_DEV_ERROR_DETECT == STD_ON)
   Std_ReturnType Det_Return;
   if(Sequence >= SEQUENCE_NUM) 
   {
    Det_Return = Det_ReportError( SPI_MODULE_ID, 0x00, SPI_CANCLE_ID, SPI_E_PARAM_SEQ);
    return ;
   }

   #endif
  /* End development error detecton*/
   
    SpiSeqResult[Sequence] = SPI_SEQ_CANCELLED;
  
  }
  #endif


 /**
  * @brief Service to transmit data on the SPI bus  
  * @param[in] Sequence : Sequence ID.
  * can be one of the values of @ref Spi_SequenceType

  * @retval Std_ReturnType
  * @par Required preconditions:
  * None
  * @par Example:
  * None
  * @code
  * Std_ReturnType Spi_SyncTransmit(Spi_SequenceType Sequence)
  * @endcode
  */
  #if((SPI_LEVEL_DELIVERED==0)||(SPI_LEVEL_DELIVERED==2))
  Std_ReturnType Spi_SyncTransmit(Spi_SequenceType Sequence)
  { uint8  job_number,channel_number,ib_number,ib_number_tx,ib_number_rx,eb_number,eb_number_tx,eb_number_rx;
   
   /* Development error detection*/
   #if (SPI_DEV_ERROR_DETECT == STD_ON)
   Std_ReturnType Det_Return;
   if(Sequence>= SEQUENCE_NUM) 
   {
     Det_Return = Det_ReportError( SPI_MODULE_ID, 0x00, SPI_SYNCTRANSMIT_ID, SPI_E_PARAM_SEQ);
     return E_NOT_OK;
   } 
   if(Spi_status == SPI_BUSY) 
   {
    Det_Return = Det_ReportError( SPI_MODULE_ID, 0x00, SPI_SYNCTRANSMIT_ID, SPI_E_SEQ_IN_PROCESS);
    return E_NOT_OK;
   }
   if(Spi_status==SPI_UNINIT) 
   {
     Det_Return = Det_ReportError(SPI_MODULE_ID, 0x00, SPI_SYNCTRANSMIT_ID, SPI_E_PARAM_UNIT);
     return E_NOT_OK;
   }
  
   #endif
  /* End development error detecton*/
    
    SpiCurrentTansmitFunctionMode = SPI_FUNCTION_SYNCHRONOUS;
    Spi_status = SPI_BUSY;
    SpiSeqResult[Sequence] = SPI_SEQ_PENDING;
    
    for(job_number=0x00; job_number<SpiSequenceContainRun[Sequence].SpiSequenceRunNumber; job_number++)
    {
       if(SpiSeqResult[Sequence] == SPI_SEQ_CANCELLED)
         break;
       else 
       { SpiRunResult[job_number] = SPI_RUN_PENDING;
         
         switch(Spi_HWofRun[job_number])
         { case 0x00:  // SPI0
           { SPI0CR1 = 0x00; //clean SPI0CR1
             SPI0CR1 |= SpiRunRegisterValueType[job_number][0];
             SPI0CR2 |= SpiRunRegisterValueType[job_number][1];
             SPI0BR |= SpiRunRegisterValueType[job_number][2];
             
             /*----Initialize the bit of registers referred to channels-------*/
             for(channel_number=0x00; channel_number<SpiRunContainChannel[job_number].SpiRunChannelNumber; channel_number++) 
             { 
                SPI0CR1 |= SpiChannelRegisterValueType[channel_number][0];
                SPI0CR2 |= SpiChannelRegisterValueType[channel_number][1];
                SPI0CR1 |= SPI0CR1_SPE_MASK; //Enable SPI unit
             /*---------------------------------------------------------------*/   
                Spi_HWstatus[0] = SPI_BUSY;  // Set the status of HW unit
             
             /*----Begin to transmit or receive data according to IB-------*/   
                if(SpiBufferUsage[channel_number]==SPI_IB_BUFFER) 
                {  ib_number = 0x00;
                   ib_number_tx = 0x00; // numbers of transmiting
                   ib_number_rx = 0x00; //numbers of receiving
                   
                    while(ib_number<Number_of_data_transmitted[channel_number])
                    { if(SPI0SR & SPI0SR_SPTEF_MASK) 
                      {
                        SPI0DRL = SpiBuffer[channel_number].Spi_BufferUsageIb.SpiBuffersTx[ib_number] ;  // send data to SPI0DR
                        ib_number_tx++;
                      } 
                      
                      if(ib_number_tx > ib_number)
                        ib_number = ib_number_tx ;
                      
                      if(SPI0SR & SPI0SR_SPIF_MASK)
                      {
                        SpiBuffer[channel_number].Spi_BufferUsageIb.SpiBuffersRx[ib_number] = SPI0DRL; //read data from SPI0DR
                        ib_number_rx++;
                      }
                      
                      if(ib_number_rx > ib_number)
                        ib_number = ib_number_rx ;
                    
                    }

                } 
                /*-------------------------------------------------------------*/
                /*----Begin to transmit or receive data according to EB-------*/
                else
                {  eb_number = 0x00;
                   eb_number_tx = 0x00;
                   eb_number_rx = 0x00;
                   
                   while(eb_number<Number_of_data_transmitted[channel_number])
                   { if(SPI0SR & SPI0SR_SPTEF_MASK) 
                      { if(SpiBuffer[channel_number].Spi_BufferUsageEb.SpiDataSrcPtr != NULL)
                           SPI0DRL = *(SpiBuffer[channel_number].Spi_BufferUsageEb.SpiDataSrcPtr++) ;  // send data to SPI0DR
                        else
                           SPI0DRL = SpiDefaultValue[channel_number];
                        eb_number_tx++;
                      } 
                   
                     if(eb_number_tx > eb_number)
                        eb_number = eb_number_tx ;
                   
                     if(SPI0SR & SPI0SR_SPIF_MASK)
                     { if(SpiBuffer[channel_number].Spi_BufferUsageEb.SpiDataSrcPtr != NULL)
                       {
                         *(SpiBuffer[channel_number].Spi_BufferUsageEb.SpiDataDesPtr++) = SPI0DRL;  // read data from SPI0DR
                         eb_number_rx++;
                       }
                       
                     }
                   
                     if(eb_number_rx > eb_number)
                        eb_number = eb_number_rx ;
                   
                   }
                   
                   
                
                }
                /*-------------------------------------------------------------*/
             }
             SPI0CR1 &= ~SPI0CR1_SPE_MASK; //Disable SPI unit
             Spi_HWstatus[0] = SPI_IDLE;  // Set the status of HW unit
             break;
           }
          
           case 0x01:  // SPI1
           { SPI1CR1 = 0x00; //clean SPI1CR1
             SPI1CR1 |= SpiRunRegisterValueType[job_number][0];
             SPI1CR2 |= SpiRunRegisterValueType[job_number][1];
             SPI1BR |= SpiRunRegisterValueType[job_number][2];
             
             for(channel_number=0x00; channel_number<SpiRunContainChannel[job_number].SpiRunChannelNumber; channel_number++) 
             {  /*----Initialize the bit of registers referred to channels-------*/
                SPI1CR1 |= SpiChannelRegisterValueType[channel_number][0];
                SPI1CR2 |= SpiChannelRegisterValueType[channel_number][1];
                SPI1CR1 |= SPI1CR1_SPE_MASK; //Enable SPI unit
                /*---------------------------------------------------------------*/
                
                Spi_HWstatus[1] = SPI_BUSY;  // Set the status of HW unit
                
                /*----Begin to transmit or receive data according to IB-------*/
                if(SpiBufferUsage[channel_number]==SPI_IB_BUFFER) 
                {  ib_number = 0x00;
                   ib_number_tx = 0x00;   // numbers of transmiting
                   ib_number_rx = 0x00;   //numbers of receiving
                    
                    while(ib_number<Number_of_data_transmitted[channel_number])
                    { if(SPI1SR & SPI1SR_SPTEF_MASK) 
                      {
                        SPI1DR = SpiBuffer[channel_number].Spi_BufferUsageIb.SpiBuffersTx[ib_number] ;  // send data to SPI1DR
                        ib_number_tx++;
                      } 
                      
                      if(ib_number_tx > ib_number)
                        ib_number = ib_number_tx ;
                      
                      if(SPI1SR & SPI1SR_SPIF_MASK)
                      {
                        SpiBuffer[channel_number].Spi_BufferUsageIb.SpiBuffersRx[ib_number] = SPI1DRL; //read data from SPI1DR
                        ib_number_rx++;
                      }
                      
                      if(ib_number_rx > ib_number)
                        ib_number = ib_number_rx ;
                    
                    }

                } 
                /*-------------------------------------------------------------*/
                /*----Begin to transmit or receive data according to EB-------*/
                else
                {  eb_number = 0x00;
                   eb_number_tx = 0x00; // numbers of transmiting
                   eb_number_rx = 0x00;  //numbers of receiving
                   
                   while(eb_number<Number_of_data_transmitted[channel_number])
                   { if(SPI1SR & SPI1SR_SPTEF_MASK) 
                      { if(SpiBuffer[channel_number].Spi_BufferUsageEb.SpiDataSrcPtr != NULL)
                           SPI1DR = *(SpiBuffer[channel_number].Spi_BufferUsageEb.SpiDataSrcPtr++) ;  // send data to SPI1DR
                        else
                           SPI1DR = SpiDefaultValue[channel_number];
                        eb_number_tx++;
                      } 
                   
                     if(eb_number_tx > eb_number)
                        eb_number = eb_number_tx ;
                   
                     if(SPI1SR & SPI1SR_SPIF_MASK)
                     { if(SpiBuffer[channel_number].Spi_BufferUsageEb.SpiDataSrcPtr != NULL)
                       {
                         *(SpiBuffer[channel_number].Spi_BufferUsageEb.SpiDataDesPtr++) = SPI1DRL;  // read data from SPI1DR
                         eb_number_rx++;
                       }
                       
                     }
                   
                     if(eb_number_rx > eb_number)
                        eb_number = eb_number_rx ;
                   
                   }
                
                }
                /*-------------------------------------------------------------*/
             
             }
             SPI1CR1 &= ~SPI1CR1_SPE_MASK; //Disable SPI unit
             Spi_HWstatus[1] = SPI_IDLE;  // Set the status of HW unit
             break;
           }
          
            case 0x02:  // SPI2
           { SPI2CR1 = 0x00; //clean SPI2CR1
             SPI2CR1 |= SpiRunRegisterValueType[job_number][0];
             SPI2CR2 |= SpiRunRegisterValueType[job_number][1];
             SPI2BR |= SpiRunRegisterValueType[job_number][2];
             
             for(channel_number=0x00; channel_number<SpiRunContainChannel[job_number].SpiRunChannelNumber; channel_number++) 
             {  /*----Initialize the bit of registers referred to channels-------*/
                SPI2CR1 |= SpiChannelRegisterValueType[channel_number][0];
                SPI2CR2 |= SpiChannelRegisterValueType[channel_number][1];
                SPI2CR1 |= SPI2CR1_SPE_MASK; //Enable SPI unit
                /*---------------------------------------------------------------*/
                Spi_HWstatus[2] = SPI_BUSY;  // Set the status of HW unit
                 
                /*----Begin to transmit or receive data according to IB-------*/
                if(SpiBufferUsage[channel_number]==SPI_IB_BUFFER) 
                {  ib_number = 0x00;
                   ib_number_tx = 0x00;   // numbers of transmiting
                   ib_number_rx = 0x00;   //numbers of receiving
                    
                    while(ib_number<Number_of_data_transmitted[channel_number])
                    { if(SPI2SR & SPI2SR_SPTEF_MASK) 
                      {
                        SPI2DRL = SpiBuffer[channel_number].Spi_BufferUsageIb.SpiBuffersTx[ib_number] ;  // send data to SPI2DRL
                        ib_number_tx++;
                      } 
                      
                      if(ib_number_tx > ib_number)
                        ib_number = ib_number_tx ;
                      
                      if(SPI2SR & SPI2SR_SPIF_MASK)
                      {
                        SpiBuffer[channel_number].Spi_BufferUsageIb.SpiBuffersRx[ib_number] = SPI2DRL; //read data from SPI2DRL
                        ib_number_rx++;
                      }
                      
                      if(ib_number_rx > ib_number)
                        ib_number = ib_number_rx ;
                    
                    }

                } 
                /*-------------------------------------------------------------*/
                /*----Begin to transmit or receive data according to EB-------*/
                else
                {  eb_number = 0x00;
                   eb_number_tx = 0x00; // numbers of transmiting
                   eb_number_rx = 0x00;  //numbers of receiving
                   
                   while(eb_number<Number_of_data_transmitted[channel_number])
                   { if(SPI2SR & SPI2SR_SPTEF_MASK) 
                      { if(SpiBuffer[channel_number].Spi_BufferUsageEb.SpiDataSrcPtr != NULL)
                           SPI2DRL = *(SpiBuffer[channel_number].Spi_BufferUsageEb.SpiDataSrcPtr++) ;  // send data to SPI2DR
                        else
                           SPI2DRL = SpiDefaultValue[channel_number];
                        eb_number_tx++;
                      } 
                   
                     if(eb_number_tx > eb_number)
                        eb_number = eb_number_tx ;
                   
                     if(SPI2SR & SPI2SR_SPIF_MASK)
                     { if(SpiBuffer[channel_number].Spi_BufferUsageEb.SpiDataSrcPtr != NULL)
                        {
                         *(SpiBuffer[channel_number].Spi_BufferUsageEb.SpiDataDesPtr++) = SPI2DRL;  // read data from SPI2DR
                         eb_number_rx++;
                        }
                       
                     }
                   
                     if(eb_number_rx > eb_number)
                        eb_number = eb_number_rx ;
                   
                   }
                
                }
                /*-------------------------------------------------------------*/
             
             }
             SPI2CR1 &= ~SPI2CR1_SPE_MASK; //Disable SPI unit
             Spi_HWstatus[2] = SPI_IDLE;  // Set the status of HW unit
             break;
           }
           default: break;
    
         }
         
       
       }
       
        SpiRunResult[job_number] = SPI_RUN_OK;
    
    }
    SpiSeqResult[Sequence] = SPI_SEQ_OK;
    Spi_status = SPI_IDLE;
    SpiCurrentTansmitFunctionMode = SPI_FUNCTION_NO_RUNNING;
    return E_OK;
  }
  #endif

 /**
  * @brief Service to set the asynchronous mechanism mode for SPI busses handled asynchronously.
  * @param[in] Mode : New mode required.
  * can be one of the values of @ref Spi_AsyncModeType

  * @retval Std_ReturnType
  * @par Required preconditions:
  * None
  * @par Example:
  * None
  * @code
  * Std_ReturnType Spi_SetAsyncMode( Spi_AsyncModeType Mode)
  * @endcode
  */
  #if(SPI_LEVEL_DELIVERED == 0x02)
  Std_ReturnType Spi_SetAsyncMode( Spi_AsyncModeType Mode)
  {
   /* Development error detection*/
   #if (SPI_DEV_ERROR_DETECT == STD_ON)
   Std_ReturnType Det_Return;
   if((Spi_status != SPI_IDLE) && (SpiCurrentTansmitFunctionMode == SPI_FUNCTION_ASYNCHRONOUS)) 
   {
     Det_Return = Det_ReportError(SPI_MODULE_ID, 0x00, SPI_SETASYNCMODE_ID, SPI_E_PARAM_UNIT);
     return E_NOT_OK;
   }
   
   
   #endif
  /* End development error detecton*/
   
    if((SpiCurrentTansmitFunctionMode == SPI_FUNCTION_NO_RUNNING)||(SpiCurrentTansmitFunctionMode == SPI_FUNCTION_SYNCHRONOUS)) 
    {
      SpiAsyncMode = Mode;
      return E_OK;
    } 
    else
    {
      return E_NOT_OK;
    }
  }
  #endif


 /**
  * @brief Service to Run call back function
  * @param[in] None

  * @retval None
  * @par Required preconditions:
  * None
  * @par Example:
  * None
  * @code
  * None
  * @endcode
  */

  void  Spi_JobCallBackFunction()
  {uint8 temp_job;
   temp_job =0x01;
  }


  /**
  * @brief Service to sequence call back function
  * @param[in] None

  * @retval None
  * @par Required preconditions:
  * None
  * @par Example:
  * None
  * @code
  * None
  * @endcode
  */

  void  Spi_SequenceCallBackFunction()
  {
    uint8 temp_sequence;
    
    temp_sequence =0x01;
  }










#endif
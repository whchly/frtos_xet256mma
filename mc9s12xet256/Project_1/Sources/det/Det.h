/*-------------------------------------------------------------------------------------
| File Name   :   Det.h
| Project Name:  
| Author      :  
| Affiliation : 
|                
|
| Description :   
| Version     :   
|--------------------------------------------------------------------------------------
| Copyright (c
|------------------------------------------------------------------------------------*/

/*
***************************************************************************************
*									Define to prevent recursive inclusion
***************************************************************************************
*/
#ifndef _DET_H
  #define _DET_H
	
/*
***************************************************************************************
*                 Include
***************************************************************************************
*/

#include "Std_Types.h"

/*
***************************************************************************************
*                 Det function switch Define
***************************************************************************************
*/

#define DET_VERSION_INFO_API STD_ON
#define DET_ERROR_HOOK STD_ON

#define DET_INIT 0x01//DET已初始化

/*
***************************************************************************************
*                 Version information Define
***************************************************************************************
*/
#if (DET_VERSION_INFO_API==STD_ON)
  #define DET_MODULE_ID                   15
  #define DET_VENDOR_ID                    0
  #define DET_SW_MAJOR_VERSION             1 
  #define DET_SW_MINOR_VERSION             0 
  #define DET_SW_PATCH_VERSION             0 
  #define DET_AR_RELEASE_MAJOR_VERSION     0 
  #define DET_AR_RELEASE_MINOR_VERSION     0 
  #define DET_AR_RELEASE_REVISION_VERSION  0 
#endif

/*
***************************************************************************************
*                 Development error of each module Define
***************************************************************************************
*/

/*-------------------------LIN模块错误宏定义------------------------------------*/
#define LIN_E_UNINIT           0x00
#define LIN_E_CHANNEL_UNINIT   0x01
#define LIN_E_INVALID_CHANNEL  0x02
#define LIN_E_INVALID_POINTER  0x03
#define LIN_E_STATE_TRANSITION 0x04
#define LIN_E_TIMEOUT             //  Assigned by DEM


/*-------------------------ADC模块错误宏定义------------------------------------*/
#define ADC_E_UNINIT               0x0a 
#define ADC_E_BUSY                 0x0b
#define ADC_E_IDLE                 0x0c
#define ADC_E_ALREADY_INITIALIZED  0x0d
#define ADC_E_PARAM_CONFIG         0x0e
#define ADC_E_PARAM_GROUP          0x15
#define ADC_E_WRONG_CONV_MODE      0x16
#define ADC_E_WRONG_TRIGG_SRC      0x17
#define ADC_E_NOTIF_CAPABILITY     0x18
#define ADC_E_BUFFER_UNINIT        0x19

/*-------------------------EEP模块错误宏定义------------------------------------*/
#define EEP_E_PARAM_CONFIG  0x10
#define EEP_E_PARAM_ADDRESS 0x11
#define EEP_E_PARAM_DATA    0x12
#define EEP_E_PARAM_LENGTH  0x13                            
#define EEP_E_UNINIT        0x20
#define EEP_E_BUSY          0x21
#define EEP_E_COM_FAILURE //Assigned externally

/*-----------------------FLS模块错误宏定义-----------------------------------*/
#define FLS_E_PARAM_CONFIG         0x01
#define FLS_E_PARAM_ADDRESS        0x02
#define FLS_E_PARAM_LENGTH         0x03
#define FLS_E_PARAM_DATA           0x04
#define FLS_E_UNINIT               0x05
#define FLS_E_BUSY                 0x06
#define FLS_E_VERIFY_ERASE_FAILED  0x07
#define FLS_E_VERIFY_WRITE_FAILED  0x08
#define FLS_E_ERASE_FAILED           //Assigned by DEM 
#define FLS_E_WRITE_FAILED           //Assigned by DEM 
#define FLS_E_READ_FAILED            //Assigned by DEM 
#define FLS_E_COMPARE_FAILED         // Assigned by DEM 
#define FLS_E_UNEXPECTED_FLASH_ID    // Assigned by DEM 

/*-----------------------WDG模块错误宏定义--------------------------------------*/
#define WDG_E_DRIVER_STATE   0x10
#define WDG_E_PARAM_MODE     0x11
#define WDG_E_PARAM_CONFIG   0x12
#define WDG_E_MODE_SWITCH_FAILED  //Assigned by DEM 
#define WDG_E_DISABLE_REJECTED    //Assigned by DEM 

/*-----------------------GPT模块错误宏定义--------------------------------------*/
#define	GPT_E_UNINIT              0x0A
#define	GPT_E_BUSY                0x0B
#define	GPT_E_NOT_STARTED         0x0C
#define	GPT_E_ALREADY_INITIALIZED 0x0D
#define GPT_E_PARAM_CONFIG        0x0E
#define	GPT_E_PARAM_CHANNEL       0x14
#define	GPT_E_PARAM_VALUE         0x15
#define	GPT_E_PARAM_MODE          0x1F

/*-----------------------ICU模块错误宏定义--------------------------------------*/
#define	ICU_E_PARAM_CONFIG				0x0A
#define	ICU_E_PARAM_CHANNEL				0x0B
#define	ICU_E_PARAM_ACTIVATION		0x0C
#define	ICU_E_PARAM_BUFFER_PTR		0x0D
#define	ICU_E_PARAM_BUFFER_SIZE 	0x0E
#define	ICU_E_PARAM_MODE					0x0F
#define	ICU_E_UNINIT							0x14
#define	ICU_E_NOT_STARTED					0x15
#define	ICU_E_BUSY_OPERATION			0x16
#define	ICU_E_ALREADY_INITIALIZED	0x17

/*-----------------------PWM模块错误宏定义--------------------------------------*/
#define	PWM_E_PARAM_CONFIG          0x10
#define	PWM_E_UNINIT                0x11
#define	PWM_E_PARAM_CHANNEL         0x12
#define	PWM_E_PERIOD_UNCHANGEABLE   0x13
#define	PWM_E_ALREADY_INITIALIZED   0x14

/*-----------------------PORT模块错误宏定义--------------------------------------*/
#define	PORT_E_PARAM_PIN                0x0A
#define	PORT_E_DIRECTION_UNCHANGEABLE   0x0B
#define	PORT_E_PARAM_CONFIG             0x0C
#define	PORT_E_PARAM_INVALID_MODE       0x0D
#define	PORT_E_MODE_UNCHANGEABLE        0x0E
#define	PORT_E_UNINIT	                  0x0F

/*-------------------------MCU模块错误宏定义------------------------------------*/
#define MCU_E_PARAM_CONFIG              0x0A
#define MCU_E_PARAM_CLOCK               0x0B
#define MCU_E_PARAM_MODE                0x0C
#define MCU_E_PARAM_RAMSECTION          0x0D
#define MCU_E_PLL_NOT_LOCKED            0x0E
#define MCU_E_UNINIT                    0x0F

/*-------------------------SPI模块错误宏定义------------------------------------*/
#define SPI_E_PARAM_CHANNEL             0x0A
#define SPI_E_PARAM_RUN                 0x0B
#define SPI_E_PARAM_SEQ                 0x0C
#define SPI_E_PARAM_LENGTH              0x0D
#define SPI_E_PARAM_UNIT                0x0E
#define SPI_E_UNINIT                    0x1A
#define SPI_E_SEQ_PENDING               0x2A
#define SPI_E_SEQ_IN_PROCESS            0x3A
#define SPI_E_ALREADY_INITIALIZED       0x4A 

  /*-------------------------DIO模块错误宏定义------------------------------------*/
#define DIO_E_PARAM_INVALID_CHANNEL_ID  10
#define DIO_E_PARAM_INVALID_PORT_ID     20
#define DIO_E_PARAM_INVALID_GROUP_ID    31


/*-------------------------CAN模块错误宏定义------------------------------------*/

#define CAN_E_PARAM_POINTER     0x01
#define CAN_E_PARAM_HANDLE      0x02
#define CAN_E_PARAM_DLC         0x03
#define CAN_E_PARAM_CONTROLLER  0x04	
#define CAN_E_UNINIT	          0x05
#define CAN_E_TRANSITION	      0x06


/*
***************************************************************************************
*     Types define
***************************************************************************************
*/

typedef struct{
	uint16 ModuleId;
	uint8  InstanceId;
	uint8  ApiId;
	uint8  ErrorId;
}Det_ErrorBufferType;

/************************全局变量****************************************************/
#define BUFF_NUM   32  //根据自己需要可以改变Error_Report数组的大小
extern Det_ErrorBufferType Error_Report[BUFF_NUM];//错误值存储
extern uint16 Error_Counter;

/*
**************************************************************************************
*     DET's functions for standard
**************************************************************************************
*/

void Det_Init(void);
Std_ReturnType Det_ReportError( uint16 ModuleId, uint8 InstanceId, uint8 ApiId, uint8 ErrorId);
void Det_Start(void);

#if (DET_VERSION_INFO_API==STD_ON)
  void Det_GetVersionInfo(Std_VersionInfoType* versioninfo);
#endif

/*************************************************************************************/
#endif
/*************************************************************************************/

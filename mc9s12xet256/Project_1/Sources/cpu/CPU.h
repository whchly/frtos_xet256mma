/*==========================================================================*/
/* PROJECT: 															*/
/* MODULE: CPU.h															*/
/* SHORT DESC: Header file of CPU core.										*/
/* IDE: CodeWarrior IDE version 5.9										*/
/* DEVICE: S12XET256MMA														*/
/* VERSION: 1.0																*/
/* DATE: 															*/
/* MODEL: Non																*/
/* AUTHOR: 																*/
/* COPYRIGHT: 															*/
/* -------------------------------------------------------------------------*/
/* Description: Function declaration of PLL, watch dog and RTI.				*/
/*																			*/
/* -------------------------------------------------------------------------*/
/* Changes: 									*/
/*==========================================================================*/

#ifndef _CPU_H_
#define	_CPU_H_
#include "Platform_Types.h"


/*---- Global variable declaration------------------------------------------*/

/*---- Global const declaration---------------------------------------------*/

/*---- Global macro definition----------------------------------------------*/
/* 16MHz oscillator: */
#define SYNR_VALUE      0x44
#define REFDV_VALUE     0x81
#define POSTDIV_VALUE   0x00


/* #define U4G_CPU_BUS_CLK		( ( u4 )48000000 ) */

/*---- Global function declaration------------------------------------------*/

/* MCU core initilalization */
extern void vdg_CPU_PLLInit(uint8 synr, uint8 refdv, uint8 postdiv);

/* RTI initilalization */
void vdg_CPU_InitRTI( uint16 systemtickHz );

/* COP initilalization */	
extern void vdg_CPU_InitCop( void );

/* feed dog operation */	
extern void vdg_CPU_FdDog( void );		

#endif	/* end of _CPU_H_ */
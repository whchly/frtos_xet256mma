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

#ifndef _PIT_H_
#define	_PIT_H_
#include "Platform_Types.h"  

/*---- Global variable declaration------------------------------------------*/

typedef enum
{
    PIT_TIM0 = 0,
    PIT_TIM1,
    PIT_TIM2,
    PIT_TIM3   
}PIT_CHX;

/*---- Global const declaration---------------------------------------------*/

/*---- Global macro definition----------------------------------------------*/



/* #define U4G_CPU_BUS_CLK		( ( u4 )48000000 ) */

/*---- Global function declaration------------------------------------------*/

/* PIT initilalization */
extern void vdg_PIT_Init(PIT_CHX pchx, uint8 ms);
		

#endif	/* end of _CPU_H_ */
/*==========================================================================*/
/* PROJECT: 															*/
/* MODULE: CPU.c															*/
/* SHORT DESC: Source code of CPU core.										*/
/* IDE: CodeWarrior IDE version 5.9										*/
/* DEVICE: 														*/
/* VERSION: 																*/
/* DATE: 2013-08-20															*/
/* MODEL: Non																*/
/* AUTHOR: 																*/
/* COPYRIGHT: 														*/
/* -------------------------------------------------------------------------*/
/* Description: Functions of PLL, watch dog and API.						*/
/*																			*/
/* -------------------------------------------------------------------------*/
/* Changes: 								*/
/*==========================================================================*/

/*---- Module Header Includes ----------------------------------------------*/
#include "Std_Types.h"
#include <MC9S12XET256.h>
#include "PIT.h"
/*---- Static macro definition----------------------------------------------*/
#define U1S_PIT_TIM0EN 0x01
#define U1S_PIT_TIM0DE 0x00  



/*---- Static variable definition-------------------------------------------*/
 typedef enum
{
    PIT_TIM0_DISABLE = 0,
    PIT_TIM0_ENABLE    
};  

/*---- Global variable definition-------------------------------------------*/

/*---- Static constant definition-------------------------------------------*/

/*---- Global constant definition-------------------------------------------*/

/*---- Static function declaration------------------------------------------*/

/********************source code*********************************************/

/****************************************************************************/
/* function name: vdg_PIT_Init(PITChn pChn,uint8 ms)											*/
/* parameter : non															*/
/* return    : non															*/
/* description: Initialization of PLL										*/
/****************************************************************************/
void vdg_PIT_Init(PIT_CHX pchx, uint8 ms)
{  

    if(pchx == PIT_TIM0)
    {   
        /*定时中断通道0使能*/
        PITCE_PCE0 = U1S_PIT_TIM0EN;                  
        PITMUX_PMUX0=0;        
        PITMTLD0 = 160 -1;        
        PITLD0 = ms-1; 
        PITINTE_PINTE0=0;                
    }    
    if(pchx == PIT_TIM1)
    {
       ; 
    }   
    if(pchx == PIT_TIM2)
    {
        ;
    }    
    if(pchx == PIT_TIM3)
    {
         ;
    }                 
 }

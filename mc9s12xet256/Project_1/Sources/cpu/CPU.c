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
#include "Platform_Types.h"
#include <MC9S12XET256.h>
#include "CPU.h"
/*---- Static macro definition----------------------------------------------*/

#define U1S_CPU_OSCCLK          8000000     /*OSC is 8MHZ*/

#define U1S_CPU_CLRPROT			0x26		/* clear PROT */
#define U1S_CPU_STPLLEVCO		0x00		/* POSTDIV = 0 to set Fpll = Fvco */
#define U1S_CPU_STREFEOSO		0x80		/* set 6MHz <= Fref <= 12MHz, Fref = Fosc */
#define U1S_CPU_ENOSC			0x80		/* enable external Oscillator */
#define U1S_CPU_VCOE96			0xC5		/* set 80MHz < Fvco <= 100MHz, Fvco = 96MHz */
#define U1S_CPU_CLRFLG			0xFF		/* clear all flags, especially LOCKIF and OSCIF */
#define U1S_CPU_CPMLG_BTS		0x09		/* shift number */
#define U1S_CPU_STPROT			0x00		/* set PROT bit to protect registers of PLL */
#define U1S_CPU_SLTBUSCLK		0x86		/* select bus clock */
#define U1S_CPU_WRAPIFE			1			/* Writable only if APIFE = 0 */
#define U1S_CPU_FDCOP1			0x55		/* feed the COP watch dog */
#define U1S_CPU_FDCOP2			0xAA		/* feed the COP watch dog */
#define U1S_CPU_ENCOP			0x43		/* enable COP */
#define U2S_CPU_APIR_K_48		48000		/* Period = 1ms */
#define U1S_CPU_APIR_K_2		2			/* Is coefficient */
#define U1S_CPU_APIR_K_1		1			/* Is coefficient */
#define U1S_CPU_COPCLK			0x01		/* COP clock source */

#define setReg8(RegName, val) (RegName = (byte)(val))




/*---- Static variable definition-------------------------------------------*/

/*---- Global variable definition-------------------------------------------*/

/*---- Static constant definition-------------------------------------------*/

/*---- Global constant definition-------------------------------------------*/

/*---- Static function declaration------------------------------------------*/

/********************source code*********************************************/

/****************************************************************************/
/* function name: vdg_CPU_Init()											*/
/* parameter : non															*/
/* return  : non															*/
/* description: Initialization of PLL										*/
/****************************************************************************/
void vdg_CPU_PLLInit(uint8 synr, uint8 refdv, uint8 postdiv)
{  
    /* Select clock source from XTAL and set bits in CLKSEL reg. */ 
    setReg8(CLKSEL, 3);  
                    
    /* PLLCTL: CME=1,PLLON=0,FM1=0,FM0=0,FSTWKP=0,PRE=0,PCE=0,SCME=1 */
    setReg8(PLLCTL, 129);               
     
    /* SYNR: VCOFRQ1=0,VCOFRQ0=1,SYNDIV5=0,SYNDIV4=0,SYNDIV3=0,SYNDIV2=1,SYNDIV1=0,SYNDIV0=0 */
    setReg8(SYNR, synr);                   /* Set the multiplier register */ 
    /* REFDV: REFFRQ1=1,REFFRQ0=0,REFDIV5=0,REFDIV4=0,REFDIV3=0,REFDIV2=0,REFDIV1=0,REFDIV0=1 */
    setReg8(REFDV, refdv);                 /* Set the divider register */ 
    /* POSTDIV:=0,=0,,POSTDIV4=0,POSTDIV3=0,POSTDIV2=0,POSTDIV1=0,POSTDIV0=0 */
    setReg8(POSTDIV, postdiv);                 /* Set the post divider register */ 
    /* PLLCTL: CME=1,PLLON=1,FM1=0,FM0=0,FSTWKP=0,PRE=0,PCE=0,SCME=1 */
    setReg8(PLLCTL, 193);                 
    while(!CRGFLG_LOCK) {                /* Wait until the PLL is within the desired tolerance of the target frequency */
    }
    /* CLKSEL: PLLSEL=1 */
    CLKSEL_PLLSEL = 1;            /* Select clock source from PLL */  
}


/****************************************************************************/
/* function name: vdg_CPU_InitApi()											*/
/* parameter : non															*/
/* return  : non															*/
/* description: Initialization of Autonomous Periodical Interrupt			*/
/****************************************************************************/

void vdg_CPU_InitRTI( uint16 systemtickHz  )
{
    /*计算装载值*/
    uint32 u4_tick =0;
    u4_tick = U1S_CPU_OSCCLK/systemtickHz;
            
    /* RTICTL的值是通过计算得到*/
    RTICTL= systemtickHz;
    CRGINT = 0x80;/*enable RTI Interrupt*/

}
/****************************************************************************/
/* function name: vdg_CPU_InitCop()											*/
/* parameter : non															*/
/* return  : non															*/
/* description: Initialization of watch dog									*/
/****************************************************************************/
void vdg_CPU_InitCop( void )
{

}


/****************************************************************************/
/* function name: vdg_CPU_FdDog()											*/
/* parameter : non															*/
/* return  : non															*/
/* description: reset the watch dog timer									*/
/****************************************************************************/
void vdg_CPU_FdDog( void )
{
}







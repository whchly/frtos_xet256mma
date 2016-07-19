/*==========================================================================*/
/* PROJECT: 															*/
/* MODULE: CPU.c															*/
/* SHORT DESC: Source code of CPU core.										*/
/* IDE: CodeWarrior IDE version 5.9										*/
/* DEVICE: 														*/
/* VERSION: 																*/
/* DATE: 															*/
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
#include "INT.h"   

#pragma CODE_SEG __NEAR_SEG NON_BANKED
static interrupt void vds_INT_UnimplementedISR(void);
static interrupt void vds_INT_RtiISR(void); 
static interrupt void vds_INT_Pit0ISR(void);
#pragma CODE_SEG DEFAULT


 
extern void near _Startup(void);
typedef void (*near tFunc)(void);
const tFunc rst_vec @0xEFFE = _Startup;



/*
 *  This is an interurpt vector table located at 0x7F80 - 0x7FFF. 
 *  The interrupt vector base is set in main function to 0x7F.
 *  See the .prm file - the flash segment ROM_4000 must be cut of to area 0x4000-0x7F7F
 */

typedef void (*near tIsrFunc)(void);
const tIsrFunc VectorTable[] @0x7F10 = 
{
  vds_INT_UnimplementedISR,    // Vector base + 0x10   Spurious interrupt
  vds_INT_UnimplementedISR,    // Vector base + 0x12   System Call Interrupt (SYS)
  vds_INT_UnimplementedISR,    // Vector base + 0x14   MPU Access Error
  vds_INT_UnimplementedISR,    // Vector base + 0x16   XGATE software error interrupt
  vds_INT_UnimplementedISR,    // Vector base + 0x18   Reserved
  vds_INT_UnimplementedISR,    // Vector base + 0x1A   Reserved
  vds_INT_UnimplementedISR,    // Vector base + 0x1C   Reserved
  vds_INT_UnimplementedISR,    // Vector base + 0x1E   Reserved
  vds_INT_UnimplementedISR,    // Vector base + 0x20   Reserved
  vds_INT_UnimplementedISR,    // Vector base + 0x22   Reserved
  vds_INT_UnimplementedISR,    // Vector base + 0x24   Reserved
  vds_INT_UnimplementedISR,    // Vector base + 0x26   Reserved
  vds_INT_UnimplementedISR,    // Vector base + 0x28   Reserved
  vds_INT_UnimplementedISR,    // Vector base + 0x2A   Reserved
  vds_INT_UnimplementedISR,    // Vector base + 0x2C   Reserved
  vds_INT_UnimplementedISR,    // Vector base + 0x2E   Reserved
  vds_INT_UnimplementedISR,    // Vector base + 0x30   Reserved
  vds_INT_UnimplementedISR,    // Vector base + 0x32   Reserved
  vds_INT_UnimplementedISR,    // Vector base + 0x34   Reserved
  vds_INT_UnimplementedISR,    // Vector base + 0x36   Reserved
  vds_INT_UnimplementedISR,    // Vector base + 0x38   Reserved
  vds_INT_UnimplementedISR,    // Vector base + 0x3A   Reserved
  vds_INT_UnimplementedISR,    // Vector base + 0x3C   ATD1 Compare Interrupt
  vds_INT_UnimplementedISR,    // Vector base + 0x3E   ATD0 Compare Interrupt
  vds_INT_UnimplementedISR,    // Vector base + 0x40   TIM Pulse accumulator input edge
  vds_INT_UnimplementedISR,    // Vector base + 0x42   TIM Pulse accumulator A overflow
  vds_INT_UnimplementedISR,    // Vector base + 0x44   TIM timer overflow
  vds_INT_UnimplementedISR,    // Vector base + 0x46   TIM timer channel 7
  vds_INT_UnimplementedISR,    // Vector base + 0x48   TIM timer channel 6
  vds_INT_UnimplementedISR,    // Vector base + 0x4A   TIM timer channel 5
  vds_INT_UnimplementedISR,    // Vector base + 0x4C   TIM timer channel 4
  vds_INT_UnimplementedISR,    // Vector base + 0x4E   TIM timer channel 3
  vds_INT_UnimplementedISR,    // Vector base + 0x50   TIM timer channel 2
  vds_INT_UnimplementedISR,    // Vector base + 0x52   TIM timer channel 1
  vds_INT_UnimplementedISR,    // Vector base + 0x54   TIM timer channel 0
  vds_INT_UnimplementedISR,    // Vector base + 0x56   SCI7
  vds_INT_UnimplementedISR,    // Vector base + 0x58   Periodic interrupt timer channel 7
  vds_INT_UnimplementedISR,    // Vector base + 0x5A   Periodic interrupt timer channel 6
  vds_INT_UnimplementedISR,    // Vector base + 0x5C   Periodic interrupt timer channel 5
  vds_INT_UnimplementedISR,    // Vector base + 0x5E   Periodic interrupt timer channel 4
  vds_INT_UnimplementedISR,    // Vector base + 0x60   Reserved
  vds_INT_UnimplementedISR,    // Vector base + 0x62   Reserved
  vds_INT_UnimplementedISR,    // Vector base + 0x64   XGATE software trigger 7
  vds_INT_UnimplementedISR,    // Vector base + 0x66   XGATE software trigger 6
  vds_INT_UnimplementedISR,    // Vector base + 0x68   XGATE software trigger 5
  vds_INT_UnimplementedISR,    // Vector base + 0x6A   XGATE software trigger 4
  vds_INT_UnimplementedISR,    // Vector base + 0x6C   XGATE software trigger 3
  vds_INT_UnimplementedISR,    // Vector base + 0x6E   XGATE software trigger 2
  vds_INT_UnimplementedISR,    // Vector base + 0x70   XGATE software trigger 1
  vds_INT_UnimplementedISR,    // Vector base + 0x72   XGATE software trigger 0
  vds_INT_UnimplementedISR,    // Vector base + 0x74   Periodic interrupt timer channel 3
  vds_INT_UnimplementedISR,    // Vector base + 0x76   Periodic interrupt timer channel 2
  vds_INT_UnimplementedISR,    // Vector base + 0x78   Periodic interrupt timer channel 1
  vds_INT_Pit0ISR,    // Vector base + 0x7A   Periodic interrupt timer channel 0
  vds_INT_UnimplementedISR,    // Vector base + 0x7C   High Temperature Interrupt (HTI)
  vds_INT_UnimplementedISR,    // Vector base + 0x7E   Autonomous periodical interrupt (API)
  vds_INT_UnimplementedISR,    // Vector base + 0x80   Low-voltage interrupt (LVI)
  vds_INT_UnimplementedISR,    // Vector base + 0x82   IIC1 Bus
  vds_INT_UnimplementedISR,    // Vector base + 0x84   SCI5
  vds_INT_UnimplementedISR,    // Vector base + 0x86   SCI4
  vds_INT_UnimplementedISR,    // Vector base + 0x88   SCI3
  vds_INT_UnimplementedISR,    // Vector base + 0x8A   SCI2
  vds_INT_UnimplementedISR,    // Vector base + 0x8C   PWM emergency shutdown
  vds_INT_UnimplementedISR,    // Vector base + 0x8E   Port P Interrupt
  vds_INT_UnimplementedISR,    // Vector base + 0x90   CAN4 transmit
  vds_INT_UnimplementedISR,    // Vector base + 0x92   CAN4 receive
  vds_INT_UnimplementedISR,    // Vector base + 0x94   CAN4 errors
  vds_INT_UnimplementedISR,    // Vector base + 0x96   CAN4 wake-up
  vds_INT_UnimplementedISR,    // Vector base + 0x98   CAN3 transmit
  vds_INT_UnimplementedISR,    // Vector base + 0x9A   CAN3 receive
  vds_INT_UnimplementedISR,    // Vector base + 0x9C   CAN3 errors
  vds_INT_UnimplementedISR,    // Vector base + 0x9E   CAN3 wake-up
  vds_INT_UnimplementedISR,    // Vector base + 0xA0   CAN2 transmit
  vds_INT_UnimplementedISR,    // Vector base + 0xA2   CAN2 receive
  vds_INT_UnimplementedISR,    // Vector base + 0xA4   CAN2 errors
  vds_INT_UnimplementedISR,    // Vector base + 0xA6   CAN2 wake-up
  vds_INT_UnimplementedISR,    // Vector base + 0xA8   CAN1 transmit
  vds_INT_UnimplementedISR,    // Vector base + 0xAA   CAN1 receive
  vds_INT_UnimplementedISR,    // Vector base + 0xAC   CAN1 errors
  vds_INT_UnimplementedISR,    // Vector base + 0xAE   CAN1 wake-up
  vds_INT_UnimplementedISR,    // Vector base + 0xB0   CAN0 transmit
  vds_INT_UnimplementedISR,    // Vector base + 0xB2   CAN0 receive
  vds_INT_UnimplementedISR,    // Vector base + 0xB4   CAN0 errors
  vds_INT_UnimplementedISR,    // Vector base + 0xB6   CAN0 wake-up
  vds_INT_UnimplementedISR,    // Vector base + 0xB8   FLASH
  vds_INT_UnimplementedISR,    // Vector base + 0xBA   FLASH Fault Detect
  vds_INT_UnimplementedISR,    // Vector base + 0xBC   SPI2
  vds_INT_UnimplementedISR,    // Vector base + 0xBE   SPI1
  vds_INT_UnimplementedISR,    // Vector base + 0xC0   IIC0 bus
  vds_INT_UnimplementedISR,    // Vector base + 0xC2   SCI6
  vds_INT_UnimplementedISR,    // Vector base + 0xC4   CRG self-clock mode
  vds_INT_UnimplementedISR,    // Vector base + 0xC6   CRG PLL lock
  vds_INT_UnimplementedISR,    // Vector base + 0xC8   Pulse accumulator B overflow
  vds_INT_UnimplementedISR,    // Vector base + 0xCA   Modulus down counter underflow
  vds_INT_UnimplementedISR,    // Vector base + 0xCC   Port H
  vds_INT_UnimplementedISR,    // Vector base + 0xCE   Port J
  vds_INT_UnimplementedISR,    // Vector base + 0xD0   ATD1
  vds_INT_UnimplementedISR,    // Vector base + 0xD2   ATD0
  vds_INT_UnimplementedISR,    // Vector base + 0xD4   SCI1
  vds_INT_UnimplementedISR,    // Vector base + 0xD6   SCI0     
  vds_INT_UnimplementedISR,    // Vector base + 0xD8   SPI0     
  vds_INT_UnimplementedISR,    // Vector base + 0xDA   TIM Pulse accumulator input edge      
  vds_INT_UnimplementedISR,    // Vector base + 0xDC   TIM Pulse accumulator A overflow      
  vds_INT_UnimplementedISR,    // Vector base + 0xDE   TIM timer overflow      
  vds_INT_UnimplementedISR,    // Vector base + 0xE0   TIM timer channel 7      
  vds_INT_UnimplementedISR,    // Vector base + 0xE2   TIM timer channel 6      
  vds_INT_UnimplementedISR,    // Vector base + 0xE4   TIM timer channel 5      
  vds_INT_UnimplementedISR,    // Vector base + 0xE6   TIM timer channel 4      
  vds_INT_UnimplementedISR,    // Vector base + 0xE8   TIM timer channel 3      
  vds_INT_UnimplementedISR,    // Vector base + 0xEA   TIM timer channel 2      
  vds_INT_UnimplementedISR,    // Vector base + 0xEC   TIM timer channel 1      
  vds_INT_UnimplementedISR,    // Vector base + 0xEE   TIM timer channel 0      
  vds_INT_RtiISR,              // Vector base + 0xF0   Real time interrupt
  vds_INT_UnimplementedISR,    // Vector base + 0xF2   IRQ      
  vds_INT_UnimplementedISR,    // Vector base + 0xF4   XIRQ      
  vds_INT_UnimplementedISR,    // Vector base + 0xF6   SWI      
  vds_INT_UnimplementedISR     // Vector base + 0xF8   Unimplemented instruction trap      
};

/*---- Static macro definition----------------------------------------------*/


/*---- Static variable definition-------------------------------------------*/

/*---- Global variable definition-------------------------------------------*/

/*---- Static constant definition-------------------------------------------*/

/*---- Global constant definition-------------------------------------------*/

/*---- Static function declaration------------------------------------------*/

/********************source code*********************************************/

void  vdg_INT_Init(void)
{
   IVBR = 0x7F;  /*set vector base*/ 
}
/****************************************************************************/
/* function name: vds_INT_UnimplementedISR()								*/
/* parameter : non															*/
/* return  : non															*/
/* description: 										                    */
/****************************************************************************/

#pragma CODE_SEG __NEAR_SEG NON_BANKED
static interrupt void vds_INT_UnimplementedISR(void)
{
  asm BGND; /*software breakpoint*/
}
#pragma CODE_SEG DEFAULT



/****************************************************************************/
/* function name: vdg_CPU_Init()											*/
/* parameter : non															*/
/* return  : non															*/
/* description: Initialization of PLL										*/
/****************************************************************************/

#pragma CODE_SEG __NEAR_SEG NON_BANKED

static interrupt void vds_INT_RtiISR(void)
{
    CRGFLG |= 0x80;/* Write 1 to clear RTIF bit */
    vPortTickInterrupt();
    
}
#pragma CODE_SEG DEFAULT

/****************************************************************************/
/* function name: vdg_CPU_Init()											*/
/* parameter : non															*/
/* return  : non															*/
/* description: Initialization of PLL										*/
/****************************************************************************/

#pragma CODE_SEG __NEAR_SEG NON_BANKED

static interrupt void vds_INT_Pit0ISR(void)
{
  PITTF = 0x01;/*清除中断标示位*/
  /*应用代码*/
  
    
}
#pragma CODE_SEG DEFAULT





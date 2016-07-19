#ifndef MSCANSTD_H
#define MSCANSTD_H     

/******************************************************************************
 * These defines allow for easier porting to other compilers. 
 ******************************************************************************/
typedef unsigned char   uint8;
typedef unsigned int    uint16;
typedef unsigned long   uint32;
typedef char            int8;
typedef int             int16;
typedef long            int32;

/******************************************************************************
 * Boolean values
 ******************************************************************************/
#ifndef TRUE
#define TRUE    (1)
#endif

#ifndef FALSE
#define FALSE   (0)
#endif

/*****************************************************************************/
#endif        

/* end msCANstd.h */ 

#define _QNIO_H__

/* modify this according to I2C device address when you use standard I2C function like I2C_XXXX except AI2C_XXXX*/
#define I2C_DEV0_ADDRESS 0x56
#define I2C_TIMEOUT_TIME    10
#define I2C_TIMEOUT_COUNT    8
#define MS_DELAY_CONST    40
/************end*********************/



/****************************************************************************************************************************
** The following routines is for LPC214X ARM I/O definition. It is just as reference for program using 2-wire and 3-wire bus.
** Pls make a further revision for your routines according to the following type.
****************************************************************************************************************************/
#define SDA 3  //P0.3
#define SCL 2  //P0.2
#define SEB 5  //P0.5
#define MOD 8  //P0.8
#define MCK 9  //P0.9
#define CEN 10 //P0.10
#define WS 7   //P0.7
#define DIN 4  //P0.4

#define SCL_1   (IO0SET=IO0SET|(1<<SCL))
#define SCL_0   (IO0CLR=IO0CLR|(1<<SCL))

#define SDA_1   (IO0SET=IO0SET|(1<<SDA))
#define SDA_0   (IO0CLR=IO0CLR|(1<<SDA))

#define SEB_1   (IO0SET=IO0SET|(1<<SEB))
#define SEB_0   (IO0CLR=IO0CLR|(1<<SEB))

#define MOD_1   (IO0SET=IO0SET|(1<<MOD))
#define MOD_0   (IO0CLR=IO0CLR|(1<<MOD))

#define MCK_1   (IO0SET=IO0SET|(1<<MCK))
#define MCK_0   (IO0CLR=IO0CLR|(1<<MCK))

#define CEN_1   (IO0SET=IO0SET|(1<<CEN))
#define CEN_0   (IO0CLR=IO0CLR|(1<<CEN))

#define WS_1   (IO0SET=IO0SET|(1<<WS))
#define WS_0   (IO0CLR=IO0CLR|(1<<WS))

#define DIN_1   (IO0SET=IO0SET|(1<<DIN))  //TX MODE
#define DIN_0   (IO0CLR=IO0CLR|(1<<DIN))  //RX MODE

#define Read_SDA      (IO0PIN>>SDA & 1) 
#define Set_Datain    (IO0DIR =IO0DIR & (~(1<<SDA)))
#define Set_Dataout   (IO0DIR =IO0DIR|(1<<SDA))

/***************************************************************************************************/

extern void Msdelay(UINT16 dly);
extern UINT8 ChipReset(UINT8 Slave) ;



extern UINT8 QND_ReadReg(UINT8 adr);
extern UINT8 QND_WriteReg(UINT8 adr, UINT8 value);



/** the following functions is for other I2C devices rather than QN800X ***/
extern UINT8 QND_I2C_WRITE(UINT8 Regis_Addr,UINT8 Data);
extern UINT8 QND_I2C_READ(UINT8 Regis_Addr);
extern UINT8 QND_I2C_NREAD(UINT8 Regis_Addr, UINT8 *buf, UINT8 n);
extern UINT8 QND_I2C_NWRITE(UINT8 Regis_Addr, UINT8 *buf, UINT8 n);


#include <stdio.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>


#include "qndriver.h"
#include "qnio.h"


#define SPRINTF(a)

UINT8 qnd_i2c;
UINT8 qnd_i2c_timeout = 0;

/*****************************************************************************************************
** Name:      Msdelay()
** Function:  time delay
*****************************************************************************************************/



/*
void Msdelay(UINT16 dly)
{ 
    usleep(dly*100);
}
*/


extern int i2c_file;
#define I2C_FILE_NAME "/dev/i2c-0"


static int I2C_Read_nbyte(unsigned char address, unsigned char reg,
                          unsigned char *inbuf, int length) {

    unsigned char inoutbuf[length+1];
    struct i2c_rdwr_ioctl_data packets;
    struct i2c_msg messages[1];
    qnd_i2c_timeout = 0;

    inoutbuf[0] = reg;

    messages[0].addr    = address|1;
    messages[0].flags   = 0;
    messages[0].len     = sizeof(inoutbuf);
    messages[0].buf     = inoutbuf;

    packets.msgs = messages;
    packets.nmsgs = 1;

    if(!i2c_file)
        i2c_file = open(I2C_FILE_NAME, O_RDWR);

    if(ioctl(i2c_file, I2C_RDWR, &packets) < 0) {
        perror("Unable to write/read data");
        qnd_i2c_timeout = 1;
        return 0;
    }

    memcpy(inbuf, inoutbuf+1, length);
    return 1;
}


static int I2C_Read_1byte(unsigned char address, unsigned char reg) {
    unsigned char inoutbuf[2];
    struct i2c_rdwr_ioctl_data packets;
    struct i2c_msg messages[2];

    qnd_i2c_timeout = 0;
    inoutbuf[0] = reg;

    messages[0].addr    = address|1;
    messages[0].flags   = 0;
    messages[0].len     = sizeof(inoutbuf);
    messages[0].buf     = inoutbuf;

    packets.msgs = messages;
    packets.nmsgs = 1;

    if(!i2c_file)
        i2c_file = open(I2C_FILE_NAME, O_RDWR);

    if(ioctl(i2c_file, I2C_RDWR, &packets) < 0) {
        perror("Unable to write/read data");
        qnd_i2c_timeout = 1;
        return 0;
    }

    return inoutbuf[1];
}


    
static int I2C_Write_nbyte(unsigned char address, unsigned char reg,
                           unsigned char *outbuf, int length) {

    char buf[length+1];
    qnd_i2c_timeout = 0;

    if(!i2c_file)
        i2c_file = open(I2C_FILE_NAME, O_RDWR);

    // Begin by setting the accelerometer's current register to X.
    if (ioctl(i2c_file, I2C_SLAVE, address) < 0) {
        /* ERROR HANDLING; you can check errno to see what went wrong */
        perror("Unable to assign slave address");
        return 1;
    }

    // Write to register 0x07, the MODE register.
    buf[0] = reg;
    memcpy(&(buf[1]), outbuf, length);
    if(write(i2c_file, buf, length+1) != length+1) {
        perror("Unable to write value");
        qnd_i2c_timeout = 1;
        return 0;
    }
    return 1;
}

static int I2C_Write_1byte(unsigned char address, unsigned char reg,
                           unsigned char data) {
    return I2C_Write_nbyte(address, reg, &data, 1);
}

 


UINT8 QND_WriteReg(UINT8 Regis_Addr,UINT8 Data)
{


	return QND_I2C_WRITE(Regis_Addr,Data);

    return 1;
}

UINT8 QND_ReadReg(UINT8 Regis_Addr)
{
    UINT8 Data;



	Data = QND_I2C_READ(Regis_Addr);

    return Data;
}

UINT8 QND_I2C_WRITE(UINT8 Regis_Addr,UINT8 Data)
{
    UINT8 ret;
    UINT8 tryCount = I2C_TIMEOUT_COUNT;
	qnd_i2c = 1;
    while(--tryCount) {
        ret = I2C_Write_1byte(I2C_DEV0_ADDRESS, Regis_Addr, Data);
        if(ret) break;
    }
    if(!tryCount) {
	SPRINTF(("QND_I2C_WRITE error!\n"));	
		return 0;
    }
	qnd_i2c = 0;
	return 1;
}

UINT8 QND_I2C_READ(UINT8 Regis_Addr)
{
    UINT8 ret;
    UINT8 tryCount = I2C_TIMEOUT_COUNT;
	qnd_i2c = 1;
    while(--tryCount) {
	ret = I2C_Read_1byte(I2C_DEV0_ADDRESS, Regis_Addr);
        if(!qnd_i2c_timeout) break;
    }
    if(!tryCount) {
	SPRINTF(("QND_I2C_READ error!\n"));	
    }
	qnd_i2c = 0;
	return ret;
}

UINT8 QND_I2C_NREAD(UINT8 Regis_Addr, UINT8 *buf, UINT8 n)
{
    UINT8 ret;
    UINT8 tryCount = I2C_TIMEOUT_COUNT;
    qnd_i2c = 1;
    while(--tryCount) {
        ret = I2C_Read_nbyte(I2C_DEV0_ADDRESS, Regis_Addr, buf, n);
        if(!qnd_i2c_timeout) break;
    }
    if(!tryCount) {
        SPRINTF(("QND_I2C_READ error!\n"));	
    }
    qnd_i2c = 0;
    return ret;
}

UINT8 QND_I2C_NWRITE(UINT8 Regis_Addr, UINT8 *buf, UINT8 n)
{
    UINT8 ret;
    UINT8 tryCount = I2C_TIMEOUT_COUNT;
    qnd_i2c = 1;
    while(--tryCount) {
        ret = I2C_Write_nbyte(I2C_DEV0_ADDRESS, Regis_Addr, buf, n);
        if(!qnd_i2c_timeout) break;
    }
    if(!tryCount) {
	    SPRINTF(("QND_I2C_READ error!\n"));	
    }
    qnd_i2c = 0;
    return ret;
}



/*************************************************************************************************************
                              end file
 ************************************************************************************************************/

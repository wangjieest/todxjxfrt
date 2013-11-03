#ifndef __I2C_H__
#define __I2C_H__

#include "pltcfg.h"

#define I2C_CLK_100K 16
#define I2C_CLK_400K 10

#define I2C_NACK 0x01
#define I2C_ACK  0x00


typedef struct I2C_Operate {
	void (*ConfigPin)(PIN_t, PIN_t);	
	void (*init)(void);
	void (*start)(void);
	void (*stop)(void);
	void (*write)(unsigned char);
	unsigned char (*read)(void);
	unsigned char (*getack)(void);
	void (*putack)(unsigned char);
} I2C_OPS_t;

extern unsigned char g_I2C_Speed;
extern I2C_OPS_t I2COps;

#endif // __I2C_H__

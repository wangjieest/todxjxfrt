#include "i2c_sim.h"



#define GET_PIN_BB_ADDR(PORT, PIN, REG) \
(PERIPH_BB_BASE + ((((unsigned int)&((PORT)##->##REG)) - PERIPH_BASE) << 5) + ((PIN) << 2))

#define SET_SCL do { *SCLPinW = 0x1; } while (0)
#define RST_SCL do { *SCLPinW = 0x0; } while (0)
// #define GET_SCL (*SCLPinR)

#define SET_SDA do { *SDAPinW = 0x1; } while (0)
#define RST_SDA do { *SDAPinW = 0x0; } while (0)
#define GET_SDA (*SDAPinR)

// static volatile unsigned int * SCLPinR;
static volatile unsigned int * SCLPinW;
static volatile unsigned int * SDAPinR;
static volatile unsigned int * SDAPinW;


unsigned char g_I2C_Speed;

void I2C_Delay() 
{ 
	unsigned char t;

	t = g_I2C_Speed;
	while (--t != 0);      
}

void SIM_I2CConfigPin(PIN_t SCLPin, PIN_t SDAPin)
{
    // SCLPinR = GET_PIN_BB_ADDR(SCLPin.Port, SCLPin.PinNo, IDR);
    SCLPinW = (unsigned int *)GET_PIN_BB_ADDR(SCLPin.Port, SCLPin.PinNo, ODR);
    SDAPinR = (unsigned int *)GET_PIN_BB_ADDR(SDAPin.Port, SDAPin.PinNo, IDR);
    SDAPinW = (unsigned int *)GET_PIN_BB_ADDR(SDAPin.Port, SDAPin.PinNo, ODR);
}

void SIM_I2C_Init()
{
	SET_SCL; 
	I2C_Delay(); 

    SET_SDA;  
	I2C_Delay(); 
} 


void SIM_I2C_Start()
{
	SET_SDA;  
	I2C_Delay(); 
 
	SET_SCL; 
	I2C_Delay(); 

	RST_SDA;  
	I2C_Delay(); 

	RST_SCL;  
	I2C_Delay(); 
}


void SIM_I2C_Stop()
{
	RST_SDA;  
	I2C_Delay(); 

	SET_SCL; 
	I2C_Delay(); 

	SET_SDA;  
	I2C_Delay(); 
}

void SIM_I2C_Write(unsigned char dat) 
{
	unsigned char t = 8; 

	do
	{
        if ((unsigned char)(dat & 0x80)) 
            SET_SDA;
        else
            RST_SDA;

		dat <<= 1;

		SET_SCL;     
		I2C_Delay();

		RST_SCL;      
		I2C_Delay();
	} while ( --t != 0 );
}



unsigned char SIM_I2C_Read()
{
	unsigned char dat;
	unsigned char t = 8;

	SET_SDA;      
	
	do
	{
		SET_SCL;     
		I2C_Delay();

		dat <<= 1;

		if ( GET_SDA ) 
			dat++;

		RST_SCL;      
		I2C_Delay();
	} while ( --t != 0 );

	return dat;
}

 

unsigned char SIM_I2C_GetAck()
{
	unsigned char Ack;

	SET_SDA;     
	I2C_Delay();

	SET_SCL;     
	I2C_Delay();

	Ack = (unsigned char)GET_SDA;

	RST_SCL;      
	I2C_Delay();

	return Ack;
}

void SIM_I2C_PutAck(unsigned char Ack)
{
    Ack &= 0x1;
    if (Ack) 
        SET_SDA;
    else
        RST_SDA;

    I2C_Delay(); 

	SET_SCL;     
	I2C_Delay();

	RST_SCL;      
	I2C_Delay();
}

/*
typedef struct I2C_Operate {
	void (*ConfigPin)(unsigned int, unsigned int);	
	void (*init)(void);
	void (*start)(void);
	void (*stop)(void);
	void (*write)(unsigned char);
	unsigned char (*read)(void);
	unsigned char (*getack)(void);
	void (*putack)(unsigned char);
} I2C_OPS_t;
*/

I2C_OPS_t I2COps = {
	SIM_I2CConfigPin,
	SIM_I2C_Init,
	SIM_I2C_Start,
	SIM_I2C_Stop,
	SIM_I2C_Write,
	SIM_I2C_Read,
	SIM_I2C_GetAck,
	SIM_I2C_PutAck
};







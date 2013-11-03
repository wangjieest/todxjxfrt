
#include "..\burn.h"

static void delay()
{
	unsigned short t = 32768;
	while (t-- > 0);
}

int M603_init(void)  
{
    I2COps.init();
    return 0;
} 

int M603_autoid(unsigned char *vid, unsigned char *did) 
{
    return 0;
}

int M603_read(long offset, long len, unsigned char *buf) 
{
    unsigned char addr = (unsigned char)offset;

    I2COps.start();

    I2COps.write(g_ChipAddr&0xFE);
    if (I2COps.getack()) 
    {
        I2COps.stop();
        return (-1); // slave nack
    }

    I2COps.write(addr);
    if (I2COps.getack()) 
    {
        I2COps.stop();
        return (-1); // slave nack
    }

    // send Sr
    I2COps.start();

    I2COps.write(g_ChipAddr | 0x01);
    if (I2COps.getack()) 
    {
        I2COps.stop();
        return (-1); // slave nack
    }

    for (; len > 0; len--) 
    {
        *buf++ = I2COps.read();
        if (len == 1) 
        {
            I2COps.putack(I2C_NACK);
            break;
        }
        else
            I2COps.putack(I2C_ACK);   
    }

    I2COps.stop();

    return 0; 
}

int M603_write(long offset, long len, unsigned char *buf) 
{
    unsigned char addr = (unsigned char)offset;
    unsigned char i;
    unsigned char limidx;
    unsigned char VCOM_LimAddr[2] = {0x18,0x31}; 
    unsigned char VCOM_InitLimit[4] = {0x00, 0x00, 0x03, 0xff};
    unsigned char tmp;
			
    for (i = 0; i < 2; i++)
    {
        I2COps.start();

        I2COps.write(g_ChipAddr&0xFE);
        if (I2COps.getack()) 
        {
            I2COps.stop();
            return (-1); // slave nack
        }			
        
        I2COps.write(VCOM_LimAddr[i]);
        if (I2COps.getack()) 
        {
            I2COps.stop();
            return (-1); // slave nack
        }		
 
        for (limidx = 0; limidx < 4; limidx++) 
        {
            I2COps.write(VCOM_InitLimit[limidx]);
            if (I2COps.getack()) 
            {
                I2COps.stop();
                return (-1);
            } 
        }		

        I2COps.stop();
        delay();
    }

    I2COps.start();

    I2COps.write(g_ChipAddr&0xFE);
    if (I2COps.getack()) 
    {
        I2COps.stop();
        return (-1); // slave nack
    }

    I2COps.write(addr);
    if (I2COps.getack()) 
    {
        I2COps.stop();
        return (-1); // slave nack
    }

    for (; len > 0; len--) 
    {
        tmp = *buf++;
        if (len == 2)
        {
            tmp |= 0x40;
            tmp &= ~0x80;
        }
        I2COps.write(tmp);
        if (I2COps.getack()) 
        {
            I2COps.stop();
            return (-1);
        } 
    }

    I2COps.stop();

    return 0;
}

int M603_erase(long offset, long len) 
{
    return 0;
}

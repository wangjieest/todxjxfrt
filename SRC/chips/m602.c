#include "..\burn.h"

static void delay()
{
	unsigned short t = 32768;
	while (t-- > 0);
}

int M602_init(void)  
{
    I2COps.init();
    return 0;
} 

int M602_autoid(unsigned char *vid, unsigned char *did) 
{
    return 0;
}

int M602_read(long offset, long len, unsigned char *buf) 
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

int M602_write(long offset, long len, unsigned char *buf) 
{
    unsigned char addr = (unsigned char)offset;
    unsigned char i;
    unsigned char key_len;
    unsigned char *pkey;
    unsigned char key_offset[4] = {0x18, 0x1c, 0x31, 0x34}; 
    unsigned char tmp;
			
    for (i = 0; i < 4; i++)
    {
        I2COps.start();

        I2COps.write(g_ChipAddr&0xFE);
        if (I2COps.getack()) 
        {
                I2COps.stop();
                return (-1); // slave nack
        }			
        
        I2COps.write(key_offset[i]);
        if (I2COps.getack()) 
        {
            I2COps.stop();
            return (-1); // slave nack
        }		

        key_len = 4;
        pkey = buf + key_offset[i]*2;
        for (; key_len > 0; key_len--) 
        {
            I2COps.write(*pkey++);
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

int M602_erase(long offset, long len) 
{
    return 0;
}

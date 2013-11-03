#include "..\burn.h"

typedef struct CM2970_CMD {
    int CmdLen;
    unsigned char *Cmd;
    int AckLen;
    unsigned char *Ack;
} CM2970_CMD_t;

// M2970 Commands
// set pll bus Freq
unsigned char Cmd_SetPLL_1[] = {0x11}; 
unsigned char Cmd_SetPLL_1_ACK[] = {0x7a};

unsigned char Cmd_SetPLL_2[] = {0x26, 0x38, 0x07, 0x00, 0x06, 0x20, 0x23, 0x10, 0x0a, 0x74}; 
unsigned char Cmd_SetPLL_2_ACK[] = {0x6c};

unsigned char Cmd_SetPLL_3[10] = {0x26, 0x38, 0x07, 0x00, 0x01, 0x08, 0x00}; 
unsigned char Cmd_SetPLL_3_ACK[] = {0x0e};

// Erase 4kbytes
unsigned char Cmd_BlkErOP[7] = {0x26, 0x38, 0x0E, 0x00, 0xFF, 0x61, 0x50}; // 0xC0
unsigned char Cmd_CmmInEn[7] = {0x26, 0x38, 0x0E, 0x00, 0xFF, 0x60, 0x21}; // 0xB0
unsigned char Cmd_WtCplFlash[6] = {0x26, 0x38, 0x0D, 0x00, 0xFF, 0x60};    // 0x20 0xb2
unsigned char Cmd_WRSROP[7] = {0x26, 0x38, 0x0E, 0x00, 0xFF, 0x61, 0x01}; // 0x91
unsigned char Cmd_DisWPBP[7] = {0x26, 0x38, 0x0E, 0x00, 0xFF, 0x64, 0x00}; // 0x95
unsigned char Cmd_CmmInEn2[7] = {0x26, 0x38, 0x0E, 0x00, 0xFF, 0x60, 0x69}; // 0xF8 
unsigned char Cmd_WtWR[6] = {0x26, 0x38, 0x0D, 0x00, 0xFF, 0x60}; // 0x68 0xfa
unsigned char Cmd_SetStartAddrBurn[6] = {0x26, 0x38, 0x0E, 0x02, 0xFF, 0x64}; // 0x97
unsigned char Cmd_Erase[7] = {0x26, 0x38, 0x0E, 0x00, 0xFF, 0x60, 0xB8}; // 0x29
unsigned char Cmd_BlkErOP2[7] = {0x26, 0x38, 0x0E, 0x00, 0xFF, 0x61, 0x20}; // 0xB0
unsigned char Cmd_CmmInEn3[7] = {0x26, 0x38, 0x0E, 0x00, 0xFF, 0x60, 0xB9}; // 0x28
unsigned char Cmd_WtErFlsh[6] = {0x26, 0x38, 0x0D, 0x00, 0xFF, 0x60}; // 0xb8  0x2a


static void delay()
{
	unsigned short t = 32768;
	while (t-- > 0);
}

static int SendCmd(unsigned char *Cmd, unsigned char len)
{
    I2COps.start();

    I2COps.write(g_ChipAddr&0xFE);
    if (I2COps.getack()) 
    {
        I2COps.stop();
        return (-1); // slave nack
    }

    for (; len > 0; len--) 
    {
        I2COps.write(*Cmd++);
        if (I2COps.getack()) 
        {
            I2COps.stop();
            return (-1);
        } 
    }

    I2COps.stop();
    
    return 0;
}


static int SendCmdAndGetAck(unsigned char *Cmd, unsigned char CmdLen, unsigned char *Ack, unsigned char AckLen)
{
    int i = 0;
    int j = 0;
    int AckReady;
    unsigned char tmp[16];

    if (AckLen > 16) 
        return (-1);
    // send cmd
    while (i++ < 3) {
        if (SendCmd(Cmd, CmdLen)) 
            continue;
        else
            break;
    }
    if (i >= 3) {
        return -1;
    }

    i = 0;
    // get ack from cm2970
    while (i++ < 1000) 
    {
        I2COps.start();

        I2COps.write(g_ChipAddr | 0x01);
        if (I2COps.getack()) 
        {
            I2COps.stop();
            return (-1); // slave nack
        }

        AckReady = 0;
        j = 0;
        do {
            tmp[j] = I2COps.read(); 
            I2COps.putack(I2C_ACK);  
            if (!AckReady) 
            {
                if (!(tmp[0] & 0x1))         
                    AckReady = 1;
            }
            else
            {
                if (++j >= AckLen)
                    break;
            }
        } while (AckReady);

        I2COps.stop();
        delay();  
        if (AckReady)
            break; 
    }

    if (AckReady)
    {
        for (j = 0; j < AckLen; j++) 
            if (tmp[j] != Ack[j]) 
                return (-1);
    }
    else 
        return (-1);

    return 0;
}


int CM2970_init(void)  
{
    I2COps.init();

    // Set PLL Freq
    if (SendCmdAndGetAck(Cmd_SetPLL_1, sizeof(Cmd_SetPLL_1), Cmd_SetPLL_1_ACK, sizeof(Cmd_SetPLL_1_ACK)))
        return (-1);

    if (SendCmdAndGetAck(Cmd_SetPLL_2, sizeof(Cmd_SetPLL_2), Cmd_SetPLL_2_ACK, sizeof(Cmd_SetPLL_2_ACK)))
        return (-1);

    if (SendCmdAndGetAck(Cmd_SetPLL_3, sizeof(Cmd_SetPLL_3), Cmd_SetPLL_3_ACK, sizeof(Cmd_SetPLL_3_ACK)))
        return (-1);

    return 0;
} 

int CM2970_autoid(unsigned char *vid, unsigned char *did) 
{
    return 0;
}

int CM2970_read(long offset, long len, unsigned char *buf) 
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

int CM2970_write(long offset, long len, unsigned char *buf) 
{
    unsigned char addr = (unsigned char)offset;
    unsigned char i, tmp;

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

int CM2970_erase(long offset, long len) 
{
    return 0;
}



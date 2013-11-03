#include "burn.h"
#include "chip.h"
#include "i2c_sim.h"
#include "pltcfg.h"
#include "uart.h"


#define I2C_CHANNEL_NUM  4 
#define SPI_CHANNEL_NUM  4 

 
typedef struct I2CChannelMap { 
    unsigned int ChanNo;
    PIN_t SCL_Pin;
    PIN_t SDA_Pin;   
} I2CChannelMap_t; 


typedef struct SPIChannelMap { 
    unsigned int ChanNo;
    PIN_t CS_Pin;
    PIN_t CL_Pin;  
    PIN_t DI_Pin;
    PIN_t DO_Pin;      
} SPIChannelMap_t; 

static I2CChannelMap_t I2CChannelMapTable[I2C_CHANNEL_NUM] = { 
    { 0, {GPIOB, 12}, {GPIOB, 14}},
    { 1, {GPIOB, 12}, {GPIOB, 14}},
    { 2, {GPIOB, 12}, {GPIOB, 14}},
    { 3, {GPIOB, 12}, {GPIOB, 14}}
};

static SPIChannelMap_t SPIChannelMapTable[SPI_CHANNEL_NUM] = { 
    { 0, {GPIOA, 8}, {GPIOA, 6}, {GPIOB, 13}, {GPIOB, 15}},
    { 1, {GPIOA, 8}, {GPIOA, 6}, {GPIOB, 13}, {GPIOB, 15}},
    { 2, {GPIOA, 8}, {GPIOA, 6}, {GPIOB, 13}, {GPIOB, 15}},
    { 3, {GPIOA, 8}, {GPIOA, 6}, {GPIOB, 13}, {GPIOB, 15}}    
};


unsigned char g_ChipAddr;
extern unsigned char BinBuf[BIN_BUF_SIZE]; 
static ROM_OPS_t RomOps;

int ChannelConfig(unsigned char channel, unsigned short *ChipIf);
static int ChipConfig(unsigned char ChipAddr, unsigned short Chip, unsigned short ChipIf);



int BurnConfig(unsigned char channel, 
              unsigned short chip, 
              unsigned char chipaddr, 
              unsigned char busspeed)
{
	unsigned short ChIf = CHIP_IF_UNK; /* chip interface type, i2c or spi */
	
    if (ChannelConfig(channel, &ChIf)) 
        return (-1);

    if (ChipConfig(chipaddr, chip, ChIf))
        return (-1);

	return 0;
}

int ChipConfig(unsigned char ChipAddr, unsigned short Chip, unsigned short ChipIf)
{
    ChipItem_t * pChipItem = ChipTable;

    if (Chip != 0) /* known chips */
    {
    	do {
    		if (pChipItem->ChipID == Chip)
    			break;
    		pChipItem++;
    	} while (pChipItem->ChipID != CHIP_TABLE_MAX_ITEM);

        // unsupported ic
        if (pChipItem->ChipID == CHIP_TABLE_MAX_ITEM) 
            return (-1);

    	// chip interface miss match
    	if (ChipIf != pChipItem->ChipIf)
    		return (-1);
    	
    	RomOps.init     = pChipItem->ChipOps.init;
    	RomOps.autoid   = pChipItem->ChipOps.autoid;
    	RomOps.read     = pChipItem->ChipOps.read;
    	RomOps.write    = pChipItem->ChipOps.write;
    	RomOps.erase    = pChipItem->ChipOps.erase;	
    }
    else // CFI flash 
        return (-1);

    g_ChipAddr = ChipAddr;

    return 0;
}


int ChannelConfig(unsigned char channel, unsigned short *ChipIf)
{
    I2CChannelMap_t *pI2CChanMapItem;
    SPIChannelMap_t *pSPIChanMapItem;
    int i;

	if (channel >= 0xf)
		return (-1);   

	if (channel < 0x7)
    {
        *ChipIf = CHIP_IF_I2C;

        pI2CChanMapItem = I2CChannelMapTable;
        for (i = 0; i < I2C_CHANNEL_NUM; i++) 
        {
            if (pI2CChanMapItem->ChanNo == channel) 
            {
                I2COps.ConfigPin(pI2CChanMapItem->SCL_Pin, pI2CChanMapItem->SDA_Pin);
                break;
            }

            if (i >= I2C_CHANNEL_NUM - 1) 
                return (-1);

            pI2CChanMapItem++;
        }
    }
    else
    {
        *ChipIf = CHIP_IF_SPI;
        channel -= 0x8;

        pSPIChanMapItem = SPIChannelMapTable;
        for (i = 0; i < SPI_CHANNEL_NUM; i++) 
        {
            if (pSPIChanMapItem->ChanNo == channel) 
            {
                // I2COps.ConfigPin(pSPIChanMapItem->SCL_Pin, pSPIChanMapItem->SDA_Pin);
                break;
            }

            if (i >= SPI_CHANNEL_NUM - 1) 
                return (-1);

            pSPIChanMapItem++;
        }
    }
    
    return 0;
}



// 
// 暂只支持offset = 0 
// 每读出512字节，向串口发送
int UPRG_read(long offset, long len)
{
	unsigned long lentor;
	
	RomOps.init();
	
	while (len > 0)
	{
		
		if (len > BIN_BUF_SIZE)
			lentor = BIN_BUF_SIZE;
		else
			lentor = len;
		
		if (!RomOps.read(offset, lentor, BinBuf))
		{
			// Send to PC
			UploadBinaries(BinBuf, lentor);
		}
        else 
            return (-1);
		
		len -= lentor;
		offset += lentor;
	}
	
	return 0;
}

int UPRG_write(long offset, long len)
{
	unsigned long lentow;
    unsigned char sync_symbol = '#';
	
	RomOps.init();

	while (len > 0)
	{
		if (len > BIN_BUF_SIZE)
			lentow = BIN_BUF_SIZE;
		else
			lentow = len;	

		// send sync symbol '#'
		UploadBinaries(&sync_symbol, 1);

		DownloadBinaries(BinBuf, lentow);

		if (RomOps.write(offset, lentow, BinBuf))
			return (-1);

		len -= lentow;
		offset += lentow;
	}

	return 0;
}

// 对于烧录器来说，只提供整片擦除就可以了，但是怎么获得flash参数？
// 每一种flash有对应的程序，就无需上位机提供参数了
// 分页、页不对齐擦除要求单片机有较大RAM
int UPRG_erase(long offset, long len)
{
    RomOps.init();
	return RomOps.erase(offset, len);
}

int UPRG_checksum(long offset, long len, unsigned long *cksm)
{
	unsigned long lentor;
	unsigned long checksum;
	unsigned short i;

	RomOps.init();
	checksum = 0;
	
	while (len > 0)
	{
		
		if (len > BIN_BUF_SIZE)
			lentor = BIN_BUF_SIZE;
		else
			lentor = len;
		
		if (!RomOps.read(offset, lentor, BinBuf))
		{
			// calc check sum
            for (i = 0; i < lentor; i++) 
            {
                checksum += BinBuf[i];
            }
		}
        else 
            return (-1);
		
		len -= lentor;
		offset += lentor;
	}

    *cksm = checksum;

    return 0;
}







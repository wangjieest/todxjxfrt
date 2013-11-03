#define __CHIP__
#include "chip.h"


//	unsigned char (*prepare)(void); 
//	unsigned char (*getid)(unsigned char *id);
//	unsigned char (*read)(long offset, long len, unsigned char *buf);
//	unsigned char (*write)(long offset, long len, unsigned char *buf);
//	unsigned char (*erase)(long offset, long len, unsigned char *buf);

#define CHIP_ITEM(CHIP, IF) {CHIP, IF, {CHIP##_init, CHIP##_autoid, CHIP##_read, CHIP##_write, CHIP##_erase}}

ChipItem_t ChipTable [] = 
{
	CHIP_ITEM(M602, CHIP_IF_I2C),      // ID does not need to math table index
    CHIP_ITEM(M603, CHIP_IF_I2C),
	CHIP_ITEM(CM2970, CHIP_IF_I2C),
	{CHIP_TABLE_MAX_ITEM, 0, {0, 0, 0, 0}}
};

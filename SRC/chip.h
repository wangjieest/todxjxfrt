#ifndef __CHIP_H__
#define __CHIP_H__

#include "burn.h"

#ifdef __CHIP__
#include "chips\M602.h"
#include "chips\M603.h"
#include "chips\cm2970.h"

#define M602   0x0001
#define CM2970  0x0002
#define M603   0x0003

#endif // __CHIP__

#define CHIP_IF_UNK 0x0000
#define CHIP_IF_I2C 0x0001
#define CHIP_IF_SPI 0x0002

#define CHIP_TABLE_MAX_ITEM 0xFFFF

typedef struct Chip_Item {
	unsigned short ChipID;
	unsigned short ChipIf;
	ROM_OPS_t ChipOps;
} ChipItem_t;

extern ChipItem_t ChipTable[];

#endif // __CHIP_H__

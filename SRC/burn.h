#ifndef __BURN_H__
#define __BURN_H__

#include "i2c_sim.h"

#define SERV_CMD_SIZE  16 
#define ACK_MSG_BUF_SZ 32


#define MC_FW_VER 0x3030
#define MC_CK_ST  0x3031
#define MC_MC_CK  0x3032
#define MC_FW_UPT 0x3033

#define ECG_PIN0   0x3130
#define ECG_PIN1   0x3131
#define ECG_PIN2   0x3132
#define ECG_PIN3   0x3133

#define ROM_CG 0x3230
#define ROM_ET 0x3330
#define ROM_RD 0x3430
#define ROM_WR 0x3530
#define ROM_ER 0x3630
#define ROM_CK 0x3730


typedef struct ROM_Operate {
	int (*init)(void) ; 
	int (*autoid)(unsigned char *vid, unsigned char *did) ;
	int (*read)(long offset, long len, unsigned char *buf) ;
	int (*write)(long offset, long len, unsigned char *buf) ;
	int (*erase)(long offset, long len) ;
} ROM_OPS_t;



typedef struct SPI_Operate {
		void (*write)(unsigned char);
} SPI_OPS_t;


extern unsigned char g_ChipAddr;
int BurnConfig(unsigned char channel, 
                unsigned short chip, 
                unsigned char chipaddr, 
                unsigned char busspeed);

int UPRG_read(long offset, long len);
int UPRG_write(long offset, long len);
int UPRG_erase(long offset, long len);
int UPRG_checksum(long offset, long len, unsigned long *cksm);

#endif // end of __BURN_H__

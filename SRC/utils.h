#ifndef __CHIP_H__
#define __CHIP_H__

union usBytes2Short {
	unsigned char ch[2];
	unsigned short val;
};


union usBytes2Long {
	unsigned char ch[4];
	unsigned long val;
};

unsigned char UPRG_atoi(unsigned char ch);

unsigned int UPRG_nBAsc2Int(unsigned char *buf, unsigned char nBytes);

#endif // __CHIP_H__

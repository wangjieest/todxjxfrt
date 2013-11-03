#ifndef __CM2970_H__
#define __CM2970_H__

int CM2970_init(void); 
int CM2970_autoid(unsigned char *vid, unsigned char *did);
int CM2970_read(long offset, long len, unsigned char *buf);
int CM2970_write(long offset, long len, unsigned char *buf);
int CM2970_erase(long offset, long len);

#endif // __CM2970_H__

#ifndef __M603_H__
#define __M603_H__

int M603_init(void); 
int M603_autoid(unsigned char *vid, unsigned char *did);
int M603_read(long offset, long len, unsigned char *buf);
int M603_write(long offset, long len, unsigned char *buf);
int M603_erase(long offset, long len);

#endif // __M603_H__

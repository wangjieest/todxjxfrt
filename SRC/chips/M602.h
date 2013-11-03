#ifndef __M602_H__
#define __M602_H__

int M602_init(void); 
int M602_autoid(unsigned char *vid, unsigned char *did);
int M602_read(long offset, long len, unsigned char *buf);
int M602_write(long offset, long len, unsigned char *buf);
int M602_erase(long offset, long len);

#endif // __M602_H__

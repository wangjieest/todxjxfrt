

unsigned char UPRG_atoi(unsigned char ch)
{
	if ((ch >= 0x30) && (ch <= 0x39))
		ch -= 0x30;
	else if ((ch >= 0x41) && (ch <= 0x5A))
		ch -= 0x37;
	else 
		ch = 0xff;
	
	return ch;
}

unsigned char UPRG_itoa()
{
	return 0;
}


unsigned int UPRG_nBAsc2Int(unsigned char *buf, unsigned char nBytes)
{
    int i;
    unsigned int Result = 0;

    for (i = 0; i < nBytes; i++) 
    {
        Result |= UPRG_atoi(*(buf + i)) << ((nBytes - 1 - i)*4);
    }

    return Result;
}

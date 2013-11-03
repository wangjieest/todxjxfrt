#include "pltcfg.h"


void UploadBinaries(unsigned char *buf, unsigned long len)
{
	while (len-- > 0)
	{
        USART_SendData(USARTy, *buf++);
        
        /* Loop until USARTy DR register is empty */ 
        while(USART_GetFlagStatus(USARTy, USART_FLAG_TXE) == RESET)
        {
        }        	
	}
}

void DownloadBinaries(unsigned char *buf, unsigned long len)
{
    while (len-- > 0) 
	{
        while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET)
        {
        }

        /* Store the received byte in RxBuffer */
        *buf++ = USART_ReceiveData(USART1); 
    }
}

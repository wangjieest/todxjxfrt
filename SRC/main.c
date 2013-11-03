#include <stdio.h>
#include <string.h>

// 定义了处理器相关的东西
#include "pltcfg.h"

#include "uart.h"
#include "utils.h"
#include "burn.h"


#define SUCC 0x31
#define FAIL 0x30

void InitDevice(void);
void ExecCmd(unsigned char *msg);
void GetFwVer(char *);
void SendAckMsg(void);


unsigned char  ucData;

// 命令字
unsigned char serv_cmd[SERV_CMD_SIZE];
char rt_msg[ACK_MSG_BUF_SZ] = {'$', 0};
char FirmwareVersion[] = "20131026v1.0.0";
unsigned char BinBuf[BIN_BUF_SIZE];  

int main(void)
{	
	unsigned char cnt;
	unsigned char state;
	// unsigned char tmp;
	
#define ACT 1
#define IDL 0
	InitDevice();
	
	cnt = 0;
	state = IDL;
	
	while (1)
	{
        // wait until receive buffer not empty
        while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET)
        {
        }

        serv_cmd[cnt] = USART_ReceiveData(USART1); 
			
        if (state == ACT)
        {
            if (serv_cmd[cnt] == '$')
            {
                cnt = 0;
                state = IDL;
            }
            else
                cnt++;
        }
        else if ((state == IDL) && (serv_cmd[cnt] == '$'))
            state = ACT;
	
		
		if (cnt >= 16)
		{
			ExecCmd(serv_cmd);
			// UploadBinaries(serv_cmd, 16);
			cnt = 0;
			state = IDL;
            // attention! uart fifo overflow
		}
	}
	
}

void ExecCmd(unsigned char *msg)
{
	unsigned short Cmd;
	unsigned char ret;
	char *p;
	unsigned long offset;
	unsigned long length;
	unsigned char i;

    // ic 配置
	unsigned char channel;
	unsigned short chip_id;
	unsigned char chip_addr;
	unsigned char busspeed;
	unsigned long checksum;
	
	Cmd = *msg++ << 8;
	Cmd |= *msg++;
	
	p = rt_msg + 1;  // skip '$'
	
	switch (Cmd)
	{
	case MC_FW_VER:
		*p++ = SUCC;
		GetFwVer(p);
		break;
	
	case MC_CK_ST:  
		*p++ = SUCC;
		*p = 0;
		break;

	case MC_MC_CK:  
		*p++ = SUCC;
		*p = 0;
		break;
	
	case MC_FW_UPT: 
		*p++ = FAIL;
		*p = 0;
		break;	
	
    case ECG_PIN0:
    case ECG_PIN1:
    case ECG_PIN2:
    case ECG_PIN3:       
		*p++ = SUCC;
        *p++ = 'F';
        *p++ = 'F';
		*p = 0;
		break;		

	case ROM_CG: 
		channel = UPRG_atoi(*msg++);

        chip_id = (unsigned short)UPRG_nBAsc2Int(msg, 4);
        msg += 4;

        chip_addr = (unsigned char)UPRG_nBAsc2Int(msg, 2);
        msg += 2;

        busspeed = (unsigned char)UPRG_nBAsc2Int(msg, 2);
	
		ret = BurnConfig(channel, chip_id, chip_addr, busspeed);
	
		// send ack
		if (!ret)
		{
			*p++ = SUCC;
			*p++ = 0x30;
		}
		else
		{
			*p++ = FAIL;
			*p++ = ret;
		}
        // 现在没有使用CFI功能，故响应字段用0x30填充
        for (i = 0; i < 10; i++) 
            *p++ = 0x30;

        *p = 0; 

		break;
	
    case ROM_ET:
		*p++ = FAIL;
		*p = 0;
		break;
	
    case ROM_RD:
        offset = UPRG_nBAsc2Int(msg, 6);
        msg += 6;
		length = UPRG_nBAsc2Int(msg, 6);
		offset = 0;
		ret = UPRG_read(offset, length);

		if (!ret)
		{
			*p++ = SUCC;
			*p++ = 0x30;
		}
		else
		{
			*p++ = FAIL;
			*p++ = 0x31;
		}
		*p = 0;

		break;
	
    case ROM_WR: 
        offset = UPRG_nBAsc2Int(msg, 6);
        msg += 6;
		length = UPRG_nBAsc2Int(msg, 6);
		ret = UPRG_write(0, length);

		if (!ret)
		{
			*p++ = SUCC;
			*p++ = 0x30;
		}
		else
		{
			*p++ = FAIL;
			*p++ = 0x31;
		}
		*p = 0;

		break;
	
    case ROM_ER: 
        offset = UPRG_nBAsc2Int(msg, 6);
        msg += 6;
		length = UPRG_nBAsc2Int(msg, 6);

        ret = UPRG_erase(0, length);
		
		if (!ret)
		{
			*p++ = SUCC;
			*p++ = 0x30;
		}
        else
		{
			*p++ = FAIL;
			*p++ = ret;
		}
		*p = 0;

        break;
	
    case ROM_CK:
        offset = UPRG_nBAsc2Int(msg, 6);
        msg += 6;
		length = UPRG_nBAsc2Int(msg, 6);

        ret = UPRG_checksum(0, length, &checksum);

		if (!ret)
		{
			*p++ = SUCC;
			*p++ = 0x30;
            // attach checksum
		}
		else
		{
			*p++ = FAIL;
			*p++ = ret;
		}
		*p = 0;

		break;
	
	default: // no such cmd
		*p++ = FAIL;
		*p = 0;		
		break;
	}
	
	SendAckMsg();
}

void SendAckMsg()
{
	unsigned char cnt;
	unsigned char len;
	
	len = strlen(rt_msg);
	
	for (cnt = 0; cnt < len; cnt++)
	{
        USART_SendData(USART1, rt_msg[cnt]);
        
        /* Loop until USARTy DR register is empty */ 
        while(USART_GetFlagStatus(USARTy, USART_FLAG_TXE) == RESET)
        {
        }
	}
}

void GetFwVer(char *buf)
{
	strcpy(buf, FirmwareVersion);
}






	  

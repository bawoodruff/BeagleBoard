#define TARGET_REQ_TRACEMSG                     0x00
#define TARGET_REQ_DEBUGMSG_ASCII               0x01
#define TARGET_REQ_DEBUGMSG_HEXMSG(size)        (0x01 | ((size & 0xff) << 8))
#define TARGET_REQ_DEBUGCHAR                    0x02

void _write_dcc(unsigned long data);

#if 0
/* 
*  we use the System Control Block DCRDR reg to simulate a arm7_9 dcc channel
* DCRDR[7:0] is used by target for status
* DCRDR[15:8] is used by target for write buffer
* DCRDR[23:16] is used for by host for status
* DCRDR[31:24] is used for by host for write buffer 
* */ 
#define NVIC_DBG_DATA_R (*((volatile unsigned short *)0xE000EDF8))

#define BUSY 1

void write_dcc(unsigned long data)
{
	int len = 4;
	while (len--)
	{
		// wait for data ready
		while (NVIC_DBG_DATA_R & BUSY);

		// write our data and set write flag - tell host there is data
		NVIC_DBG_DATA_R = (unsigned short)(((data & 0xff) << 8) | BUSY);

		data >>= 8;
	}
} 
#endif

void write_dcc_str(const char *msg)
{
	long len;
	unsigned long dcc_data;

	for (len = 0; msg[len] && (len < 65536); len++);

	_write_dcc(TARGET_REQ_DEBUGMSG_ASCII | ((len & 0xffff) << 16));

	while (len > 0)
	{
		dcc_data = msg[0]
				| ((len > 1) ? msg[1] << 8 : 0x00)
				| ((len > 2) ? msg[2] << 16 : 0x00)
				| ((len > 3) ? msg[3] << 24 : 0x00);
		_write_dcc(dcc_data);

		msg += 4;
		len -= 4;
	}
}

void write_dcc_char(char msg)
{
	_write_dcc(TARGET_REQ_DEBUGCHAR | ((msg & 0xff) << 16));
}

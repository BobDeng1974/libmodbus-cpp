#include <assert.h>
#include "Modbus.h"
#include "xstring.h"
#include "xtime.h"

#define getinfo(m,s) \
do{\
	for(uint8_t i = 0; i < m.count; i++)\
		s += xstring("[%d%04d]=%-3d  ", m.fcode, (m.offset + i + 1), m[i]);\
}while(0)

int main(int argc, char **argv)
{
	ModbusServer TCP("192.168.1.203", 8899, true);

	TCP.Mapping(0, 0x10000, 0, 0x10000, 0, 0x10000, 0, 0x10000);
	for(int i = 0; i < 0x10000; i++)
	{
		TCP.Bit(i) = i % 2;
		TCP.InputBit(i) = i % 3;
		TCP.Register(i) = 0xAA;
		TCP.InputRegister(i) = 0x55;
	}
	while(1)
	{
		int rc = TCP.Receive();
		for(int i = 0; i < rc; i++)
		{
			TCP.fcode = MODBUS_FC_READ_COILS;
			printf("%02X ", TCP[i]);
		}
		if(rc == -1)
		{
			TCP.Create("192.168.1.203", 8899, true);
		}
		usleep(100);
	}

	return 0;
}

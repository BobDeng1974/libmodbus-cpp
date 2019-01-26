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

	TCP.SetSlave(17);
	while(1)
	{
		TCP.Receive();
		usleep(100);
	}

	return 0;
}

#include <assert.h>
#include "Modbus.h"
#include "xstring.h"
#include "xtime.h"
#include <errno.h>

#define getinfo(m,s) \
do{\
	for(uint8_t i = 0; i < m.count; i++)\
		s += xstring("[%d%04d]=%-3d  ", m.fcode, (m.offset + i + 1), m[i]);\
}while(0)

int main(int argc, char **argv)
{
	ModbusClient TCP("192.168.1.203", 8899);
	ModbusClient RTU("/dev/ttySX4", 9600, 'N', 8, 1);
	uint8_t id[] = {1, 2, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 22}; 
	uint8_t sendcount = 0;

	assert( RTU.SetTimeout(50) );
	while(1)
	{
		xtime time;
		xstring info;

		sendcount++;
		TCP.SetSlave(17);
		TCP.SetDebug(17);
		info += xstring("\n%02d: ", TCP.slave);
		if(TCP.ReadBits(0, 5))
		{
			getinfo(TCP, info);
		}
		if(TCP.ReadRegisters(3, 6))
		{
			getinfo(TCP, info);
		}
		else 
		{
			switch(errno)
			{
				case EBADF:
				case EPIPE:
					TCP.Create("192.168.1.203", 8899);
					break;
			}
		}
		for(int i = 0; i < sizeof(id); i++)
		{
			RTU.SetSlave(id[i]);
			info += xstring("\n%02d: ", id[i]);

			xtime ftime;
			if(RTU.ReadRegisters(0, 3))
			{
				getinfo(RTU, info);
			}
			info += xstring("mdiff=%2d  ", ftime.mdiff());

			ftime.update();
			if(RTU.ReadInputRegisters(0, 5))
			{
				getinfo(RTU, info);
			}
			info += xstring("mdiff=%2d", ftime.mdiff());
		}
		system("clear");
		printf("\nsendcount=%d, mdiff=%d %s\n", sendcount, time.mdiff(), info.data());
		fflush(stdout);
	}

	return 0;
}

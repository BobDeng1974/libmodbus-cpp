#include <assert.h>
#include "ModbusRtu.h"

#define showregisters(M,C) \
	do{\
		for(int i = 0; i < C; i++)\
			printf("[%d]=%d\n", i, M.GetRegister(i));\
	}while(0)

int main(int argc, char **argv)
{
	ModbusRtu modbus;

	assert( modbus.Create("/dev/ttySX4", 9600, 'N', 8, 1) );
	assert( modbus.SetDebug(1) );
	assert( modbus.Connect() );
	assert( modbus.SetResponseTimeout(0, 500000) );
	while(1)
	{
		for(int i = 1; i < 3; i++)
		{
			modbus.SetSlave(i);
			modbus.ReadRegisters(0, 3);
			showregisters(modbus, modbus.GetCount());
			modbus.ReadInputRegisters(0, 5);
			showregisters(modbus, modbus.GetCount());
		}
		sleep(1);
		system("clear");
	}

	return 0;
}

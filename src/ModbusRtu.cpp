#include <string.h>
#include "ModbusRtu.h"

ModbusRtu::ModbusRtu(void)
{
	count = 0;
	context = NULL;
	memset(data, 0, sizeof(data));
}
ModbusRtu::~ModbusRtu(void)
{
	Destroy();
}
bool ModbusRtu::Create(const string& fname, int baud, int parity, int bsize, int stop)
{
	if(context)
	{
		modbus_free(context);
		context = NULL;
	}
	return context = modbus_new_rtu(fname.data(), baud, parity, bsize, stop);
}
bool ModbusRtu::Connect(void)
{
	return modbus_connect(context) != -1;
}
void ModbusRtu::Close(void)
{
	if(context)modbus_close(context);
}
void ModbusRtu::Destroy(void)
{
	if(context)
	{
		modbus_close(context);
		modbus_free(context);
		context = NULL;
	}
}
bool ModbusRtu::SetResponseTimeout(int sec, int usec)
{
	if(NULL == context)return false;
	return modbus_set_response_timeout(context, sec, usec)==0;
}
bool ModbusRtu::SetByteTimeout(int sec, int usec)
{
	if(NULL == context)return false;
	return modbus_set_byte_timeout(context, sec, usec)==0;
}
bool ModbusRtu::SetSerialMode(int mode)
{
	if(NULL == context)return false;
	return modbus_rtu_set_serial_mode(context, mode)==0;
}
bool ModbusRtu::SetSlave(int slave)
{
	if(NULL ==context)return false;
	return modbus_set_slave(context, slave)==0;
}
bool ModbusRtu::SetDebug(int debug)
{
	if(NULL ==context)return false;
	return modbus_set_debug(context, debug)==0;
}

bool ModbusRtu::ReadBits(int address, int number)
{
	if(NULL == context)return false;
	count = modbus_read_bits(context, address, number, data);
	return count = count > 0 ?  count : 0;
}
bool ModbusRtu::ReadInputBits(int address, int number)
{
	if(NULL == context)return false;
	count = modbus_read_input_bits(context, address, number, data);
	return count = count > 0 ?  count : 0;
}
bool ModbusRtu::ReadRegisters(int address, int number)
{
	if(NULL == context)return false;
	count = modbus_read_registers(context, address, number, (uint16_t*)data);
	return count = count > 0 ?  count : 0;
}
bool ModbusRtu::ReadInputRegisters(int address, int number)
{
	if(NULL == context)return false;
	count = modbus_read_input_registers(context, address, number, (uint16_t*)data);
	return count = count > 0 ?  count : 0;
}

const uint8_t ModbusRtu::GetCount(void)const
{
	return count;
}
const uint8_t  ModbusRtu::GetBit(int index)const
{
	return data[index];
}
const uint16_t ModbusRtu::GetRegister(int index)const
{
	return ((uint16_t*)data)[index];
}

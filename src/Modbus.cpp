#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "Modbus.h"

Modbus::Modbus(void)
{
	ctx = NULL;
	slave  = 0;
	fcode  = 0;
	debug  = 0;
	offset = 0;
	memset(data.bits, 0, sizeof(data));
}
Modbus::~Modbus(void)
{
	Destroy();
}
void Modbus::Destroy(void)
{
	if(ctx)
	{
		modbus_close(ctx);
		modbus_free(ctx);
		ctx = NULL;
	}
}
bool Modbus::SetSlave(int islave)
{
	return ctx ? (modbus_set_slave(ctx, slave = islave)==0) : false;
}
bool Modbus::SetDebug(int idebug)
{
	debug = idebug;
	return ctx ? (modbus_set_debug(ctx, debug)==0) : false;
}
bool Modbus::SetTimeout(int msec)
{
	return ctx ? (modbus_set_response_timeout(ctx, msec/1000, (msec%1000)*1000)==0) : false;
}
const uint16_t Modbus::operator[](uint8_t i)const
{
	if(i < count)
	{
		switch(fcode)
		{
			case MODBUS_FC_READ_COILS:
			case MODBUS_FC_READ_DISCRETE_INPUTS:
				return data.bits[i];
			case MODBUS_FC_READ_HOLDING_REGISTERS:
			case MODBUS_FC_READ_INPUT_REGISTERS:
				return data.regs[i&0x7F];
		}
	}
	return uint16_t(0);
}
//ModbusClient
ModbusClient::ModbusClient(const string& ip, int port, bool debug)
{
	if(!ip.empty())Create(ip, port);
}
ModbusClient::ModbusClient(const string& fname, int baud, int parity, int bsize, int stop, bool debug)
{
	Create(fname, baud, parity, bsize, stop);
}
bool ModbusClient::Create(const string& ip, int port, bool debug)
{
	Destroy();
	if(ctx = modbus_new_tcp(ip.data(), port))
	{
		modbus_set_debug(ctx, debug);
		return modbus_connect(ctx) != -1;
	}
	return false;
}
bool ModbusClient::Create(const string& fname, int baud, int parity, int bsize, int stop, bool debug)
{
	Destroy();
	if(ctx = modbus_new_rtu(fname.data(), baud, parity, bsize, stop))
	{
		modbus_set_debug(ctx, debug);
		return modbus_connect(ctx) != -1;
	}
	return false;
}
bool ModbusClient::ReadBits(int ioffset, int icount)
{
	count  = icount;
	offset = ioffset;
	fcode  = MODBUS_FC_READ_COILS;
	return ctx ? (modbus_read_bits(ctx, offset, count, data.bits)==count) : false;
}
bool ModbusClient::ReadInputBits(int ioffset, int icount)
{
	count  = icount;
	offset = ioffset;
	fcode  = MODBUS_FC_READ_DISCRETE_INPUTS;
	return ctx ? (modbus_read_input_bits(ctx, offset, count, data.bits)==count) : false;
}
bool ModbusClient::ReadRegisters(int ioffset, int icount)
{
	offset = ioffset;
	count  = icount;
	fcode  = MODBUS_FC_READ_HOLDING_REGISTERS;
	return ctx ? (modbus_read_registers(ctx, offset, count, data.regs)==count) : false;
}
bool ModbusClient::ReadInputRegisters(int ioffset, int icount)
{
	offset  = ioffset;
	count	= icount;
	fcode   = MODBUS_FC_READ_INPUT_REGISTERS;
	return ctx ? (modbus_read_input_registers(ctx, offset, count, data.regs)==count) : false;
}
//ModbusMapping
ModbusMapping::ModbusMapping(void)
{
	nullvalue = 0;
	mapping = NULL;
}
ModbusMapping::~ModbusMapping(void)
{
	Free();
}
void ModbusMapping::Free(void)
{
	if(mapping)
	{
		modbus_mapping_free(mapping);
		mapping = NULL;
	}
}
bool ModbusMapping::Mapping(int nb, int nib, int nreg, int nireg)
{
	return Mapping(0, nb, 0, nib, 0, nreg, 0, nireg);
}
bool ModbusMapping::Mapping(int sb, int nb, int sib, int nib, int sreg, int nreg, int sireg, int nireg)
{
	Free();
	return mapping = modbus_mapping_new_start_address(sb, nb, sib, nib, sreg, nreg, sireg, nireg);
}
uint8_t& ModbusMapping::Bit(int i)
{
	if( (mapping)
	&&  (mapping->nb_bits > 0)
	&&  (mapping->start_bits < i)
	&&  (mapping->start_bits + mapping->nb_bits > i) )
	{
		return mapping->tab_bits[i];
	}
	return *(uint8_t*)&nullvalue;
}
uint8_t& ModbusMapping::InputBit(int i)
{
	if( (mapping)
	&&  (mapping->nb_input_bits > 0)
	&&  (mapping->start_input_bits < i)
	&&  (mapping->start_input_bits + mapping->nb_input_bits > i) )
	{
		return mapping->tab_input_bits[i];
	}
	return *(uint8_t*)&nullvalue;
}
uint16_t& ModbusMapping::Register(int i)
{
	if( (mapping)
	&&  (mapping->nb_registers > 0)
	&&  (mapping->start_registers < i)
	&&  (mapping->start_registers + mapping->nb_registers > i) )
	{
		return mapping->tab_registers[i];
	}
	return nullvalue;
}
uint16_t& ModbusMapping::InputRegister(int i)
{
	if( (mapping)
	&&  (mapping->nb_input_registers > 0)
	&&  (mapping->start_input_registers < i)
	&&  (mapping->start_input_registers + mapping->nb_input_registers > i) )
	{
		return mapping->tab_input_registers[i];
	}
	return nullvalue;
}
//ModbusServer
ModbusServer::~ModbusServer(void)
{
	if(ssock != -1)
	{
		close(ssock);
		ssock = -1;
	}
}
ModbusServer::ModbusServer(const string& ip, int port, bool debug)
{
	ssock = -1;
	if(!ip.empty())Create(ip, port, debug);
}
ModbusServer::ModbusServer(const string& fname, int baud, int parity, int bsize, int stop, bool debug)
{
	ssock = -1;
	Create(fname, baud, parity, bsize, stop, debug);
}
bool ModbusServer::Create(const string& ip, int port, bool debug)
{
	if(ssock != -1)
	{
		close(ssock);
		ssock = -1;
	}
	Destroy();
	if(ctx = modbus_new_tcp(ip.data(), port))
	{
		ctx->s = -1;
		modbus_set_debug(ctx, debug);
		ssock = modbus_tcp_listen(ctx, 1);
		return	modbus_tcp_accept(ctx, &ssock) != -1;
	}
	return false;
}
bool ModbusServer::Create(const string& fname, int baud, int parity, int bsize, int stop, bool debug)
{
	Destroy();
	if(ctx = modbus_new_rtu(fname.data(), baud, parity, bsize, stop))
	{
		modbus_set_debug(ctx, debug);
		return modbus_connect(ctx) != -1;
	}
	return false;
}
int ModbusServer::Receive(void)
{
	if(NULL == ctx || NULL == mapping)
	{
		return -1;
	}
	int len = modbus_receive(ctx, data.bits);
	if(len > 0)
	{
		count = uint8_t(len);
		modbus_reply(ctx, data.bits, len, mapping);
	}
	return len;
}

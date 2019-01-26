#ifndef __MODBUS_H__
#define __MODBUS_H__
#include <string>
#include "modbus.h"
using namespace std;

//ModbusData
typedef union
{
	uint8_t  bits[256];
	uint16_t regs[128];
}ModbusData;
//Modbus
class Modbus
{
protected:
	modbus_t*	ctx;
	ModbusData	data;
public:
	uint8_t debug;
	uint8_t fcode;
	uint8_t count;
	uint8_t slave;
	uint8_t offset;
public:
	Modbus(void);
	~Modbus(void);
public:
	void Destroy(void);
	bool SetDebug(int);
	bool SetSlave(int);
	bool SetTimeout(int);
public:
	bool Create(const string&, int, bool debug=false);
	bool Create(const string&, int, int, int, int, bool debug=false);
};
//ModbusClient
class ModbusClient: public Modbus
{
public:
	ModbusClient(const string& ip="", int port=-1, bool debug=false);
	ModbusClient(const string&, int, int, int, int, bool debug=false);
public:
public:
	bool ReadBits(int, int);
	bool ReadInputBits(int, int);
	bool ReadRegisters(int, int);
	bool ReadInputRegisters(int, int);
public:
	const uint16_t operator[](uint8_t)const;
};
class ModbusMapping : public Modbus
{
protected:
	uint16_t nullvalue;
	modbus_mapping_t *mapping;
public:
	ModbusMapping(void);
	~ModbusMapping(void);
public:
	void Free(void);
	bool Mapping(int, int, int, int);
	bool Mapping(int, int, int, int, int, int, int, int);
public:
	uint8_t& Bit(int);
	uint8_t& InputBit(int);
	uint16_t& Register(int);
	uint16_t& InputRegister(int);
};
//ModbusServer
class ModbusServer : public ModbusMapping 
{
public:
	ModbusServer(const string& ip="", int port=-1, bool debug=false);
	ModbusServer(const string&, int, int, int, int, bool debug=false);
public:
	int Receive(void);
};

#endif//__MODBUS_H__

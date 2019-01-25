#ifndef __MODBUSRTU_H__
#define __MODBUSRTU_H__
#include <string>
#include "modbus.h"
using namespace std;

class ModbusRtu
{
protected:
	modbus_t* context;
	uint8_t	data[256];
	uint8_t count;
public:
	ModbusRtu(void);
	~ModbusRtu(void);
public:
	bool Create(const string&, int, int, int, int);
	bool Connect(void);
	void Close(void);
	void Destroy(void);
public:
	bool SetResponseTimeout(int, int);
	bool SetByteTimeout(int, int);
	bool SetSerialMode(int);
	bool SetSlave(int);
	bool SetDebug(int);
public:
	bool ReadBits(int, int);
	bool ReadInputBits(int, int);
	bool ReadRegisters(int, int);
	bool ReadInputRegisters(int, int);
public:
	const uint8_t GetCount(void)const;
	const uint8_t GetBit(int)const;
	const uint16_t GetRegister(int)const;
};

#endif//__MODBUSRTU_H__

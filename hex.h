//*****************************************************************
// FILE:      hex.h
// AUTHOR:    Dan Lyngaas
// LOGON ID:  z1796040
// DUE DATE:  April 16, 2020
// 
// PURPOSE:   Definition of hex class
//*****************************************************************
#ifndef HEX_H
#define HEX_H
#include<string>
#include<stdint.h>

	std::string hex8(uint8_t i);
	std::string hex32(uint32_t i);
	std::string hex0x32(uint32_t i);

#endif

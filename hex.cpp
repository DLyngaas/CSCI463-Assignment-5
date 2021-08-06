//*****************************************************************
// FILE:      hex.cpp
// AUTHOR:    Dan Lyngaas
// LOGON ID:  z1796040
// DUE DATE:  April 16, 2020
// 
// PURPOSE:   Implementation of memory class
//*****************************************************************
#include<string>
#include <sstream>
#include <stdint.h>
#include <iostream>
#include <iomanip>

//*****************************************************************
// Function: hex8
// Use: returns a corrected string for hex output of a 8 bit character
// Parameters: uint8_t
// Outputs: string
//*****************************************************************
std::string hex8(uint8_t i)
{
	std::ostringstream os;
	os << std::hex << std::setfill('0') << std::setw(2) << static_cast<uint16_t>(i);
	return os.str();
}

//*****************************************************************
// Function: hex32
// Use: returns a corrected string for hex output of a 32 bit character
// Parameters: uint32_t
// Outputs: string
//*****************************************************************
std::string hex32(uint32_t i)
{
	std::ostringstream os;
	os << std::hex << std::setfill('0') << std::setw(8) << i;
	return os.str();	
}

//*****************************************************************
// Function: hex32
// Use: returns a corrected string for hex output with a 0x lead of a 32 bit character
// Parameters: uint32_t
// Outputs: string
//*****************************************************************
std::string hex0x32(uint32_t i)
{
	return std::string("0x")+hex32(i);
}


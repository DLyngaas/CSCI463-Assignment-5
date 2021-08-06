//*****************************************************************
// FILE:      registerfile.cpp
// AUTHOR:    Dan Lyngaas
// LOGON ID:  z1796040
// DUE DATE:  April 16, 2020
// 
// PURPOSE:   Implementation of registerfile class
//*****************************************************************
#include "hex.h"
#include "memory.h"
#include "registerfile.h"
#include<string>
#include<stdint.h>
#include <iostream>
#include <iomanip>

//*****************************************************************
// Function: Constructor
// Use: Creates a registerfile object
// Parameters: none
// Outputs: none
//*****************************************************************
registerfile::registerfile()
{
	//sets register 0 to 0
	regs[0] = 0x0;
	
	//sets rest of registers to f0f0f0f0
	for(int i = 1; i < 32; i++ )
	{
		regs[i] = 0xf0f0f0f0;
	}
	regs[31]= 0xf0f0f0f0;
}

//*****************************************************************
// Function: set
// Use: set the value of the input at the address of the input
// Parameters: uint32_t for the address and int32_t for the value
// Outputs: void
//*****************************************************************
void registerfile::set(uint32_t r, int32_t val)
{
	if(r == 0x0)
	{
		return;
	}
	else
	{
		regs[r]=val;
		return;
	}
}

//*****************************************************************
// Function: get
// Use: gets the value of the input at the address of the input
// Parameters: uint32_t for the address of the wanted value
// Outputs: int32_t
//*****************************************************************
int32_t registerfile::get(uint32_t r) const
{
	return regs[r];
}

//*****************************************************************
// Function: dump
// Use: outputs the contents of the registers formatted
// Parameters: void
// Outputs: void
//*****************************************************************
void registerfile::dump() const
{
	std::string regcounter = ""; 
	for(int i = 0; i < 32; i+=8)
	{
		if(i>0)
		std::cout << std::endl;
		
		regcounter = "x" + std::to_string(i);
		std::cout << std::setw(3) << regcounter;
		for(int j = 0; j < 8; j++)
		{
			std::cout << " " << hex32(regs[j+i]);
		}
	}
}

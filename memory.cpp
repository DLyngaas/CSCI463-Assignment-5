//*****************************************************************
// FILE:      memory.cpp
// AUTHOR:    Dan Lyngaas
// LOGON ID:  z1796040
// DUE DATE:  April 16, 2020
// 
// PURPOSE:   Implementation of memory class
//*****************************************************************
#include <cstddef>
#include <string>
#include <stdint.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include "memory.h"

//*****************************************************************
// Function: Constructor
// Use: Creates a memory object with an input as the sizeof the array
// Parameters: uint32_t
// Outputs: none
//*****************************************************************
memory::memory(uint32_t siz)
{
	siz = (int)((int)siz+15)&0xfffffff0; // round the length up mod-16
	
	//Creates the memory array for the object
	uint8_t *memarray = new uint8_t[(int)siz];
	
	//Intializes all addresses in memory to 0xa5
	for(int i = 0; i<(int)siz; i++)
	{
		memarray[i] = 0xa5;
	}
	//Stores the inputted size and constructed array
	size = siz;
	mem = memarray;
}

//*****************************************************************
// Function: Destructor
// Use: Deletes the allocated memory object to free memory
// Parameters: none
// Outputs: none
//*****************************************************************
memory::~memory()
{
	delete mem;
}

//*****************************************************************
// Function: check address
// Use: Checks to see if input number is a valid address 
//      by comparing against the size
// Parameters: uint32_t
// Outputs: boolean
//*****************************************************************
bool memory::check_address(uint32_t i) const
{
	//If the input address is greater than the size or less than 0 dont assign that address
	if((int)i < (int)size || (int)i <= 0)
	{
		return true;
	}
	//Output an error if out of range
	std::cout << "WARNING: Address out of range: " << hex0x32(i) << "\n";
	return false;
}

//*****************************************************************
// Function: get size
// Use: Returns the size of the memory array
// Parameters: none
// Outputs: uint32_t
//*****************************************************************
uint32_t memory::get_size() const
{
	//returns the int size of the memory array
	return (int)size;
}

//*****************************************************************
// Function: get8
// Use: Returns the 8 bit value of the address inputted in memory
// Parameters: uint32_t for the address
// Outputs: uint8_t for the value
//*****************************************************************
uint8_t memory::get8(uint32_t addr) const
{
	//Checks address if okay then return value else return 0
	if(check_address(addr))
	{
		return mem[addr];
	}
	return 0;
}

//*****************************************************************
// Function: get16
// Use: Returns the 16 bit value of the address inputted in memory
// Parameters: uint32_t for the address
// Outputs: uint16_t for the value
//*****************************************************************
uint16_t memory::get16(uint32_t addr) const
{
	//Grabs 2 8-bit entries in memory then concats them and outputs result
	uint16_t small = (get8(addr));
	uint16_t big = ((get8(addr+1)) << 8);
	//Returns little Endian order
	return big | small;
}

//*****************************************************************
// Function: get32
// Use: Returns the 32 bit value of the address inputted in memory
// Parameters: uint32_t for the address
// Outputs: uint32_t for the value
//*****************************************************************
uint32_t memory::get32(uint32_t addr) const
{
	//Grabs 2 16-bit entries in memory then concats them and outputs result
	uint32_t small = (get16(addr));
	uint32_t big = ((get16(addr+2)) << 16);
	//Returns little Endian order
	return big | small;
}

//*****************************************************************
// Function: set8
// Use: Sets the value of the address in memory from parameters
// Parameters: uint32_t for the address and uint8_t for value
// Outputs: none
//*****************************************************************
void memory::set8(uint32_t addr,uint8_t val)
{
	//Checks address if true then assigns the value in memory
	if(check_address(addr))
	{
		mem[addr] = val;
	}
}

//*****************************************************************
// Function: set16
// Use: Sets the value of the address in memory from parameters
// Parameters: uint32_t for the address and uint16_t for value
// Outputs: none
//*****************************************************************
void memory::set16(uint32_t addr,uint16_t val)
{
	//Splits the 16 bit into 2 parts and assigns into correct memory positions
	set8(addr,(uint8_t)(val));
	set8(addr+1, (uint8_t)(val >> 8));
}

//*****************************************************************
// Function: set32
// Use: Sets the value of the address in memory from parameters
// Parameters: uint32_t for the address and uint32_t for value
// Outputs: none
//*****************************************************************
void memory::set32(uint32_t addr,uint32_t val)
{
	//Splits the 32 bit into 2 parts and assigns into correct memory positions
		set16(addr, (uint16_t)(val));
		set16(addr+2, (uint16_t)(val >> 16));
}

//*****************************************************************
// Function: dump
// Use: Dumps the entirety of stored memory and any characters from hex
// Parameters: none
// Outputs: none
//*****************************************************************
void memory::dump() const
{
	//Create char array for ASCII characters
	char ascii[16];
	
	//First loop for each line of dump
	for(int i = 0; i<(((int)size/16)); i++)
	{
		//Prints the value of each section of memory
		std::cout << hex32((i*16)) << ": ";
		
		//Second loop for first half of a line of dump
		for(int j = 0; j<8; j++)
		{
			std::cout << hex8((get8((uint32_t)(j+(16*i))))) << " ";
		}
		std::cout << " ";
		
		//Third loop for second half of a line of dump
		for(int k = 0; k<8; k++)
		{
			std::cout <<  hex8(get8((uint32_t)(8+k)+16*i)) << " ";
		}
		
		//Fourth loop to print out ASCII values of the memory of each line
		std::cout << "*";
		for(int l = 0; l<16; l++)
		{
		uint8_t ch = get8(l+(16*i));
		ascii[l%16] = isprint(ch) ? ch : '.';
		std::cout << ascii[l%16];
		}
		std::cout << "*";
		std::cout << std::endl;	
	}
}

//*****************************************************************
// Function: load file
// Use: Loads file from program parameters into memory array
// Parameters: string of the address of the file
// Outputs: bool
//*****************************************************************
bool memory::load_file(const std::string &fname)
{
	//Loads argument file into ifstream
	std::ifstream infile(fname, std::ios::in|std::ios::binary);
	if(infile)
	{
		//Iterates through file and assigns each byte into same space into memory starting at 0
	    infile.seekg (0, infile.end);
    	int length = infile.tellg();
    	infile.seekg (0, infile.beg);
		int i = 0;
		while(i<length)
		{
			uint16_t buffer[] = {0};
			infile.read((char*)buffer,1);
			set8(i,buffer[0]);
			i++;
		}
		return true;
	}
	//If can't open through an error
	std::cerr << "Can't open file '"<< fname <<"' for reading.\n";
	return false;
}

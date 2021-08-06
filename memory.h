//*****************************************************************
// FILE:      memory.h
// AUTHOR:    Dan Lyngaas
// LOGON ID:  z1796040
// DUE DATE:  April 16, 2020
// 
// PURPOSE:   Definition of memory class
//*****************************************************************
#ifndef memory_H
#define memory_H
#include "hex.h"
#include<string>
#include<stdint.h>
class memory
{
	public :
		//Constructor
		memory(uint32_t siz);
		//Destructor
		~memory();
		//functions
		bool check_address(uint32_t i) const;
		uint32_t get_size() const;

		uint8_t get8(uint32_t addr) const;
		uint16_t get16(uint32_t addr) const;
		uint32_t get32(uint32_t addr) const;
		
		void set8(uint32_t addr,uint8_t val);
		void set16(uint32_t addr,uint16_t val);
		void set32(uint32_t addr,uint32_t val);
		
		void dump() const;
		
		bool load_file(const std::string &fname);

	private :
		//class variables
		uint8_t *mem ; // the actual memory buffer
		uint32_t size ;
};

#endif

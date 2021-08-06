//*****************************************************************
// FILE:      registerfile.h
// AUTHOR:    Dan Lyngaas
// LOGON ID:  z1796040
// DUE DATE:  April 16, 2020
// 
// PURPOSE:   Definition of registerfile class
//*****************************************************************
#ifndef registerfile_H
#define registerfile_H
#include "hex.h"
#include "memory.h"
#include<string>
#include<stdint.h>
class registerfile
{
	public :
		//Constructor
		registerfile();
		
		//functions
		void set(uint32_t r, int32_t val);
		int32_t get(uint32_t r) const;
		void dump() const;
		
	private :
		//class variables
		int32_t regs [32];
};

#endif

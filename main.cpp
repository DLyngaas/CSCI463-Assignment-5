/*********************************************************************
   PROGRAM:    CSCI 463 Assignment 5 - RISC V Dissassembler
   PROGRAMMER: Dan Lyngaas
   LOGON ID:   z1796040
   DUE DATE:   April 16, 2020
 
   FUNCTION:   A C++ program to simulate a RISC V Disassembler

*********************************************************************/
#include "memory.h"
#include "rv32i.h"
#include <cstddef>
#include <string>
#include <stdint.h>
#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;
void usage();
int main ( int argc , char **argv)
{
	if(argc != 4)
		usage();

	memory mem(stoul(argv[1],0,16));
	
	if(!mem.load_file(argv[3]))
		usage();
		
 	rv32i sim(&mem);
	sim.run(stoul(argv[2]));
	mem.dump();
	
	return 0;
}

void usage()
{
	std::cout << "Usage: RISC V Disassembler -  hex for length of memory and ,  inputfile name \n";
}

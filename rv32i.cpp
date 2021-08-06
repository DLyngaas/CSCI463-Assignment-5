//*****************************************************************
// FILE:      rv32i.cpp
// AUTHOR:    Dan Lyngaas
// LOGON ID:  z1796040
// DUE DATE:  April 16, 2020
// 
// PURPOSE:   Implementation of rv32i class
//*****************************************************************
#include<string>
#include <sstream>
#include <stdint.h>
#include <iostream>
#include <iomanip>
#include "rv32i.h"

//*****************************************************************
// Function: Constructor
// Use: Creates a memory object with an input as the sizeof the array
// Parameters: pointer to memory object
// Outputs: none
//*****************************************************************

rv32i::rv32i(memory *m)
{
	mem = m;
}

//*****************************************************************
// Function: run
// Use: runs the inputted binary file and outputs the result
// Parameters: uint32_t limit for the how long it runs
// Outputs: void
//*****************************************************************
void rv32i::run(uint32_t limit)
{
	//sets register 2 to the size of memory
	regs.set(2,mem->get_size());
	
	//address of input instruction
	uint32_t address = 0;
	//instruction counter
	insn_counter++;
	//while loop stops either if halt is set or reaches end of limit
	while(!halt &&  (limit > insn_counter))
	{
		//assigns address of instruction
		address = mem->get32(pc);
		
		//outputs new line only after first line output
		if(insn_counter>1)
			std::cout << std::endl;
		
		//dumps registers
		dump();
		
		//outputs instruction
		std::cout << hex32(pc) << ": " << std::hex << hex32(address) << "  " << exec(address);
		
		//increments counter
		insn_counter++;
	}
	//last dump of registers
	dump();
}

//*****************************************************************
// Function: exec_add
// Use: executes the add function
// Parameters: uint32_t the instruction
// Outputs: string of output
//*****************************************************************
std::string rv32i::exec_add(uint32_t insn)
{
	//variables for add instruction from instruction
	std::ostringstream os;
	uint32_t rd = get_rd(insn); // get the rd register number
	
	int32_t rs1 = get_rs1(insn); // get the first register address
	rs1 = regs.get(rs1); // get the first register data
	
	int32_t rs2 = get_rs2(insn); // get the second register address
	rs2 = regs.get(rs2); // get the second register data
	
	int32_t rs = rs1 + rs2; // perform the instruction
	regs.set(rd,rs); // store the result
	
	pc += 4; // increment the pc register
	
	//write and output the formatted instruction
	os << std::dec << " x" << rd << " = " << hex0x32(rs1) << " + " << hex0x32(rs2) << " = " << hex0x32(rs);
	return os.str();
}

//*****************************************************************
// Function: exec_addi
// Use: executes the addi function
// Parameters: uint32_t the instruction
// Outputs: string of output
//*****************************************************************
std::string rv32i::exec_addi(uint32_t insn)
{
	//variables for add instruction from instruction
	std::ostringstream os;
	uint32_t rd = get_rd(insn); // get the rd register number
	
	int32_t rs1 = get_rs1(insn); // get the first register address
	rs1 = regs.get(rs1); // get the first register data
	
	int32_t imm_i = get_imm_i(insn); //  get the imm_i from the instruction
	
	int32_t rs = rs1 + imm_i; // perform the instruction
	regs.set(rd,rs); // store the result in the register
	
	pc += 4; // increment the pc register
	
	//write and output the formatted instruction
	os << std::dec << " x" << rd << " = " << hex0x32(rs1) << " + " << hex0x32(imm_i) << " = " << hex0x32(rs);
	return os.str();
}

//*****************************************************************
// Function: exec_and
// Use: executes the and function
// Parameters: uint32_t the instruction
// Outputs: string of output
//*****************************************************************
std::string rv32i::exec_and(uint32_t insn)
{
	std::ostringstream os;
	uint32_t rd = get_rd(insn); // get the rd register number
	
	int32_t rs1 = get_rs1(insn); // get the first register address
	rs1 = regs.get(rs1); // get the first register data
	
	int32_t rs2 = get_rs2(insn); // get the second register address
	rs2 = regs.get(rs2); // get the second register data
	
	int32_t rs = rs1 & rs2; // perform the instruction
	regs.set(rd,rs); // store the result
	
	pc += 4; // increment the pc register
	
	//write and output the formatted instruction
	os << std::dec << " x" << rd << " = " << hex0x32(rs1) << " & " << hex0x32(rs2) << " = " << hex0x32(rs);
	return os.str();
}

//*****************************************************************
// Function: exec_andi
// Use: executes the andi function
// Parameters: uint32_t the instruction
// Outputs: string of output
//*****************************************************************
std::string rv32i::exec_andi(uint32_t insn)
{
	//variables for add instruction from instruction
	std::ostringstream os;
	uint32_t rd = get_rd(insn); // get the rd register number
	
	int32_t rs1 = get_rs1(insn); // get the first register address
	rs1 = regs.get(rs1); // get the first register data
	
	int32_t imm_i = get_imm_i(insn); //  get the imm_i from the instruction
	
	int32_t rs = rs1 & imm_i; // perform the instruction
	regs.set(rd,rs); // store the imm_u value into rd
	
	pc += 4; // increment the pc register
	
	//write and output the formatted instruction
	os << std::dec << " x" << rd << " = " << hex0x32(rs1) << " & " << hex0x32(imm_i) << " = " << hex0x32(rs);
	return os.str();	
}

//*****************************************************************
// Function: exec_auipc
// Use: executes the auipc function
// Parameters: uint32_t the instruction
// Outputs: string of output
//*****************************************************************
std::string rv32i::exec_auipc(uint32_t insn)
{
	std::ostringstream os;
	uint32_t rd = get_rd(insn); // get the rd register number
	
	int32_t imm_u = get_imm_u(insn);
	
	int32_t rs = pc + imm_u;
	
	regs.set(rd,rs); // store the imm_u value into rd
	
	os << std::dec << " x" << rd << " = " << hex0x32(pc) << " + " << hex0x32(imm_u) << " = " << hex0x32(rs);
	pc += 4;
	return os.str();
}

//*****************************************************************
// Function: exec_beq
// Use: executes the beq function
// Parameters: uint32_t the instruction
// Outputs: string of output
//*****************************************************************
std::string rv32i::exec_beq(uint32_t insn)
{
	std::ostringstream os;
	
	int32_t rs1 = get_rs1(insn); // get the first register address
	rs1 = regs.get(rs1); // get the first register data
	
	int32_t rs2 = get_rs2(insn); // get the second register address
	rs2 = regs.get(rs2); // get the second register data
	
	int32_t imm_b = get_imm_b(insn); // get the imm_b from the instruction
	
	if(rs1 == rs2) // perform the instruction
	{
		pc += imm_b;
	}
	else
	{
		pc += 4;
	}
	
	//output the result
	os << std::dec << " pc += (" << hex0x32(rs1) << " == "  << hex0x32(rs2) << " ? " << hex0x32(imm_b) << " : 4) = " << hex0x32(pc);
	return os.str();
}

//*****************************************************************
// Function: exec_bge
// Use: executes the bge function
// Parameters: uint32_t the instruction
// Outputs: string of output
//*****************************************************************
std::string rv32i::exec_bge(uint32_t insn)
{
	std::ostringstream os;
	
	int32_t rs1 = get_rs1(insn); // get the first register address
	rs1 = regs.get(rs1); // get the first register data
	
	int32_t rs2 = get_rs2(insn); // get the second register address
	rs2 = regs.get(rs2); // get the second register data
	
	int32_t imm_b = get_imm_b(insn); // get the imm_b from the instruction
	
	if(rs1 >= rs2) // perform the instruction
	{
		pc += imm_b;
	}
	else
	{
		pc += 4;
	}
	
	//output the result
	os << std::dec << " pc += (" << hex0x32(rs1) << " >= "  << hex0x32(rs2) << " ? " << hex0x32(imm_b) << " : 4) = " << hex0x32(pc);
	return os.str();
}

//*****************************************************************
// Function: exec_bgeu
// Use: executes the bgeu function
// Parameters: uint32_t the instruction
// Outputs: string of output
//*****************************************************************
std::string rv32i::exec_bgeu(uint32_t insn)
{
	std::ostringstream os;
	
	uint32_t rs1 = get_rs1(insn); // get the first register address
	rs1 = regs.get(rs1); // get the first register data
	
	uint32_t rs2 = get_rs2(insn); // get the second register address
	rs2 = regs.get(rs2); // get the second register data
	
	int32_t imm_b = get_imm_b(insn); // get the imm_b from the instruction
	
	if(rs1 >= rs2) // perform the instruction
	{
		pc += imm_b;
	}
	else
	{
		pc += 4;
	}
	
	//output the result
	os << std::dec << " pc += (" << hex0x32(rs1) << " >=U "  << hex0x32(rs2) << " ? " << hex0x32(imm_b) << " : 4) = " << hex0x32(pc);
	return os.str();
}

//*****************************************************************
// Function: exec_blt
// Use: executes the blt function
// Parameters: uint32_t the instruction
// Outputs: string of output
//*****************************************************************
std::string rv32i::exec_blt(uint32_t insn)
{
	std::ostringstream os;
	
	int32_t rs1 = get_rs1(insn); // get the first register address
	rs1 = regs.get(rs1); // get the first register data
	
	int32_t rs2 = get_rs2(insn); // get the second register address
	rs2 = regs.get(rs2); // get the second register data
	
	int32_t imm_b = get_imm_b(insn); // get the imm_b from the instruction
	
	if(rs1 < rs2) // perform the instruction
	{
		pc += imm_b;
	}
	else
	{
		pc += 4;
	}
	
	//output the result
	os << std::dec << " pc += (" << hex0x32(rs1) << " < "  << hex0x32(rs2) << " ? " << hex0x32(imm_b) << " : 4) = " << hex0x32(pc);
	return os.str();
}

//*****************************************************************
// Function: exec_bltu
// Use: executes the bltu function
// Parameters: uint32_t the instruction
// Outputs: string of output
//*****************************************************************
std::string rv32i::exec_bltu(uint32_t insn)
{
	std::ostringstream os;
	
	uint32_t rs1 = get_rs1(insn); // get the first register address
	rs1 = regs.get(rs1); // get the first register data
	
	uint32_t rs2 = get_rs2(insn); // get the second register address
	rs2 = regs.get(rs2); // get the second register data
	
	int32_t imm_b = get_imm_b(insn); // get the imm_b from the instruction
	
	if(rs1 < rs2) // perform the instruction
	{
		pc += imm_b;
	}
	else
	{
		pc += 4;
	}
	
	//output the result
	os << std::dec << " pc += (" << hex0x32(rs1) << " <U "  << hex0x32(rs2) << " ? " << hex0x32(imm_b) << " : 4) = " << hex0x32(pc);
	return os.str();
}

//*****************************************************************
// Function: exec_bne
// Use: executes the bne function
// Parameters: uint32_t the instruction
// Outputs: string of output
//*****************************************************************
std::string rv32i::exec_bne(uint32_t insn)
{
	std::ostringstream os;
	
	int32_t rs1 = get_rs1(insn); // get the first register address
	rs1 = regs.get(rs1); // get the first register data
	
	int32_t rs2 = get_rs2(insn); // get the second register address
	rs2 = regs.get(rs2); // get the second register data
	
	int32_t imm_b = get_imm_b(insn); // get the imm_b from the instruction
	
	if(rs1 != rs2) // perform the instruction
	{
		pc += imm_b;
	}
	else
	{
		pc += 4;
	}
	
	//output the result
	os << std::dec << " pc += (" << hex0x32(rs1) << " != "  << hex0x32(rs2) << " ? " << hex0x32(imm_b) << " : 4) = " << hex0x32(pc);
	return os.str();
}

//*****************************************************************
// Function: exec_jal
// Use: executes the jal function
// Parameters: uint32_t the instruction
// Outputs: string of output
//*****************************************************************
std::string rv32i::exec_jal(uint32_t insn)
{
	std::ostringstream os;
	
	int32_t rs = 0; // result storage
	
	uint32_t rd = get_rd(insn); // get the rd register number
	
	int32_t imm_j = get_imm_j(insn); // get the imm_j from the instruction
	
	if(rd != 0) // perform the instruction
	{
		rs = pc + 4;
	}
	
	regs.set(rd,rs); // store the imm_u value into rd
	
	// output the result
	os << std::dec << " x" << rd << " = " << hex0x32(rs) << ",  pc = " << hex0x32(pc) << " + " << hex0x32(imm_j) << " = " << hex0x32((pc+imm_j));
	
	pc += imm_j; // increment the pc by imm_j
	return os.str();
}

//*****************************************************************
// Function: exec_jalr
// Use: executes the jalr function
// Parameters: uint32_t the instruction
// Outputs: string of output
//*****************************************************************
std::string rv32i::exec_jalr(uint32_t insn)
{
	std::ostringstream os;
	uint32_t rd = get_rd(insn); // get the rd register number
	
	int32_t rs1 = get_rs1(insn); // get the first register address
	rs1 = regs.get(rs1); // get the first register data
	
	int32_t imm_i = get_imm_i(insn); // get the imm_i from the instruction
	
	int32_t rs1i = ((rs1+imm_i) & ~1); // perform the instruction
	
	int32_t rs = pc + 4; // increment the pc register
	
	regs.set(rd,rs); // store the result value into rd
	
	pc = rs1i; // jump to the next instruction

	// output the result
	os << std::dec << " x" << rd << " = " << hex0x32(rs) << ",  pc = (" << hex0x32(imm_i) << " + " << hex0x32(rs1) << ") & 0xfffffffe = " << std::hex << hex0x32(rs1i);
	return os.str();
}

//*****************************************************************
// Function: exec_lb
// Use: executes the lb function
// Parameters: uint32_t the instruction
// Outputs: string of output
//*****************************************************************
std::string rv32i::exec_lb(uint32_t insn)
{
	std::ostringstream os;
	uint32_t rd = get_rd(insn); // get the rd register number
	
	int32_t rs1 = get_rs1(insn); // get the first register address
	rs1 = regs.get(rs1); // get the first register data
	
	int32_t imm_i = get_imm_i(insn); // get the imm_i from the instruction
	
	int32_t rs = mem->get8(rs1+imm_i); // get the byte from storage
	
	if(rs & 0x00000080) // remove the leading bits
	{
		rs |= 0xFFFFFF80;
	}
	regs.set(rd,rs); // store the value into rd
	pc += 4; // increment the pc register
	// output the result
	os << std::dec << " x" << rd << " = sx(m8(" << hex0x32(rs1) << " + " << hex0x32(imm_i) << ")) = " << hex0x32(rs);
	return os.str();
}

//*****************************************************************
// Function: exec_lbu
// Use: executes the lbu function
// Parameters: uint32_t the instruction
// Outputs: string of output
//*****************************************************************
std::string rv32i::exec_lbu(uint32_t insn)
{
	std::ostringstream os;
	uint32_t rd = get_rd(insn); // get the rd register number
	
	uint32_t rs1 = get_rs1(insn); // get the first register address
	rs1 = regs.get(rs1); // get the first register data
	
	uint32_t imm_i = get_imm_i(insn); // get the imm_i from the instruction
	
	uint32_t rs = mem->get8(rs1+imm_i); // get the stored byte from the unsigned address

	regs.set(rd,rs); // store the result value into rd
	pc += 4; // increment the pc register
	// output the result
	os << std::dec << " x" << rd << " = zx(m8(" << hex0x32(rs1) << " + " << hex0x32(imm_i) << ")) = " << hex0x32(rs);
	return os.str();
}

//*****************************************************************
// Function: exec_lh
// Use: executes the lh function
// Parameters: uint32_t the instruction
// Outputs: string of output
//*****************************************************************
std::string rv32i::exec_lh(uint32_t insn)
{
	std::ostringstream os;
	uint32_t rd = get_rd(insn); // get the rd register number
	
	int32_t rs1 = get_rs1(insn); // get the first register address
	rs1 = regs.get(rs1); // get the first register data
	
	int32_t imm_i = get_imm_i(insn); // get the imm_i from the instruction
	
	int32_t rs = mem->get16(rs1+imm_i); // get the halfword from the address
	
	if(rs & 0x00008000) // remove the leading bits
	{
		rs |= 0xFFFF8000;
	}
	regs.set(rd,rs); // store the result value into rd
	pc += 4; // increment the pc register
	// output the result
	os << std::dec << " x" << rd << " = sx(m16(" << hex0x32(rs1) << " + " << hex0x32(imm_i) << ")) = " << hex0x32(rs);
	return os.str();
}

//*****************************************************************
// Function: exec_lhu
// Use: executes the lhu function
// Parameters: uint32_t the instruction
// Outputs: string of output
//*****************************************************************
std::string rv32i::exec_lhu(uint32_t insn)
{
	std::ostringstream os;
	uint32_t rd = get_rd(insn); // get the rd register number
	
	uint32_t rs1 = get_rs1(insn); // get the first register address
	rs1 = regs.get(rs1); // get the first register data
	
	uint32_t imm_i = get_imm_i(insn); // get the imm_i from the instruction
		
	int32_t rs = mem->get16(rs1+imm_i); //get the halfword at the unsigned address

	regs.set(rd,rs); // store the value into rd
	pc += 4; // increment the pc register
	// output the result
	os << std::dec << " x" << rd << " = zx(m16(" << hex0x32(rs1) << " + " << hex0x32(imm_i) << ")) = " << hex0x32(rs);
	return os.str();
}

//*****************************************************************
// Function: exec_lui
// Use: executes the lui function
// Parameters: uint32_t the instruction
// Outputs: string of output
//*****************************************************************
std::string rv32i::exec_lui(uint32_t insn)
{
	std::ostringstream os;
	uint32_t rd = get_rd(insn); // get the rd register number
	int32_t imm_u = get_imm_u(insn); // get the imm_u value
	regs.set(rd,imm_u); // store the imm_u value into rd
	pc += 4; // advance the pc past the end of this instruction

	// Return a string indicating the operation that has taken place
	os << std::dec << " x" << rd << " = " << hex0x32(imm_u);
	return os.str();
}

//*****************************************************************
// Function: exec_lw
// Use: executes the lw function
// Parameters: uint32_t the instruction
// Outputs: string of output
//*****************************************************************
std::string rv32i::exec_lw(uint32_t insn)
{
	std::ostringstream os;
	uint32_t rd = get_rd(insn); // get the rd register number
	
	uint32_t rs1 = get_rs1(insn); // get the first register address
	rs1 = regs.get(rs1); // get the first register data
	
	int32_t imm_i = get_imm_i(insn); // get the imm_i from the instruction
	
	int32_t rs = mem->get32(rs1+imm_i); // get the word at the address
	
	regs.set(rd,rs); // store the word value into rd
	pc += 4; // increment the pc register
	// output the result
	os << std::dec << " x" << rd << " = sx(m32(" << hex0x32(rs1) << " + " << hex0x32(imm_i) << ")) = " << hex0x32(rs);
	return os.str();
}

//*****************************************************************
// Function: exec_or
// Use: executes the or function
// Parameters: uint32_t the instruction
// Outputs: string of output
//*****************************************************************
std::string rv32i::exec_or(uint32_t insn)
{
	std::ostringstream os;
	uint32_t rd = get_rd(insn); // get the rd register number
	
	int32_t rs1 = get_rs1(insn); // get the first register address
	rs1 = regs.get(rs1); // get the first register data
	
	
	int32_t rs2 = get_rs2(insn); // get the second register address
	rs2 = regs.get(rs2); // get the second register data
	
	int32_t rs = rs1 | rs2; // perform the instruction
	
	regs.set(rd,rs); // store the value into rd
	pc += 4; // increment the pc register
	// output the result
	os << std::dec << " x" << rd << " = " << hex0x32(rs1) << " | " << hex0x32(rs2) << " = " << hex0x32(rs);
	return os.str();
}

//*****************************************************************
// Function: exec_ori
// Use: executes the ori function
// Parameters: uint32_t the instruction
// Outputs: string of output
//*****************************************************************
std::string rv32i::exec_ori(uint32_t insn)
{
	std::ostringstream os;
	uint32_t rd = get_rd(insn); // get the rd register number
	
	int32_t rs1 = get_rs1(insn); // get the first register address
	rs1 = regs.get(rs1); // get the first register data
	
	
	int32_t imm_i = get_imm_i(insn);  // get the imm_i from the instruction
	
	int32_t rs = rs1 | imm_i; // perform the instruction
	regs.set(rd,rs); // store the value into rd
	pc += 4; // increment the pc register
	// output the result
	os << std::dec << " x" << rd << " = " << hex0x32(rs1) << " | " << hex0x32(imm_i) << " = " << hex0x32(rs);
	return os.str();
}

//*****************************************************************
// Function: exec_sb
// Use: executes the sb function
// Parameters: uint32_t the instruction
// Outputs: string of output
//*****************************************************************
std::string rv32i::exec_sb(uint32_t insn)
{
	std::ostringstream os;
	
	int32_t rs1 = get_rs1(insn); // get the first register address
	rs1 = regs.get(rs1); // get the first register data
	
	int32_t rs2 = get_rs2(insn); // get the second register address
	rs2 = regs.get(rs2); // get the second register data
	
	int32_t imm_s = get_imm_s(insn); // get the imm_s value from the instruction

	mem->set8((rs1+imm_s),rs2); // perform the instruction
	
	pc += 4; // increment the pc register
	// output the result
	os << std::dec << " m8(" << hex0x32(rs1) << " + " << hex0x32(imm_s) << ") = " << hex0x32(mem->get8(rs1+imm_s));
	return os.str();
}

//*****************************************************************
// Function: exec_sh
// Use: executes the sh function
// Parameters: uint32_t the instruction
// Outputs: string of output
//*****************************************************************
std::string rv32i::exec_sh(uint32_t insn)
{
	std::ostringstream os;
	
	int32_t rs1 = get_rs1(insn); // get the first register address
	rs1 = regs.get(rs1); // get the first register data
	
	int32_t rs2 = get_rs2(insn); // get the second register address
	rs2 = regs.get(rs2); // get the second register data
	
	int32_t imm_s = get_imm_s(insn); // get the imm_s value from the instruction

	mem->set16((rs1+imm_s),rs2); // perform the instruction
	
	pc += 4; // increment the pc register
	// output the result
	os << std::dec << " m16(" << hex0x32(rs1) << " + " << hex0x32(imm_s) << ") = " << hex0x32(mem->get16(rs1+imm_s));
	return os.str();
}

//*****************************************************************
// Function: exec_sll
// Use: executes the sll function
// Parameters: uint32_t the instruction
// Outputs: string of output
//*****************************************************************
std::string rv32i::exec_sll(uint32_t insn)
{
	std::ostringstream os;
	uint32_t rd = get_rd(insn); // get the rd register number
	
	int32_t rs1 = get_rs1(insn); // get the first register address
	rs1 = regs.get(rs1); // get the first register data
	
	int32_t rs2 = get_rs2(insn); // get the second register address
	rs2 = regs.get(rs2); // get the second register data
	
	int32_t rs = (rs1 << (rs2%XLEN)); // perform the instruction
	
	regs.set(rd,rs); // store the value into rd
	
	pc += 4; // increment the pc register
	// output the result
	os << std::dec << " x" << rd << " = " << hex0x32(rs1) << " << " << (rs2%XLEN) << " = " << hex0x32(rs);
	return os.str();
}

//*****************************************************************
// Function: exec_slli
// Use: executes the slli function
// Parameters: uint32_t the instruction
// Outputs: string of output
//*****************************************************************
std::string rv32i::exec_slli(uint32_t insn)
{
	std::ostringstream os;
	uint32_t rd = get_rd(insn); // get the rd register number
	
	int32_t rs1 = get_rs1(insn); // get the first register address
	rs1 = regs.get(rs1); // get the first register data
	
	int32_t shamt = get_rs2(insn); // get the shamt value from the instruction
	
	int32_t rs = (rs1 << shamt); // perform the instruction
	
	regs.set(rd,rs); // store the value into rd
	
	pc += 4; // increment the pc register
	// output the result
	os << std::dec << " x" << rd << " = " << hex0x32(rs1) << " << " << shamt << " = " << hex0x32(rs);
	return os.str();
}

//*****************************************************************
// Function: exec_slt
// Use: executes the slt function
// Parameters: uint32_t the instruction
// Outputs: string of output
//*****************************************************************
std::string rv32i::exec_slt(uint32_t insn)
{
	std::ostringstream os;
	uint32_t rd = get_rd(insn); // get the rd register number
	
	int32_t rs1 = get_rs1(insn); // get the first register address
	rs1 = regs.get(rs1); // get the first register data
	
	int32_t rs2 = get_rs2(insn); // get the second register address
	rs2 = regs.get(rs2); // get the second register data
	
	int32_t rs=0; // default result to 0
	
	if(rs1 < rs2) // perform instruction
	{
		rs = 1;
	}
	else
	{
		rs = 0;
	}
	regs.set(rd,rs); // store the value into rd
	
	pc += 4; // increment the pc register
	// output the result
	os << std::dec << " x" << rd << " = (" << hex0x32(rs1) << " < " << hex0x32(rs2) << ") ? 1 : 0 = " << hex0x32(rs);
	return os.str();
}

//*****************************************************************
// Function: exec_slti
// Use: executes the slti function
// Parameters: uint32_t the instruction
// Outputs: string of output
//*****************************************************************
std::string rv32i::exec_slti(uint32_t insn)
{
	std::ostringstream os;
	uint32_t rd = get_rd(insn); // get the rd register number
	
	int32_t rs1 = get_rs1(insn); // get the first register address
	rs1 = regs.get(rs1); // get the first register data
	
	int32_t imm_i = get_imm_i(insn); // get the imm_i value from the instruction
	
	int32_t rs=0; // default the result to 0
	
	if(rs1 < imm_i) // perform the instruction
	{
		rs = 1;
	}
	else
	{
		rs = 0;
	}
	regs.set(rd,rs); // store the value into rd
	
	pc += 4; // increment the pc register
	// output the result
	os << std::dec << " x" << rd << " = (" << hex0x32(rs1) << " < " << imm_i << ") ? 1 : 0 = " << hex0x32(rs);
	return os.str();
}

//*****************************************************************
// Function: exec_sltiu
// Use: executes the sltiu function
// Parameters: uint32_t the instruction
// Outputs: string of output
//*****************************************************************
std::string rv32i::exec_sltiu(uint32_t insn)
{
	std::ostringstream os;
	uint32_t rd = get_rd(insn); // get the rd register number
	
	uint32_t rs1 = get_rs1(insn); // get the first register address
	rs1 = regs.get(rs1); // get the first register data
	
	uint32_t imm_i = get_imm_i(insn); // get the imm_i value from the instruction
	
	int32_t rs=0; // default the result to 0
	
	if(rs1 < imm_i) // perform the instruction
	{
		rs = 1;
	}
	else
	{
		rs = 0;
	}
	regs.set(rd,rs); // store the value into rd
	
	pc += 4; // increment the pc register
	// output the result
	os << std::dec << " x" << rd << " = (" << hex0x32(rs1) << " <U " << imm_i << ") ? 1 : 0 = " << hex0x32(rs);
	return os.str();
}

//*****************************************************************
// Function: exec_sltu
// Use: executes the sltu function
// Parameters: uint32_t the instruction
// Outputs: string of output
//*****************************************************************
std::string rv32i::exec_sltu(uint32_t insn)
{
	std::ostringstream os;
	uint32_t rd = get_rd(insn); // get the rd register number
	
	uint32_t rs1 = get_rs1(insn); // get the first register address
	rs1 = regs.get(rs1); // get the first register data
	
	uint32_t rs2 = get_rs2(insn); // get the second register address
	rs2 = regs.get(rs2); // get the second register data
	
	int32_t rs=0; // default the result to 0
	
	if(rs1 < rs2) // perform the instruction
	{
		rs = 1;
	}
	else
	{
		rs = 0;
	}
	
	regs.set(rd,rs); // store the value into rd
	
	pc += 4; // increment the pc register
	// output the result
	os << std::dec << " x" << rd << " = (" << hex0x32(rs1) << " <U " << hex0x32(rs2) << ") ? 1 : 0 = " << hex0x32(rs);
	return os.str();
}

//*****************************************************************
// Function: exec_sra
// Use: executes the sra function
// Parameters: uint32_t the instruction
// Outputs: string of output
//*****************************************************************
std::string rv32i::exec_sra(uint32_t insn)
{
	std::ostringstream os;
	uint32_t rd = get_rd(insn); // get the rd register number
	
	int32_t rs1 = get_rs1(insn); // get the first register address
	rs1 = regs.get(rs1); // get the first register data
	
	int32_t rs2 = get_rs2(insn); // get the second register address
	rs2 = regs.get(rs2); // get the second register data
	
	int32_t rs = (rs1 >> (rs2%XLEN)); // perform the instruction
	
	regs.set(rd,rs); // store the value into rd
	
	pc += 4; // increment the pc register
	// output the result
	os << std::dec << " x" << rd << " = " << hex0x32(rs1) << " >> " << (rs2%XLEN) << " = " << hex0x32(rs);
	return os.str();
}

//*****************************************************************
// Function: exec_srai
// Use: executes the srai function
// Parameters: uint32_t the instruction
// Outputs: string of output
//*****************************************************************
std::string rv32i::exec_srai(uint32_t insn)
{
	std::ostringstream os;
	uint32_t rd = get_rd(insn); // get the rd register number
	
	int32_t rs1 = get_rs1(insn); // get the first register address
	rs1 = regs.get(rs1); // get the first register data
	
	int32_t shamt = get_rs2(insn); // get the shamt from the instruction
	
	int32_t rs = (rs1 >> shamt); // perform the instructiom
	
	regs.set(rd,rs); // store the value into rd
	
	pc += 4; // increment the pc register
	// output the result
	os << std::dec << " x" << rd << " = " << hex0x32(rs1) << " >> " << shamt << " = " << hex0x32(rs);
	return os.str();
}

//*****************************************************************
// Function: exec_srl
// Use: executes the srl function
// Parameters: uint32_t the instruction
// Outputs: string of output
//*****************************************************************
std::string rv32i::exec_srl(uint32_t insn)
{
	std::ostringstream os;
	uint32_t rd = get_rd(insn); // get the rd register number
	
	uint32_t rs1 = get_rs1(insn); // get the first register address
	rs1 = regs.get(rs1); // get the first register data
	
	int32_t rs2 = get_rs2(insn); // get the second register address
	rs2 = regs.get(rs2); // get the second register data
	
	int32_t rs = (rs1 >> (rs2%XLEN)); // perform the instructiom
	
	regs.set(rd,rs); // store the value into rd
	
	pc += 4; // increment the pc register
	// output the result
	os << std::dec << " x" << rd << " = " << hex0x32(rs1) << " >> " << (rs2%XLEN) << " = " << hex0x32(rs);
	return os.str();
}

//*****************************************************************
// Function: exec_srli
// Use: executes the srli function
// Parameters: uint32_t the instruction
// Outputs: string of output
//*****************************************************************
std::string rv32i::exec_srli(uint32_t insn)
{
	std::ostringstream os;
	uint32_t rd = get_rd(insn); // get the rd register number
	
	uint32_t rs1 = get_rs1(insn); // get the first register address
	rs1 = regs.get(rs1); // get the first register data
	
	uint32_t shamt = get_rs2(insn); // get the shamt from the instruction
	
	int32_t rs = (rs1 >> shamt); // perform the instructiom
	
	regs.set(rd,rs); // store the value into rd
	
	pc += 4; // increment the pc register
	// output the result
	os << std::dec << " x" << rd << " = " << hex0x32(rs1) << " >> " << shamt << " = " << hex0x32(rs);
	return os.str();
}

//*****************************************************************
// Function: exec_sub
// Use: executes the sub function
// Parameters: uint32_t the instruction
// Outputs: string of output
//*****************************************************************
std::string rv32i::exec_sub(uint32_t insn)
{
	std::ostringstream os;
	uint32_t rd = get_rd(insn); // get the rd register number
	
	int32_t rs1 = get_rs1(insn); // get the first register address
	rs1 = regs.get(rs1); // get the first register data
	
	int32_t rs2 = get_rs2(insn); // get the second register address
	rs2 = regs.get(rs2); // get the second register data
	
	int32_t rs = rs1 - rs2; // perform the instructiom
	
	regs.set(rd,rs); // store the value into rd
	
	pc += 4; // increment the pc register
	// output the result
	os << std::dec << " x" << rd << " = " << hex0x32(rs1) << " - " << hex0x32(rs2) << " = " << hex0x32(rs);
	return os.str();
}

//*****************************************************************
// Function: exec_sw
// Use: executes the sw function
// Parameters: uint32_t the instruction
// Outputs: string of output
//*****************************************************************
std::string rv32i::exec_sw(uint32_t insn)
{
	std::ostringstream os;
	
	int32_t rs1 = get_rs1(insn); // get the first register address
	rs1 = regs.get(rs1); // get the first register data
		
	int32_t rs2 = get_rs2(insn); // get the second register address
	rs2 = regs.get(rs2); // get the second register data
	
	int32_t imm_s = get_imm_s(insn); // get the imm_s from the instruction
	
	mem->set32((rs1+imm_s),rs2); // store the word at the address
	
	pc += 4; // increment the pc register
	// output the result
	os << std::dec << " m32(" << hex0x32(rs1) << " + " << hex0x32(imm_s) << ") = " << hex0x32(mem->get32((rs1+imm_s)));
	return os.str();
}

//*****************************************************************
// Function: exec_xor
// Use: executes the xor function
// Parameters: uint32_t the instruction
// Outputs: string of output
//*****************************************************************
std::string rv32i::exec_xor(uint32_t insn)
{
	std::ostringstream os;
	uint32_t rd = get_rd(insn); // get the rd register number
	
	int32_t rs1 = get_rs1(insn); // get the first register address
	rs1 = regs.get(rs1); // get the first register data
		
	int32_t rs2 = get_rs2(insn); // get the second register address
	rs2 = regs.get(rs2); // get the second register data
	
	int32_t rs = rs1 ^ rs2; // perform the instructiom
	
	regs.set(rd,rs); // store the value into rd
	
	pc += 4; // increment the pc register
	// output the result
	os << std::dec << " x" << rd << " = " << hex0x32(rs1) << " ^ " << hex0x32(rs2) << " = " << hex0x32(rs);
	return os.str();
}

//*****************************************************************
// Function: exec_xori
// Use: executes the xori function
// Parameters: uint32_t the instruction
// Outputs: string of output
//*****************************************************************
std::string rv32i::exec_xori(uint32_t insn)
{
	std::ostringstream os;
	uint32_t rd = get_rd(insn); // get the rd register number
	
	int32_t rs1 = get_rs1(insn); // get the first register address
	rs1 = regs.get(rs1); // get the first register data
		
	int32_t imm_i = get_imm_i(insn); // get the imm_i value from the instruction
	
	int32_t rs = rs1 ^ imm_i; // perform the instructiom
	
	regs.set(rd,rs); // store the value into rd
	
	pc += 4; // increment the pc register
	// output the result
	os << std::dec << " x" << rd << " = " << hex0x32(rs1) << " ^ " << hex0x32(imm_i) << " = " << hex0x32(rs);
	return os.str();
}

//*****************************************************************
// Function: exec_ebreak
// Use: executes the ebreak function
// Parameters: uint32_t the instruction
// Outputs: string of output
//*****************************************************************
std::string rv32i::exec_ebreak(uint32_t insn)
{
(void)insn; // shut up unused argument compiler warning
halt = true; // stop the simulator
return "HALT"; // the operation comment
}

//*****************************************************************
// Function: get_opcode
// Use: returns the opcode from the submitted command
// Parameters: uint32_t
// Outputs: uint32_t
//*****************************************************************
uint32_t rv32i::get_opcode(uint32_t insn)
{
	int32_t opcode = (insn & 0x0000007f);
	return opcode;
}

//*****************************************************************
// Function: get_rd
// Use: returns the rd parameter from the submitted command
// Parameters: uint32_t
// Outputs: uint32_t
//*****************************************************************
uint32_t rv32i::get_rd(uint32_t insn)
{
	int32_t rd = (insn & 0x00000fC0) >> (7-0); // extract & shift bits 0 - 4
	return rd;
}

//*****************************************************************
// Function: get_funct3
// Use: returns the funct_3 parameter from the submitted command
// Parameters: uint32_t
// Outputs: uint32_t
//*****************************************************************
uint32_t rv32i::get_funct3(uint32_t insn)
{
	int32_t funct3 = (insn & 0x00007000) >> (12-0); // extract & shift bits 0 - 2
	return funct3;
}

//*****************************************************************
// Function: get_rs1
// Use: returns the rs1 parameter from the submitted command
// Parameters: uint32_t
// Outputs: uint32_t
//*****************************************************************
uint32_t rv32i::get_rs1(uint32_t insn)
{
	int32_t rs1 = (insn & 0x000F8000) >> (15-0); // extract & shift bits 0 - 4
	return rs1;
}

//*****************************************************************
// Function: get_rs2
// Use: returns the rs2 parameter from the submitted command
// Parameters: uint32_t
// Outputs: uint32_t
//*****************************************************************
uint32_t rv32i::get_rs2(uint32_t insn)
{
	int32_t rs2 = (insn & 0x01F00000) >> (20-0); // extract & shift bits 0 - 4
	return rs2;
}

//*****************************************************************
// Function: get_funct7
// Use: returns the funct7 parameter from the submitted command
// Parameters: uint32_t
// Outputs: uint32_t
//*****************************************************************
uint32_t rv32i::get_funct7(uint32_t insn)
{
	int32_t funct7 = (insn & 0xFE000000) >> (25-0); // extract & shift bits 0 - 6
	return funct7;
}

//*****************************************************************
// Function: get_imm_i
// Use: returns the adjusted command in imm_i format
// Parameters: uint32_t
// Outputs: uint32_t
//*****************************************************************
int32_t rv32i::get_imm_i(uint32_t insn)
{
	int32_t imm_i=(insn & 0xfff00000) >> (20-0); // extract & shift bits 0 - 11
	
	if (insn & 0x80000000) // sign - extend
		imm_i |= 0xFFFFF000;
		
	return imm_i;
}

//*****************************************************************
// Function: get_imm_u
// Use: returns the adjusted command in imm_u format
// Parameters: uint32_t
// Outputs: uint32_t
//*****************************************************************
int32_t rv32i::get_imm_u(uint32_t insn)
{
	int32_t imm_u=(insn & 0xfffff000); // extract bits 0 - 11
	
	return imm_u;	
}

//*****************************************************************
// Function: get_imm_b
// Use: returns the adjusted command in imm_b format
// Parameters: uint32_t
// Outputs: uint32_t
//*****************************************************************
int32_t rv32i::get_imm_b(uint32_t insn)
{
	int32_t imm_b= (insn & 0x7E000000) >> (25-5); // extract & shift bits 5 - 10
	imm_b |= (insn & 0x00000F00) >> (8-1); // extract & shift bits 1 - 4
	
	if (insn & 0x00000080) // move bit in 7th position to the 11th position
		imm_b |= 0x00000800;
		
	if (insn & 0x80000000) // sign - extend
		imm_b |= 0xfffff000;
		
	return imm_b;
}

//*****************************************************************
// Function: get_imm_s
// Use: returns the adjusted command in imm_s format
// Parameters: uint32_t
// Outputs: uint32_t
//*****************************************************************
int32_t rv32i::get_imm_s(uint32_t insn)
{
	int32_t imm_s=(insn & 0xfe000000) >> (25-5); // extract & shift bits 5 - 11
	imm_s |= (insn & 0x00000f80) >> (7-0); // extract & shift bits 0 - 4
	
	if (insn & 0x80000000) // sign - extend
		imm_s |= 0xfffff000;
	return imm_s;
}

//*****************************************************************
// Function: get_imm_j
// Use: returns the adjusted command in imm_j format
// Parameters: uint32_t
// Outputs: uint32_t
//*****************************************************************
int32_t rv32i::get_imm_j(uint32_t insn)
{
	int32_t imm_j=(insn & 0x7fe00000) >> (21-1); // extract & shift bits 1 - 11
	imm_j |= ((insn & 0x00100000) >> (20-11)); // move bit in 20th position to the 11th position
	imm_j |= (insn & 0x000FF000); // extract bits 12 - 19
	
	if (insn & 0x80000000) // sign - extend
		imm_j |= 0xfff00000;
	
	return imm_j;
}

//*****************************************************************
// Function: run
// Use: runs the inputted binary file through the decoder
// Parameters: void
// Outputs: void
//*****************************************************************
std::string rv32i::exec(uint32_t insn)
{
	std::ostringstream os; // output stream
	std::ostringstream temp_os; // temp output stream to format width
	std::string execution = ""; // string returned from each execute command
	int32_t new_address; // new address for branch commands
	
	switch (get_opcode(insn)) //find the correct command based on the op code; execute it and output it
	{
		case 0b1110011: // ECALL
			if(insn & 0x100000)
			{
				os << "ebreak                    // " << exec_ebreak(insn) << std::endl << insn_counter <<" instructions executed.\n";
	    	}
	    	else
	    	{
				os << "ecall                     // " << exec_ebreak(insn) << std::endl << insn_counter <<" instructions executed.\n";
			}
			break;

		case 0b0110111: // LUI
			temp_os << std::dec << get_rd(insn) << ",0x" << std::hex << ((get_imm_u(insn) >> 12)&0x0fffff);
			os << "lui    x" << std::setw(18) << std::left << temp_os.str();
			os << "//" << exec_lui(insn);
			break;

		case 0b0010111: // AUIPC
			temp_os << std::dec << get_rd(insn) << ",0x" << std::hex << ((get_imm_u(insn) >> 12)&0x0fffff);
			os << "auipc  x" << std::setw(18) << std::left << temp_os.str();
			os << "//" << exec_auipc(insn);
			break;

		case 0b1101111: // JAL
			temp_os << std::dec << get_rd(insn) << ",0x" << std::hex << (get_imm_j(insn) + pc);
			os << "jal    x" << std::setw(18) << std::left << temp_os.str();
			os << "//" << exec_jal(insn);
			break;

		case 0b1100111: // JALR
			temp_os << std::dec << get_rd(insn) << "," << ((get_imm_i(insn))) << "(x" << get_rs1(insn) << ")";
			os << "jalr   x" << std::setw(18) << std::left << temp_os.str();
			os << "//" << exec_jalr(insn);	
			break;

		case 0b1100011: // B-TYPE
			new_address=(pc+get_imm_b(insn));
			switch (get_funct3(insn))
			{
				case 0b000: 
					os << "beq    "; 
					execution = exec_beq(insn);
					break;
				case 0b001: 
					os << "bne    ";
					execution = exec_bne(insn);
					break;
				case 0b100: 
					os << "blt    "; 
					execution = exec_blt(insn);
					break;
				case 0b101: 
					os << "bge    ";
					execution = exec_bge(insn);
					break;
				case 0b110: 
					os << "bltu   ";
					execution = exec_bltu(insn);
					break;
				case 0b111: 
					os << "bgeu   ";
					execution = exec_bgeu(insn);
					break;
				default:
					os << "ERROR";
					return os.str();
			}
			temp_os << std::dec << get_rs1(insn) << ",x" << get_rs2(insn) << ",0x" << std::hex << new_address;
			os << "x" << std::setw(18) << std::left << temp_os.str();
			os << "//" << execution;
			break;

		case 0b0000011: // I-TYPE LB-LHU
			switch (get_funct3(insn))
			{
				case 0b000:
					os << "lb     ";
					execution = exec_lb(insn);
					break;
		
				case 0b001:
					os << "lh     ";
					execution = exec_lh(insn);
					break;
		
				case 0b010:
					os << "lw     ";
					execution = exec_lw(insn);
					break;
		
				case 0b100:
					os << "lbu    ";
					execution = exec_lbu(insn);
					break;
		
				case 0b101:
					os << "lhu    ";
					execution = exec_lhu(insn);
					break;
		
				default:
					os << "ERROR";
					return os.str();
			}
			temp_os << std::dec << get_rd(insn) << "," << ((get_imm_i(insn))) << "(x" << get_rs1(insn) << ")";
			os << "x" << std::setw(18) << std::left << temp_os.str();
			os << "//" << execution;
			break;

		case 0b0100011: // S - type store instructions
			switch (get_funct3(insn))
			{
				case 0b000:
					os << "sb     ";
					execution = exec_sb(insn);
					break;
				case 0b001:
					os << "sh     ";
					execution = exec_sh(insn);
				break;
				case 0b010:
					os << "sw     ";
					execution = exec_sw(insn);
					break;
				default:
			 		os << "ERROR";
					return os.str();
			}
			temp_os << std::dec << get_rs2(insn) << "," << get_imm_s(insn) << "(x" << get_rs1(insn) << ")";
			os << "x" << std::setw(18) << std::left << temp_os.str();
			os << "//" << execution;
			break;

		case 0b0010011: // I-TYPE ADDI-SRAI
			switch (get_funct3(insn))
			{
				case 0b000:
					os << "addi   ";
					execution = exec_addi(insn);
					break;
				case 0b010:
					os << "slti   ";
					execution = exec_slti(insn);
					break;
				case 0b011:
					os << "sltiu  ";
					execution = exec_sltiu(insn);
					break;
				case 0b100:
					os << "xori   ";
					execution = exec_xori(insn);
					break;
				case 0b110:
					os << "ori    ";
					execution = exec_ori(insn);
					break;
				case 0b111:
					os << "andi   ";
					execution = exec_andi(insn);
					break;
				case 0b001:
					os << "slli   ";
					execution = exec_slli(insn);
					break;
				case 0b101:
					switch (get_funct7(insn))
					{
						case 0b0000000:
							os << "srli   ";
							execution = exec_srli(insn);
							break;
						case 0b0100000:
							execution = exec_srai(insn);
							temp_os << std::dec << get_rd(insn) << ",x" << get_rs1(insn) << "," << (get_imm_i(insn) & 0x0000001F);
							os << "srai   x" << std::setw(18) << std::left << temp_os.str();
							os << "//" << execution;
							return os.str();
							break;
						default:
							os << "ERROR";
							return os.str();
							break;
					}
					break;
				default:
					os << "ERROR";
					return os.str();
					break;
			}
			temp_os << std::dec << get_rd(insn) << ",x" << get_rs1(insn) << "," << get_imm_i(insn);
			os << "x" << std::setw(18) << std::left << temp_os.str();
			os << "//" << execution;
			break;

		case 0b0110011: // R-TYPE
			switch (get_funct3(insn))
			{
				case 0b000:
					switch (get_funct7(insn))
					{
						case 0b0000000:
							os << "add    ";
							execution = exec_add(insn);
							break;
						case 0b0100000:
							os << "sub    ";
							execution = exec_sub(insn);
							break;
						default:
							os << "ERROR";
							return os.str();
							break;
					}
					break;
				case 0b001:
					os << "sll    ";
					execution = exec_sll(insn);
					break;
				case 0b010:
					os << "slt    ";
					execution = exec_slt(insn);
					break;
				case 0b011:
					os << "sltu   ";
					execution = exec_sltu(insn);
					break;
				case 0b100:
					os << "xor    ";
					execution = exec_xor(insn);
					break;
				case 0b101:
					switch (get_funct7(insn))
					{
						case 0b0000000:
							os << "srl    ";
							execution = exec_srl(insn);
							break;
						case 0b0100000:
							os << "sra    ";
							execution = exec_sra(insn);
							break;
						default:
							os << "ERROR";
							return os.str();
						break;
					}
					break;
				case 0b110:
					os << "or     ";
					execution = exec_or(insn);
					break;
				case 0b111:
					os << "and    ";
					execution = exec_and(insn);
					break;
				default:
					os << "ERROR";
					return os.str();
					break;
			}
			temp_os << std::dec << get_rd(insn) << ",x" << get_rs1(insn) << ",x" << get_rs2(insn);
			os << "x" << std::setw(18) << std::left << temp_os.str();
			os << "//" << execution;
			break;

		case 0b0001111: // FENCE
			switch((insn & 0xf000000) >> (24-0))
			{
				case 0b1000:
					temp_os << "i,";
					break;
				case 0b0100:
					temp_os << "o,";
					break;
				case 0b0011:
					temp_os << "rw,";
					break;
				case 0b0010:
					temp_os << "r,";
					break;
				case 0b0001:
					temp_os << "w,";
					break;
				case 0b1111:
					temp_os << "iorw,";
					break;
				default:
					temp_os << "ERROR";
					break;
			}
			switch((insn & 0xf00000) >> (20-0))
			{
				case 0b1000:
					temp_os << "i";
					break;
				case 0b0100:
					temp_os << "o";
					break;
				case 0b0011:
					temp_os << "rw";
					break;
				case 0b0010:
					temp_os << "r";
					break;
				case 0b0001:
					temp_os << "w";
					break;
				case 0b1111:
					temp_os << "iorw";
					break;
				default:
					temp_os << "ERROR";
					break;
			}
			os << "fence  " << std::setw(19) << std::left << temp_os.str() << "// fence";
			pc+=4;
			break;
		
		default:
			os << "ERROR";
			break;
	}
	return os.str();
}

//*****************************************************************
// Function: dump const
// Use: outputs the contents of the registers including the pc register
// Parameters: void
// Outputs: void
//*****************************************************************
void rv32i::dump() const
{
	regs.dump();
	std::cout << "\n pc " << hex32(pc) << std::endl;
}

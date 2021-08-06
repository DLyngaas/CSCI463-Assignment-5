//*****************************************************************
// FILE:      rv32i.h
// AUTHOR:    Dan Lyngaas
// LOGON ID:  z1796040
// DUE DATE:  April 16, 2020
// 
// PURPOSE:   Definition of rv32i class
//*****************************************************************
#ifndef rv32i_H
#define rv32i_H
#include "hex.h"
#include "memory.h"
#include "registerfile.h"
#include<string>
#include<stdint.h>
class rv32i
{
	public :
		//Constructor
		rv32i(memory *m);
		
		void run(uint32_t limit);
		
		//functions
		static uint32_t get_opcode(uint32_t insn);
		
		//get part functions
		static uint32_t get_rd(uint32_t insn);
		static uint32_t get_funct3(uint32_t insn);
		static uint32_t get_rs1(uint32_t insn);
		static uint32_t get_rs2(uint32_t insn);
        static uint32_t get_funct7(uint32_t insn);
        static int32_t get_imm_i(uint32_t insn);
        static int32_t get_imm_u(uint32_t insn);
        static int32_t get_imm_b(uint32_t insn);
        static int32_t get_imm_s(uint32_t insn);
        static int32_t get_imm_j(uint32_t insn);
        
        void dump() const;
        
        //excution functions
        std::string exec_add(uint32_t insn);
        std::string exec_addi(uint32_t insn);
        std::string exec_and(uint32_t insn);
        std::string exec_andi(uint32_t insn);
        std::string exec_auipc(uint32_t insn);
        std::string exec_beq(uint32_t insn);
        std::string exec_bge(uint32_t insn);
        std::string exec_bgeu(uint32_t insn);
        std::string exec_blt(uint32_t insn);
        std::string exec_bltu(uint32_t insn);
        std::string exec_bne(uint32_t insn);
        std::string exec_jal(uint32_t insn);
        std::string exec_jalr(uint32_t insn);
        std::string exec_lb(uint32_t insn);
        std::string exec_lbu(uint32_t insn);
        std::string exec_lh(uint32_t insn);
        std::string exec_lhu(uint32_t insn);
        std::string exec_lui(uint32_t insn);
        std::string exec_lw(uint32_t insn);
        std::string exec_or(uint32_t insn);
        std::string exec_ori(uint32_t insn);
        std::string exec_sb(uint32_t insn);
        std::string exec_sh(uint32_t insn);
        std::string exec_sll(uint32_t insn);
        std::string exec_slli(uint32_t insn);
        std::string exec_slt(uint32_t insn);
        std::string exec_slti(uint32_t insn);
        std::string exec_sltiu(uint32_t insn);
        std::string exec_sltu(uint32_t insn);
        std::string exec_sra(uint32_t insn);
        std::string exec_srai(uint32_t insn);
        std::string exec_srl(uint32_t insn);
        std::string exec_srli(uint32_t insn);
        std::string exec_sub(uint32_t insn);
        std::string exec_sw(uint32_t insn);
        std::string exec_xor(uint32_t insn);
        std::string exec_xori(uint32_t insn);
        std::string exec_ebreak(uint32_t insn);
		std::string exec(uint32_t insn);

	private :
		//class variables
		memory * mem;
		uint32_t pc;
		registerfile regs;
		bool halt = { false };
		uint64_t insn_counter = { 0 };
		static constexpr uint32_t XLEN = 32;
};

#endif

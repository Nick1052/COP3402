// CDA 3103C - Final project
// Group 63:
// - Nicolas Gonzalez
// - Loy Pereyra
// - Bryce Eihl

#include "spimcore.h"

/* ALU */
/* 10 Points */
void ALU(unsigned A,unsigned B,char ALUControl,unsigned *ALUresult,char *Zero) {
    
    switch(ALUControl) {
        case 0x0:
            *ALUresult = A + B;
            break;
        case 0x1:
            *ALUresult = A - B;
            break;
        case 0x2:
            if((int)A < (int)B)
                *ALUresult = 1;
            else
                *ALUresult = 0;
        case 0x3:
            if(A < B)
                *ALUresult = 1;
            else
                *ALUresult = 0;
            break;
        case 0x4:
            *ALUresult = (A & B);
            break;
        case 0x5:
            *ALUresult = (A | B);
            break;
        case 0x6:
            *ALUresult = (B << 16);
            break;
        case 0x7:
            *ALUresult = ~A;
            break;
    }
    
    if(*ALUresult == 0x0)
        *Zero = 1;
    else
        *Zero = 0;
}
/* instruction fetch */
/* 10 Points */
int instruction_fetch(unsigned PC,unsigned *Mem,unsigned *instruction) {

    unsigned index = PC >> 2;

    if(PC % 4 != 0) return 1;
        
    *instruction = Mem[index];

    return 0;

}
/* instruction partition */
/* 10 Points */
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1,unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec) {
    
        *op = (instruction >> 26) & 0x0000003f;
        *r1 = (instruction >> 21) & 0x0000001f;
        *r2 = (instruction >> 16) & 0x0000001f;
        *r3 = (instruction >> 11) & 0x0000001f;
        *funct = instruction & 0x0000003f;
        *offset = instruction & 0X0000ffff;
        *jsec = instruction & 0x03ffffff;
}
/* instruction decode */
/* 15 Points */
int instruction_decode(unsigned op,struct_controls *controls) {
    
    switch(op) {
        case 0x00:
            controls->RegDst = 1;
            controls->RegWrite = 1;
            controls->ALUSrc = 0;
            controls->MemRead = 0;
            controls->MemWrite = 0;
            controls->MemtoReg = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->ALUOp = 7;
            break;
        case 0x08:
            controls->RegDst = 0;
            controls->RegWrite = 1;
            controls->ALUSrc = 1;
            controls->MemRead = 0;
            controls->MemWrite = 0;
            controls->MemtoReg = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->ALUOp = 0;
            break;
        case 0x0a:
            controls->RegDst = 1;
            controls->RegWrite = 1;
            controls->ALUSrc = 0;
            controls->MemRead = 0;
            controls->MemWrite = 0;
            controls->MemtoReg = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->ALUOp = 2;
            break;
        case 0x0b:
            controls->RegDst = 1;
            controls->RegWrite = 1;
            controls->ALUSrc = 0;
            controls->MemRead = 0;
            controls->MemWrite = 0;
            controls->MemtoReg = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->ALUOp = 3;
            break;
        case 0x4:
            controls->RegDst = 2;
            controls->RegWrite = 0;
            controls->ALUSrc = 0;
            controls->MemRead = 0;
            controls->MemWrite = 0;
            controls->MemtoReg = 2;
            controls->Jump = 0;
            controls->Branch = 1;
            controls->ALUOp = 1;
            break;
        case 0x23:
            controls->RegDst = 0;
            controls->RegWrite = 1;
            controls->ALUSrc = 1;
            controls->MemRead = 1;
            controls->MemWrite = 0;
            controls->MemtoReg = 1;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->ALUOp = 0;
            break;
        case 0x0f:
            controls->RegDst = 0;
            controls->RegWrite = 1;
            controls->ALUSrc = 1;
            controls->MemRead = 0;
            controls->MemWrite = 0;
            controls->MemtoReg = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->ALUOp = 6;
            break;
        case 0x2b:
            controls->RegDst = 2;
            controls->RegWrite = 0;
            controls->ALUSrc = 1;
            controls->MemRead = 0;
            controls->MemWrite = 1;
            controls->MemtoReg = 2;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->ALUOp = 0;
            break;
        case 0x2:
            controls->RegDst = 0;
            controls->RegWrite = 0;
            controls->ALUSrc = 0;
            controls->MemRead = 0;
            controls->MemWrite = 0;
            controls->MemtoReg = 0;
            controls->Jump = 1;
            controls->Branch = 0;
            controls->ALUOp = 0;
            break;
        default:
            return 1;
    }
    return 0;
}
/* Read Register */
/* 5 Points */
void read_register(unsigned r1,unsigned r2,unsigned *Reg,unsigned *data1,unsigned *data2) {

    *data1 = Reg[r1];
    *data2 = Reg[r2];
}
/* Sign Extend */
/* 10 Points */
void sign_extend(unsigned offset,unsigned *extended_value) {   

    if(offset >> 15 == 1)
        *extended_value = (offset | 0xffff0000);
    else
        *extended_value = (offset & 0x0000ffff);
}
/* ALU operations */
/* 10 Points */
int ALU_operations(unsigned data1,unsigned data2,unsigned extended_value,unsigned funct,char ALUOp,char ALUSrc,unsigned *ALUresult,char *Zero) {
    
    if(ALUOp == 7) {
        switch(funct) {
            case 0x20:
                ALUOp = 0;
                break;
            case 0x22:
                ALUOp = 1;
                break;
            case 0x24:
                ALUOp = 4;
                break;
            case 0x25:
                ALUOp = 5;
                break;
            case 0x2a:
                ALUOp = 2;
                break;
            case 0x2b:
                ALUOp = 3;
                break;
            default:
                return 1;
        }
    }

    ALU(data1, (ALUSrc == 1)? extended_value: data2, ALUOp, ALUresult, Zero);

    return 0; 
}
/* Read / Write Memory */
/* 10 Points */
int rw_memory(unsigned ALUresult,unsigned data2,char MemWrite,char MemRead,unsigned *memdata,unsigned *Mem) {

    if(MemRead == 1) {
        if(ALUresult % 4 != 0)
            return 1;
        *memdata = Mem[ALUresult >> 2];
    }
    if(MemWrite == 1) {
        if(ALUresult % 4 != 0)
            return 1;
        Mem[ALUresult >> 2] = data2;
    }
    return 0;
}
/* Write Register */
/* 10 Points */
void write_register(unsigned r2,unsigned r3,unsigned memdata,unsigned ALUresult,char RegWrite,char RegDst,char MemtoReg,unsigned *Reg) {

    if(RegWrite == 1) {
        if(MemtoReg == 1) {
            if(RegDst == 1) 
                Reg[r3] = memdata;
            else
                Reg[r2] = memdata; 
        }
        else {
            if(RegDst == 1) 
                Reg[r3] = ALUresult;
            else
                Reg[r2] = ALUresult; 
        }
    }
}
/* PC update */
/* 10 Points */
void PC_update(unsigned jsec,unsigned extended_value,char Branch,char Jump,char Zero,unsigned *PC) {
    
    *PC += 4;
    
    if(Jump == 1)
        *PC = (jsec << 2) | (*PC & 0xf0000000);
    else if(Branch == 1 && Zero == 1)
        *PC += (extended_value << 2);
}

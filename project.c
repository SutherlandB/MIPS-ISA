/******************************************************************************/
/* Usual suspects to include  */
/******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// define BIT type as a char (i.e., one byte)
typedef char BIT;
#define TRUE 1
#define FALSE 0
#define UNDEF -1

// useful constants
BIT ONE[32] = {TRUE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
  FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
  FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
  FALSE};
BIT TWO[32] = {FALSE, TRUE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
  FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
  FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
  FALSE};
BIT ZERO[32] = {FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
  FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
  FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
  FALSE};
BIT REG_THIRTY_ONE[5] = {TRUE, TRUE, TRUE, TRUE, TRUE};
BIT THIRTY_TWO[32] = {FALSE, FALSE, FALSE, FALSE, FALSE, TRUE, FALSE, FALSE, FALSE,
  FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
  FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
  FALSE};


/******************************************************************************/
/* Function prototypes */
/******************************************************************************/
BIT not_gate(BIT A);
BIT or_gate(BIT A, BIT B);
BIT or_gate3(BIT A, BIT B, BIT C);
BIT and_gate(BIT A, BIT B);
BIT and_gate3(BIT A, BIT B, BIT C);
BIT xor_gate(BIT A, BIT B);
BIT nor_gate(BIT A, BIT B);
BIT nand_gate(BIT A, BIT B);

void decoder2(BIT I0, BIT I1, BIT* O0, BIT* O1, BIT* O2, BIT* O3);
void decoder3(BIT* I, BIT EN, BIT* O);
void decoder5(BIT* I, BIT* O);
BIT multiplexor2(BIT S, BIT I0, BIT I1);
void multiplexor2_32(BIT S, BIT* I0, BIT* I1, BIT* Output);
BIT multiplexor4(BIT S0, BIT S1, BIT I0, BIT I1, BIT I2, BIT I3);

void copy_bits(BIT* A, BIT* B);
void print_binary(BIT* A);
void convert_to_binary(int a, BIT* A, int length);
void convert_to_binary_char(int a, char* A, int length);
int binary_to_integer(BIT* A);

int get_instructions(BIT Instructions[][32]);

void Instruction_Memory(BIT* ReadAddress, BIT* Instruction);
void Control(BIT* OpCode,
  BIT* RegDst, BIT* Jump, BIT* Branch, BIT* MemRead, BIT* MemToReg,
  BIT* ALUOp, BIT* MemWrite, BIT* ALUSrc, BIT* RegWrite, BIT* jal, BIT* jr);
void Read_Register(BIT* ReadRegister1, BIT* ReadRegister2,
  BIT* ReadData1, BIT* ReadData2);
void Write_Register(BIT RegWrite, BIT* WriteRegister, BIT* WriteData);
void ALU_Control(BIT* ALUOp, BIT* funct, BIT* ALUControl);
void adder1(BIT A, BIT B, BIT CarryIn, BIT* CarryOut, BIT* Sum);
void ALU1(BIT A, BIT B, BIT Op0, BIT Op1, BIT CarryIn, BIT Binvert, BIT Less,
  BIT* Result, BIT* CarryOut, BIT* Set);
void ALU(BIT* ALUControl, BIT* Input1, BIT* Input2, BIT* Zero, BIT* Result);
void Data_Memory(BIT MemWrite, BIT MemRead,
  BIT* Address, BIT* WriteData, BIT* ReadData);
void Extend_Sign16(BIT* Input, BIT* Output);
void updateState();

/******************************************************************************/
/* Functions provided for your convenience */
/******************************************************************************/
BIT not_gate(BIT A)
{
  if (A)
    return FALSE;
  else
    return TRUE;
}

BIT or_gate(BIT A, BIT B)
{
  if (A || B)
    return TRUE;
  else
    return FALSE;
}

BIT or_gate3(BIT A, BIT B, BIT C)
{
  return or_gate(A, or_gate(B, C));
}

BIT and_gate(BIT A, BIT B)
{
  if (A && B)
    return TRUE;
  else
    return FALSE;
}

BIT and_gate3(BIT A, BIT B, BIT C)
{
  return and_gate(A, and_gate(B, C));
}

BIT and_gate6(BIT A, BIT B, BIT C, BIT D, BIT E, BIT F)
{
  return and_gate(and_gate3(A, B, C), and_gate3(D, E, F));
}
BIT and_gate32(BIT* result)
{
  BIT G=TRUE;
  for(int i = 0; i < 32; i++){
    G = and_gate(G, result[i]);
  }
  return G;
}

BIT xor_gate(BIT A, BIT B)
{
  if (A ^ B)
    return TRUE;
  else
    return FALSE;
}

BIT nor_gate(BIT A, BIT B)
{
  return not_gate(or_gate(A, B));
}

BIT nand_gate(BIT A, BIT B)
{
  return not_gate(and_gate(A, B));
}

void decoder2(BIT I0, BIT I1, BIT* O0, BIT* O1, BIT* O2, BIT* O3)
{
  // Note: The input -> output mapping is slightly modified from before
  *O0 = and_gate(not_gate(I1), not_gate(I0));
  *O1 = and_gate(not_gate(I1), I0);
  *O2 = and_gate(I1, not_gate(I0));
  *O3 = and_gate(I1, I0);

  return;
}

void decoder3(BIT* I, BIT EN, BIT* O)
{
  O[0] = and_gate3(not_gate(I[2]), not_gate(I[1]), not_gate(I[0]));
  O[1] = and_gate3(not_gate(I[2]), not_gate(I[1]), I[0]);
  O[2] = and_gate3(not_gate(I[2]), I[1], not_gate(I[0]));
  O[3] = and_gate3(not_gate(I[2]), I[1], I[0]);
  O[4] = and_gate3(I[2], not_gate(I[1]), not_gate(I[0]));
  O[5] = and_gate3(I[2], not_gate(I[1]), I[0]);
  O[6] = and_gate3(I[2], I[1], not_gate(I[0]));
  O[7] = and_gate3(I[2], I[1], I[0]);
  
  O[0] = and_gate(EN, O[0]);
  O[1] = and_gate(EN, O[1]);
  O[2] = and_gate(EN, O[2]);
  O[3] = and_gate(EN, O[3]);
  O[4] = and_gate(EN, O[4]);
  O[5] = and_gate(EN, O[5]);
  O[6] = and_gate(EN, O[6]);
  O[7] = and_gate(EN, O[7]);
  
  return;
}

void decoder5(BIT* I, BIT* O)
{
   BIT EN[4] = {FALSE};
   decoder2(I[3], I[4], &EN[0], &EN[1], &EN[2], &EN[3]);
   decoder3(I, EN[3], &O[24]);
   decoder3(I, EN[2], &O[16]);
   decoder3(I, EN[1], &O[8]);
   decoder3(I, EN[0], &O[0]);
}

BIT multiplexor2(BIT S, BIT I0, BIT I1)
{
  BIT x0 = and_gate(not_gate(S), I0);
  BIT x1 = and_gate(S, I1);
  return or_gate(x0, x1);
}

void multiplexor2_32(BIT S, BIT* I0, BIT* I1, BIT* Output)
{
  for (int i = 0; i < 32; ++i) {
    BIT x0 = and_gate(not_gate(S), I0[i]);
    BIT x1 = and_gate(S, I1[i]);
    Output[i] = or_gate(x0, x1);
  }
}

BIT multiplexor4(BIT S0, BIT S1, BIT I0, BIT I1, BIT I2, BIT I3)
{
  BIT x0, x1, x2, x3 = FALSE;
  decoder2(S0, S1, &x0, &x1, &x2, &x3);

  BIT y0 = and_gate(x0, I0);
  BIT y1 = and_gate(x1, I1);
  BIT y2 = and_gate(x2, I2);
  BIT y3 = and_gate(x3, I3);

  BIT z0 = or_gate(y0, y1);
  BIT z1 = or_gate(y2, y3);

  return or_gate(z0, z1);
}


/******************************************************************************/
/* Helper functions */
/******************************************************************************/
void copy_bits(BIT* A, BIT* B)
{
  for (int i = 0; i < 32; ++i)
    B[i] = A[i];
}

void print_binary(BIT* A)
{
  for (int i = 31; i >= 0; --i)
    printf("%d", A[i]);
}

void convert_to_binary(int a, BIT* A, int length)
{
  if (a >= 0) {
    for (int i = 0; i < length; ++i) {
      A[i] = (a % 2 == 1 ? TRUE : FALSE);
      a /= 2;
    }
  } else {
    a += 1;
    for (int i = 0; i < length; ++i) {
      A[i] = (a % 2 == 0 ? TRUE : FALSE);
      a /= 2;
    }
  }
}

void convert_to_binary_char(int a, char* A, int length)
{
  if (a >= 0) {
    for (int i = 0; i < length; ++i) {
      A[i] = (a % 2 == 1 ? '1' : '0');
      a /= 2;
    }
  } else {
    a += 1;
    for (int i = 0; i < length; ++i) {
      A[i] = (a % 2 == 0 ? '1' : '0');
      a /= 2;
    }
  }
}

int binary_to_integer(BIT* A)
{
  unsigned a = 0;
  unsigned mult = 1;

  for (int i = 0; i < 32; ++i) {
    a += A[i]*mult;
    mult *= 2;
  }

  return (int)a;
}

void indexing(BIT* index, BIT** input, BIT* output){
    BIT choose_output [32] = {FALSE};
    decoder5(index, choose_output); //TODO & isn't need Im pretty sure
    for(int i=0; i<32;i++){
        multiplexor2_32(choose_output[i],output, input[i],output);
    }
}


void set_register(BIT* input, BIT* output) {
  if (strcmp(input, "v0") == 0){
    strcpy(output, "01000");
  } else if(strcmp(input, "a0") == 0){
    strcpy(output, "00100");
  } else if(strcmp(input, "t0") == 0){
    strcpy(output, "00010");
  } else if(strcmp(input, "t1") == 0){
    strcpy(output, "10010");
  } else if(strcmp(input, "s0") == 0){
    strcpy(output, "00001");
  } else if(strcmp(input, "s1") == 0){
    strcpy(output, "10001");
  } else if(strcmp(input, "sp") == 0){
    strcpy(output, "10111");
  } else if(strcmp(input, "ra") == 0){
    strcpy(output, "11111");
  } else if(strcmp(input, "zero") == 0){
      strcpy(output, "00000");
  }
}
/******************************************************************************/
/* Parsing functions */
/******************************************************************************/

// TODO: Implement any helper functions to assist with parsing

int get_instructions(BIT Instructions[][32]) {
  char line[256] = {0};
  int instruction_count=0;
  while (fgets(line, 256, stdin) != NULL) {
    BIT output[32] = {FALSE};
    char inst[256] = {0};
    char op1[256] = {0};
    char op2[256] = {0};
    char op3[256] = {0};
    sscanf(line, "%s %s %s %s", inst, op1, op2, op3);
    
    char temp_output[33] = {0};
    char rs[6] = {0};
    char rt[6] = {0};
    char rd[6] = {0};
    char imm[17] = {0};
    char address[27] = {0};
   
    // i type instructions
    if (strcmp(inst, "lw") == 0) { // DONE
      char* x = "110001";
      convert_to_binary_char(atoi(op3), imm, 16);
      set_register(op1, rt);
      set_register(op2, rs);
      strncpy(&temp_output[0], imm, 16);
      strncpy(&temp_output[16], rt, 5);
      strncpy(&temp_output[21], rs, 5);
      strncpy(&temp_output[26], x, 6);      
    } else if (strcmp(inst, "sw") == 0) { // DONE
      char* x = "110101";
      convert_to_binary_char(atoi(op3), imm, 16);
      set_register(op1, rt);
      set_register(op2, rs);
      strncpy(&temp_output[0], imm, 16);
      strncpy(&temp_output[16], rt, 5);
      strncpy(&temp_output[21], rs, 5);
      strncpy(&temp_output[26], x, 6);   
    } else if (strcmp(inst, "beq")==0) { // DONE
      char* x = "001000";
      convert_to_binary_char(atoi(op3), imm, 16);
      set_register(op1, rs);
      set_register(op2, rt);
      strncpy(&temp_output[0], imm, 16);
      strncpy(&temp_output[16], rt, 5);
      strncpy(&temp_output[21], rs, 5);
      strncpy(&temp_output[26], x, 6); 
    } else if (strcmp(inst, "addi")==0) {
      char* x = "000100";
      convert_to_binary_char(atoi(op3), imm, 16);
      set_register(op1, rt);
      set_register(op2, rs);
      strncpy(&temp_output[0], imm, 16);
      strncpy(&temp_output[16], rt, 5);
      strncpy(&temp_output[21], rs, 5);
      strncpy(&temp_output[26], x, 6); 
  
    // r type instructions
    } else if (strcmp(inst, "add") == 0) { // DONE
      char* x = "000001";
      char* y = "00000";
      char* z = "000000";
      set_register(op1, rd);
      set_register(op2, rs);
      set_register(op3, rt);
      strncpy(&temp_output[0], x, 6);
      strncpy(&temp_output[6], y, 5);
      strncpy(&temp_output[11], rd, 5);
      strncpy(&temp_output[16], rt, 5);
      strncpy(&temp_output[21], rs, 5);
      strncpy(&temp_output[26], z, 6);      
    } else if (strcmp(inst, "sub") == 0) {
      char* x = "010001";
      char* y = "00000";
      char* z = "000000";
      set_register(op1, rd);
      set_register(op2, rs);
      set_register(op3, rt);
      strncpy(&temp_output[0], x, 6);
      strncpy(&temp_output[6], y, 5);
      strncpy(&temp_output[11], rd, 5);
      strncpy(&temp_output[16], rt, 5);
      strncpy(&temp_output[21], rs, 5);
      strncpy(&temp_output[26], z, 6);
    } else if (strcmp(inst, "and") == 0) {
      char* x = "001001";
      char* y = "00000";
      char* z = "000000";
      set_register(op1, rd);
      set_register(op2, rs);
      set_register(op3, rt);
      strncpy(&temp_output[0], x, 6);
      strncpy(&temp_output[6], y, 5);
      strncpy(&temp_output[11], rd, 5);
      strncpy(&temp_output[16], rt, 5);
      strncpy(&temp_output[21], rs, 5);
      strncpy(&temp_output[26], z, 6);  
    } else if (strcmp(inst, "or") == 0) {
      char* x = "101001";
      char* y = "00000";
      char* z = "000000";
      set_register(op1, rd);
      set_register(op2, rs);
      set_register(op3, rt);
      strncpy(&temp_output[0], x, 6);
      strncpy(&temp_output[6], y, 5);
      strncpy(&temp_output[11], rd, 5);
      strncpy(&temp_output[16], rt, 5);
      strncpy(&temp_output[21], rs, 5);
      strncpy(&temp_output[26], z, 6);
    } else if (strcmp(inst, "slt") == 0) {
      char* x = "010101";
      char* y = "00000";
      char* z = "000000";
      set_register(op1, rd);
      set_register(op2, rs);
      set_register(op3, rt);
      strncpy(&temp_output[0], x, 6);
      strncpy(&temp_output[6], y, 5);
      strncpy(&temp_output[11], rd, 5);
      strncpy(&temp_output[16], rt, 5);
      strncpy(&temp_output[21], rs, 5);
      strncpy(&temp_output[26], z, 6);
    } else if (strcmp(inst, "jr") == 0) {
      char* x = "000100";
      char* y = "00000";
      char* z = "000000";
      set_register(op1, rs);
      set_register(op2, rd);
      set_register(op3, rt);
      strncpy(&temp_output[0], x, 6);
      strncpy(&temp_output[6], y, 5);
      strncpy(&temp_output[11], rd, 5);
      strncpy(&temp_output[16], rt, 5);
      strncpy(&temp_output[21], rs, 5);
      strncpy(&temp_output[26], z, 6);
  
    // j type instructions
    } else if (strcmp(inst, "j") == 0) { // DONE
      char* x = "010000";
      convert_to_binary_char(atoi(op1), address, 26);
      strncpy(&temp_output[0], address, 26);
      strncpy(&temp_output[26], x, 6);      
    } else if (strcmp(inst, "jal") == 0) { // DONE
      char* x = "110000";
      convert_to_binary_char(atoi(op1), address, 26);
      strncpy(&temp_output[0], address, 26);
      strncpy(&temp_output[26], x, 6);      
    }
    for (int i = 0; i < 32; i++) { output[i] = (temp_output[i] == '1' ? TRUE : FALSE); }
    copy_bits(output, Instructions[instruction_count]);
    instruction_count++;
  }
  return instruction_count;
}


/******************************************************************************/
/* Program state - memory spaces, PC, and control */
/******************************************************************************/
BIT PC[32]                  = {FALSE};
BIT MEM_Instruction[32][32] = {FALSE}; //First dimension is the instruction number, second dimension is the bit representation of the instruction
BIT MEM_Data[32][32]        = {FALSE};
BIT MEM_Register[32][32]    = {FALSE};


BIT RegDst    = FALSE;
BIT Jump      = FALSE;
BIT Branch    = FALSE;
BIT MemRead   = FALSE;
BIT MemToReg  = FALSE;
BIT ALUOp[2]  = {FALSE};
BIT MemWrite  = FALSE;
BIT ALUSrc    = FALSE;
BIT RegWrite  = FALSE;
BIT Zero      = FALSE;
BIT ALUControl[4] = {FALSE};
BIT jal       = FALSE;
BIT jr        = FALSE;
BIT dummy     = FALSE;

void print_instruction()
{
  unsigned pc = binary_to_integer(PC);
  printf("PC: %d\n", pc);
  printf("Instruction: ");
  print_binary(MEM_Instruction[pc]);
  printf("\n");
}

void print_state()
{
  printf("Data: ");
  for (int i = 0; i < 32; ++i) {
    printf("%d ", binary_to_integer(MEM_Data[i]));
  }
  printf("\n");

  printf("Register: ");
  for (int i = 0; i < 32; ++i) {
    printf("%d ", binary_to_integer(MEM_Register[i]));
  }
  printf("\n");
}


/******************************************************************************/
/* Functions that you will implement */
/******************************************************************************/
void Instruction_Memory(BIT* ReadAddress, BIT* Instruction)  //Nate
{
  // TODO: Implement instruction memory
  // Input: 32-bit instruction address
  // Output: 32-bit binary instruction
  // Note: Useful to use a 5-to-32 decoder here
/*   indexing(ReadAddress, MEM_Instruction, Instruction); */
/* void indexing(BIT* index, BIT** input, BIT* output){ */

/*   BIT choose_output[32] = {FALSE}; */
/*   decoder5(ReadAddress, choose_output); // TODO & isn't need Im pretty sure */
/*   for (int i = 0; i < 32; i++) { */
/*     multiplexor2_32(choose_output[i], output, input[i], output); */
/*   } */

  BIT choose_output[32] = {FALSE};
  decoder5(ReadAddress, choose_output);
  for (int i = 0; i < 32; i++) {
    multiplexor2_32(choose_output[i], Instruction, MEM_Instruction[i],
                    Instruction);
  }
}

void Control(BIT* OpCode,
  BIT* RegDst, BIT* Jump, BIT* Branch, BIT* MemRead, BIT* MemToReg,
  BIT* ALUOp, BIT* MemWrite, BIT* ALUSrc, BIT* RegWrite, BIT* jal, BIT* jr)
{
  // TODO: Set control bits for everything
  // Input: opcode field from the instruction
  // OUtput: all control lines get set
  // Note: Can use SOP or similar approaches to determine bits
  //I-format bit instructions
  BIT lw =   and_gate( and_gate3( OpCode[5],not_gate(OpCode[4]),not_gate(OpCode[3]) ), and_gate3( not_gate(OpCode[2]),OpCode[1],OpCode[0] ));
  BIT sw =   and_gate( and_gate3( OpCode[5],not_gate(OpCode[4]),OpCode[3] ), and_gate3( not_gate(OpCode[2]),OpCode[1],OpCode[0] ));
  BIT beq =  and_gate( and_gate3( not_gate(OpCode[5]),not_gate(OpCode[4]),not_gate(OpCode[3]) ), and_gate3( OpCode[2],not_gate(OpCode[1]),not_gate(OpCode[0]) ));
  BIT addi = and_gate( and_gate3( not_gate(OpCode[5]),not_gate(OpCode[4]),OpCode[3] ), and_gate3( not_gate(OpCode[2]),not_gate(OpCode[1]),not_gate(OpCode[0]) ));
  //R-format instructions
  BIT and =  and_gate( and_gate3( not_gate(OpCode[5]), not_gate(OpCode[4]), not_gate(OpCode[3]) ), and_gate3( not_gate(OpCode[2]), not_gate(OpCode[1]), not_gate(OpCode[0]) ));
  BIT or =   and_gate( and_gate3( not_gate(OpCode[5]), not_gate(OpCode[4]), not_gate(OpCode[3]) ), and_gate3( not_gate(OpCode[2]), not_gate(OpCode[1]), not_gate(OpCode[0]) ));
  BIT add =  and_gate( and_gate3( not_gate(OpCode[5]), not_gate(OpCode[4]), not_gate(OpCode[3]) ), and_gate3( not_gate(OpCode[2]), not_gate(OpCode[1]), not_gate(OpCode[0]) ));
  BIT sub =  and_gate( and_gate3( not_gate(OpCode[5]), not_gate(OpCode[4]), not_gate(OpCode[3]) ), and_gate3( not_gate(OpCode[2]), not_gate(OpCode[1]), not_gate(OpCode[0]) ));
  //BIT slt =  and_gate( and_gate3( not_gate(OpCode[5]), not_gate(OpCode[4]), not_gate(OpCode[3]) ), and_gate3( not_gate(OpCode[2]), not_gate(OpCode[1]), not_gate(OpCode[0]) ));
  *jr  =  and_gate( and_gate3( not_gate(OpCode[5]), not_gate(OpCode[4]), OpCode[3]), and_gate3( not_gate(OpCode[2]), not_gate(OpCode[1]), not_gate(OpCode[0]) ));

  BIT j =    and_gate( and_gate3( not_gate(OpCode[5]), not_gate(OpCode[4]), not_gate(OpCode[3]) ), and_gate3 (not_gate(OpCode[2]), (OpCode[1]),  not_gate(OpCode[0]) ));
  *jal =  and_gate( and_gate3( not_gate(OpCode[5]), not_gate(OpCode[4]), not_gate(OpCode[3]) ), and_gate3 (not_gate(OpCode[2]), (OpCode[1]), (OpCode[0]) ));

  BIT R_type = or_gate(or_gate(or_gate(and, or), or_gate(add, sub)), FALSE);
  BIT I_type = or_gate( or_gate(lw,sw), or_gate(beq,addi) );
  BIT J_type = or_gate(j,*jal);
  //setting the bits for control.
  *RegDst = R_type;
  *Jump = J_type;
  *Branch = beq;
  *MemRead = lw;
  *MemToReg = lw;
  *MemWrite = sw;
  *ALUSrc = and_gate(not_gate(*Branch), I_type);
  *RegWrite = and_gate3(not_gate(sw),not_gate(beq),not_gate(J_type));

  ALUOp[1] = R_type;
  ALUOp[0] = beq;
}



void Read_Register(BIT* ReadRegister1, BIT* ReadRegister2,
  BIT* ReadData1, BIT* ReadData2)
{
  // TODO: Implement register read functionality
  // Input: two 5-bit register addresses
  // Output: the values of the specified registers in ReadData1 and ReadData2
  // Note: Implementation will be very similar to instruction memory circuit
  
  //how do we get the values in the registers?
  BIT t1[32] = {FALSE};
  BIT t2[32] ={FALSE};
  decoder5(ReadRegister1,t1);
  decoder5(ReadRegister2,t2);
  //now t1 holds reg1
  //t2 holds reg2
  for (int i =0;i<32;i++){
    multiplexor2_32(t1[i],ReadData1,MEM_Register[i], ReadData1);
    multiplexor2_32(t2[i],ReadData2,MEM_Register[i], ReadData2);
  }
  //now you can return 
}
void Write_Register(BIT RegWrite, BIT* WriteRegister, BIT* WriteData)
{
  // TODO: Implement register write functionality
  // Input: one 5-bit register address, data to write, and control bit
  // Output: None, but will modify register file
  // Note: Implementation will again be similar to those above
   BIT t1[32] = {FALSE};
   decoder5(WriteRegister,t1);
   for(int i=0;i<32;i++){
     //regwrite is 1
     multiplexor2_32(and_gate(RegWrite,t1[i]),MEM_Register[i], WriteData ,MEM_Register[i]);
   }

  
}

void ALU_Control(BIT* ALUOp, BIT* funct, BIT* ALUControl)
{
  // TODO: Implement ALU Control circuit
  // Input: 2-bit ALUOp from main control circuit, 6-bit funct field from the
  //        binary instruction
  // Output:4-bit ALUControl for input into the ALU
  // Note: Can use SOP or similar approaches to determine bits

  BIT add = and_gate( and_gate3( (funct[5]), not_gate(funct[4]), not_gate(funct[3]) ), and_gate3( not_gate(funct[2]), not_gate(funct[1]), not_gate(funct[0]) ));
  BIT sub = and_gate( and_gate3( (funct[5]), not_gate(funct[4]), not_gate(funct[3]) ), and_gate3( not_gate(funct[2]), (funct[1]), not_gate(funct[0]) ));
  BIT or =  and_gate( and_gate3( (funct[5]), not_gate(funct[4]), not_gate(funct[3]) ), and_gate3( (funct[2]), not_gate(funct[1]), (funct[0]) ));
  BIT slt = and_gate6((funct[5]), not_gate(funct[4]),         (funct[3]), not_gate(funct[2]),         (funct[1]),  not_gate(funct[0]));

  ALUControl[3] = or_gate3(ALUOp[0], sub, slt);
  ALUControl[2] = or_gate3(sub, slt, ALUOp[0]);
  ALUControl[1] = or_gate(or_gate3(add, sub, slt), not_gate(ALUOp[1]));
  ALUControl[0] = or_gate(slt, or);
}

void adder1(BIT A, BIT B, BIT CarryIn, BIT* CarryOut, BIT* Sum)
{
  // TODO: implement a 1-bit adder
  // Note: you can copy+paste this from your or my Lab 5
  
  BIT x0 = xor_gate(A, B);
  *Sum = xor_gate(CarryIn, x0);
  
  BIT y0 = and_gate(x0, CarryIn);
  BIT y1 = and_gate(A, B);
  *CarryOut = or_gate(y0, y1);
}


void ALU1(BIT A, BIT B, BIT Binvert, BIT CarryIn, BIT Less, 
  BIT Op0, BIT Op1, BIT* Result, BIT* CarryOut, BIT* Set)
{
  // TODO: implement a 1-bit ALU 
  // Note: this will need modifications from Lab 5 to account for 'slt'
  // See slide 30 in Chapter-3d
  BIT x0 = multiplexor2(Binvert, B, not_gate(B));

  BIT y0 = and_gate(A, x0);
  BIT y1 = or_gate(A, x0);

  BIT y2 = FALSE;
  adder1(A, x0, CarryIn, CarryOut, &y2);
  *Set = y2;


  *Result = multiplexor4(Op0, Op1, y0, y1, y2, Less);
}

void ALU(BIT* ALUControl, BIT* Input1, BIT* Input2, BIT* Zero, BIT* Result)
{   
  // TODO: implement a 32-bit ALU
  // You'll need to essentially implement a 32-bit ripple addder here
  // See slide 31 in Chapter-3d
  // TODO: implement a 32-bit ALU
  // You'll need to essentially implement a 32-bit ripple addder here
  // See slide 31 in Chapter-3d

  BIT operation0 = ALUControl[0];
  BIT operation1 = ALUControl[1];
  BIT CarryIn = ALUControl[3];
  BIT BefCar = CarryIn;
  BIT Binvert = ALUControl[2];
  
  
  BIT Set = FALSE;
  BIT CarryOut_temp = FALSE;
  BIT AftCar = CarryIn;

  for(unsigned int i = 0; i < 32; i++){
    ALU1(Input1[i], Input2[i], Binvert, AftCar, 0, operation0, operation1, &Result[i], &CarryOut_temp, &Set);
    AftCar = CarryOut_temp;
  }

  ////
  BIT temp_r[32]={FALSE};
  BIT c_out =FALSE;
  BIT set_temp =FALSE;
  BIT vout =FALSE;
  for(unsigned int i = 0; i < 32; i++){
    ALU1(Input1[i], Input2[i], TRUE, vout, 0, FALSE, TRUE, &temp_r[i], &c_out, &set_temp);
    vout = c_out;
  }
  //////////////

  *Zero = and_gate32(temp_r);
  //final call to the ALU1 for the 0th inputs of the values passed in.
  ALU1(Input1[0], Input2[0], Binvert, BefCar, Set,operation0, operation1,
       &Result[0], &CarryOut_temp, &CarryOut_temp);
       
}


void Data_Memory(BIT MemWrite, BIT MemRead, 
  BIT* Address, BIT* WriteData, BIT* ReadData)
{
  // TODO: Implement data memory
  // Input: 32-bit address, control flags for read/write, and data to write
  // Output: data read if processing a lw instruction
  // Note: Implementation similar as above
  BIT t[32];
  decoder5(Address,t);
  for(int i =0;i<32;i++){
     //this is to write in the case where we do an operation such as store word
     multiplexor2_32(and_gate(t[i],MemWrite),MEM_Data[i],WriteData,MEM_Data[i]);
     //this is to read in an operation such as sw
     multiplexor2_32(and_gate(t[i],MemRead),ReadData,MEM_Data[i],ReadData);
  }
  
}

void Extend_Sign16(BIT* I, BIT* O)
{
  // TODO: Implement 16-bit to 32-bit sign extender
  // Copy Input to Output, then extend 16th Input bit to 17-32 bits in Output
    //populate the lower 16 bits
  for(int i = 0; i < 16; i++){
    O[i] = I[i];
  }
    //populate the upper 16 bits with multiplexor output 
  for(int i = 16; i < 32; i++){
    O[i] = multiplexor2(I[15], FALSE, TRUE);
  }
}

void updateState()
{
  // TODO: Implement the full datapath here
  // Essentially, you'll be figuring out the order in which to process each of
  // the sub-circuits comprising the entire processor circuit. It makes it
  // easier to consider the pipelined version of the process, and handle things
  // in order of the pipeline. The stages and operations are:
  // Fetch - load instruction from instruction memory
  // Decode - set control bits and read from the register file
  // Execute - process ALU
  // Memory - read/write data memory
  // Write Back - write to the register file
  // Update PC - determine the final PC value 
  BIT Instruction[32] = {FALSE};
  BIT OP[6] = {FALSE};
  BIT RRegister1[5] = {FALSE};
  BIT RRegister2[5] = {FALSE};
  BIT MUX0[5] = {FALSE};
  BIT WriteReg[5] = {FALSE};
  BIT fField[6] = {FALSE};
  BIT sixteenSignExtend[16] = {FALSE};
  BIT ThirtyTwoSignExtend[32] = {FALSE};
  BIT RData1[32] = {FALSE};
  BIT RData2[32] = {FALSE};
  BIT ReadData2MResult[32] = {FALSE};
  BIT ALU_Result[32] = {FALSE};
  BIT DataMem[32] = {FALSE}; 
  BIT updateWriteDat[32] = {FALSE};
  BIT UpdatePCVal[32] = {FALSE};
  BIT addy4[4] = {FALSE, TRUE, FALSE, FALSE}; //4 is 0100...  
  BIT JumpAddress[32] = {FALSE};
  BIT PC_SignExtend[32] = {FALSE};
  BIT bRezz[32] = {FALSE};

  // Fetch - load instruction from instruction memory
  Instruction_Memory(PC, Instruction);//fetch instruction from memory.


  // Decode - set control bits and read from the register file

  for(unsigned int i = 0; i < 6; i++){
    OP[i] = Instruction[26 + i];
  }

  Control(OP, &RegDst, &Jump, &Branch, &MemRead, &MemToReg, ALUOp, &MemWrite, &ALUSrc, &RegWrite, &jal, &jr);
                
  
  //Funct Field
  for(unsigned int i = 0; i < 6; i++){
    fField[i] = Instruction[i];
  }

  //Read Register1
  for(unsigned int i = 0; i < 5; i++){
    RRegister1[i] = Instruction[21 + i];
  }

  //Read Register2
  for(unsigned int i = 0; i < 5; i++){
    RRegister2[i] = Instruction[16 + i];
    }

  //Write Register
  for(unsigned int i = 0; i < 5; i++){
    MUX0[i] = Instruction[11 + i];
    }

  for (unsigned int i = 0; i < 5; i++) {
    WriteReg[i] = multiplexor2(RegDst, RRegister2[i], MUX0[i]);
  }
  //Sign Extend
  for(unsigned int i = 0; i < 16; i++){
    sixteenSignExtend[i] = Instruction[i];
  }

  //Sign Extend
  Extend_Sign16(sixteenSignExtend, ThirtyTwoSignExtend);

  Read_Register(RRegister1, RRegister2, RData1, RData2);

  // Ex
  multiplexor2_32(ALUSrc,RData2,ThirtyTwoSignExtend,ReadData2MResult);

  //ALUControl
  ALU_Control(ALUOp, fField, ALUControl);

  ALU(ALUControl, RData1, ReadData2MResult, &Zero, ALU_Result);

  // Memory - read/write data memory
  Data_Memory(MemWrite, MemRead, ALU_Result, RData2, DataMem);


  // Write back to the register memory

  multiplexor2_32(MemToReg, ALU_Result, DataMem, updateWriteDat);

  jr  =  and_gate6(not_gate(fField[5]), not_gate(fField[4]), (fField[3]), not_gate(fField[2]), not_gate(fField[1]), not_gate(fField[0]));
  Write_Register(and_gate(RegWrite, not_gate(jr)), WriteReg, updateWriteDat);

  //Add 4 to the PC
  ALU(addy4, PC, ONE, &dummy, UpdatePCVal);
 
  //Add PC+1 
  ALU(addy4, UpdatePCVal, ThirtyTwoSignExtend, &dummy, PC_SignExtend);

  Write_Register(jal, REG_THIRTY_ONE, UpdatePCVal); 

  BIT decisionBit = and_gate(Zero, Branch);

  //bRezz is the output stored form the branch decision in the multiplexor
  multiplexor2_32(decisionBit, UpdatePCVal, PC_SignExtend, bRezz);
  for(unsigned int i = 0; i < 4; i++){
  JumpAddress[i + 28] = UpdatePCVal[i + 28];
  }
//jump address 0-25 bits for the jump address
  for(unsigned int i = 0; i < 26; i++){
    JumpAddress[i] = Instruction[i];
  }
  multiplexor2_32(Jump, bRezz, JumpAddress, PC);

  multiplexor2_32(jr, PC, RData1, PC); 
  
  //making new PC addy based off jump
  //copying the 28-31 bits for the jump address


}


/******************************************************************************/
/* Main */
/******************************************************************************/

int main()
{
  setbuf(stdout, NULL);

  // parse instructions into binary format
  int counter = get_instructions(MEM_Instruction);

  // load program and run
  copy_bits(ZERO, PC);
  copy_bits(THIRTY_TWO, MEM_Register[29]);

  int i = 0;
  while (binary_to_integer(PC) < counter) {
    print_instruction();
    updateState();
    print_state();
    /* printf("-------\n\n"); */
    if(i == 200)
      break;
    i++;
  }

  return 0;
}

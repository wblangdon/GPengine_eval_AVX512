//extern const char* rev = "$Revision: 1.2 $";
// Magpie fixed stub for evolving AVX eval for GPengine.cpp
//
//Changes
//WBL  3 Nov 2025 compile mutated code sparately
//WBL 17 Oct 2025 bugfix error2, allow 16 bit answers
//WBL 14 Oct 2025 check all registers (not just R0) add using mprotect
//WBL 12 Oct 2025 Based on div_perf.cpp r1.17 and GPengine.cpp r1.119
//WBL 11 Oct 2025 organise combined div.cpp as 3 functions
//WBL 10 Oct 2025 combine div_funcs.cpp with div.cpp
//WBL  8 Oct 2025 Switch to AVX based on GPengine.cpp r1.65 Interpret16
//WBL  7 Oct 2025 Switch to instruction count, clean output for fit.bat
//WBL  2 Oct 2025 based on interpret from GPengine.cpp r1.119, main.cpp r1.22


typedef unsigned int __uint32_t;
typedef __uint32_t uint32_t;
//#include <immintrin.h>

//from GPengine.h r1.56
typedef int OP;
typedef OP instr[4];

const int IntRangeStart = 0;
const int IntRangeEnd = 127;

const int NumVar = 8;
enum { plus_op, minus_op, mul_op, div_op, NumOp };

//share between main and eval
typedef unsigned char retval;
//regtype chosen by Magpie
#define npar 64

extern
void Interpret64(
const int InstrLen, const instr *Instr, regtype registers[NumVar*npar], const uint32_t div32[256*256]);

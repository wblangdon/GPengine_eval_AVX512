//extern const char* rev = "$Revision: 1.2 $";
// Magpie fixed stub for evolving AVX eval for GPengine.cpp
//
//Changes
//WBL 17 Oct 2025 bugfix error2, allow 16 bit answers
//WBL 14 Oct 2025 check all registers (not just R0) add using mprotect
//WBL 12 Oct 2025 Based on div_perf.cpp r1.17 and GPengine.cpp r1.119
//WBL 11 Oct 2025 organise combined div.cpp as 3 functions
//WBL 10 Oct 2025 combine div_funcs.cpp with div.cpp
//WBL  8 Oct 2025 Switch to AVX based on GPengine.cpp r1.65 Interpret16
//WBL  7 Oct 2025 Switch to instruction count, clean output for fit.bat
//WBL  2 Oct 2025 based on interpret from GPengine.cpp r1.119, main.cpp r1.22

typedef unsigned      char uint8_t;
typedef unsigned short int uint16_t;
typedef unsigned       int uint32_t;

#include <immintrin.h>

//from GPengine.h r1.56
#include "eval.h"
#define InstrArg InstrArg_magpie
#define InstrReg InstrReg_magpie
#include "eval.cpp"


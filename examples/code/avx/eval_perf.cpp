//extern const char* rev = "$Revision: 1.28 $";
// Magpie fixed stub for evolving AVX eval for GPengine.cpp
//
//Changes
//WBL 10 Nov 2025 Magpie to tune regtype 
//WBL  3 Nov 2025 compile mutated code sparately
//WBL 17 Oct 2025 bugfix error2, allow 16 bit answers
//WBL 14 Oct 2025 check all registers (not just R0) add using mprotect
//WBL 12 Oct 2025 Based on div_perf.cpp r1.17 and GPengine.cpp r1.119
//WBL 11 Oct 2025 organise combined div.cpp as 3 functions
//WBL 10 Oct 2025 combine div_funcs.cpp with div.cpp
//WBL  8 Oct 2025 Switch to AVX based on GPengine.cpp r1.65 Interpret16
//WBL  7 Oct 2025 Switch to instruction count, clean output for fit.bat
//WBL  2 Oct 2025 based on interpret from GPengine.cpp r1.119, main.cpp r1.22


/* compile:
  g++ -fpermissive -fmax-errors=1 -Dregtype=??? eval_perf.cpp 
 */

#include <stdlib.h>
#include <iostream>
#include <assert.h>
#include <string.h>
#include <fstream>
#include <stdio.h>

//#include <immintrin.h>

#include <climits>

#include <malloc.h>
#include <signal.h>
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>

//from GPengine.h r1.56
#include "eval.h"

#define validvar(x)    (x>= 0 && x < NumVar)
#define validreg(x)    (x > IntRangeEnd && x <= (IntRangeEnd + NumVar))
#define validopcode(x) (x>= 0 && x < NumOp)
#define validvalue(x)  (x >= IntRangeStart && x <= IntRangeEnd)
#define validInstr(x) (\
  validvar(   x[0]) && \
  validreg(   x[1]) && \
  validopcode(x[2]) && \
  (validreg(x[3]) || validvalue(x[3])))

using namespace std;

unsigned int rrand(int max) 
{
	return (rand()%max);	
}

//Based on https://github.com/wblangdon/linux_perf_api/blob/main/demo_perf.c
//and test_prog.c r1.4
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/perf_event.h>
#include <asm/unistd.h>
static long
perf_event_open(struct perf_event_attr *hw_event, pid_t pid,
		int cpu, int group_fd, unsigned long flags){
  int ret;
  ret = syscall(__NR_perf_event_open, hw_event, pid, cpu,group_fd, flags);
  return ret;
}
int perf_start(){
  struct perf_event_attr pe;
  int fd;

  memset(&pe, 0, sizeof(pe));
  //set up perf's event data structure.
  //There are many things perf can report, here ask for instruction count
  //Note your CPU may not support all perf's options
  pe.type = PERF_TYPE_HARDWARE;	//PERF_TYPE_HW_CACHE;
  //pe.type = PERF_TYPE_SOFTWARE;
  pe.size = sizeof(pe);
  //PERF_COUNT_HW_CACHE_MAX
  //PERF_COUNT_HW_CACHE_OP_MAX
  //PERF_COUNT_HW_CACHE_RESULT_MAX
  //? not yet ? PERF_COUNT_SW_MAX
  pe.config = PERF_COUNT_HW_INSTRUCTIONS; //PERF_COUNT_HW_CACHE_MISSES;
  //pe.config = PERF_COUNT_SW_CPU_CLOCK;
  pe.disabled = 1;
  pe.exclude_kernel = 1;
  pe.exclude_hv = 1;

  fd = perf_event_open(&pe, 0, -1, -1, 0);
  if (fd == -1) {
    fprintf(stderr, "Error on perf_event_open %llx\n", pe.config);
    return EXIT_FAILURE;
  }

  //ioctl(fd, PERF_EVENT_IOC_RESET, 0);
  //ioctl(fd, PERF_EVENT_IOC_ENABLE, 0);
  return fd;
}
long long perf_end(const int fd) {
  long long count = -1;
  ioctl(fd, PERF_EVENT_IOC_DISABLE, 0);
  if(errno != 0) {//check all is well 
    //Might get for example errno == EBADF 9 /* Bad file number */ bad fd ?
    fprintf(stderr,"perf_end(%d) Opps errno is %d\n",fd,errno);
    exit(errno);
  }
  read(fd, &count, sizeof(count)); //instructions is first 8 bytes read from fd
  return count;
}

const int FitnessCaseNum = 64;
int regO[4]; //4 programs
int reg1[4];
int reg2[4];
unsigned char Output[FitnessCaseNum];
unsigned char Input1[4][FitnessCaseNum];
unsigned char Input2[4][FitnessCaseNum];

void check_diversity(const int p){
  for(int i=0;i<FitnessCaseNum;i+=8){
    int count[256];
    memset(count,0,sizeof(count));
    for(int j=0;j<8;j++){
      unsigned char out = Output[i+j];
      count[out]++;
      if(count[out] >= 0 /*7*/){
	cout<<"Program "<<p<<" "
	    <<"Output["<<(i+j)<<"] "<<(int)Output[i+j]<<" check_diversity"<<endl;
	for(int k=0;k<FitnessCaseNum;k++){
	  cout<<k<<" "<<(int)Output[k]; if(k%8!=7) cout<<"\t"; else cout<<endl;
	}
	i=FitnessCaseNum;
	break; //exit(99);
      }
    }
  }
}


unsigned char prot_div(const unsigned char op1, const unsigned char op2){
  if (op2 == 0) return 0;
  return op1/op2;
}
void settest(const int p, const int i, const int op1, const int op2, const int debug){
  assert(  p>=0 &&   p<4);
  assert(  i>=0 &&   i<FitnessCaseNum);
  Output[i] = prot_div(op1,op2);
  Input1[p][i] = op1;
  Input2[p][i] = op2;
  //cout<<"settest("<<i<<","<<op1<<","<<op2<<","<<debug<<") "
  //    <<(int)Output[i]<<endl;
  assert(op1>=0 && op1<256);
  assert(op2>=0 && op2<256);
}
void divtests(){
  for(int p=0;p<4;p++){
  /*
  64 fitness cases.
  no read or writes outside inputs or outputs
    50% div 0 output 0
    50% else output   0 eg  10/11 :  4 12%
	     output   1 eg  12/11 :  4 12%
	     output 255 ie 255/1  :  4 12%
	     output >=1 eg 200/1  :  4 12%
	     output >=2 eg 174/72 : 16 25%
  */
  for(int i=0;i<FitnessCaseNum;i++){
    if(rrand(2)) settest(p,i,rrand(256),0,-1);
    else {
      const int r   = rrand(8);
      const int op2 = 2+rrand(254);
      switch (r){
      case 0: settest(p,i,rrand(op2),     op2,r); break;
      case 1: {
       	const int op1 = op2+((op2<128)? rrand(op2) : rrand(256-op2));
	settest(p,i,op1,                  op2,r); break;}
      case 2:  settest(p,i,255,             1,r); break;
      case 3:  settest(p,i,rrand(256),      1,r); break;
      default: {// >=2 
	const int ratio = 2+rrand(125);
	const int op2   = 1+rrand(255/ratio);
	const int op1   = ratio*op2+rrand(256-ratio*op2);
	settest(p,i,op1,                  op2,r); break;}
      }//switch r
    }//else
  }//endfor i
  //check_diversity(p);
  }//endfor p
}

int InstrArg(const OP code)
{
	const int x = code-IntRangeEnd-1;
	assert(code<=IntRangeEnd || (x    >= 0 &&    x < NumVar));
	if(code>IntRangeEnd) return x;       //code for Var
	else                 return INT_MAX; //code is constant, do not use
}

retval InstrArg(const OP code, const retval reg[], const int answers[NumVar])
{
	if(code>IntRangeEnd) {
	  const int x = InstrArg(code);
	  assert(answers[x]>=0 && answers[x]<256);
	}
	if(code>IntRangeEnd) return reg[InstrArg(code)]; //code for Var
	else                 return code;                //code is constant
}

int InstrReg(const OP code) //return code not value
{
	const int x = code-IntRangeEnd-1;
	assert(   x >= 0 &&    x < NumVar);
	return x;
}

retval InstrReg(const OP code, const retval reg[], const int answers[NumVar])
{
	const int x = InstrReg(code);
	assert(answers[x]>=0 && answers[x]<256);
	return reg[x];
}

void eval(const int prog, const int pc, const int thread,
	  const instr I, retval reg[NumVar], int answers[NumVar]){
  retval val;
  const retval op1 = InstrReg(I[1],reg,answers);
  const retval op2 = InstrArg(I[3],reg,answers);
  switch(I[2]) {
  case plus_op: 
    val = op1+op2;
    break;
  case minus_op: 
    val = op1-op2;
    break;
  case mul_op: 				
    val = op1*op2;
    break;
  case div_op: 
    if (op2 == 0)
      val = 0;
    else
      val = op1/op2;
    break;
  default:
    exit(10);
  }
  reg[I[0]]     = val;
  answers[I[0]] = val;
  /*if(thread  == 0 || thread  == npar-1 **prog==0 && pc==3**)
  cout<<"eval "<<prog<<" "<<pc<<" "<<thread<<":\t"
      <<I[1]<<"("<<(int)op1<<") "
      <<I[2]<<" "
      <<I[3]<<"("<<(int)op2<<") "
      <<"\tgives "<<(int)val<<" into "<<I[0]<<endl;
  assert(op1!=252 && op2 != 252); //could be ok but probably an error
  */
}
void evaln(const int prog, const int pc,
	   const instr I, retval regs[npar][NumVar],int answers[npar][NumVar]){
  assert(validInstr(I));  
  for(int i=0;i<npar;i++) eval(prog,pc,i,I,regs[i],answers[i]);
}

OP GenerateReg(const int nreg, const int regs[]){
  return IntRangeEnd + regs[rrand(nreg)] + 1; //code for Var
}
OP GenerateArg(const int nreg, const int regs[]){// Randomly choose a var or a const
  if(rrand(10)<2) return GenerateReg(nreg,regs); //code for Var
  else            return (IntRangeEnd-IntRangeStart)*double(rand())/RAND_MAX+IntRangeStart;
}
int ChooseOp(const int ops[NumOp]){
  int nmissing = 0;
  int missing[NumOp];
  for(int i=0;i<NumOp;i++){
    if(ops[i]==0){
      missing[nmissing] = i;
      nmissing++;
    }
  }
  return (nmissing)? missing[rrand(nmissing)] : rrand(NumOp);
}
void tests(const int regO[4], const int reg1[4], const int reg2[4],
	   instr prog[4][4], int answers[4][npar][NumVar]){
  //assert(FitnessCaseNum/16==4);
  memset(answers,255,4*npar*NumVar*sizeof(int)); //-1 means not over written
  int ops[NumOp] = {0,0,0,0};
  
  for(int i=0;i<4;i++){
    assert(i*npar+(npar-1)<FitnessCaseNum*4);
    retval regs[npar][NumVar]; memset(regs,252,sizeof(regs));
    for(int j=0;j<npar;j++) answers[i][j][reg1[i]] = regs[j][reg1[i]] = Input1[i][j];
    for(int j=0;j<npar;j++) answers[i][j][reg2[i]] = regs[j][reg2[i]] = Input2[i][j];

    //fabricate 4 instruction GPengine program starting with protected div
    //CGPengine::GenerateInstr(instr I)
    prog[i][0][0] = rrand(NumVar);
    prog[i][0][1] = IntRangeEnd + reg1[i] + 1; //code for Var, GenerateReg()
    prog[i][0][2] = div_op;
    prog[i][0][3] = IntRangeEnd + reg2[i] + 1; //code for Var, GenerateArg() 
    evaln(i,0,prog[i][0],regs,answers[i]);
    int r3[3] = {reg1[i],reg2[i],prog[i][0][0]};
    ops[prog[i][0][2]]++;

    /*follow protected div by three (almost) random instructions */
    prog[i][1][0] = rrand(NumVar);
    prog[i][1][1] = GenerateReg(3,r3);
    prog[i][1][2] = (i<2)? rrand(NumOp) : ChooseOp(ops); //ensure test all ops eventually
    prog[i][1][3] = GenerateArg(3,r3);
    evaln(i,1,prog[i][1],regs,answers[i]);
    int r4[4]; memcpy(r4,r3,sizeof(r3)); r4[4-1]=prog[i][0][0];
    ops[prog[i][1][2]]++;

    prog[i][2][0] = rrand(NumVar);
    prog[i][2][1] = GenerateReg(4,r4);
    prog[i][2][2] = (i<2)? rrand(NumOp) : ChooseOp(ops); //ensure test all ops eventually
    prog[i][2][3] = GenerateArg(4,r4);
    evaln(i,2,prog[i][2],regs,answers[i]);
    int r5[5]; memcpy(r5,r4,sizeof(r4)); r5[5-1]=prog[i][0][0];
    ops[prog[i][2][2]]++;

    prog[i][3][0] = regO[i];
    prog[i][3][1] = GenerateReg(5,r5);
    prog[i][3][2] = (i<2)? rrand(NumOp) : ChooseOp(ops); //ensure test all ops eventually
    prog[i][3][3] = GenerateArg(5,r5);
    evaln(i,3,prog[i][3],regs,answers[i]);
    ops[prog[i][3][2]]++;
  }//endfor i<4
  assert(ops[0] && ops[1] && ops[2] && ops[3]);
}

void protect(const void* target, const size_t size, const int mode){
  const size_t pagesize = sysconf(_SC_PAGE_SIZE);
  //cout<<"pagesize="<<pagesize<<endl;
  assert(pagesize == 4096);
  const long long start = (long long) target    & (~4095);
  const long long end   = start+size+pagesize-1 & (~4095);
  const size_t len = end-start;
  //printf("protect(%p,%lld,%d) %p %p %lld\n",target,size,mode, start,end,len);
  const int status = mprotect((void*)start, len, mode);
  assert(status != -1);
}

//https://stackoverflow.com/questions/12778949/cuda-memory-alignment
__attribute__((aligned(4096)))
unsigned char div32_0[4096];
__attribute__((aligned(4096)))
uint32_t div32[256*256];
__attribute__((aligned(4096)))
unsigned char div32_2[4096];

void init_div(){
  for(int op1=0;op1<256;op1++){
  for(int op2=0;op2<256;op2++){
    if (op2 == 0)
      div32[op1*256+op2] = 0;
    else
      div32[op1*256+op2] = (retval)(op1/op2);
  }}
  protect(div32_0,sizeof(div32_0),PROT_NONE);
  protect(div32,  sizeof(div32),  PROT_READ);
  protect(div32_2,sizeof(div32_2),PROT_NONE);
}

inline int abs(const int x){
  //assert(x >= -255 && x <= 255);
  return (x<0)? -x : x;
}

int main(const int argc, const char *argv[]) {
  //cout<<  "eval_perf $Revision: 1.28 $"<<flush;
  int seed = 2035742;
  for(int i = 1; i<argc;i++) {
    if(*argv[i]=='s' || *argv[i]=='S') seed=atoi(&argv[i][1]);
  }
  //cout<< " seed=" <<seed<<flush;
  //cout<< " regtype size " <<sizeof(regtype)<<" bytes"<<flush;
  //cout<<endl;
  
  srand(seed);
  divtests();
  init_div();
  //use same registers for 64 test cases, leave unusual pattern in others
  for (int i=0;i<4;i++) {
    regO[i] = rrand(8);
    reg1[i] = rrand(8);
    reg2[i] = rrand(8);
    /*cout<<" regO["<<i<<"]="<<regO[i]
    <<" reg1["<<i<<"]="<<reg1[i]
    <<" reg2["<<i<<"]="<<reg2[i]
    <<endl;
    */
  }

  __attribute__((aligned(4096)))
  unsigned char prog_0[4096];
  __attribute__((aligned(4096)))
  instr prog[4][4];
  __attribute__((aligned(4096)))
  unsigned char prog_2[4096];

  int answers[4][npar][NumVar];
  tests(regO,reg1,reg2,prog,answers);
  protect(prog_0,sizeof(prog_0),PROT_NONE);
  protect(prog,  sizeof(prog),  PROT_READ);
  protect(prog_2,sizeof(prog_2),PROT_NONE);
    
  const int regsize = NumVar*npar;
  const int regsize2 = 4096/sizeof(regtype); //expand for mprotect
  assert(regsize*sizeof(regtype)<=4096);

  //evolved code uses registers
  __attribute__((aligned(4096)))
  unsigned char registers0[4096];
  protect(registers0,sizeof(registers0),PROT_NONE);
  __attribute__((aligned(4096)))
  regtype registers[regsize2];
  __attribute__((aligned(4096)))
  unsigned char registers2[4096];
  protect(registers2,sizeof(registers2),PROT_NONE);

  long long times[4];
  
  const int fd = perf_start();
  int errors = 0;
// run program from div.cpp r1.6
for (int i=0;i<4;i++) {
  memset(registers,0x5a,sizeof(registers));
  const int r1 = reg1[i]*npar; assert(r1>=0 && r1<regsize-npar+1);
  const int r2 = reg2[i]*npar; assert(r2>=0 && r2<regsize-npar+1);
  const int rO = regO[i]*npar; assert(rO>=0 && rO<regsize-npar+1);
  assert(r1 != r2);
  for(int j=0;j<npar;j++) registers[r1+j] = Input1[i][j];
  for(int j=0;j<npar;j++) registers[r2+j] = Input2[i][j];

  ioctl(fd, PERF_EVENT_IOC_RESET, 0);
  ioctl(fd, PERF_EVENT_IOC_ENABLE, 0);

  Interpret64(4, prog[i], registers, div32);
  
  times[i] = perf_end(fd);
  //cout<<"Interpret "<<flush;
  //cout<<"times["<<i<<"] "<<times[i]<<endl;

  /*for (int k=0;k<regsize;k++) {
    cout<<"registers["<<k<<"] "<<(int)registers[k]<<flush;
                          assert(i>=0 && i<4);
    const int j = k%npar; assert(j>=0 && j<npar);
    const int r = k/npar; assert(r>=0 && r<NumVar);
    if(answers[i][j][r] != -1) {
      cout<<" answers["<<i<<"]["<<j<<"]["<<r<<"] "<<answers[i][j][r]<<flush;
    }
    if(k%4<3) cout<<"\t"; else cout<<endl;
  }*/
  
  //regO[0]=1 reg1[0]=0 reg2[0]=4 after more prng needed by divtests
  //regO[1]=1 reg1[1]=2 reg2[1]=5
  //regO[2]=7 reg1[2]=1 reg2[2]=5
  //regO[3]=0 reg1[3]=3 reg2[3]=6

  //assert(regO[i] == 6||regO[i]==4||regO[i]==1||regO[i]==3);
  //assert(reg1[i] == 4||reg1[i]==5||reg1[i]==4||reg1[i]==7);
  //assert(reg2[i] == 6||reg2[i]==7||reg2[i]==2||reg2[i]==3);

  //check all registers
  regtype pattern1;memset(&pattern1,0x5a,sizeof(pattern1));
  for (int k=regsize;k<regsize2;k++) {
    if(registers[k] != pattern1 ){
      cerr<<"i="<<i<<" registers["<<k<<"]="<<(int)registers[k]<<" "
	  <<"corrupted!! "
	  <<endl;
      //print simple number otherwise Magpie says RUN_PARSE_ERROR
      cout<<"MAGPIE_FITNESS: 100000000"<<endl;
      exit(3); //use exit to avoid segfault due to PROT_NONE;
    }
  }
  {int nak = 0; int e;
  for (int k=0;k<regsize;k++) {
    const int r = k/npar;
    const int j = k%npar;
    if(answers[i][j][r] == -1){if(registers[k] != pattern1)        {nak=4; e=k; break;}}
    if(answers[i][j][r] != -1){if(registers[k]<0||registers[k]>255){nak=5; e=k; break;}}
  }
  if(nak){
    const int k = e;
    const int r = k/npar;
    const int j = k%npar;
    cerr<<"i="<<i<<" registers["<<k<<"]="<<(int)registers[k]<<" "
	  <<"corrupted!! reg="<<r<<" j="<<j<<" nak="<<nak<<endl;
      //print simple number otherwise Magpie says RUN_PARSE_ERROR
      cout<<"MAGPIE_FITNESS: 100000000"<<endl;
      exit(4); //use exit to avoid segfault due to PROT_NONE;
  }}
  for (int k=0;k<regsize;k++) {
                          assert(i>=0 && i<4);
    const int j = k%npar; assert(j>=0 && j<npar);
    const int r = k/npar; assert(r>=0 && r<NumVar);
    if(answers[i][j][r] == -1) continue;
    const int expected = answers[i][j][r];
    assert(expected>=0 && expected<256);
    const int error2 = expected - registers[k];
    /*
    if(expected != registers[k])
      cout<<"answers["<<i<<"]["<<j<<"]["<<r<<"]="
	  <<answers[i][j][r]<<" != "
	  <<"registers["<<k<<"] "
	  <<(int)registers[k]<<endl;
    */
    const int e = abs(error2);
    const int limite = 80000000/(4*regsize2);
    errors += (e>limite)? limite : e;
  }//endfor k

 }//endfor i
  close(fd); //perf_end();
  long long sum = 0;
  for (int i=0;i<4;i++) {
    /*
    cout<<"i"<<i
	<<" "<<times[i]
	<<endl;
    */
    const long long limit = 10000000/4;
    if(times[i]>limit) sum += limit;
    else               sum += times[i];
  }
  //cerr<<"errors "<<errors<<" "
  //    <<"sum "<<sum<<endl;
  //print simple number otherwise Magpie says RUN_PARSE_ERROR
  if(errors>0) cout<<"MAGPIE_FITNESS: "<<10000000+errors<<endl;
  else         cout<<"MAGPIE_FITNESS: "<<sum<<endl;
  exit(errors!=0);//use exit to avoid segfault due to PROT_NONE
}

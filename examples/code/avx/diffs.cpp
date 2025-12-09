#ifdef AVX
#include <immintrin.h>
#define up64(x) (64*(((int)(x)+63)/64))
#else
error this version needs avx512
#endif
        memset(I.output,253,up64(FitnessCase->Num())*sizeof(retval));
        I.output = new retval[up64(FitnessCase->Num())];
void CGPengine::InstrArg64(const OP code, const retval reg[], retval out[64]) const
{
        if(code>IntRangeEnd){
                const int x = InstrArg(code);
                memcpy(out,&reg[x*64],64*sizeof(retval));
        }
        else
                  memset(out,code,      64*sizeof(retval));
}
void CGPengine::InstrReg64(const OP code, const retval reg[], retval out[64]) const
{
        const int x = InstrReg(code);
        memcpy(out,&reg[x*64],64*sizeof(retval));
}
        for (int i=0;i<FitnessCase->Num();i+=64)
                Interpret64(I,i);
                for(int j=0;j<64;j++) {
                if(i+j < FitnessCase->Num()){
                const int error = FitnessCase->Output(i+j) - I.output[i+j];
                }
                const int x = i+j+64+1;
                  fprintf(stderr,"i=%4d j=%2d I.output[%4d] %3d\n",i,j,x,I.output[x]);
                }
        for (int i=0;i<NumVar;i++) reg[i] = FitnessCase->Input(fcase,i);
void CGPengine::Interpret64(Individual &I, const int fcase)
{
        retval reg[NumVar*64];
        for (int i=0;i<NumVar;i++) {
          for(int j=0;j<64;j++) {
            reg[i*64+j] = (fcase+j < FitnessCase->Num())? FitnessCase->Input(fcase+j,i) : 252;
        }}
        for (int i=I.InstrLen2-1;i>=0;i--)
        {
                retval val[64];
                retval op1[64],op2[64];
                InstrReg64(I.Instr2[i][1],reg,op1);
                InstrArg64(I.Instr2[i][3],reg,op2);
                switch(int(I.Instr2[i][2]))
                {
                        case plus:
                                for(int j=0;j<64;j++) val[j] = op1[j]+op2[j];
                                break;
                        case minus:
                                for(int j=0;j<64;j++) val[j] = op1[j]-op2[j];
                                break;
                        case mul:
                                for(int j=0;j<64;j++) val[j] = op1[j]*op2[j];
                                break;
                        case div:
                                for(int j=0;j<64;j++){
                                if (op2[j] == 0)
                                        val[j] = 0;
                                else
                                        val[j] = op1[j]/op2[j];
                                }
                                break;
                }
                memcpy(&reg[int(I.Instr2[i][0])*64],val,64);
        }
        memcpy(&I.output[fcase],reg,64);
}
                        case plus: {
                                const __m512i a = _mm512_loadu_epi8(op1);
                                const __m512i b = _mm512_loadu_epi8(op2);
                                const __m512i c = _mm512_add_epi8(a,b);
                                retval val2[64];
                                _mm512_storeu_epi8(val2,c);
                        }
#define up16(x) (16*(((int)(x)+15)/16))
  int i;
  for (i=0;i<FitnessCase->Num();i++){
    fprintf(stderr,"sanity output %p I.fitness %d != %d, Cases %d\n",I.output,I.fitness,fitness,i);
        memset(I.output,253,up16(FitnessCase->Num())*sizeof(retval));
        retval output0[1201];
        for (int i=0;i<FitnessCase->Num();i++)
                int16_t valx[16];
                int16_t op1x[16],op2x[16];
                InstrReg16(I.Instr2[i][1],reg,op1x);
                InstrArg16(I.Instr2[i][3],reg,op2x);
                case plus:{
                  for(int j=0;j<16;j++) val[j] = op1[j]+op2[j];
                  const __m256i a = _mm256_loadu_si256((__m256i_u*)op1x);
                  const __m256i b = _mm256_loadu_si256((__m256i_u*)op2x);
                  const __m256i c = _mm256_add_epi16(a,b);
                  int16_t valx[16];
                  _mm256_storeu_si256((__m256i_u*)valx,c);
                  for(int j=0;j<16;j++) valx[j] = valx[j] & 255;
                  break;
                }
                case minus:
                  for(int j=0;j<16;j++) val[j] = op1[j]-op2[j];
                  break;
                case mul:
                  for(int j=0;j<16;j++) val[j] = op1[j]*op2[j];
                  break;
                case div:
                  for(int j=0;j<16;j++){
                    if (op2[j] == 0)
                      val[j] = 0;
                    else
                      val[j] = op1[j]/op2[j];
                  }
                  break;
void CGPengine::InstrArg16(const OP code, const retval reg[], int16_t out[16]) const
{
        if(code>IntRangeEnd){
                const int x = InstrArg(code);
                for(int i=0;i<16;i++) out[i] = reg[x*16+i];
        }
        else
                for(int i=0;i<16;i++) out[i] = code;
}
void CGPengine::InstrReg16(const OP code, const retval reg[], int16_t out[16]) const
{
        const int x = InstrReg(code);
        for(int i=0;i<16;i++) out[i] = reg[x*16+i];
}
                case div: {
                }
error this version needs avx
                if(I.Instr2[i][2]==div) {
  retval inputs[NumVar*up16(1201)] = {252};
        if(inputs[0] == 252){
          for(int f=0;f<up16(FitnessCase->Num());f+=16) {
          for(int i=0;i<NumVar;i++) {
            inputs[f*NumVar + i*16 + j] = (f+j < FitnessCase->Num())? FitnessCase->Input(f+j,i) : 252;
        }}}}
        memcpy(reg,&inputs[fcase*NumVar],NumVar*16*sizeof( retval));
  retval div8[256*256];
void init_div(){
  for(int op1=0;op1<256;op1++){
  for(int op2=0;op2<256;op2++){
    if (op2 == 0)
      div8[op1*256+op2] = 0;
    else
      div8[op1*256+op2] = op1/op2;
  }}
}
        if(inputs[0] == 252){init_div();
                    val[j] = div8[op1[j]*256+op2[j]];
                    */
uint32_t div32[256*256];
    div32[op1*256+op2] = div8[op1*256+op2];
#ifndef NDEBUG
                  int x[16];
                  for(int j=0;j<16;j++){
                    x[j] = op1[j]*256+op2[j];
                  }
                    memcpy(&reg[int(I.Instr2[i][0])*16+j*8],output,8);
                    double* output64 = (double*) &reg[int(I.Instr2[i][0])*16+j*8];
#ifndef NDEBUG
                    retval* output = (retval*) output64;
                    for(int k=0;k<8;k++) output[k]=51;
#endif
                    uint32_t Indices[8];
                    {
                    uint32_t op1y[8],op2y[8];
                    InstrReg8(I.Instr2[i][1],reg,j*8,op1y);
                    InstrArg8(I.Instr2[i][3],reg,j*8,op2y);
                    const __m256i a =    _mm256_loadu_si256((__m256i_u*)op1y);
                    const __m256i a256 = _mm256_slli_si256(a, 1);
                    const __m256i b =    _mm256_loadu_si256((__m256i_u*)op2y);
                    const __m256i c =    _mm256_add_epi32(a256,b);
                    _mm256_storeu_si256((__m256i_u*)Indices,c);
                    }
void CGPengine::InstrArg8(const OP code, const retval reg[], const int j, uint32_t out[8]) const
{
        if(code>IntRangeEnd){
                const int x = InstrArg(code);
                for(int i=0;i<8;i++) {
                  const int xx = x*16+j+i;
                  out[i] = reg[x*16+j+i];
                }
        }
        else
                for(int i=0;i<8;i++) out[i] = code;
}
void CGPengine::InstrReg8(const OP code, const retval reg[], const int j, uint32_t out[8]) const
{
        const int x = InstrReg(code);
        for(int i=0;i<8;i++) {
          const int xx = x*16+j+i;
          out[i] = reg[xx];
        }
}
                    double* output64 = (double*) &reg[int(I.Instr2[i][0])*16+j*8];
#ifndef NDEBUG
                    retval* output = (retval*)output64;
#endif
                    const __m256i by =   _mm256_loadu_si256((__m256i_u*)op2y);
                    const __m256i index_vec = _mm256_add_epi32(a256,by);
                    const __m256i a    = InstrReg8(I.Instr2[i][1],reg,j*8);
                    const __m256i by   = InstrArg8(I.Instr2[i][3],reg,j*8);
__m256i CGPengine::InstrArg8(const OP code, const retval reg[], const int j) const
{
#ifndef NDEBUG
        uint32_t out[8];
        uint32_t tmp[8];
        __m256i out8;
#endif
        if(code>IntRangeEnd){
                const int x = InstrArg(code);
#ifndef NDEBUG
                for(int i=0;i<8;i++) {
                  const int xx = x*16+j+i;
                  out[i] = reg[x*16+j+i];
                }
#endif
                const __m256i index = _mm256_setr_epi32(0,1,2,3,4,5,6,7);
                const __m256i a     = _mm256_i32gather_epi32((int*)&reg[x*16+j],index,1);
                const __m256i mask  = _mm256_set1_epi32(255);
#ifndef NDEBUG
                out8  = _mm256_and_si256(mask,a);
#else
                return _mm256_and_si256(mask,a);
#endif
        }
        else {
#ifndef NDEBUG
                for(int i=0;i<8;i++) out[i] = code;
                out8 = _mm256_set1_epi32(code);
#else
                return _mm256_set1_epi32(code);
#endif
        }
#ifndef NDEBUG
        _mm256_storeu_si256((__m256i_u*)tmp,out8);
        return out8;
#endif
}
__m256i CGPengine::InstrReg8(const OP code, const retval reg[], const int j) const
{
        const int x = InstrReg(code);
#ifndef NDEBUG
        uint32_t out[8];
        uint32_t tmp[8];
        for(int i=0;i<8;i++) {
          const int xx = x*16+j+i;
          out[i] = reg[xx];
        }
#endif
        const __m256i index = _mm256_setr_epi32(0,1,2,3,4,5,6,7);
        const __m256i a     = _mm256_i32gather_epi32((int*)&reg[x*16+j],index,1);
        const __m256i mask  = _mm256_set1_epi32(255);
        const __m256i out8  = _mm256_and_si256(mask,a);
#ifndef NDEBUG
        _mm256_storeu_si256((__m256i_u*)tmp,out8);
#endif
        return out8;
}
                const __m256i a = InstrReg16(I.Instr2[i][1],reg);
                const __m256i b = InstrArg16(I.Instr2[i][3],reg);
__m256i CGPengine::InstrArg16(const OP code, const retval reg[]) const
{
        int16_t out[16];
        int16_t tmp[16];
        if(code>IntRangeEnd){
                const int x = InstrArg(code);
                for(int i=0;i<16;i++) out[i] = reg[x*16+i];
        const __m256i index1   = _mm256_setr_epi32(0, 1, 2, 3, 4, 5, 6, 7);
        const __m256i a        = _mm256_i32gather_epi32((int*)&reg[x*16],index1,1);
        const __m256i mask     = _mm256_set1_epi32(255);
        const __m256i aa       = _mm256_and_si256(mask,a);
        const __m256i shuffle  = _mm256_setr_epi8( 0,  1, 4, 5, 8, 9,12,13,-1,-1,-1,-1,-1,-1,-1,-1,
                                                   16,17,20,21,24,25,28,29,-1,-1,-1,-1,-1,-1,-1,-1);
        const __m256i aaa      = _mm256_shuffle_epi8(aa,shuffle);
        const __m256i perm1    = _mm256_setr_epi32( 0, 1, 4, 5, 7, 7, 7, 7 );
        const __m256i aaaa     = _mm256_permutevar8x32_epi32(aaa,perm1);
        const __m256i index2   = _mm256_setr_epi32( 8, 9,10,11,12,13,14,15);
        const __m256i b        = _mm256_i32gather_epi32((int*)&reg[x*16],index2,1);
        const __m256i bb       = _mm256_and_si256(mask,b);
        const __m256i bbb      = _mm256_shuffle_epi8(bb,shuffle);
        const __m256i perm2    = _mm256_setr_epi32(7, 7, 7, 7, 0, 1, 4, 5);
        const __m256i bbbb     = _mm256_permutevar8x32_epi32(bbb,perm2);
        const __m256i out16 = _mm256_or_si256(aaaa,bbbb);
        _mm256_storeu_si256((__m256i_u*)tmp,out16);
                return out16;
        }
        else {
                for(int i=0;i<16;i++) out[i] = code;
                const __m256i out16 = _mm256_set1_epi16(code);
                _mm256_storeu_si256((__m256i_u*)tmp,out16);
                return out16;
        }
}
__m256i CGPengine::InstrReg16(const OP code, const retval reg[]) const
{
        int16_t out[16];
        int16_t tmp[16];
        const int x = InstrReg(code);
        for(int i=0;i<16;i++) out[i] = reg[x*16+i];
        const __m256i index1   = _mm256_setr_epi32(0, 1, 2, 3, 4, 5, 6, 7);
        const __m256i a        = _mm256_i32gather_epi32((int*)&reg[x*16],index1,1);
        const __m256i mask     = _mm256_set1_epi32(255);
        const __m256i aa       = _mm256_and_si256(mask,a);
        const __m256i shuffle  = _mm256_setr_epi8( 0,  1, 4, 5, 8, 9,12,13,-1,-1,-1,-1,-1,-1,-1,-1,
                                                   16,17,20,21,24,25,28,29,-1,-1,-1,-1,-1,-1,-1,-1);
        const __m256i aaa      = _mm256_shuffle_epi8(aa,shuffle);
        const __m256i perm1    = _mm256_setr_epi32( 0, 1, 4, 5, 7, 7, 7, 7 );
        const __m256i aaaa     = _mm256_permutevar8x32_epi32(aaa,perm1);
        const __m256i index2   = _mm256_setr_epi32( 8, 9,10,11,12,13,14,15);
        const __m256i b        = _mm256_i32gather_epi32((int*)&reg[x*16],index2,1);
        const __m256i bb       = _mm256_and_si256(mask,b);
        const __m256i bbb      = _mm256_shuffle_epi8(bb,shuffle);
        const __m256i perm2    = _mm256_setr_epi32(7, 7, 7, 7, 0, 1, 4, 5);
        const __m256i bbbb     = _mm256_permutevar8x32_epi32(bbb,perm2);
        const __m256i out16 = _mm256_or_si256(aaaa,bbbb);
        _mm256_storeu_si256((__m256i_u*)tmp,out16);
        return out16;
}
#ifndef NDEBUG
#endif
#ifndef NDEBUG
#endif
#ifndef NDEBUG
#endif
#ifndef NDEBUG
#else
                return _mm256_set1_epi16(code);
#endif
#ifndef NDEBUG
#endif
#ifndef NDEBUG
#endif
#ifndef NDEBUG
#endif
                int16_t tmp[16];
                {
                const __m256i shuffle = _mm256_setr_epi8( 0, 2, 4, 6, 8,10,12,14,-1,-1,-1,-1,-1,-1,-1,-1,
                                                          16,18,20,22,24,26,28,30,-1,-1,-1,-1,-1,-1,-1,-1);
                const __m256i cc      = _mm256_shuffle_epi8(c,shuffle);
                const __m256i perm    = _mm256_setr_epi32( 0, 1, 4, 5, 7, 7, 7, 7 );
                const __m256i ccc     = _mm256_permutevar8x32_epi32(cc,perm);
                const __m128i cccc    = _mm256_castsi256_si128(ccc);
                _mm_store_si128((__m128i*)&reg[int(I.Instr2[i][0])*16],cccc);
                _mm256_storeu_si256((__m256i_u*)tmp,ccc);
                }
;

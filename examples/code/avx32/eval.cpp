int InstrArg(const OP code)
{
        const int x = code-IntRangeEnd-1;
        return x;
}
__m256i InstrArg8(const OP code, const retval reg[], const int j)
{
        if(code>IntRangeEnd){
                const int x = InstrArg(code);
                const __m256i index = _mm256_setr_epi32(0,1,2,3,4,5,6,7);
                const __m256i a     = _mm256_i32gather_epi32((int*)&reg[x*npar+j],index,1);
                const __m256i mask  = _mm256_set1_epi32(255);
                return _mm256_and_si256(mask,a);
        }
        else {
                return _mm256_set1_epi32(code);
        }
}
#if !(defined(regtype8) || defined(regtype16) || defined(regtype32))
__m256i InstrArg16(const OP code, const retval reg[], const int j)
{
        if(code>IntRangeEnd){
                const int x = InstrArg(code);
        const __m256i index1   = _mm256_setr_epi32(0, 1, 2, 3, 4, 5, 6, 7);
        const __m256i a        = _mm256_i32gather_epi32((int*)&reg[x*npar+j],index1,1);
        const __m256i mask     = _mm256_set1_epi32(255);
        const __m256i aa       = _mm256_and_si256(mask,a);
        const __m256i shuffle  = _mm256_setr_epi8( 0,  1, 4, 5, 8, 9,12,13,-1,-1,-1,-1,-1,-1,-1,-1,
                                                   16,17,20,21,24,25,28,29,-1,-1,-1,-1,-1,-1,-1,-1);
        const __m256i aaa      = _mm256_shuffle_epi8(aa,shuffle);
        const __m256i perm1    = _mm256_setr_epi32( 0, 1, 4, 5, 7, 7, 7, 7 );
        const __m256i aaaa     = _mm256_permutevar8x32_epi32(aaa,perm1);
        const __m256i index2   = _mm256_setr_epi32( 8, 9,10,11,12,13,14,15);
        const __m256i b        = _mm256_i32gather_epi32((int*)&reg[x*npar+j],index2,1);
        const __m256i bb       = _mm256_and_si256(mask,b);
        const __m256i bbb      = _mm256_shuffle_epi8(bb,shuffle);
        const __m256i perm2    = _mm256_setr_epi32(7, 7, 7, 7, 0, 1, 4, 5);
        const __m256i bbbb     = _mm256_permutevar8x32_epi32(bbb,perm2);
        const __m256i out16 = _mm256_or_si256(aaaa,bbbb);
                return out16;
        }
        else {
                return _mm256_set1_epi16(code);
        }
}
#else
__m512i InstrArg16(const OP code, const uint8_t registers[], const int j)
{
        if(code>IntRangeEnd){
                const int x = InstrArg(code);
        const __m128i a    = _mm_loadu_epi8(&registers[x*npar+j]);
        const __m512i aa   = _mm512_cvtepi8_epi32(a);
        const __m512i zero = _mm512_set1_epi32(0);
        const __mmask64 k  = 0xeeeeeeeeeeeeeeee;
        return _mm512_mask_blend_epi8(k, aa, zero);
        }
        else {
                return _mm512_set1_epi32(code);
        }
}
__m512i InstrArg16(const OP code, const uint16_t registers[], const int j)
{
        if(code>IntRangeEnd){
                const int x = InstrArg(code);
        const __m256i a    = _mm256_loadu_epi16(&registers[x*npar+j]);
        const __m512i aa   = _mm512_cvtepi16_epi32(a);
        const __m512i mask = _mm512_set1_epi16(255);
        return _mm512_and_si512(mask,aa);
        }
        else {
                return _mm512_set1_epi32(code);
        }
}
__m512i InstrArg16(const OP code, const uint32_t registers[], const int j)
{
        if(code>IntRangeEnd){
                const int x = InstrArg(code);
        return _mm512_loadu_epi32(&registers[x*npar+j]);
        }
        else {
                return _mm512_set1_epi32(code);
        }
}
#endif
__m512i InstrArg32(const OP code, const uint8_t registers[], const int j)
{
        if(code>IntRangeEnd){
                const int x = InstrArg(code);
        const __m256i a    = _mm256_loadu_epi8(&registers[x*npar+j]);
        const __m512i aa   = _mm512_cvtepi8_epi16(a);
        const __m512i zero = _mm512_set1_epi32(0);
        const __mmask64 k  = 0xaaaaaaaaaaaaaaaa;
        return _mm512_mask_blend_epi8(k, aa, zero);
        }
        else {
                return _mm512_set1_epi16(code);
        }
}
__m512i InstrArg32(const OP code, const uint16_t registers[], const int j)
{
        if(code>IntRangeEnd){
                const int x = InstrArg(code);
        return _mm512_loadu_epi16(&registers[x*npar+j]);
        }
        else {
                return _mm512_set1_epi16(code);
        }
}
__m512i InstrArg64(const OP code, const uint8_t registers[])
{
        if(code>IntRangeEnd){
                const int x = InstrArg(code);
                return _mm512_loadu_epi8(&registers[x*npar]);
        }
        else {
                return _mm512_set1_epi8(code);
        }
}
int InstrReg(const OP code)
{
        const int x = code-IntRangeEnd-1;
        return x;
}
__m256i InstrReg8(const OP code, const retval reg[], const int j)
{
        const int x = InstrReg(code);
        const __m256i index = _mm256_setr_epi32(0,1,2,3,4,5,6,7);
        const __m256i a     = _mm256_i32gather_epi32((int*)&reg[x*npar+j],index,1);
        const __m256i mask  = _mm256_set1_epi32(255);
        const __m256i out8  = _mm256_and_si256(mask,a);
        return out8;
}
#if !(defined(regtype8) || defined(regtype16) || defined(regtype32))
__m256i InstrReg16(const OP code, const retval reg[], const int j)
{
        const int x = InstrReg(code);
        const __m256i index1   = _mm256_setr_epi32(0, 1, 2, 3, 4, 5, 6, 7);
        const __m256i a        = _mm256_i32gather_epi32((int*)&reg[x*npar+j],index1,1);
        const __m256i mask     = _mm256_set1_epi32(255);
        const __m256i aa       = _mm256_and_si256(mask,a);
        const __m256i shuffle  = _mm256_setr_epi8( 0,  1, 4, 5, 8, 9,12,13,-1,-1,-1,-1,-1,-1,-1,-1,
                                                   16,17,20,21,24,25,28,29,-1,-1,-1,-1,-1,-1,-1,-1);
        const __m256i aaa      = _mm256_shuffle_epi8(aa,shuffle);
        const __m256i perm1    = _mm256_setr_epi32( 0, 1, 4, 5, 7, 7, 7, 7 );
        const __m256i aaaa     = _mm256_permutevar8x32_epi32(aaa,perm1);
        const __m256i index2   = _mm256_setr_epi32( 8, 9,10,11,12,13,14,15);
        const __m256i b        = _mm256_i32gather_epi32((int*)&reg[x*npar+j],index2,1);
        const __m256i bb       = _mm256_and_si256(mask,b);
        const __m256i bbb      = _mm256_shuffle_epi8(bb,shuffle);
        const __m256i perm2    = _mm256_setr_epi32(7, 7, 7, 7, 0, 1, 4, 5);
        const __m256i bbbb     = _mm256_permutevar8x32_epi32(bbb,perm2);
        const __m256i out16 = _mm256_or_si256(aaaa,bbbb);
        return out16;
}
#else
__m512i InstrReg16(const OP code, const uint8_t registers[], const int j)
{
        const int x = InstrReg(code);
        const __m128i a    = _mm_loadu_epi8(&registers[x*npar+j]);
        const __m512i aa   = _mm512_cvtepi8_epi32(a);
        const __m512i zero = _mm512_set1_epi32(0);
        const __mmask64 k  = 0xeeeeeeeeeeeeeeee;
        return _mm512_mask_blend_epi8(k, aa, zero);
}
__m512i InstrReg16(const OP code, const uint16_t registers[], const int j)
{
        const int x = InstrReg(code);
        const __m256i a    = _mm256_loadu_epi16(&registers[x*npar+j]);
        const __m512i aa   = _mm512_cvtepi16_epi32(a);
        const __m512i mask = _mm512_set1_epi16(255);
        return _mm512_and_si512(mask,aa);
}
__m512i InstrReg16(const OP code, const uint32_t registers[], const int j)
{
        const int x = InstrReg(code);
        return _mm512_loadu_epi32(&registers[x*npar+j]);
}
#endif
__m512i InstrReg32(const OP code, const uint8_t registers[], const int j)
{
        const int x = InstrReg(code);
        const __m256i a    = _mm256_loadu_epi8(&registers[x*npar+j]);
        const __m512i aa   = _mm512_cvtepi8_epi16(a);
        const __m512i zero = _mm512_set1_epi32(0);
        const __mmask64 k  = 0xaaaaaaaaaaaaaaaa;
        return _mm512_mask_blend_epi8(k, aa, zero);
}
__m512i InstrReg32(const OP code, const uint16_t registers[], const int j)
{
        const int x = InstrReg(code);
        return _mm512_loadu_epi16(&registers[x*npar+j]);
}
__m512i InstrReg64(const OP code, const uint8_t registers[])
{
        const int x = InstrReg(code);
        return _mm512_loadu_epi8(&registers[x*npar]);
}
void Interpret64(const int InstrLen, const instr *Instr, regtype registers[NumVar*npar], const uint32_t div32[256*256]) {
for (int i=0;i<InstrLen;i++) {
  if(Instr[i][2]==div_op) {
  for(int j=0;j<npar;j+=16){
    const __m512i a          = InstrReg16(Instr[i][1],registers,j);
    const __m512i a256       = _mm512_slli_epi32(a, 8);
    const __m512i by         = InstrArg16(Instr[i][3],registers,j);
    const __m512i index_vec  = _mm512_add_epi32(a256,by);
    const __m512i result_vec = _mm512_i32gather_epi32(index_vec, div32, 4);
#ifdef regtype8
    const __m128i c          = _mm512_cvtepi32_epi8(result_vec);
    _mm_store_si128((__m128i*)&registers[Instr[i][0]*npar+j], c);
#else
#ifdef regtype16
    const __m256i c          = _mm512_cvtepi32_epi16(result_vec);
    _mm256_store_si256((__m256i*)&registers[Instr[i][0]*npar+j], c);
#else
    _mm512_store_epi32((__m512i*)&registers[Instr[i][0]*npar+j],result_vec);
#endif
#endif
  }
  } else {
    switch(Instr[i][2])
      {
      case plus_op:{
#ifdef regtype8
        const __m512i a = InstrReg64(Instr[i][1],registers);
        const __m512i b = InstrArg64(Instr[i][3],registers);
        __m512i c = _mm512_add_epi8 (a, b);
        _mm512_store_epi32((__m512i*)&registers[Instr[i][0]*npar],c);
#else
#ifdef regtype16
        for(int j=0;j<npar;j+=32){
        const __m512i a = InstrReg32(Instr[i][1],registers,j);
        const __m512i b = InstrArg32(Instr[i][3],registers,j);
        __m512i c = _mm512_add_epi16(a, b);
        const __m512i mask = _mm512_set1_epi16(255);
        const __m512i cc   = _mm512_and_si512(mask,c);
        _mm512_store_epi32((__m512i*)&registers[Instr[i][0]*npar+j],cc);
        }
#else
        for(int j=0;j<npar;j+=16){
        const __m512i a = InstrReg16(Instr[i][1],registers,j);
        const __m512i b = InstrArg16(Instr[i][3],registers,j);
        __m512i c = _mm512_add_epi32(a, b);
        const __m512i mask = _mm512_set1_epi32(255);
        const __m512i cc   = _mm512_and_si512(mask,c);
        _mm512_store_epi32((__m512i*)&registers[Instr[i][0]*npar+j],cc);
        }
#endif
#endif
        break;
      }
      case minus_op:{
#ifdef regtype8
        const __m512i a = InstrReg64(Instr[i][1],registers);
        const __m512i b = InstrArg64(Instr[i][3],registers);
        __m512i c = _mm512_sub_epi8 (a, b);
        _mm512_store_epi32((__m512i*)&registers[Instr[i][0]*npar],c);
#else
#ifdef regtype16
        for(int j=0;j<npar;j+=32){
        const __m512i a = InstrReg32(Instr[i][1],registers,j);
        const __m512i b = InstrArg32(Instr[i][3],registers,j);
        __m512i c = _mm512_sub_epi16(a, b);
        const __m512i mask = _mm512_set1_epi16(255);
        const __m512i cc   = _mm512_and_si512(mask,c);
        _mm512_store_epi32((__m512i*)&registers[Instr[i][0]*npar+j],cc);
        }
#else
        for(int j=0;j<npar;j+=16){
        const __m512i a = InstrReg16(Instr[i][1],registers,j);
        const __m512i b = InstrArg16(Instr[i][3],registers,j);
        __m512i c = _mm512_sub_epi32(a, b);
        const __m512i mask = _mm512_set1_epi32(255);
        const __m512i cc   = _mm512_and_si512(mask,c);
        _mm512_store_epi32((__m512i*)&registers[Instr[i][0]*npar+j],cc);
        }
#endif
#endif
        break;
      }
      case mul_op:{
#ifndef regtype32
        for(int j=0;j<npar;j+=32){
          const __m512i a  = InstrReg32(Instr[i][1],registers,j);
          const __m512i b  = InstrArg32(Instr[i][3],registers,j);
          const __m512i c  = _mm512_mullo_epi16(a, b);
#ifdef regtype8
          const __m256i cc = _mm512_cvtepi16_epi8(c);
          _mm256_storeu_si256((__m256i_u*)&registers[Instr[i][0]*npar+j],cc);
#else
          const __m512i mask = _mm512_set1_epi16(255);
          const __m512i cc   = _mm512_and_si512(mask,c);
          _mm512_store_epi32((__m512i*)&registers[Instr[i][0]*npar+j],cc);
#endif
        }
#else
        for(int j=0;j<npar;j+=16){
          const __m512i a  = InstrReg16(Instr[i][1],registers,j);
          const __m512i b  = InstrArg16(Instr[i][3],registers,j);
          const __m512i c  = _mm512_mullo_epi32(a, b);
          const __m512i mask = _mm512_set1_epi32(255);
          const __m512i cc   = _mm512_and_si512(mask,c);
          _mm512_store_epi32((__m512i*)&registers[Instr[i][0]*npar+j],cc);
        }
#endif
        break;
      }
      }
  }
 }
}

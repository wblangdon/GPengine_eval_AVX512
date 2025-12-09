__m512i InstrArg32(const OP code, const retval registers[], const int j)
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
__m256i InstrReg8(const OP code, const retval reg[], const int j)
{
        const int x = InstrReg(code);
        const __m256i index = _mm256_setr_epi32(0,1,2,3,4,5,6,7);
        const __m256i a     = _mm256_i32gather_epi32((int*)&reg[x*npar+j],index,1);
        const __m256i mask  = _mm256_set1_epi32(255);
        const __m256i out8  = _mm256_and_si256(mask,a);
        return out8;
}
__m512i InstrReg32(const OP code, const retval registers[], const int j)
        const __m256i a    = _mm256_loadu_epi8(&registers[x*npar+j]);
        const __m512i aa   = _mm512_cvtepi8_epi16(a);
        const __m512i zero = _mm512_set1_epi32(0);
        const __mmask64 k  = 0xaaaaaaaaaaaaaaaa;
        return _mm512_mask_blend_epi8(k, aa, zero);
__m512i InstrReg64(const OP code, const retval registers[])
        return _mm512_loadu_epi8(&registers[x*npar]);
        for(int j=0;j<npar/32;j++){
          const __m512i a  = InstrReg32(Instr[i][1],reg,j*32);
          const __m512i b  = InstrArg32(Instr[i][3],reg,j*32);
          const __m512i c  = _mm512_mullo_epi16(a, b);
          const __m256i cc = _mm512_cvtepi16_epi8(c);
          _mm256_storeu_si256((__m256i_u*)&reg[int(Instr[i][0])*npar+j*32],cc);
        }
#if(0)
#else
__m512i InstrArg16(const OP code, const retval registers[], const int j)
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
#endif
#if(0)
#else
__m512i InstrReg16(const OP code, const retval registers[], const int j)
{
        const int x = InstrReg(code);
        const __m128i a    = _mm_loadu_epi8(&registers[x*npar+j]);
        const __m512i aa   = _mm512_cvtepi8_epi32(a);
        const __m512i zero = _mm512_set1_epi32(0);
        const __mmask64 k  = 0xeeeeeeeeeeeeeeee;
        return _mm512_mask_blend_epi8(k, aa, zero);
}
#endif
  for(int j=0;j<npar;j+=16){
    const __m512i zero = _mm512_set1_epi32(0);
    const __m512i a          = InstrReg16(Instr[i][1],reg,j);
    const __m512i a256       = _mm512_slli_epi32(a, 8);
    const __m512i by         = InstrArg16(Instr[i][3],reg,j);
    const __m512i index_vec  = _mm512_add_epi32(a256,by);
    const __m512i result_vec = _mm512_i32gather_epi32(index_vec, div32, 4);
    const __m128i c          = _mm512_cvtepi32_epi8(result_vec);
    _mm_store_si128((__m128i*)&reg[int(Instr[i][0])*npar+j], c);
  }
__m512i InstrArg16(const OP code, const uint8_t registers[], const int j)
__m512i InstrArg32(const OP code, const uint8_t registers[], const int j)
__m512i InstrArg64(const OP code, const uint8_t registers[])
__m512i InstrReg16(const OP code, const uint8_t registers[], const int j)
__m512i InstrReg32(const OP code, const uint8_t registers[], const int j)
__m512i InstrReg64(const OP code, const uint8_t registers[])
      case mul_op:{
      }
    const __m512i a          = InstrReg16(Instr[i][1],registers,j);
    const __m512i by         = InstrArg16(Instr[i][3],registers,j);
    _mm_store_si128((__m128i*)&registers[int(Instr[i][0])*npar+j], c);
        _mm512_store_epi32((__m512i*)&registers[int(Instr[i][0])*npar],c);
        _mm512_store_epi32((__m512i*)&registers[int(Instr[i][0])*npar],c);
          const __m512i a  = InstrReg32(Instr[i][1],registers,j*32);
          const __m512i b  = InstrArg32(Instr[i][3],registers,j*32);
          _mm256_storeu_si256((__m256i_u*)&registers[int(Instr[i][0])*npar+j*32],cc);
        for(int j=0;j<npar;j+=32){
          const __m512i a  = InstrReg32(Instr[i][1],registers,j);
          const __m512i b  = InstrArg32(Instr[i][3],registers,j);
          _mm256_storeu_si256((__m256i_u*)&registers[int(Instr[i][0])*npar+j],cc);
        }
__m512i InstrArg16(const OP code, const uint16_t registers[], const int j)
{
        if(code>IntRangeEnd){
                const int x = InstrArg(code);
        const __m256i a    = _mm256_loadu_epi16(&registers[x*npar+j]);
        const __m512i aa   =  _mm512_cvtepi16_epi32(a);
        const __m512i mask = _mm512_set1_epi16(255);
        return _mm512_and_si512(mask,aa);
        }
        else {
                return _mm512_set1_epi32(code);
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
__m512i InstrReg16(const OP code, const uint16_t registers[], const int j)
{
        const int x = InstrReg(code);
        const __m256i a    = _mm256_loadu_epi16(&registers[x*npar+j]);
        const __m512i aa   = _mm512_cvtepi16_epi32(a);
        const __m512i mask = _mm512_set1_epi16(255);
        return _mm512_and_si512(mask,aa);
}
__m512i InstrReg32(const OP code, const uint16_t registers[], const int j)
{
        const int x = InstrReg(code);
        return _mm512_loadu_epi16(&registers[x*npar+j]);
}
#ifdef regtype8
#else
    const __m256i c          = _mm512_cvtepi32_epi16(result_vec);
    _mm256_store_si256((__m256i*)&registers[int(Instr[i][0])*npar+j], c);
#endif
#ifdef regtype8
#else
        for(int j=0;j<npar;j+=32){
        const __m512i a = InstrReg32(Instr[i][1],registers,j);
        const __m512i b = InstrArg32(Instr[i][3],registers,j);
        __m512i c = _mm512_add_epi16(a, b);
        const __m512i mask = _mm512_set1_epi16(255);
        const __m512i cc   = _mm512_and_si512(mask,c);
        _mm512_store_epi32((__m512i*)&registers[int(Instr[i][0])*npar+j],cc);
        }
#endif
#ifdef regtype8
#else
        for(int j=0;j<npar;j+=32){
        const __m512i a = InstrReg32(Instr[i][1],registers,j);
        const __m512i b = InstrArg32(Instr[i][3],registers,j);
        __m512i c = _mm512_sub_epi16(a, b);
        const __m512i mask = _mm512_set1_epi16(255);
        const __m512i cc   = _mm512_and_si512(mask,c);
        _mm512_store_epi32((__m512i*)&registers[int(Instr[i][0])*npar+j],cc);
        }
#endif
#ifdef regtype8
#else
          const __m512i mask = _mm512_set1_epi16(255);
          const __m512i cc   = _mm512_and_si512(mask,c);
          _mm512_store_epi32((__m512i*)&registers[Instr[i][0]*npar+j],cc);
#endif
        const __m512i aa   = _mm512_cvtepi16_epi32(a);
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
__m512i InstrReg16(const OP code, const uint32_t registers[], const int j)
{
        const int x = InstrReg(code);
        return _mm512_loadu_epi32(&registers[x*npar+j]);
}
#ifdef regtype16
#else
    _mm512_store_epi32((__m512i*)&registers[Instr[i][0]*npar+j],result_vec);
#endif
#else
#ifdef regtype16
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
#ifdef regtype16
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
#ifndef regtype32
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
#if !(defined(regtype8) || defined(regtype16) || defined(regtype32))
#if !(defined(regtype8) || defined(regtype16) || defined(regtype32))
                const __m256i a     = _mm256_i32gather_epi32((int*)&reg[x*npar+j],index,1);
__m256i InstrArg16(const OP code, const retval reg[], const int j)
        const __m256i a        = _mm256_i32gather_epi32((int*)&reg[x*npar+j],index1,1);
        const __m256i b        = _mm256_i32gather_epi32((int*)&reg[x*npar+j],index2,1);
__m256i InstrReg16(const OP code, const retval reg[], const int j)
        const __m256i a        = _mm256_i32gather_epi32((int*)&reg[x*npar+j],index1,1);
        const __m256i b        = _mm256_i32gather_epi32((int*)&reg[x*npar+j],index2,1);
        const __m256i a     = _mm256_i32gather_epi32((int*)&reg[x*npar+j],index,1);
void Interpret64(const int InstrLen, const instr *Instr, regtype registers[NumVar*npar], const uint32_t div32[256*256]) {
#if regtype == uint8_t
  retval* reg = registers;
#else
  retval reg[NumVar*npar];
  for(int i=0;i<NumVar*npar;i++){
    reg[i] = registers[i];
  }
#endif
__m512i InstrArg64(const OP code, const retval registers[])
{
        if(code>IntRangeEnd){
                const int x = InstrArg(code);
                return _mm512_loadu_epi8(&registers[x*npar]);
        }
        else {
                return _mm512_set1_epi8(code);
        }
}
__m512i InstrReg64(const OP code, const retval registers[])
{
        const int x = InstrReg(code);
        return _mm512_loadu_epi8(&registers[x*npar]);
}

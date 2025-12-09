int InstrArg(const OP code)
{
        const int x = code-IntRangeEnd-1;
        return x;
}
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
__m512i InstrReg16(const OP code, const uint8_t registers[], const int j)
{
        const int x = InstrReg(code);
        const __m128i a    = _mm_loadu_epi8(&registers[x*npar+j]);
        const __m512i aa   = _mm512_cvtepi8_epi32(a);
        const __m512i zero = _mm512_set1_epi32(0);
        const __mmask64 k  = 0xeeeeeeeeeeeeeeee;
        return _mm512_mask_blend_epi8(k, aa, zero);
}
__m512i InstrReg32(const OP code, const uint8_t registers[], const int j)
{
        const int x = InstrReg(code);
        const __m256i a    = _mm256_loadu_epi8(&registers[x*npar+j]);
        const __m512i aa   = _mm512_cvtepi8_epi16(a);
        const __m512i zero = _mm512_set1_epi32(0);
        const __mmask64 k  = 0xaaaaaaaaaaaaaaaa;
        return _mm512_mask_blend_epi8(k, aa, zero);
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
    const __m128i c          = _mm512_cvtepi32_epi8(result_vec);
    _mm_store_si128((__m128i*)&registers[Instr[i][0]*npar+j], c);
  }
  } else {
    switch(Instr[i][2])
      {
      case plus_op:{
        const __m512i a = InstrReg64(Instr[i][1],registers);
        const __m512i b = InstrArg64(Instr[i][3],registers);
        __m512i c = _mm512_add_epi8 (a, b);
        _mm512_store_epi32((__m512i*)&registers[Instr[i][0]*npar],c);
        break;
      }
      case minus_op:{
        const __m512i a = InstrReg64(Instr[i][1],registers);
        const __m512i b = InstrArg64(Instr[i][3],registers);
        __m512i c = _mm512_sub_epi8 (a, b);
        _mm512_store_epi32((__m512i*)&registers[Instr[i][0]*npar],c);
        break;
      }
      case mul_op:{
        for(int j=0;j<npar;j+=32){
          const __m512i a  = InstrReg32(Instr[i][1],registers,j);
          const __m512i b  = InstrArg32(Instr[i][3],registers,j);
          const __m512i c  = _mm512_mullo_epi16(a, b);
          const __m256i cc = _mm512_cvtepi16_epi8(c);
          _mm256_storeu_si256((__m256i_u*)&registers[Instr[i][0]*npar+j],cc);
        }
        break;
      }
      }
  }
 }
}

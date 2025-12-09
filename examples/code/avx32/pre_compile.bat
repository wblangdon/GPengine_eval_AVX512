#WBL 7 October 2025 compile code for AVX Magpie experiments
# to speed up GPengine interpreter

#Modifications:
#WBL 15 Nov 2025 something odd try eval_perf.cpp -g
#WBL 13 Nov 2025 support for compile.bat r1.18a

echo $0 ' $Revision: 1.2 $ ' "start $1 $HOST" `pwd` `date -Ins`

g++ -g -c -Dregtype8  -Dregtype=uint8_t  -fmax-errors=1 -fpermissive -fconcepts -march=skylake-avx512 -DAVX eval_perf.cpp -o eval_perf_8.o
if($status) exit $status

g++ -O3 -c -Dregtype8  -Dregtype=uint8_t  -fmax-errors=1 -fpermissive -fconcepts -march=skylake-avx512 -DAVX eval_h.cpp -o eval_h_8.o
if($status) exit $status

g++ -g -c -Dregtype16 -Dregtype=uint16_t -fmax-errors=1 -fpermissive -fconcepts -march=skylake-avx512 -DAVX eval_perf.cpp -o eval_perf_16.o
if($status) exit $status

g++ -O3 -c -Dregtype16 -Dregtype=uint16_t -fmax-errors=1 -fpermissive -fconcepts -march=skylake-avx512 -DAVX eval_h.cpp -o eval_h_16.o
if($status) exit $status

g++ -g -c -Dregtype32 -Dregtype=uint32_t -fmax-errors=1 -fpermissive -fconcepts -march=skylake-avx512 -DAVX eval_perf.cpp -o eval_perf_32.o
if($status) exit $status

g++ -O3 -c -Dregtype32 -Dregtype=uint32_t -fmax-errors=1 -fpermissive -fconcepts -march=skylake-avx512 -DAVX eval_h.cpp -o eval_h_32.o
if($status) exit $status

echo "$0 done" `date -Ins`



#WBL 7 October 2025 compile code for AVX Magpie experiments
# to speed up GPengine interpreter

#Modifications:
#WBL 18 Nov 2025 Remove code not used by regtype8
#WBL 15 Nov 2025 Add -g on link. Check no -O3, if ok compile again with -O3
#WBL 13 Nov 2025 for my 512 rewrite -Dreg {uint8_t,uint16_t,uint32_t}[uint8_t]
#WBL  7 Nov 2025 Add -DAVX
#WBL  6 Nov 2025 Add display MAGPIE_COUNTER MAGPIE_PATCH
#WBL  3 Nov 2025 Can we fixup common compilation errors?
#WBL 13 Oct 2025 Switch to eval
#WBL  9 Oct 2025 check have not changed feedstock XML, but changed something
#WBL  8 Oct 2025 Added __m256i AVX
#WBL  7 Oct 2025 based on tsd-extra/compile.bat, div_perf.make r1.2
#   malloc_extras/fit.bat r1.73

setenv start `pwd`
setenv test_srt `gawk -v dir="$start" '\
END{s=index(dir,"/_magpie_work/");\
if(s==0){print "../test.srt"; exit}\
split(substr(dir,s+1),t,"/");\
printf("%s_magpie_logs/%s_test.srt\n",substr(dir,1,s),t[2])}' /dev/null`

if(!(-e $test_srt)) cp /dev/null $test_srt

echo $0 ' $Revision: 1.24 $ ' "start $1 $HOST" `pwd` `date -Ins` >> $test_srt
echo "MAGPIE_PATCH($MAGPIE_COUNTER) $MAGPIE_PATCH" >> $test_srt

if(-e ../__original__) then
diff ../__original__ .  >> $test_srt
setenv save $status
if($save > 1) then
  echo "error on diff dir status $save"      >> $test_srt
  exit 10
endif
endif

setenv comp "g++ -c -fmax-errors=1 -fpermissive -fconcepts -march=skylake-avx512 -DAVX eval_h.cpp"
setenv link "g++ -g -o eval_perf.exe"

if( $1 == "" || $1 == "regsize8" ) then
  setenv eval eval_h_8
  setenv compile "$comp -g  -o $eval.o -Dregtype8  -Dregtype=uint8_t "
  setenv comp_O3 "$comp -O3 -o $eval.o -Dregtype8  -Dregtype=uint8_t"
  setenv link    "$link $eval.o eval_perf_8.o"
else if( $1 == "regsize16" ) then
  setenv eval eval_h_16
  setenv compile "$comp -g  -o $eval.o -Dregtype16 -Dregtype=uint16_t"
  setenv comp_O3 "$comp -O3 -o $eval.o -Dregtype16 -Dregtype=uint16_t"
  setenv link    "$link $eval.o eval_perf_16.o"
else if( $1 == "regsize32" ) then
  setenv eval eval_h_32
  setenv compile "$comp -g  -o $eval.o -Dregtype32 -Dregtype=uint32_t"
  setenv comp_O3 "$comp -O3 -o $eval.o -Dregtype32 -Dregtype=uint32_t"
  setenv link    "$link $eval.o eval_perf_32.o"
else
  echo "$0 unknown input $1" >> $test_srt
  echo "$0 unknown input $1"
  exit 99
endif

rm -f $eval.o > /dev/null
rm -f eval_perf.exe > /dev/null

#not -DNDEBUG
#no fixup

echo "$compile" >> $test_srt
      $compile >>& $test_srt
if($status) exit $status;

#ls -ltr >> $test_srt
echo $link >>  $test_srt
     $link >>& $test_srt
setenv save $status

echo -n "Check without O3 " >>  $test_srt
eval_perf.exe >>& $test_srt
setenv save $status
if($save) then
  echo "error on eval_perf.exe status $save" >> $test_srt
  exit $save
endif

rm -f $eval.o > /dev/null
rm -f eval_perf.exe > /dev/null

echo "$comp_O3" >> $test_srt
      $comp_O3 >>& $test_srt
if($status) exit $status;

if(-e ../__original__) then
diff -a ../__original__/$eval.o . >& /dev/null  
setenv save $status
if($save > 1) then
  echo "error on diff $eval.o $MAGPIE_COUNTER status $save"  >> $test_srt
  exit $save
endif
if($save == 0) then
  if($MAGPIE_COUNTER != "WARM") then
    echo "$eval.o $MAGPIE_COUNTER not changed"               >> $test_srt
    exit 1
  endif
endif # eval_h.o not changed
endif

echo "O3 link $link" >>  $test_srt
     $link >>& $test_srt
setenv save $status

echo "$0 done $MAGPIE_COUNTER, status $save" `date -Ins`     >> $test_srt
exit $save


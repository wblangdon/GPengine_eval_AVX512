#WBL 8 June 2023 just run test.seq and check its output

#Modifications:
#WBL  6 Nov 2025 add MAGPIE_COUNTER
#WBL 13 Oct 2025 Switch to eval
#WBL  7 Oct 2025 for AVX diveval
#WBL  3 Jun 2025 increase length, add thrsh (cf r1.71)
#WBL 19 May 2025 for rand_malloc.c revert to r1.51 clang
#WBL 28 Mar 2025 for clang test case from MySQL
#WBL 27 Mar 2025 for gcc test case from MySQL
#WBL 21 Feb 2025 bugfix missed one ../test.srt
#                Replace timeout with limit cpu (still have Magpie timeouts)
#WBL 20 Feb 2025 Back fit r1.43 z3 fixes for gem5 (cf r1.36,r1.37)
#WBL 19 Feb 2025 (for when run by magpie) move ../test.srt so not deleted at end
#WBL 16 Feb 2025 Bugfix fit. For z3 CMA-Es allow to continue on massif status 1
#WBL 15 Feb 2025 Allow to continue on massif status 101, remove additional debug
#WBL 15 Feb 2025 for z3 reuse gem5 fit.bat r1.36
#WBL  2 Sep 2024 rm $G.out0_, for CMA-ES send output to stdout not stderr
#WBL 29 Aug 2024 trap more gem5 variable output, allow mutant to continue on timeout
#WBL 28 Aug 2024 Avoid using /tmp/ so can have multiple simultaneous runs
#    on ERROR stop magpie
#WBL 27 Aug 2024 tidy
#WBL 22 Jul 2024 Allow omiting M_MMAP_MAX_tune etc
#WBL 16 Jul 2024 For debug/demo replace mallopt by show output of getenv
#WBL 11 Jul 2024 Bugfix use mmax+mmap not max, cf r1.25
#WBL  9 Jul 2024 Extract fitness from malloc_stats, vary only PARAMS
#WBL 20 Aug 2023 trw without turning on any DPRINTF
#WBL 18 Aug 2023 Replace external perf by test_prog.c
#WBL 17 Aug 2023 HACK use c1.bat's gem5_m_c1 as reference
#WBL 12 Aug 2023 Try to reduced noise by putting test.bat(r1.6) test_1.bat(r1.7) inline here
#   and making files local
#WBL 10 Aug 2023 increase ntests from 3 to 11
#WBL  8 Aug 2023 rewrite rnafold_extras/fit.bat r1.8 for gem5
#WBL  1 Jul 2023 allow any number of parameters
#WBL 27 Jun 2023 format for Magpie perf_instructions
#WBL  9 Jun 2023 Add ntests
#WBL  8 Jun 2023 Give bad fitness if fails on any test

setenv start `pwd`
setenv test_srt `gawk -v dir="$start" '\
END{s=index(dir,"/_magpie_work/");\
if(s==0){print "../test.srt"; exit}\
split(substr(dir,s+1),t,"/");\
printf("%s_magpie_logs/%s_test.srt\n",substr(dir,1,s),t[2])}' /dev/null`

if(!(-e $test_srt)) cp /dev/null $test_srt 

rm -f perf.out > /dev/null

eval_perf.exe >& perf.out
#div_perf.exe >& perf.out
setenv save $status

cat perf.out >> $test_srt
grep MAGPIE_FITNESS: perf.out

echo "$0 done $MAGPIE_COUNTER," `grep MAGPIE_FITNESS: perf.out` "status $save" `date -Ins` >> $test_srt
exit $save


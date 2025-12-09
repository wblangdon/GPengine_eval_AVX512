#WBL 9 July 2024
#https://github.com/bloa/magpie

#Modifications:
#WBL 30 Sep 2025 For AVX version of info_lgp/GPengine.cpp (magpie 30 Sep 2025)
#WBL 19 May 2025 For rand_malloc.c
#WBL  1 Apr 2025 For redis
#WBL 28 Mar 2025 For clang
#WBL 27 Mar 2025 For gcc
#WBL 19 Feb 2025 For new (my hack) Magpie use Python-3.11.5
#WBL 15 Feb 2025 rewrite for z3 (based on magpie/gem5/mallopt_1.bat Jul 10  2024
#WBL  6 Sep 2023 check /tmp/no_ok_cc_mutant.flag

echo "$0 start $HOST" `pwd` `date`

setenv PATH /opt/Python/Python-3.11.5/bin:"$PATH"
echo $PATH
gcc --version
ldd --version
python3 --version

echo "Magpie local_search:\n"

python3 -m magpie.bin.local_search --scenario examples/scenario/avx.txt


echo "$0 done, status $status" `date`
#

Example of using Magpie to speedup the performance critical interpret of GPengine (Peter Nordin's) linear genetic programming system. Here set up with just the usual four arithmetic operations (+ - times and protected division) operating on unsigned char data types for extended evolution on the Mackey-Glass chaotic time series.

The C++ interpreter's eval function has been extracted from GPengine and modified to operator on 64 test cases simultaneously using Intel's 512 bit AVX instruction set.

Speedup is measured by Linux' perf instruction count. Which Magpie's local search tries to minify.

Asummes new GCC g++ compiler (here 11.5.0) Assumes new Python (here 3.11.5) AVX instruction set (your hardware must support at least -march=skylake-avx512). Assumes Magpie (5 Nov 2025) https://github.com/bloa/magpie

It is recommened that you create examples/avx_extra to hold the development versions of the files in the examples/avx directory.

For simplicity duplicate directories are provided (32) for 8,16,32 bit experiments and for only 8-bit registers (the second experiment, which is the default).
If you wish to run the first more complicated experiment, adjust run.bat to use scenario32 etc.
and also run pre_compile.bat in examples/code/avx32 to create eval_perf_8.o and eval_h_8.o
(This step is also needed if you are not using g++ 11.5.0

```
@Misc{langdon:2025:eval_avx512,
  author =       "William B. Langdon",
  title =        "Improving a Parallel {C++ Intel AVX-512 SIMD} Linear
                 Genetic Programming Interpreter",
  howpublished = "ArXiv",
  year =         "2025",
  month =        "9 " # dec,
  keywords =     "genetic algorithms, genetic programming, genetic
                 improvement, linear GP, LGP, GI, SBSE, computer program
                 tuning, MMX, transplantation, testing interpreters,
                 test output distribution, non-functional GI, LUT,
                 entropy, dogfooding",
  URL =          "https://arxiv.org/abs/2512.09157",
  size =         "12 pages",
  abstract =     "We extend recent 256 SSE vector work to 512 AVX giving
                 a four fold speedup. We use MAGPIE (Machine Automated
                 General Performance Improvement via Evolution of
                 software) to speedup a C++ linear genetic programming
                 interpreter. Local search is provided with three
                 alternative hand optimised codes, revision history and
                 the Intel 512 bit AVX512VL documentation as C++ XML.
                 Magpie is applied to the new Single Instruction
                 Multiple Data (SIMD) parallel interpreter for Peter
                 Nordin's linear genetic programming GPengine. Linux
                 mprotect sandboxes whilst performance is given by perf
                 instruction count. In both cases, in a matter of hours
                 local search reliably sped up 114 or 310 lines of
                 manually written parallel SIMD code for the Intel
                 Advanced Vector Extensions (AVX) by 2 percent.",
  notes =        "new 512bit version of \cite{langdon:2026:GI}",
}
```

TODO....
This work will be presented at GI@ICSE 2026, 12 April, Rio (Brazil) https://geneticimprovementofsoftware.com/events/icse2026

@InProceedings{Langdon:2026:GI,
    author = "William B. Langdon and Carol Hanna",
    title = "Improving a Parallel {C++ Intel SSE SIMD} Linear Genetic Programming Interpreter",
    booktitle = "15th International Workshop on Genetic Improvement @ICSE 2026",
    year = "2026",
    editor = "Aymeric Blot and Oliver Krauss",
    address = "Rio de Janeiro",
    month = "12 April",
    url = {https://geneticimprovementofsoftware.com/events/icse2026},
    code_url = "https://github.com/wblangdon/rand_malloc",
    size = "8 pages",
}

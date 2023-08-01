COMPILER=icc
FFLAGS=-O2 -xCORE-AVX2
#FFLAGS=-g
#FFLAGS=-O3 -ipo -xCORE-AVX512 -qopt-zmm-usage=high
BATCH=sbatch job.sh

#COMPILER=pgcc
#FFLAGS=-g -tp barcelona-64
#FFLAGS=-O3 -fast -tp barcelona-64
#BATCH=qsub job.sge TG-TRA120006

# Choose the LAPACKLIB based on the compiler and machine.
# For icc, try mkl.
# For pgi (on lonestar only), try llapack or lacml or lacml_mp.

LAPACKLIB=mkl

.PHONY: all list submit clean package

COMMONFILES=options.c options.h flag.h la_flag.h
NRFILES=nr.c nrutil.c nrutil.h
GSLFILES=gsl.c
LAPACKFILES=lapack.c

all: nr gsl $(LAPACKLIB)

flag.h: Makefile
	echo 'const char* g_flag="$(COMPILER) $(FFLAGS)";' > flag.h

la_flag.h: Makefile
	echo 'const char* g_flag="$(COMPILER) $(FFLAGS) -$(LAPACKLIB)";' > la_flag.h

nr: $(NRFILES) $(COMMONFILES)
	$(COMPILER) $(FFLAGS) nr.c nrutil.c options.c -o nr

gsl: $(GSLFILES) $(COMMONFILES)
	$(COMPILER) $(FFLAGS) gsl.c options.c -I$(TACC_GSL_INC) -L$(TACC_GSL_LIB) -lgsl -lgslcblas -lm -o gsl


mkl: $(LAPACKFILES) $(COMMONFILES)
	$(COMPILER) $(FFLAGS) -DLALIB lapack.c options.c -mkl -o lapack

mkl_lonestar: $(LAPACKFILES) $(COMMONFILES)
	$(COMPILER) $(FFLAGS) -DMKL_ILP64 -Wl,-rpath,$(TACC_MKL_LIB) -I$(TACC_MKL_INC) -L$(TACC_MKL_LIB) lapack.c options.c -Wl,--start-group -lmkl_intel_ilp64 -lmkl_sequential -lmkl_core -Wl,--end-group -lpthread -o lapack

llapack: $(LAPACKFILES) $(COMMONFILES)
	$(COMPILER) $(FFLAGS) -DLALIB lapack.c options.c -llapack -o lapack -pgf77libs -pgf90libs -lblas

lacml: $(LAPACKFILES) $(COMMONFILES)
	$(COMPILER) $(FFLAGS) -DLALIB -fpic -Mcache_align lapack.c options.c -I$(TACC_ACML_INC) -Wl,-rpath,$(TACC_ACML_LIB) -L$(TACC_ACML_LIB) -lacml -o lapack

lacml_mp: $(LAPACKFILES) $(COMMONFILES)
	$(COMPILER) $(FFLAGS) -DLALIB -fpic -Mcache_align lapack.c options.c -I$(TACC_ACML_INC_MP) -Wl,-rpath,$(TACC_ACML_LIB_MP) -L$(TACC_ACML_LIB) -lacml_mp -o lapack

count:
	@echo nr `cat $(NRFILES) $(COMMONFILES) | wc -l`
	@echo gsl `cat $(GSLFILES) $(COMMONFILES) | wc -l`

list:
	find . -type f -perm +100 -size +10k -exec {} -f \;

submit:
	$(BATCH)

clean:
	-rm -f *~ *.o ludecomp.o* core.*
	-find . -type f -perm /100 -size +20k -exec rm {} \;

distclean:
	make clean
	-rm -f *.txt *flag.h

package:
	make distclean
	cd ..; rm -f ludecomp.tgz; tar zcvf ludecomp.tgz ludecomp


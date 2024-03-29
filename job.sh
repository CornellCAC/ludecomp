#!/bin/bash
#SBATCH -J ludecomp        # job name
#SBATCH -o ludecomp.o%j    # output/error file (%j = jobID)
#SBATCH -N 1               # number of nodes required
#SBATCH -n 1               # number of MPI tasks requested
#SBATCH -p development     # queue (partition)
#SBATCH -t 00:05:00        # run time (hh:mm:ss)

date
env|sort>variables.txt
module load gsl
set -x

export MATRIX=3000
#export OMP_NUM_THREADS=56
OUT=t$SLURM_JOB_ID.txt

echo Shell is $SHELL
count=0
for lib in nr gsl lapack
do
  /usr/bin/time -f '%e' ./$lib -n $MATRIX 1> $OUT 2> nrtime.txt
  if test $? -eq 0
  then
    export RUNTIME=`cat nrtime.txt`
    # The application is compiled so that -f shows the compiler flags.
    echo $RUNTIME `./$lib -f` >> results.txt
    # (The next several lines pertain only to live workshops at Cornell.)
    # export COMPILER=`./$lib -f|cut -d' ' -f2-|sed "s/ /%20/g"`
    # The django web page needs a unique job id when it records results, but we
    # run several timings per job, so we pad the actual job ID with $count.
    # curl -G -d user=$USER -d jobid=$SLURM_JOB_ID$count -d run_time=$RUNTIME -d compiler=$COMPILER -d library=$lib -d arguments=$MATRIX http://consultrh5.cac.cornell.edu/intro_to_ranger/
  fi
  count=$((count+1))
done

date
hostname

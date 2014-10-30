#include <stdio.h>
#include <stdlib.h>

#include "options.h"

/* update ey45@cornell.edu 052010 */

// Lapack LU decomposition is
// DGETRF( M, N, A, LDA, IPIV, INFO )
// M x N matrix, A.
// LDA is leading dimension of A. LDA >=max(1,M)
// IPIV (output) The pivot indices for 1 to min(M,N)
// info 0 on success, <0 if ith arg had illegal value,
// info >0 if U(i,i) is exactly zero.

typedef size_t lapack_int;
     
int main (int argc, char** argv)
{
  int rhs_idx;
  lapack_int n = 1000;
  lapack_int lda = 0;
  lapack_int lu_ok = 0;
  lapack_int solve_ok = 0;
  size_t ai, aj, bi;
  double* a_data = 0;
  double* b_data = 0;
  lapack_int* ipiv = 0;
  size_t sizen = 0;
  lapack_int ldb = 0;
  lapack_int nrhs = 1;
  char trans = 'N';
  lapack_int lwork;
  double* work;
  char* output_file = 0;
  FILE* fout = 0;

  options(argc, argv, &sizen, &output_file);

  n = (lapack_int) sizen;
  lda = n;
  ldb = n;
  lwork = n;

  a_data = malloc(sizeof(double*)*n*n);
  b_data = malloc(sizeof(double)*n);
  ipiv   = malloc(sizeof(lapack_int)*n);
  work   = malloc(sizeof(double)*lwork);

  srand(42);
  for (ai=0;ai<n;ai++) {
    for (aj=0;aj<n;aj++) {
      /* NOTICE: fortran and C use different ordering, one of them is col major the other row major */
      a_data[ai+aj*n] = rand();
    }
  }
  for (bi=0;bi<n;bi++) {
    b_data[bi]=rand();
  }

  dgetrf_(&n, &n, a_data, &lda, ipiv, &lu_ok);

  dgetrs_(&trans, &n, &nrhs, a_data, &lda, ipiv, b_data, &ldb, &solve_ok);

  if (output_file) {
    fout = fopen(output_file,"w");
    for (rhs_idx=0; rhs_idx<n; rhs_idx++) {
      /* NOTICE: the output is the updated right hand side */
      fprintf(fout, "%g\n", b_data[rhs_idx]);
    }
    free(output_file);
  }
     
  free(a_data);
  free(b_data);
  return 0;
}

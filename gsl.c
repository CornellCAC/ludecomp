#include <stdio.h>
#include <stdlib.h>
#include <gsl/gsl_linalg.h>
#include "options.h"

     
int main (int argc, char** argv)
{
  int n = 1000;
  size_t ai, aj, bi;
  double* a_data = 0;
  double* b_data = 0;
  size_t sizen = 0;
  char* output_file = 0;
  FILE* fout = 0;

  options(argc, argv, &sizen, &output_file);

  n = (int) sizen;

  a_data = malloc(sizeof(double)*n*n);
  b_data = malloc(sizeof(double)*n);

  srand(42);
  for (ai=0;ai<n;ai++) {
    for (aj=0;aj<n;aj++) {
      a_data[ai*n+aj] = rand();
    }
  }
  for (bi=0;bi<n;bi++) {
    b_data[bi]=rand();
  }

  gsl_matrix_view m 
    = gsl_matrix_view_array (a_data, n, n);
     
  gsl_vector_view b
    = gsl_vector_view_array (b_data, n);
     
  gsl_vector *x = gsl_vector_alloc (n);
       
  int s;
     
  gsl_permutation * p = gsl_permutation_alloc (n);
     
  gsl_linalg_LU_decomp (&m.matrix, p, &s);
     
  gsl_linalg_LU_solve (&m.matrix, p, &b.vector, x);
       
  if (output_file) {
    fout = fopen(output_file,"w");
    gsl_vector_fprintf (fout, x, "%g");
    free(output_file);
  }
     
  gsl_permutation_free (p);
  gsl_vector_free (x);

  free(a_data);
  free(b_data);
  return 0;
}

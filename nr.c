#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "nrutil.h"
#include "options.h"
#define TINY 1.0e-20

void ludcmp(double **a, int n, int* indx, double* d)
{
  int i, imax, j, k;
  double big,dum,sum,temp;
  double *vv;

  vv = dvector(1,n);
  *d=1.0;
  for (i=1;i<=n;i++) {
    big = 0.0;
    for (j=1;j<=n;j++)
      if ((temp=fabs(a[i][j])) > big) big=temp;
    if (big==0.0) nrerror("Singular matrix in routine ludcmp");
    vv[i]=1.0/big;
  }
  for (j=1;j<=n;j++) {
    for (i=1;i<j;i++) {
      sum=a[i][j];
      for (k=1;k<i;k++) sum -= a[i][k]*a[k][j];
      a[i][j]=sum;
    }
    big = 0.0;
    for (i=j;i<=n;i++) {
      sum=a[i][j];
      for (k=1;k<j;k++)
	sum -= a[i][k]*a[k][j];
      a[i][j]=sum;
      if ( (dum=vv[i]*fabs(sum)) >= big) {
	big = dum;
	imax = i;
      }
    }
    if (j!= imax) {
      for (k=1;k<=n;k++) {
	dum = a[imax][k];
	a[imax][k]=a[j][k];
	a[j][k]=dum;
      }
      *d = -(*d);
      vv[imax]=vv[j];
    }
    indx[j]=imax;
    if (a[j][j] == 0.0) a[j][j] = TINY;
    if (j!=n) {
      dum = 1.0/(a[j][j]);
      for (i=j+1;i<=n;i++) a[i][j] *= dum;
    }
  }
  free_dvector(vv,1,n);
}
    


void lubksb(double **a, int n, int* indx, double b[])
{
  int i,ii=0,ip,j;
  double sum;
  
  for (i=1; i<=n; i++){
    ip = indx[i];
    sum = b[ip];
    b[ip]=b[i];
    if (ii) {
      for (j=ii;j<=i-1;j++) {
	sum -= a[i][j]*b[j];
      }
    } else if (sum) {
      ii = i;
    }
    b[i] = sum;
  }
  for (i=n; i>=1; i--) {
    sum =b[i];
    for (j=i+1; j<=n;j++) sum -= a[i][j]*b[j];
    b[i]=sum/a[i][i];
  }
}


int main(int argc, char** argv)
{
  double **a, *b, d;
  int n, *indx;
  size_t i,j,bIdx,sizen;
  char* output_file;
  FILE* fout = 0;
  
  options(argc, argv, &sizen, &output_file);

  n = (int) sizen;

  a = dmatrix(1,n, 1,n);
  b = dvector(1, n);
  indx = ivector(1,n);

  srand(42);
  for (i=1;i<=n; i++) {
    for (j=1;j<=n; j++) {
      a[i][j] = rand();
    }
  }
  for (bIdx=1; bIdx<=n; bIdx++) {
    b[bIdx] = rand();
  }
  
  ludcmp(a,n,indx,&d);
  lubksb(a,n,indx,b);

  if (output_file) {
    fout = fopen(output_file,"w");
    for (bIdx=1;bIdx<=n; bIdx++) {
      fprintf(fout,"%g\n",b[bIdx]);
    }
    free(output_file);
  }

  free_ivector(indx,1,n);
  free_dvector(b, 1, n);
  free_dmatrix(a, 1,n, 1,n);
  return 0;
}

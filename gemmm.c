#include "myhead.h"

void gemmm(m,k,n,a,lda,b,ldb,c,ldc)
int m,k,n,lda,ldb,ldc;
float *a,*b,*c;
{
   int i,j,l;
   for(i=0;i<m;i++) 
      for(j=0;j<n;j++)
         for(l=0;l<k;l++)
            c[i*ldc+j] += a[i*lda+l]*b[l*ldb+j];
   return; 
}


void typemat(m,n,lda,newtp)
int m,n,lda;
MPI_Datatype *newtp;
{
   MPI_Type_vector(m,n,lda,MPI_FLOAT,newtp);
   return;
}


void scopy(m,n,a,lda,b,ldb)
int m,n,lda,ldb;
float *a,*b;
{
   int i,j;
   for(i=0;i<m;i++)
      for(j=0;j<n;j++)
         b[i*ldb+j] = a[i*lda+j];

   return ;
}

void setinitab(p,myrow,mycol,m,k,n,a,lda,b,ldb)
int m,k,n,lda,ldb,p,myrow,mycol;
float *a,*b;
{
   int i,j,offsidea,offsideb;
   
   offsidea = m*myrow + k*mycol;
   offsideb = k*myrow + n*mycol;
   for(i=0;i<m;i++) 
      for(j=0;j<k;j++)
         a[i*lda+j] = i+j+offsidea;
   
   for(i=0;i<k;i++)
      for(j=0;j<n;j++)
         b[i*ldb+j] = 1.0 - 2.0 * ((i+j+offsideb)%2);

   return;
}

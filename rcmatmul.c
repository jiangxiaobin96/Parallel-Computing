#include "myhead.h"

void init_a(m,k,lda,a,iam)
int m,k,lda,iam;
float a[][lda];
{
   int i,j,offside;
   
   offside = iam * m;
   for(i=0;i<m;i++)
      for(j=0;j<k;j++)
         a[i][j] = offside + i + j;

   return ;
}

void init_b(k,n,ldb,b,iam)
int k,n,ldb,iam;
float b[][ldb];
{
   int i,j,offside;
   
   offside = iam*n;
   for(i=0;i<k;i++)
      for(j=0;j<n;j++)
         b[i][j] = 1.0 - 2.0 * ((i+j+offside)%2);
   
   return;
}

//串行矩阵乘
void matmul(m,k,n,lda,a,ldb,b,ldc,c)
int m,k,n,lda,ldb,ldc;
float a[][lda],b[][ldb],c[][ldc];
{
   int i,j,l;
  
   for(i=0;i<m;i++)
      for(j=0;j<n;j++) {
         c[i][j] = 0.0;
         for(l=0;l<k;l++)
            c[i][j] += a[i][l] * b[l][j];
      }
   return ;
}

void rcmatmul(comm,np,iam,m,k,n,lda,a,ldb,b,ldc,c,ldw,w)
MPI_Comm comm;
int np,iam,n,k,m,lda,ldb,ldc,ldw;
float a[][lda],b[][ldb],c[][ldc],w[][ldw];
{
   int i,front,next,l;
   MPI_Datatype rectb,rectw;
   MPI_Status st;

   MPI_Type_vector(k,n,ldb,MPI_FLOAT,&rectb);
   MPI_Type_vector(k,n,ldw,MPI_FLOAT,&rectw);
   MPI_Type_commit(&rectb);
   MPI_Type_commit(&rectw);
   l = iam*n;
   front = (np+iam-1)%np;
   next = (iam+1)%np;

   for(i=0;i<np-1;i++) {
      if(i%2 == 0) {
         matmul(m,k,n,lda,a,ldb,b,ldc,&c[0][l]);
         MPI_Sendrecv(b,1,rectb,front,1,w,1,rectw,next,1,comm,&st);
      }
      else {
         matmul(m,k,n,lda,a,ldw,w,ldc,&c[0][l]);
         MPI_Sendrecv(w,1,rectw,front,1,b,1,rectb,next,1,comm,&st);
      }
      l += n;
      if(l == np * n) l=0;
   }
   
   if( (np-1)%2 == 0 )
      matmul(m,k,n,lda,a,ldb,b,ldc,&c[0][l]);
   else
      matmul(m,k,n,lda,a,ldw,w,ldc,&c[0][l]);

   MPI_Type_free(&rectb);
   MPI_Type_free(&rectw);

   return;
}



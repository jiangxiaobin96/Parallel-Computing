#include "myhead.h"

void diagonal(m,n,lda,rect,newtp)
int m,n,lda;
MPI_Datatype *rect,*newtp;
{
   MPI_Datatype oldtype[2];
   int blklen[2];
   MPI_Aint displs[2];
   
    
   MPI_Type_vector(m,n,lda,MPI_FLOAT,rect);  //rect是一个2*3的矩阵
   oldtype[0] = *rect;
   oldtype[1] = *rect;
   blklen[0] = 1;
   blklen[1] = 1;
   displs[0] = 0;
   displs[1] = sizeof(float)*(m*lda+n);
   MPI_Type_struct(2,blklen,displs,oldtype,newtp);  

  return ;
}

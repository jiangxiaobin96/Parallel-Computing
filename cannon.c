#include "myhead.h"

void cannon(rowcom,colcom,p,myrow,mycol,m,k,n,a,lda,b,ldb,c,ldc,at,ldaw,bt,ldbw)
MPI_Comm rowcom,colcom;
int p,myrow,mycol,m,k,n,lda,ldb,ldc,ldaw,ldbw;
float *a,*b,*c,*at,*bt;
{
   int i,j,l;
   int front,next;
   MPI_Datatype btp,attp,bttp;
   MPI_Status st;
   
//   typemat(m,k,lda,&atp);
   typemat(k,n,ldb,&btp);
   typemat(m,n,ldaw,&attp);
   typemat(m,n,ldbw,&bttp);
//   MPI_Type_commit(&atp);
   MPI_Type_commit(&btp);
   MPI_Type_commit(&attp);
   MPI_Type_commit(&bttp);

   l = myrow;   //每一个进程只有一个myrow=mycol所以每一个进程只做一次
   front = (myrow-1+p) % p;
   next = (myrow+1) % p;
   for(i=0;i<m;i++)
      for(j=0;j<n;j++) 
          c[i*ldc+j]=0.0;
   for(i=0;i<p;i++) {
      if(mycol == l) scopy(m,k,a,lda,at,ldaw); 
      MPI_Bcast(at,1,attp,l,rowcom); //把A矩阵对角线行广播，在行通讯子里
      gemmm(m,k,n,at,ldaw,b,ldb,c,ldc);
      if(i == p-1) continue;
      MPI_Sendrecv(b,1,btp,front,1,bt,1,bttp,next,1,colcom,&st); //移动数据是在列通讯子里,通过front和next向上移动
      scopy(k,n,bt,ldbw,b,ldb);
      l = (l+1)%p;
   }
   return ; 
}

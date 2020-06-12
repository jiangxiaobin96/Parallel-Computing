#include "myhead.h"

void ring(m,n,comm,np,iam)
int m,*n,np,iam;
MPI_Comm comm;
{
  int front ,next;
  MPI_Status st,sts[2];
  MPI_Request reqs[2],sreq,rreq;

  front = (np+iam-1) % np;
  next = (iam+1) % np;

//形成一个环通信,并行效率高
/*
  if(iam % 2 == 0) {
    MPI_Send(&m,1,MPI_INT,next,1,comm);
    MPI_Recv(n,1,MPI_INT,front,1,comm,&st);
  }
  else {
    MPI_Recv(n,1,MPI_INT,front,1,comm,&st);
    MPI_Send(&m,1,MPI_INT,next,1,comm);
  }
*/

//有一个为空
/*
  if(iam == 0) front = MPI_PROC_NULL;
  if(iam == np - 1) next = MPI_PROC_NULL;
*/

//用合成函数，系统选择接收顺序，效率高
  MPI_Sendrecv(&m,1,MPI_INT,next,1,n,1,MPI_INT,front,1,comm,&st);
  //MPI_Recv(n,1,MPI_INT,front,1,comm,&st);
  
//非阻塞式，要与wait配合使用
 /*
  MPI_Isend(&m,1,MPI_INT,next,1,comm,&reqs[0]);
  MPI_Irecv(n,1,MPI_INT,front,1,comm,&reqs[1]);
  MPI_Waitalll(2,reqs,sts);
*/

//分别wait的方法
/*
  MPI_Isend(&m,1,MPI_INT,next,1,comm,&sreq);
  MPI_Irecv(n,1,MPI_INT,front,1,comm,&rreq);
  MPI_Wait(&sreq,&st);
  MPI_Wait(&rreq,&st);
*/

  return;
}

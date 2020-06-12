#include "myhead.h"

void proc2d(comm,np,iam,p,q,rowcom,colcom,rowid,colid)
MPI_Comm comm,*rowcom,*colcom;
int np,iam,p,q,*rowid,*colid;
{
   int color,key,pxq;
   MPI_Comm valcom;   

   pxq = p*q;
   if(np < pxq) return;
   
   if(iam < pxq) color = 0;
   else color = MPI_UNDEFINED;
   key = iam;
   MPI_Comm_split(comm,color,key,&valcom); //小于pxq的每一个进程都有一个新的通讯子valcom
   
   if( valcom == MPI_COMM_NULL) return;
   color = iam / q;
   key = iam;
   MPI_Comm_split(valcom,color,key,rowcom);

   color = iam % q;
   key = iam;
   MPI_Comm_split(valcom,color,key,colcom);
   
   MPI_Comm_rank(*colcom,rowid);
   MPI_Comm_rank(*rowcom,colid);

   return ;
}

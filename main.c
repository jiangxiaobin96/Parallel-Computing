#include "myhead.h"
//void ring(int m,int* n,MPI_Comm comm,int np,int iam);
#define maxup 16
void main(argc, argv)
int argc;
char **argv;
{
   MPI_Comm comm;
   int np, iam;
   int n,k,m,narray[100],marray[101],matr[11][25];
   MPI_Datatype newtp,rect;
   MPI_Status st;
   abc x[10];
   MPI_Aint sizeabc,extnewtp;
   int i,j,en,offside;
   float a[31][57],b[53][59],c[31][61],w[51][53],u[37][41];
   float rhs[31],xx[31];

   int rcounts[maxup];
   int displs[maxup];

   typedef struct{float a;int m;} floatint;
   floatint mxl,resmxl; 

   MPI_Comm rowcom,colcom;
   int rowid,colid,p;

   MPI_Group grp1,newgrp;
   int ranks[10],gnp,giam;

#define chkcannon
/* start the mip environment */
   mybegin(&argc,&argv,&comm,&np,&iam);
   
/* main body here */
#ifdef chkring
   m = iam;
   ring(m,&n,comm,np,iam);   
   printf("\nIn proc. %d! n=%d\n",iam,n);
#endif

#ifdef chkcannon
   p = 3;
   if(np < 9) return;
   proc2d(comm,np,iam,p,p,&rowcom,&colcom,&rowid,&colid);
   m = 11;
   k = 10;
   n = 12;
   if(iam < 9) {
      setinitab(p,rowid,colid,m,k,n,&a[0][0],57,&b[0][0],59);
      cannon(rowcom,colcom,p,rowid,colid,m,k,n,&a[0][0],57, \
             &b[0][0],59,&c[0][0],61,&w[0][0],53,&u[0][0],41);
      
      printf("\nc=%f,%f,%f,%f on Proc %d\n", \
          c[1][1],c[1][2],c[1][3],c[1][4],iam);
   }
#endif


#ifdef chkgroup
   MPI_Comm_group(comm,&grp1);
   ranks[0] = 1;  //把进程1放进0group中
   ranks[1] = 3;  //把进程3放进1group中
   MPI_Group_incl(grp1,2,ranks,&newgrp);
   MPI_Group_size(newgrp,&gnp);
   MPI_Group_rank(newgrp,&giam);
   printf("\nThe proc %d,group %d\n",iam,giam);
#endif

#ifdef chkiteration
   en = 5;
   n = en*np;
   for(i=0;i<n;i++) rhs[i] = i;
   offside = iam*en;
   for(i=0;i<n;i++)
      for(j=0;j<en;j++) {
         a[i][j] = 0.0;
         if(i == (j+offside)) a[i][j] = 0.5;
      }
   iteration(comm,np,iam,n,en,&a[0][0],57,rhs,xx,10);
   printf("\nx=%f,%f on Proc %d\n",xx[0],xx[1],iam);
#endif

#ifdef chksngl
   a[0][0] = iam + 1.0;
   snglscan(comm,iam,a[0][0],2,&b[0][0]);
   if(iam == 2) 
       printf("\nsum=%f\n",b[0][0]);
   MPI_Scan(&a[0][0],&b[0][0],1,MPI_FLOAT,MPI_SUM,comm);
   printf("\nEach proc value = %f, in %d\n",b[0][0],iam);
#endif

#ifdef chkproc2d
//   if(np < 12) return;
   proc2d(comm,np,iam,3,4,&rowcom,&colcom,&rowid,&colid);
   printf("\nProc %d = (%d,%d)\n",iam,rowid,colid);
#endif

#ifdef  chkreduce
   mxl.a = (iam+1)*20;
   mxl.m = iam;
   MPI_Allreduce(&mxl,&resmxl,1,MPI_FLOAT_INT,MPI_MAXLOC,comm);
   printf("\nA=%f,loc=%d in Proc %d\n",resmxl.a,resmxl.m,iam);
#endif

#ifdef chkmatmul
   //int bnp[3];
   m = 11;
   k = 45;
   n = 12;
   init_a(m,k,57,a,iam);
   init_b(k,n,59,b,iam);
   rcmatmul(comm,np,iam,m,k,n,57,a,59,b,61,c,53,w);
   printf("\nc=%f,%f,%f,%f on Proc %d\n", \
          c[1][1],c[1][2],c[1][3],c[1][4],iam);
   
#endif

#ifdef chkbcast
   if(iam == 0) {
      for(i=0;i<31;i++)
         for(j=0;j<57;j++)
            a[i][j] = i+j;
   }
   MPI_Bcast(&a[0][0],np*5,MPI_FLOAT,0,comm);
   printf("\nA=%f,%f,%f,%f,%f in Proc %d\n",a[0][0],a[0][1],a[0][2],a[0][3],a[0][4],iam);
   MPI_Scatter(&a[0][0],5,MPI_FLOAT,&a[1][0],5,MPI_FLOAT,1,comm);
   MPI_Allgather(&a[1][0],5,MPI_FLOAT,&a[2][0],5,MPI_FLOAT,comm);
   printf("\nA=%f,%f,%f,%f,%f in Proc %d\n",a[2][0],a[2][1],a[2][2],a[2][3],a[2][4],iam);
   MPI_Alltoall(&a[0][0],5,MPI_FLOAT,&a[1][0],5,MPI_FLOAT,comm);
   printf("\nA=%f,%f,%f,%f,%f in Proc %d\n",a[1][0],a[1][1],a[1][2],a[1][3],a[1][4],iam);

   MPI_Reduce(&a[1][0],&a[0][0],1,MPI_FLOAT,MPI_SUM,0,comm);
   printf("\nA=%f in proc %d\n",a[0][0],iam);
/*   
   for(i=0;i<31;i++)
         for(j=0;j<57;j++)
            a[i][j] = i+j;
   MPI_Gather(&a[0][0],3,MPI_FLOAT,&a[1][0],3,MPI_FLOAT,0,comm);//把其他进程的a00,a01,a02发到0进程的a10位置开始往后赋值
   printf("\nA=%f,%f,%f,%f,%f,%f\n", \
          a[1][0],a[1][1],a[1][2],a[1][3],a[1][4],a[1][5]);
   MPI_Scatter(&a[1][0],3,MPI_FLOAT,&a[0][0],3,MPI_FLOAT,0,comm);

   for(i=0;i<np;i++) {
      rcounts[i] = 3;
   int rcounts[maxup];
      displs[i] = i*5;
   }
   MPI_Gatherv(&a[0][0],3,MPI_FLOAT,&a[1][0],rcounts,displs,MPI_FLOAT,0,comm);
    printf("\nA=%f,%f,%f,%f,%f,%f in Proc %d\n", \
          a[1][0],a[1][1],a[1][2],a[1][5],a[1][6],a[1][7],iam);
*/
#endif

#ifdef chkdiagonal
   diagonal(2,3,57,&rect,&newtp);
   MPI_Type_commit(&rect);
   MPI_Type_commit(&newtp);
  
   if(iam == 0) {
      for(i=0;i<31;i++) 
         for(j=0;j<57;j++) 
            a[i][j] = i+j;
      MPI_Send(a,3,newtp,1,5,comm);
   }
   else if(iam == 1) {
      MPI_Recv(a,3,newtp,0,5,comm,&st);
      printf("MPI_UB=%d\n",MPI_UB);
      printf("\nA=%f,%f,%f,%f,%f,%f,%f,%f\n", \
             a[2][3],a[2][4],a[2][5],a[2][6], \
             a[1][3],a[1][4],a[1][5],a[1][6]);      
   }
   MPI_Type_free(&rect);
   MPI_Type_free(&newtp);
#endif

#ifdef chkstruct
   mpistruct(&newtp);
   MPI_Type_commit(&newtp);
   if(iam == 0) {
      for(m=0;m<10;m++) {
         x[m].a = m;
         x[m].b[0] = 20.0*(m+1);
         x[m].b[1] = 30.0*(m+1);
         x[m].c[0] = 'a'+3*m;
         x[m].c[1] = 'b'+3*m;
         x[m].c[2] = 'c'+3*m;
      }
      MPI_Send(x,4,newtp,1,5,comm);
      
      sizeabc = sizeof(abc);
      MPI_Type_extent(newtp,&extnewtp);
      printf("\nsizeof=%ld, and extent=%ld\n",sizeabc,extnewtp);
   }
   if(iam == 1) {
      MPI_Recv(x,4,newtp,0,5,comm,&st);
      printf("\nvalues are %d, %f, %f, %c, %c, %c \n", \
              x[0].a,x[0].b[0],x[0].b[1],x[0].c[0],x[0].c[1],x[0].c[2]);
      printf("\nvalues are %d, %f, %f, %c, %c, %c \n", \
              x[1].a,x[1].b[0],x[1].b[1],x[1].c[0],x[1].c[1],x[1].c[2]);
      printf("\nvalues are %d, %f, %f, %c, %c, %c \n", \
              x[2].a,x[2].b[0],x[2].b[1],x[2].c[0],x[2].c[1],x[2].c[2]);
      printf("\nvalues are %d, %f, %f, %c, %c, %c \n", \
              x[3].a,x[3].b[0],x[3].b[1],x[3].c[0],x[3].c[1],x[3].c[2]);
   }
   MPI_Type_free(&newtp);
#endif

#ifdef chkdatatype
  for(m=0;m<100;m++) narray[m] = m;
  datatype('i',&newtp);
  MPI_Type_commit(&newtp);
  if(iam == 0) {
    //for(m=0;m<10;m++)
      // for(n=0;n<25;n++) matr[m][n] = m+n;
    MPI_Send(narray,1,newtp,1,5,comm);
  }
  if(iam == 1) {
    MPI_Recv(marray,1,newtp,0,5,comm,&st);
    printf("\nData on Proc%d are %d, %d, %d, %d, %d\n", \
            iam,marray[0],marray[1],marray[2], \ 
            marray[5],marray[6]);
  }
  MPI_Type_free(&newtp);
#endif

/* terminate, exit mpi */
   myend();
   
}

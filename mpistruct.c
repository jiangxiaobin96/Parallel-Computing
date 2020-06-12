
#include "myhead.h"

//typedef struct{int a;float b[2];char c[3];} abc;

void mpistruct(newtp)
MPI_Datatype *newtp;
{
  abc s;
  MPI_Datatype oldtp[3];
  int blklen[3];
  MPI_Aint displs[3];
  
  MPI_Address(&s.a,&displs[0]);
  MPI_Address(&s.b[0],&displs[1]);
  MPI_Address(&s.c[0],&displs[2]);

  displs[1] -= displs[0];
  displs[2] -= displs[0];
  displs[0] = 0;

  blklen[0] = 1;
  blklen[1] = 2;
  blklen[2] = 3;

  oldtp[0] = MPI_INT;
  oldtp[1] = MPI_FLOAT;
  oldtp[2] = MPI_CHAR;

  MPI_Type_struct(3,blklen,displs,oldtp,newtp); 
}

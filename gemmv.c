/*
 * @Author: Xiaobin
 * @Date: 2020-06-11 11:59:16
 * @LastEditors: Xiaobin
 * @LastEditTime: 2020-06-11 20:38:18
 * @Description: 
 */ 
#include "myhead.h"

void gemmv(m,n,a,lda,x,y)
int m,n,lda;
float *a,*x,*y;
{
   int i,j;
   //y是向量
   //for(i=0;i<m;i++) y[i] = b[i];
   for(i=0;i<m;i++) 
      for(j=0;j<n;j++)
         y[i] += a[i*lda+j]*x[j];   //a[i][j]*x[j]

   return ;
}

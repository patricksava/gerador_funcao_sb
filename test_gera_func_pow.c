#include <math.h>
#include <stdio.h>
#include "gera_func.h"

typedef double (*func_ptr) (double x);
/*
double add (int x,int y) {
  return (double)(x+y);
}*/

int main (void) {
   Parametro params[2];
   func_ptr f_quadrado = NULL;
   double d;
   int i;

   params[0].tipo = CHAR_PAR;
   params[0].amarrado = 1;
   //params[0].posicao = 1;
   params[0].valor.v_int = 4;
   params[1].tipo = CHAR_PAR;
   params[1].amarrado = 1;
   params[1].valor.v_int = 2;

   f_quadrado = (func_ptr) gera_func (pow, 2, params);

   for (i = 1; i <= 10; i++) {
      d = f_quadrado(i);
      printf("%d ^ 2 = %lf\n", i, d);
   }

   libera_func(f_quadrado);
   return 0;
}
/*
#include<stdio.h> 
int add (int x) { 
   return x+1; 
} 

int main(){ 
   unsigned char codigo[] = {0x55,0x89,0xe5,0xff,0x75,0x08,0xe8,0xfc,0xff,0xff,0xff,0x89,0xec,0x5d,0xc3}; 
   int a = (int)&add - (int)&codigo[11]; 
   *((int*)&codigo[7]) = a; 
   typedef int (*funcp) (int x); 
   funcp f = (funcp)codigo; 
   int i = (*f)(10); 
   printf("%d \n",i); 
   return 0; 
}
*/
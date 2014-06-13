#include <stdio.h>
#include <string.h>
#include "gera_func.h"

int boo (int b,double a) {
  return a+b;
}

typedef int (*func_ptr) (int a,double b);

int main (void) {
  Parametro params[2];
  func_ptr foo = NULL;

  params[0].tipo = INT_PAR;
  params[0].amarrado = 0;
  params[0].posicao = 1;
  //params[0].valor.v_int = 4;
  params[1].tipo = DOUBLE_PAR;
  params[1].amarrado = 0;
  //params[1].valor.v_double = 1.0;
  params[1].posicao = 2;

  foo = gera_func (boo, 2, params);
  //a = 2; b = 3;
  printf ("boo() = %d\n",foo(1,1.0));
  //printf ("foo(%d,%d) = %d\n", a, b, foo(a,b));

  return 0;
}
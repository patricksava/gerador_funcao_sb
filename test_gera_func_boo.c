#include <stdio.h>
#include <string.h>
#include "gera_func.h"

int boo (int a) {
  return a;
}

typedef int (*func_ptr) (int a);

int main (void) {
  Parametro params[2];
  func_ptr foo = NULL;

  params[0].tipo = INT_PAR;
  params[0].amarrado = 0;
  params[0].posicao = 2;
  //params[0].valor.v_int = 4;
 /* params[1].tipo = DOUBLE_PAR;
  params[1].amarrado = 1;
  params[1].valor.v_double = 1.0;
  //params[1].posicao = 1;
*/
  foo = gera_func (boo, 1, params);
  //a = 2; b = 3;
  printf ("boo() = %d\n",foo(1));
  //printf ("foo(%d,%d) = %d\n", a, b, foo(a,b));

  return 0;
}
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "gera_func.h"

void* gera_func(void* f, int n, Parametro params[]){
	
	/*
		0x55 	  - push   %ebp
		0x89 0xe5 - mov    %esp,%ebp
		0x55 	  - push   %ebx
	*/	
	unsigned char code_header[] = {0x55,0x89,0xe5};
	/* 
		0x5d	   - pop    %ebx
		0x89 0xec  - mov    %ebp,%esp
    	0x5d	   - pop    %ebp
    	0xc3	   - ret    

	*/
	unsigned char code_footer[] = {0x89,0xec,0x5d,0xc3};
	/*
		0xe8 	  - call endereço(0,0,0,0)
	*/
	unsigned char code_call[] = {0xe8, 0, 0, 0, 0};

	unsigned char* code;
	unsigned char* code_initializer;
	unsigned char* code_finalizer;

	
	//para cada parametro tem um push (1 byte) e o endereco(4 bytes)
	code_initializer = (unsigned char*) malloc( n * 5 );
	code_finalizer = (unsigned char*) malloc( n );
	
	//o tamanho total do codigo sao os push com os enderecos e a cabeça e cauda de codigo
	code = (unsigned char*) malloc( (n * 5) + n + 12 );
	
	code_initializer[0] = 0;
	
	code[0] = 0;

	*((int*)&code_call[1]) = ((int)( (int)&f - (int)&code_call[5])); 

	// Gerar código de máquina conforme os parâmetros passados
	int i;
	for( i = 0; i < n; i++ ){

		if(params[i].amarrado){
			int x;
			unsigned char body[2];
			body[0] = 0x68; //pushl
			switch (params[i].tipo){
				case INT_PAR:
					*((int*)&body[1]) = params[i].valor.v_int;
					//pushl params[i].valor.v_int
					break;
				case PTR_PAR:
					*((int*)&body[1]) = params[i].valor.v_ptr;
					//pushl params[i].valor.v_ptr
					break;
				case CHAR_PAR:
					body[0] = 0x6A; //pushl
					body[1] = params[i].valor.v_char;
					//pushl params[i].valor.v_char
					break;
				case DOUBLE_PAR:
					//body[1] = params[i].valor.v_double;
					//pushl params[i].valor.v_double
					break;

				default:
					break;
			}

			memcpy(&code_initializer[i*2], body, 2);
			code_finalizer[i] = 0x5d;
		
		} 
		else {
			switch (params[i].tipo){
				case INT_PAR: //4 bytes
					// pushl 4+(4*posicao)(%ebp)
					break;
				case PTR_PAR: //4 bytes

					// pushl 4+(4*posicao)(%ebp)
					break;
				case CHAR_PAR: //1 byte
					// pushl 4+(posicao)(%ebp)
					break;
				case DOUBLE_PAR: //8 bytes
					// pushl 4+(4*posicao)(%ebp)
					// pushl 4+(4*(posicao+1))(%ebp)
					break;

				default:
					break;
			}
		}
		
		
	}

	memcpy(code, code_header, 3);
	memcpy(&code[ 3 ], code_initializer, n*5);

	memcpy(&code[ n*5 + 3 ], code_call,5);
	
	memcpy(&code[ n*5 + 8 ], code_finalizer, n);
	memcpy(&code[ n*5 + 8 + n ], code_footer,4);	

	//free(&code_initializer);

	return code;
}

void libera_func(void* func){
	if(func != NULL)
		free(func);
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
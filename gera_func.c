#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "gera_func.h"

void* gera_func(void* f, int n, Parametro params[]){
	
	/*
		0x55 	  - push   %ebp
		0x89 0xe5 - mov    %esp,%ebp
	*/	
	unsigned char code_header[] = {0x55,0x89,0xe5};
	/* 
		0x89 0xec  - mov    %ebp,%esp
    	0x5d	   - pop    %ebp
    	0xc3	   - ret    

	*/
	unsigned char code_footer[] = {0x89,0xec,0x5d,0xc3};
	/*
		0xe8 	  - call endereço(0,0,0,0)
	*/
	unsigned char code_call[] = {0xe8, 0xfc, 0xff, 0xff, 0xff};

	unsigned char* code;
	unsigned char* code_initializer;

	
	//para cada parametro tem um push (1 byte) e o endereco(4 bytes)
	code_initializer = (unsigned char*) malloc( n * 10 );
	
	//o tamanho total do codigo sao os push com os enderecos e a cabeça e cauda de codigo
	code = (unsigned char*) malloc( (n * 5) + 12 );
	int i;
	int qtdDinamicos = 0;
	int* pos_param = (int*) malloc( n * sizeof(int) );
	
	for(i = 0; i < n; i++){
		if(!params[i].amarrado){
			pos_param[params[i].posicao-1] = i;
			qtdDinamicos++;
		}
	}
	
	int* offset_param = (int*) malloc( qtdDinamicos * sizeof(int) );
	offset_param[0] = 8;
	for(i = 1; i < qtdDinamicos; i++){
		int type_offset = 4;
		if(params[pos_param[i-1]].tipo == DOUBLE_PAR)
			type_offset += 4;

		offset_param[i] = offset_param[i-1] + type_offset;
	}

		
	int body_tam = 0;
	int body_tam_delta = 0;
	// Gerar código de máquina conforme os parâmetros passados
	
	for( i = n-1; i >= 0; i-- ){
		unsigned char *body;
		if(	params[i].tipo == DOUBLE_PAR){
			if(params[i].amarrado){
				body_tam_delta = 10;
				body = (unsigned char*) malloc( body_tam_delta );
				unsigned int* pointer;
				pointer = &(params[i].valor.v_double);
				body[0] = 0x68; //pushl
				*((int*)&body[1]) = pointer[0];
				body[5] = 0x68; //pushl
				*((int*)&body[6]) = pointer[1];
			}
			else{
				body_tam_delta = 6;
				body = (unsigned char*) malloc( body_tam_delta );
				body[0] = 0xff; //pushl
				body[1] = 0x75; //pushl
				body[2] = offset_param[params[i].posicao-1];
				body[3] = 0xff; //pushl
				body[4] = 0x75; //pushl
				body[5] = offset_param[params[i].posicao-1] + 4;
			}
		}
		else{
			if(params[i].amarrado){
				body_tam_delta = 5;
				body = (unsigned char*) malloc( body_tam_delta );
				body[0] = 0x68; //pushl
				switch (params[i].tipo){
					case INT_PAR:
						//pushl params[i].valor.v_int
						*((int*)&body[1]) = params[i].valor.v_int;
						break;
					case PTR_PAR:
						//pushl params[i].valor.v_ptr
						*((int*)&body[1]) = params[i].valor.v_ptr;
						break;
					case CHAR_PAR:
						//pushl params[i].valor.v_char
						*((int*)&body[1]) = params[i].valor.v_char;
						break;
					default:
						break;
				}
			} 
			else {
				body_tam_delta = 3;
				body = (unsigned char*) malloc( body_tam_delta );
				body[0] = 0xff; //pushl
				body[1] = 0x75; //pushl
				body[2] = offset_param[params[i].posicao-1];
			}
		}
		memcpy(&code_initializer[body_tam], body, body_tam_delta);	
		body_tam += body_tam_delta;		
	}
	int tam = 0;
	int tam_code_call;
	memcpy(code, code_header, 3);
	tam += 3;
	memcpy(&code[ tam ], code_initializer, body_tam);
	tam += body_tam;
	memcpy(&code[ tam ], code_call,5);
	tam_code_call = tam + 1;
	tam += 5;
	memcpy(&code[ tam ], code_footer,4);

	int a = (int)f - (int)&code[tam];
	*((int*)&code[tam_code_call]) = a;	
	 

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
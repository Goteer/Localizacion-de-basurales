#ifndef __ORDENES_H__

#define __ORDENES_H__

#include "sitio.h"

void ordenes_ayuda();
error_t ordenes_salida(FILE* file_in,FILE* file_out,int muni);
int array_search(int* array,int arrayLength, int target);
char *cargar_linea(FILE* file_in,char *cadena);
#endif

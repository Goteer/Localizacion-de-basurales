#ifndef __ORDENES_H__

#define __ORDENES_H__

#include "sitio.h"

typedef struct {
    char sitios_id[8];
    double sitios_latitud;
    double sitios_longitud;
    int municipios_id;
	int suma;
    char municipios_descripcion[200];
} municipio_t;

void ordenes_ayuda();
error_t ordenes_salida(FILE* file_in,FILE* file_out,int muni);
int array_search(municipio_t* array , int arrayLength, int target);
char *cargar_linea(FILE* file_in,char *cadena);


#endif

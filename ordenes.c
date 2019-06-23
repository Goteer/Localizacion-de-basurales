#include <stdio.h>
#include <string.h>

void ordenes_ayuda(){

	puts("El programa admite los siguientes argumentos:\n -h|-a|--help|--ayuda: Muestra que argumentos admite el programa con una ayuda breve para cada uno.\n -l|--leer <archivo>: Carga el archivo CSV de nombre \"archivo\".\n -s|--salida <archivo>: Indica el nombre del archivo donde se generará la secuencia de puntos. Si se indica como archivo \"-\" o se omite este argumento (solo se escribe -s|-a|--help|--ayuda), la salida se envia a la salida estandar.\n -m|--municipio: Indica que se generara un unico punto por cada municipio.\n");

}

void ordenes_salida(FILE* file_in, FILE* file_out,int muni){

						//Reservo de antemano espacio para los string porque no puedo saber cuanto reservar a no ser que

						//Declaracion de variables requeridas para procesar el archivo
						//sitios_id = 0
						char sitios_denominacion[64];

						//sitios_direccion
						double sitios_latitud= 0; // latitud minima es -90.000000 (10 chars)
						double sitios_longitud= 0; // longitud minima es -180.000000 (11 chars)
						int municipios_id= 0;
						//municipios_descripcion;
						char sitios_tipologia[16];
						//sitios_estado;
						int scan_result;
						int muni_actual = -1;

						if (muni){
							//Solo imprimir uno por municipio
							puts("Work in progress. Exiting...\n");
						}else{
							//Imprimir todos
							while ((scan_result=fscanf(file_in,"%*[^,],%[^,],%*[^,],%lf,%lf,%d,%*[^,],%[^,],%*[^\n]",sitios_denominacion,&sitios_latitud,&sitios_longitud,&municipios_id,sitios_tipologia))!=EOF){
								if (scan_result>=5){
									if (strcmp(sitios_tipologia,"punto de arrojo")==0){
										fprintf(file_out,"%lf,%lf{%s: %s}<tan-dot>\n",sitios_latitud,sitios_longitud,sitios_tipologia,sitios_denominacion);
									}else{
										if(strcmp(sitios_tipologia,"microbasural")==0){
											fprintf(file_out,"%lf,%lf{%s: %s}<yellow-dot>\n",sitios_latitud,sitios_longitud,sitios_tipologia,sitios_denominacion);
										}else{
											if(strcmp(sitios_tipologia,"basural")==0){
												fprintf(file_out,"%lf,%lf{%s: %s}<orange-dot>\n",sitios_latitud,sitios_longitud,sitios_tipologia,sitios_denominacion);
											}else{
												fprintf(file_out,"%lf,%lf{%s: %s}<default-dot>\n",sitios_latitud,sitios_longitud,sitios_tipologia,sitios_denominacion);
											}
										}
									}
								}else{
									fscanf(file_in,"%*[^\n]"); //Si hubo error de lectura, pasar de la linea para poder continuar
								}
							}

						}
}

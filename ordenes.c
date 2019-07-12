#include <stdio.h>
#include <string.h>
#include "sitio.h"

void ordenes_ayuda(){

	puts("El programa admite los siguientes argumentos:\n -h|-a|--help|--ayuda: Muestra que argumentos admite el programa con una ayuda breve para cada uno.\n -l|--leer <archivo>: Carga el archivo CSV de nombre \"archivo\".\n -s|--salida <archivo>: Indica el nombre del archivo donde se generará la secuencia de puntos. Si se indica como archivo \"-\" o se omite este argumento (solo se escribe -s|-a|--help|--ayuda), la salida se envia a la salida estandar.\n -m|--municipio: Indica que se generara un unico punto por cada municipio.\n");

}
/*
error_t ordenes_salida_old(FILE* file_in, FILE* file_out,int muni){

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
									return ERROR_FALTAN_CAMPOS;
								}
							}

						}
						return OK;
}*/

int array_search(const int** array,int arrayLength, int target){

	int found = -1;
	for (int i=0;i<arrayLength && found==-1;i++){
		if (array[i][0] == target){
			found=i;
		}
	}
	return found;
}

error_t ordenes_salida(FILE* file_in, FILE* file_out,int muni){
	error_t err = OK;
	size_t limite = 2048; //CARGAR LINEA DE FORMA DINAMICA _________!!!!!!!!!!!!!!!!$()"/$=(/"/&)")"
	char linea[limite];
	sitio_t sitio;
	char color[12] = "";
	char tipo[16] = "";

	fgets(linea,limite,file_in);//Saco la primera linea que contiene los nombres de columna
	fgets(linea,limite,file_in);
	err = linea_csv_a_sitio_t(linea, &sitio);

	if (!muni){ //SI NO SE USO LA OPCION -m

		while (err == OK && !feof(file_in)){
			switch (sitio.sitios_tipologia){
				case TL_PUNTO_DE_ARROJO:
					strcpy(color,"tan");
					strcpy(tipo,"Punto de arrojo");
				break;
			    case TL_MICROBASURAL:
					strcpy(color,"yellow");
					strcpy(tipo,"Microbasural");
				break;
				case TL_BASURAL:
					strcpy(color,"orange");
					strcpy(tipo,"Basural");
				break;
				case TL_MACROBASURAL:
					strcpy(color,"default");
					strcpy(tipo,"Macrobasural");
			}

		  fprintf(file_out,"%s,%s{%s: %s}<%s-dot>\n",sitio.sitios_latitud,sitio.sitios_longitud,tipo,sitio.sitios_denominacion,color);

		  fgets(linea,limite,file_in);
		  err = linea_csv_a_sitio_t(linea, &sitio);
		}

	}else{ //SI SE USO LA OPCION -m

		int cantMunicipios = 0;
		int **municipios = (int**)(malloc(sizeOf(int)*2));
		int pos = -1;
		int valor = -1;
		while (err == OK && !feof(file_in)){

			switch (sitio.sitios_tipologia){
				case TL_PUNTO_DE_ARROJO:
					valor=1;
				break;
			    case TL_MICROBASURAL:
					valor=2;
				break;
				case TL_BASURAL:
					valor=3;
				break;
				case TL_MACROBASURAL:
					valor=4;
			}

			if ((pos = array_search(municipios,cantMunicipios,sitio.municipios_id))==-1){ //Si no estaba guardado el municipio
				municipios[cantMunicipios][0] = sitio.municipios_id;
				municipios[cantMunicipios][1] = valor;
				cantMunicipios++;
				realloc(municipios,sizeOf(int)*2*(cantMunicipios+1));
			}else{ //Si el municipio ya fue registrado
				municipios[pos][1]+=valor;
			}


		  fgets(linea,limite,file_in);
		  err = linea_csv_a_sitio_t(linea, &sitio);
		}





		free(municipios);
	}

	return err;
}

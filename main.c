#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h> //para el getopt_long() y el struct option



int main (int argc, char** argv){

//Errores:
//exit(1): Error de lectura de parametros, formato incorrecto
//exit(2): Falta un argumento de un parametro
//exit(3): Un parametro requiere de otro que no ha sido ingresado.
//exit(4): Un archivo no pudo leerse.
//exit(5): Un archivo no pudo escribirse.


	if (argc>1){


		int entrada = 0,salida = 0;
		int orden = 0;
		for (int i=1;i<argc;i++){ //Reviso si en algun lugar aparece el argumento que pide ayuda
			if ((strcmp(argv[i],"-h")==0)|| (strcmp(argv[i],"--help")==0) || (strcmp(argv[i],"-a")==0) || (strcmp(argv[i],"--ayuda")==0)) {
				orden = 'h';
			}
			if ((strcmp(argv[i],"-l")==0) || (strcmp(argv[i],"--leer")==0)){
				entrada++:
			}
			if ((strcmp(argv[i],"-s")==0) || (strcmp(argv[i],"--salida")==0)){
				salida++;
			}
		}

		if (entrada >1 || salida >1){
			puts("Los parametros -l (--leer) y -s (--salida) no pueden repetirse. Cancelando operación...");
			exit(1);
		}else{
			 //COMIENZO DE RECOLECCION DE PARAMETROS -----------------------------------------------------

			FILE *file_in;
			FILE *file_out;


			static const struct option longOpts[] = { //opciones largas para los flags, para getopt_long
	    		{ "leer", required_argument, NULL, 'l' },
				{ "salida", required_argument, NULL, 's' },
				{ "municipio", no_argument, NULL, 'm' },
	    		{ NULL, no_argument, NULL, 0 }
			};



			if (orden == 'h'){
				ordenes_ayuda();
			}else{

				char* ruta_entrada = NULL; //optarg va a apuntar al string guardado en el vector de argumentos, puedo simplemente asignar estos punteros.
				char * ruta_salida = NULL;
				int muni = 0; //muni = false
				opterr=0; //Silecio prints de error por defecto ya que necesito un parametro con argumento opcional.
				int longIndice = 0;
				while ((orden=getopt_long(argc,argv,":l:s:m",),longOpts,&longIndice) != -1){
					switch(orden){
						case 'l':
						ruta_entrada = optarg;
						break;
						case 's':
						if (entrada<1){
							puts("Se ingresó el parámetro de salida pero no el de entrada. Intente de nuevo añadiendo -l <ruta de archivo de entrada>");
							exit(3);
						}else{
							ruta_salida=optarg;
						}
						break;
						case 'm':
						if (salida<1){
							puts("Se ingresó el parametro de restriccion de uno por municipio, pero no hay parametro que indique la salida. Intente de nuevo añadiendo -s o -s <ruta archivo de salida>");
							exit(3);
						}else{
							muni = 1; //muni = true;
						}
						break;
						case ':':
							switch (optopt){
								case 'l':
									puts("-l (--leer) requiere una ruta de archivo.");
									exit(2);
								break;
								case 's':
									puts("No se especificó archivo de salida, se imprimirá el resultado en pantalla.")
									file_out = stdout;
								break;
								default:
									puts("Error de lectura de parametros. Cancelando operacion...");
									exit(1);
								break;
								}
						break;
						case '?':
							puts("Se encontro un parámetro no válido, cancelando operación...");
							exit(1);
						break;
					}

				}//fin while

				//FIN DE RECOLECCION DE PARAMETROS---------------------------------------------------------


				//COMIENZO DEL PROGRAMA PRINCIPAL ---------------------------------------------------

				if (ruta_salida != NULL && strcmp(ruta_salida,"-")!=0){
					file_out = fpopen(ruta_salida,"wb");
					if (file_out == NULL){
						puts("El archivo de salida no pudo abrirse o no existe. ¿Está abierto en otro programa?");
						exit(4);
					}
				}

				file_in=fpopen(ruta_entrada,"r");

				//Reservo de antemano espacio para los string porque no puedo saber cuanto reservar a no ser que

				//Declaracion de variables requeridas para procesar el archivo
				//sitios_id = 0
				char[64] sitios_denominacion= NULL;
				//sitios_direccion
				double sitios_latitud= 0; // latitud minima es -90.000000 (10 chars)
				double sitios_longitud= 0; // longitud minima es -180.000000 (11 chars)
				int municipios_id= NULL;
				//municipios_descripcion;
				char* sitios_tipologia= NULL;
				//sitios_estado;
				int scan_result;
				int muni_actual = -1;

				if (muni){
					//Solo imprimir uno por municipio

				}else{
					//Imprimir todos

					while ((scan_result=fscanf(file_in,"%*s,%s,%*s,%lf,%lf,%d,%*s,%s,%*s\n",sitios_denominacion,sitios_latitud,sitios_longitud,municipios_id,sitios_tipologia))!=EOF)){
						if (strcmp(sitios_tipologia,"punto de arrojo")==0){
							fprintf(file_out,"%lf,%lf{%s: %s}%s<tan-dot>\n",sitios_latitud,sitios_longitud,sitios_tipologia,sitios_denominacion);
						}else{
							if(strcmp(sitios_tipologia,"microbasural")==0){
								fprintf(file_out,"%lf,%lf{%s: %s}%s<yellow-dot>\n",sitios_latitud,sitios_longitud,sitios_tipologia,sitios_denominacion);
							}else{
								if(strcmp(sitios_tipologia,"basural")==0){
									fprintf(file_out,"%lf,%lf{%s: %s}%s<orange-dot>\n",sitios_latitud,sitios_longitud,sitios_tipologia,sitios_denominacion);
								}else{
									fprintf(file_out,"%lf,%lf{%s: %s}%s<default-dot>\n",sitios_latitud,sitios_longitud,sitios_tipologia,sitios_denominacion);
								}
							}
						}

					}

				}






				if (file_out != stdout){
					fclose(file_out);
				}
				//FIN DEL PROGRAMA PRINCIPAL -------------------------------------------------


			} //fin if
		}
	}else{
		puts("Se requieren parámetros para ejecutar el programa. Para más información use el parametro -h (--help) o -a (--ayuda). Ej: localizador.exe -h");
	}




	return 0;
}

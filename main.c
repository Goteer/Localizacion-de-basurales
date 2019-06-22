#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h> //para el getopt_long()



int main (int argc, char** argv){

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

				char* ruta_entrada = NULL;
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
							puts("Parámetro no válido, cancelando operación...");
							exit(1);
						break;
					}

				}//fin while

				//FIN DE RECOLECCION DE PARAMETROS---------------------------------------------------------



				//FIN DEL PROGRAMA PRINCIPAL -------------------------------------------------


			} //fin if
		}
	}else{
		puts("Se esperan parámetros. Para más información use el parametro -h (--help) o -a (--ayuda). Ej: localizador.exe -h");
	}




	return 0;
}

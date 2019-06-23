#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h> //para el getopt_long() y el struct option
#include "ordenes.h"


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
		for (int i=1;i<argc;i++){ //Reviso si en algun lugar aparece el argumento que pide ayuda y cuento la cantidad de veces que aparecen los argumentos obligatorios
			if ((strcmp(argv[i],"-h")==0)|| (strcmp(argv[i],"--help")==0) || (strcmp(argv[i],"-a")==0) || (strcmp(argv[i],"--ayuda")==0)) {
				orden = 'h';
			}
			if ((strcmp(argv[i],"-l")==0) || (strcmp(argv[i],"--leer")==0)){
				entrada++;
			}
			if ((strcmp(argv[i],"-s")==0) || (strcmp(argv[i],"--salida")==0)){
				salida++;
			}
		}

		if (entrada >1 || salida >1){
			puts("Los parametros -l (--leer) y -s (--salida) no pueden repetirse. Cancelando operacion...\n");
			exit(1);
		}

		if (orden != 'h' && (entrada <1 || salida <1)){
			puts("Tanto el parametro -l (--leer) como el parametro -s (--salida) son obligatorios. Cancelando operacion...\n");
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
				while ((orden=getopt_long(argc,argv,":l:s:m",longOpts,&longIndice)) != -1){
					switch(orden){
						case 'l':
						ruta_entrada = optarg;
						break;
						case 's':
						if (entrada<1){
							puts("Se ingresó el parametro de salida pero no el de entrada. Intente de nuevo añadiendo -l <ruta de archivo de entrada>\n");
							exit(3);
						}else{
							if (strcmp(optarg,"-")==0){
								file_out=stdout;
							}else{
								ruta_salida=optarg;
							}
						}
						break;
						case 'm':
						if (salida<1){
							puts("Se ingreso el parametro de restriccion de uno por municipio, pero no hay parametro que indique la salida. Intente de nuevo añadiendo -s o -s <ruta archivo de salida>\n");
							exit(3);
						}else{
							muni = 1; //muni = true;
						}
						break;
						case ':':
							switch (optopt){
								case 'l':
									puts("-l (--leer) requiere una ruta de archivo.\n");
									exit(2);
								break;
								case 's':
									puts("No se especifico archivo de salida, se imprimirá el resultado en pantalla.\n");
									file_out = stdout;
								break;
								default:
									puts("Error de lectura de parametros. Cancelando operacion...\n");
									exit(1);
								break;
								}
						break;
						case '?':
							puts("Se encontro un parametro no válido, cancelando operación...\n");
							exit(1);
						break;
					}

				}//fin while

				//FIN DE RECOLECCION DE PARAMETROS---------------------------------------------------------


				//COMIENZO DEL PROGRAMA PRINCIPAL ---------------------------------------------------

				if (ruta_salida != NULL && strcmp(ruta_salida,"-")!=0){
					file_out = fopen(ruta_salida,"wb");
					if (file_out == NULL){
						puts("El archivo de salida no pudo abrirse o no existe. ¿Esta abierto en otro programa?\n");
						exit(4);
					}
				}

				file_in=fopen(ruta_entrada,"rb");
				if (file_in!=NULL){
					ordenes_salida(file_in,file_out,muni);
				}else{
					puts("El archivo de entrada no pudo abrirse o no existe. ¿Esta abierto en otro programa?\n");
					exit(4);
				}



				if (file_out != stdout){
					fclose(file_out);
				}
				//FIN DEL PROGRAMA PRINCIPAL -------------------------------------------------


			} //fin if
		}
	}else{
		puts("Se requieren parametros para ejecutar el programa. Para más informacion use el parametro -h (--help) o -a (--ayuda). Ej: localizador.exe -h\n");
	}




	return 0;
}

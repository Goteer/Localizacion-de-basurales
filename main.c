#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h> //para el getopt_long() y el struct option
#include "ordenes.h"
#include "sitio.h"


int main (int argc, char** argv) {

//Errores:
//exit(65): Falta un argumento de un parametro
//exit(66): Falta nombre de archivo en -l
//exit(67): Error de lectura de parametros, formato incorrecto o Un parametro requiere de otro que no ha sido ingresado.
//exit(68): Un archivo no pudo leerse.
//exit(69): Un archivo no pudo escribirse.
//exit(70): Campo muy largo
//exit(71): Faltan campos
//exit(72): Campo no es numerico
//exit(73): Tipologia no valida

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



		if (orden != 'h' && (entrada <1)){
			fputs("El parametro -l (--leer) es obligatorio. Cancelando operacion...\n",stderr);
			exit(67);
		}else{

			if (entrada >1 || salida >1){
				fputs("Los parametros -l (--leer) y -s (--salida) no pueden repetirse. Cancelando operacion...\n",stderr);
				exit(67);
			}

			 //COMIENZO DE RECOLECCION DE PARAMETROS

			if (orden == 'h'){
				ordenes_ayuda();
			}else{

				FILE *file_in;
				FILE *file_out;
				file_out=stdout;

				static const struct option longOpts[] = { //opciones largas para los flags, para getopt_long
		    		{ "leer", required_argument, NULL, 'l' },
					{ "salida", required_argument, NULL, 's' },
					{ "municipio", no_argument, NULL, 'm' },
		    		{ NULL, no_argument, NULL, 0 }
				};

				char* ruta_entrada = NULL; //optarg va a apuntar al string guardado en el vector de argumentos, puedo simplemente asignar estos punteros.
				char * ruta_salida = NULL;
				int muni = 0; //muni = false
				opterr=0; //Silecio prints de error por defecto ya que necesito un parametro con argumento opcional.
				int longIndice = 0;
				while ((orden=getopt_long(argc,argv,":l:s:m",longOpts,&longIndice)) != -1){
					switch(orden){
						case 'l':
						if (optarg[0] != '-'){
							ruta_entrada = optarg;
						}else{
							fputs("-l (--leer) requiere una ruta de archivo.\n",stderr);
							exit(66);
						}

						break;
						case 's':
						if (entrada<1){
							fputs("Se ingresó el parametro de salida pero no el de entrada. Intente de nuevo añadiendo -l <ruta de archivo de entrada>\n",stderr);
							exit(67);
						}else{
							if (optarg[0] == '-'){
								if (strcmp(optarg,"-")!=0){ //Si el guion no era literal, era otro parametro
									optind--;
									puts("No se especifico archivo de salida, se imprimirá el resultado en pantalla."); //Esto es una funcion del programa, no un error.

								}

							}else{
								ruta_salida=optarg;
							}
						}
						break;
						case 'm':
						if (salida<1){
							fputs("Se ingreso el parametro de restriccion de uno por municipio, pero no hay parametro que indique la salida. Intente de nuevo añadiendo -s o -s <ruta archivo de salida>\n",stderr);
							exit(67);
						}else{
							muni = 1; //muni = true;
						}
						break;
						case ':':
							switch (optopt){
								case 'l':
									fputs("-l (--leer) requiere una ruta de archivo.\n",stderr);
									exit(66);
								break;
								case 's':
									puts("No se especifico archivo de salida, se imprimirá el resultado en pantalla."); //Esto es una funcion del programa, no un error.
									file_out = stdout;
								break;
								default:
									fputs("Error de lectura de parametros. Cancelando operacion...\n",stderr);
									exit(67);
								break;
								}
						break;
						case '?':
							fputs("Se encontro un parametro no válido, cancelando operación...\n",stderr);
							exit(67);
						break;
					}

				}//fin while

				//FIN DE RECOLECCION DE PARAMETROS---------------------------------------------------------


				//COMIENZO DEL PROGRAMA PRINCIPAL ---------------------------------------------------

				error_t err = OK;
				int error = 0;
				if (ruta_salida != NULL && strcmp(ruta_salida,"-")!=0){
					file_out = fopen(ruta_salida,"wb");
					if (file_out == NULL){
						fputs("El archivo de salida no pudo abrirse o crearse. ¿Es un archivo de solo lectura o una ubicacion protegida?\n",stderr);
						exit(68);
					}
				}

				file_in=fopen(ruta_entrada,"rb");
				if (file_in!=NULL){
					err = ordenes_salida(file_in,file_out,muni);
					if (err != OK){
						error=70;
						switch (err){
							case ERROR_CAMPO_MUY_LARGO:
							fputs("Se encontro un campo demasiado largo.",stderr); //exit(70): Campo muy largo
							break;
						    case ERROR_FALTAN_CAMPOS:
							fputs("Formato incorrecto: Faltan campos.",stderr); //exit(71): Faltan campos
							error++;
							break;
							case ERROR_CAMPO_NO_ES_NUMERICO:
							fputs("Formato incorrecto: Se esperaba un campo numerico, pero se encontro otro tipo.",stderr); //exit(72): Campo no es numerico
							error+=2;
							break;
							case ERROR_TIPOLOGIA_NO_VALIDA:
							fputs("Formato incorrecto: Se encontro una tipologia no valida.",stderr); //exit(73): Tipologia no valida
							error+=3;
							break;
							case OK: //Este caso solo esta para quitar un warning. No tiene sentido.
							break;
						}
						//Se retornará el error luego de cerrar el archivo, no hace falta un exit.
					}

				}else{
					fputs("El archivo de entrada no pudo abrirse o no existe. ¿Esta abierto en otro programa?\n",stderr);
					exit(68);
				}


				fclose(file_in);
				fclose(file_out);

				return err;
				//FIN DEL PROGRAMA PRINCIPAL -------------------------------------------------


			} //fin if
		}
	}else{
		fputs("Se requieren parametros para ejecutar el programa. Para más informacion use el parametro -h (--help) o -a (--ayuda). Ej: localizador.exe -h\n",stderr);
	}




	return 0;
}

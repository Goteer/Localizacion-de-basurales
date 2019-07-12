#ifndef SITIO_H
#define SITIO_H
#include <stdbool.h>
#include <stdio.h>

typedef enum {
    OK,
    ERROR_CAMPO_MUY_LARGO,
    ERROR_FALTAN_CAMPOS,
    ERROR_CAMPO_NO_ES_NUMERICO,
    ERROR_TIPOLOGIA_NO_VALIDA,
} error_t;

typedef enum {
    TL_PUNTO_DE_ARROJO,
    TL_MICROBASURAL,
    TL_BASURAL,
    TL_MACROBASURAL,
} tipologia_t;

typedef struct {
    char sitios_id[8];
    char sitios_denominacion[200];
    char sitios_direccion[200];
    char sitios_latitud[20];
    char sitios_longitud[20];
    int municipios_id;
    char municipios_descripcion[200];
    tipologia_t sitios_tipologia;
    char sitios_estado[200];
} sitio_t;

/**
 * Dado un fragmento de línea de un archivo csv que comienza en `*comienzo`,
 * copia el campo a `destino` y establece `*comienzo` al siguiente campo a
 * menos que se haya llegado al fin de la línea.
 *
 * @param comienzo puntero al fragmento de línea del cuál extraer el campo,
 * comienzo se actualizará para apuntar al próximo campo o al final del string.
 * comienzo debe apuntar a una línea que no contenga fin de línea ni espacios
 * al final ni entre las `,` de los campos.
 * @param destino string con al menos `longitud_max` bytes, en `destino` se
 * almacenará el campo leído, terminado con un caracter nulo.
 * @param longitud_max es la cantidad de bytes que puede alojar `destino`
 * incluyendo el caracter nulo, leer un campo que no entre en `destino` devolverá
 * error.
 * @param ultimo indica si el campo a leer es el último de la línea. Si un
 * campo no es el último y se alcanza el final de la línea la función retornará
 * error.
 *
 * @return OK (o 0) si no hay error, no-cero si hay error.
 */
error_t csv_extraer_campo(const char **comienzo, char *destino, size_t longitud_max, bool ultimo);

/**
 * Dada una línea que puede contener fin de línea o espacios al final y un
 * puntero a una estructura `sitio_t`, se cargará la estructura con los
 * campos leídos.
 * La línea se modificará truncándola si contiene espacios o fines de línea
 * al final.
 *
 * @param linea string con una línea de CSV
 * @param sitio puntero a una estructura donde se cargará lo leido.
 *
 * @return OK (o 0) si no hay error, no-cero si hay error.
 */
error_t linea_csv_a_sitio_t(char *linea, sitio_t *sitio);

#endif

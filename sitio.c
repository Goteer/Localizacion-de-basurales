/*
sitios_id,sitios_denominacion,sitios_direccion,sitios_latitud,sitios_longitud,municipios_id,municipios_descripcion,sitios_tipologia,sitios_estado
*/
#include "sitio.h"
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

static void rstrip(char *linea) {
    size_t len = strlen(linea);
    // Recorremos la linea desde el final eliminando espacios,
    // tabs, fines de línea y otros caracteres de espacio.
    for (size_t i = len - 1; i > 0 && isspace(linea[i]); i--){
        linea[i] = '\0';
    }
}

error_t csv_extraer_campo(const char **comienzo, char *destino, size_t longitud_max, bool ultimo) {
    size_t i;
    // Calcular longitud del campo
    for (i = 0; i < longitud_max && (*comienzo)[i] != '\0' && (*comienzo)[i] != ','; i++);
    // Verificar errores
    if (i == longitud_max) return ERROR_CAMPO_MUY_LARGO;
    if (!ultimo && (*comienzo)[i] == '\0') return ERROR_FALTAN_CAMPOS;
    // Copiar campo a `destino`
    strncpy(destino, *comienzo, i);
    // Truncar la cadena `destino` por precaución (ver man strncpy)
    destino[i] = 0;
    if ((*comienzo)[i] == '\0') {
        *comienzo += i;     // *comienzo apuntará a una cadena vacía
    }
    else {
        *comienzo += i + 1; // salteamos la coma
    }
    return OK;
}

error_t linea_csv_a_sitio_t(char *linea, sitio_t *sitio) {
    error_t err;
    char muni_id[20], tipologia[40];
    const char *comienzo_campo = linea;

    rstrip(linea);

    err = csv_extraer_campo(&comienzo_campo, sitio->sitios_id, 12, false);

    if (err != OK) return err;

    err = csv_extraer_campo(&comienzo_campo, sitio->sitios_denominacion, 200, false);
    if (err != OK) return err;

    err = csv_extraer_campo(&comienzo_campo, sitio->sitios_direccion, 200, false);
    if (err != OK) return err;

    err = csv_extraer_campo(&comienzo_campo, sitio->sitios_latitud, 20, false);
    if (err != OK) return err;

    err = csv_extraer_campo(&comienzo_campo, sitio->sitios_longitud, 20, false);
    if (err != OK) return err;

    err = csv_extraer_campo(&comienzo_campo, muni_id, 20, false);
    if (err != OK) return err;
    err = csv_extraer_campo(&comienzo_campo, sitio->municipios_descripcion, 200, false);
    if (err != OK) return err;

    err = csv_extraer_campo(&comienzo_campo, tipologia, 40, false);
    if (err != OK) return err;

    err = csv_extraer_campo(&comienzo_campo, sitio->sitios_estado, 200, true);
    if (err != OK) return err;

    char *fin_nro;
    sitio->municipios_id = strtol(muni_id, &fin_nro, 10);
    if (!(muni_id[0] != '\0' && *fin_nro == '\0')) return ERROR_CAMPO_NO_ES_NUMERICO;

    for (int i = 0; tipologia[i] != '\0'; i++) {
        tipologia[i] = tolower(tipologia[i]);
    }
    if (strcmp(tipologia, "punto de arrojo") == 0) {
        sitio->sitios_tipologia = TL_PUNTO_DE_ARROJO;
    }
    else if (strcmp(tipologia, "microbasural") == 0) {
        sitio->sitios_tipologia = TL_MICROBASURAL;
    }
    else if (strcmp(tipologia, "basural") == 0) {
        sitio->sitios_tipologia = TL_BASURAL;
    }
    else if (strcmp(tipologia, "macrobasural") == 0) {
        sitio->sitios_tipologia = TL_MACROBASURAL;
    }
    else {
        return ERROR_TIPOLOGIA_NO_VALIDA;
    }
    return OK;
}

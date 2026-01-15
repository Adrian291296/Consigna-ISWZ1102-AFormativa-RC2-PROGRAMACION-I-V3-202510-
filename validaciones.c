#include <stdio.h>
#include <stdlib.h> // strtof, strtol
#include <string.h>
#include "validaciones.h"

#define TAM_BUFFER 100

// Leer float validado con fgets
float leerFloatRango(const char *mensaje, float min, float max)
{
    char buffer[TAM_BUFFER];
    char *fin;
    float valor;
    int valido = 0;

    do
    {
        printf("%s", mensaje);

        if (fgets(buffer, TAM_BUFFER, stdin) == NULL)
        {
            printf("Error al leer entrada.\n");
            continue;
        }

        valor = strtof(buffer, &fin);

        // Verifica si no se ingresó número
        if (buffer == fin)
        {
            printf("Error: entrada no numerica.\n");
            continue;
        }

        // Verifica caracteres inválidos (excepto salto de línea)
        while (*fin == ' ' || *fin == '\n')
            fin++;
        if (*fin != '\0')
        {
            printf("Error: caracteres invalidos detectados.\n");
            continue;
        }

        // Verifica rango
        if (valor < min || valor > max)
        {
            printf("Error: el valor debe estar entre %.2f y %.2f\n", min, max);
            continue;
        }

        valido = 1;

    } while (!valido);

    return valor;
}

// Leer entero validado con fgets
int leerEnteroRango(const char *mensaje, int min, int max)
{
    char buffer[TAM_BUFFER];
    char *fin;
    long valor;
    int valido = 0;

    do
    {
        printf("%s", mensaje);

        if (fgets(buffer, TAM_BUFFER, stdin) == NULL)
        {
            printf("Error al leer entrada.\n");
            continue;
        }

        valor = strtol(buffer, &fin, 10);

        if (buffer == fin)
        {
            printf("Error: entrada no numerica.\n");
            continue;
        }

        while (*fin == ' ' || *fin == '\n')
            fin++;
        if (*fin != '\0')
        {
            printf("Error: caracteres invalidos detectados.\n");
            continue;
        }

        if (valor < min || valor > max)
        {
            printf("Error: el valor debe estar entre %d y %d\n", min, max);
            continue;
        }

        valido = 1;

    } while (!valido);

    return (int)valor;
}

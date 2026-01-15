#include <stdio.h>
#include "contaminacion.h"

int main()
{
    Zona zonas[ZONAS];

    inicializarZonas(zonas);
    cargarDatos(zonas);

    for (int i = 0; i < ZONAS; i++)
    {
        ingresarDatos(&zonas[i]);
        calcularPromedio(&zonas[i]);
        verificarActual(&zonas[i]);
        predecirContaminacion(&zonas[i]);
        verificarAlertas(&zonas[i]);
    }

    guardarDatos(zonas);
    generarReporte(zonas);

    printf("\nSistema ejecutado correctamente.\n");
    return 0;
}

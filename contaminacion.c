#include <stdio.h>
#include <string.h>
#include "contaminacion.h"
#include "validaciones.h"

// Inicializar zonas
void inicializarZonas(Zona *zonas)
{
    for (int i = 0; i < ZONAS; i++)
    {
        zonas[i].id = i + 1;
        sprintf(zonas[i].nombre, "Zona %d", i + 1);

        for (int j = 0; j < DIAS; j++)
        {
            zonas[i].historico[j].co2 = 300 + j;
            zonas[i].historico[j].so2 = 10 + j * 0.2;
            zonas[i].historico[j].no2 = 20 + j * 0.3;
            zonas[i].historico[j].pm25 = 15 + j * 0.4;
        }
    }
}

// Ingreso de datos actuales
void ingresarDatos(Zona *zona)
{
    printf("\n--- %s ---\n", zona->nombre);

    zona->actual.co2 = leerFloatRango(
        "CO2 (0 - 1000 ppm): ", 0, 1000);

    zona->actual.so2 = leerFloatRango(
        "SO2 (0 - 100 ug/m3): ", 0, 100);

    zona->actual.no2 = leerFloatRango(
        "NO2 (0 - 100 ug/m3): ", 0, 100);

    zona->actual.pm25 = leerFloatRango(
        "PM2.5 (0 - 500 ug/m3): ", 0, 500);

    zona->clima.temperatura = leerFloatRango(
        "Temperatura (0 - 50 C): ", 0, 50);

    zona->clima.viento = leerFloatRango(
        "Velocidad del viento (0 - 100 km/h): ", 0, 100);

    zona->clima.humedad = leerFloatRango(
        "Humedad relativa (0 - 100 %): ", 0, 100);
}

// Cargar históricos
void cargarDatos(Zona *zonas)
{
    FILE *f = fopen("datos.txt", "r");
    if (!f)
        return;

    for (int i = 0; i < ZONAS; i++)
    {
        for (int j = 0; j < DIAS; j++)
        {
            fscanf(f, "%f %f %f %f",
                   &zonas[i].historico[j].co2,
                   &zonas[i].historico[j].so2,
                   &zonas[i].historico[j].no2,
                   &zonas[i].historico[j].pm25);
        }
    }
    fclose(f);
}

// Guardar históricos
void guardarDatos(Zona *zonas)
{
    FILE *f = fopen("datos.txt", "w");
    if (!f)
        return;

    for (int i = 0; i < ZONAS; i++)
    {
        for (int j = 0; j < DIAS; j++)
        {
            fprintf(f, "%.2f %.2f %.2f %.2f\n",
                    zonas[i].historico[j].co2,
                    zonas[i].historico[j].so2,
                    zonas[i].historico[j].no2,
                    zonas[i].historico[j].pm25);
        }
    }
    fclose(f);
}

// Promedio histórico 30 días
void calcularPromedio(Zona *zona)
{
    zona->promedio.co2 = 0;
    zona->promedio.so2 = 0;
    zona->promedio.no2 = 0;
    zona->promedio.pm25 = 0;

    for (int i = 0; i < DIAS; i++)
    {
        zona->promedio.co2 += zona->historico[i].co2;
        zona->promedio.so2 += zona->historico[i].so2;
        zona->promedio.no2 += zona->historico[i].no2;
        zona->promedio.pm25 += zona->historico[i].pm25;
    }

    zona->promedio.co2 /= DIAS;
    zona->promedio.so2 /= DIAS;
    zona->promedio.no2 /= DIAS;
    zona->promedio.pm25 /= DIAS;
}

// Predicción 24h
void predecirContaminacion(Zona *zona)
{
    zona->prediccion = zona->promedio;

    if (zona->clima.viento < 5)
    {
        zona->prediccion.pm25 *= 1.3;
        zona->prediccion.co2 *= 1.2;
    }

    if (zona->clima.temperatura > 30)
    {
        zona->prediccion.no2 *= 1.2;
    }
}

// Verificación actual
void verificarActual(Zona *zona)
{
    if (zona->actual.pm25 > LIM_PM25)
        printf("!! PM2.5 actual excede limite OMS en %s\n", zona->nombre);
}

// Alertas predictivas
void verificarAlertas(Zona *zona)
{
    if (zona->prediccion.pm25 > LIM_PM25 ||
        zona->prediccion.co2 > LIM_CO2 ||
        zona->prediccion.no2 > LIM_NO2 ||
        zona->prediccion.so2 > LIM_SO2)
    {
        printf("\nALERTA AMBIENTAL EN %s\n", zona->nombre);
        generarRecomendaciones(zona);
    }
}

// Recomendaciones
void generarRecomendaciones(Zona *zona)
{
    printf("Recomendaciones:\n");

    if (zona->prediccion.co2 > LIM_CO2)
        printf("- Reducir trafico vehicular\n");

    if (zona->prediccion.so2 > LIM_SO2)
        printf("- Controlar emisiones industriales\n");

    if (zona->prediccion.no2 > LIM_NO2)
        printf("- Restringir combustion urbana\n");

    if (zona->prediccion.pm25 > LIM_PM25)
        printf("- Suspender actividades al aire libre\n");
}

// Reporte
void generarReporte(Zona *zonas)
{
    FILE *f = fopen("reporte.txt", "w");
    if (!f)
        return;

    fprintf(f, "REPORTE AMBIENTAL COMPLETO\n\n");

    for (int i = 0; i < ZONAS; i++)
    {
        fprintf(f, "%s\n", zonas[i].nombre);

        fprintf(f, "Promedio PM2.5 (30 dias): %.2f\n",
                zonas[i].promedio.pm25);

        fprintf(f, "Prediccion PM2.5 (24h): %.2f\n",
                zonas[i].prediccion.pm25);

        // Evaluación
        if (zonas[i].promedio.pm25 > LIM_PM25 ||
            zonas[i].prediccion.pm25 > LIM_PM25)
        {
            fprintf(f, "ESTADO: ALERTA AMBIENTAL\n");
            fprintf(f, "Recomendaciones:\n");

            if (zonas[i].prediccion.pm25 > LIM_PM25)
                fprintf(f, "- Suspender actividades al aire libre\n");

            if (zonas[i].prediccion.co2 > LIM_CO2)
                fprintf(f, "- Reducir trafico vehicular\n");

            if (zonas[i].prediccion.so2 > LIM_SO2)
                fprintf(f, "- Limitar emisiones industriales\n");
        }
        else
        {
            fprintf(f, "ESTADO: NIVELES ACEPTABLES\n");
        }

        fprintf(f, "\n");
    }

    fclose(f);
}


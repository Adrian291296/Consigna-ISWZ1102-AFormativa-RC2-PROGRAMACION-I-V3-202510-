#ifndef CONTAMINACION_H
#define CONTAMINACION_H

#define ZONAS 5
#define DIAS 30

// Límites OMS
#define LIM_CO2 400
#define LIM_SO2 20
#define LIM_NO2 40
#define LIM_PM25 25

typedef struct
{
    float co2;
    float so2;
    float no2;
    float pm25;
} Contaminantes;

typedef struct
{
    float temperatura;
    float viento;
    float humedad;
} Clima;

typedef struct
{
    int id;
    char nombre[40];
    Contaminantes actual;
    Contaminantes historico[DIAS];
    Contaminantes promedio;
    Contaminantes prediccion;
    Clima clima;
} Zona;

// Funciones
void inicializarZonas(Zona *zonas);
void ingresarDatos(Zona *zona);
void cargarDatos(Zona *zonas);
void guardarDatos(Zona *zonas);

void calcularPromedio(Zona *zona);
void predecirContaminacion(Zona *zona);

void verificarActual(Zona *zona);
void verificarAlertas(Zona *zona);
void generarRecomendaciones(Zona *zona);

void generarReporte(Zona *zonas);

#endif

#ifndef EXPERIMENTACION_H
#define EXPERIMENTACION_H

#include <string>
#include <vector>

class ArbolSistemaArchivos;

struct ResultadoExperimento {
    int numDirectorios;        // Número de directorios en la configuración
    int numArchivos;           // Número de archivos en la configuración
    double tiempoCreacion;     // Tiempo de creación del árbol en milisegundos
    double tiempoBusqueda;     // Tiempo promedio de búsqueda en nanosegundos
    double tiempoEliminacion;  // Tiempo promedio de eliminación en nanosegundos
    double tiempoInsercion;    // Tiempo promedio de inserción en nanosegundos
    int alturaArbol;           // Altura del árbol creado
    int numeroNodos;           // Número total de nodos en el árbol

    ResultadoExperimento() : numDirectorios(0), numArchivos(0), tiempoCreacion(0.0),
                           tiempoBusqueda(0.0), tiempoEliminacion(0.0), tiempoInsercion(0.0),
                           alturaArbol(0), numeroNodos(0) {}
};

void crearDirectorioPrueba(const std::string& rutaBase, int numDirectorios, int numArchivos);
double medirTiempoCreacion(const std::string& rutaBase);
double medirTiempoBusqueda(ArbolSistemaArchivos& arbol, const std::vector<std::string>& rutas);
double medirTiempoEliminacion(ArbolSistemaArchivos& arbol, const std::vector<std::string>& rutas);
double medirTiempoInsercion(ArbolSistemaArchivos& arbol, const std::vector<std::string>& directorios);
ResultadoExperimento ejecutarExperimento(const std::string& rutaBase, int numDirectorios, int numArchivos);
void ejecutarTodosLosExperimentos(const std::string& rutaBase);
void guardarResultados(const std::vector<ResultadoExperimento>& resultados, const std::string& nombreArchivo);
void mostrarResumen(const std::vector<ResultadoExperimento>& resultados);
void limpiarDirectoriosPrueba(const std::string& rutaBase);

// Constantes para los experimentos
extern const int REP;                                     // Número de repeticiones (100,000)
extern const int NUM_DIRECTORIOS_INSERCION;              
extern const std::vector<std::string> NOMBRES_ARCHIVOS;  

#endif // EXPERIMENTACION_H

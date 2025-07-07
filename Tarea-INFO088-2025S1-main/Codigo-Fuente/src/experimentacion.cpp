#include "experimentacion.h"
#include "tree.h"
#include <chrono>
#include <random>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <filesystem>

// Constantes para los experimentos
const int REP = 100000; // 100,000 repeticiones
const int NUM_DIRECTORIOS_INSERCION = 2000; // Para experimentos de inserción

// Nombres de archivos para inserción
const std::vector<std::string> NOMBRES_ARCHIVOS = {
    "test1.txt", "test2.txt", "test3.txt", "test4.txt", "test5.txt",
    "data1.csv", "data2.csv", "data3.csv", "data4.csv", "data5.csv",
    "config1.ini", "config2.ini", "config3.ini", "config4.ini", "config5.ini",
    "backup1.bak", "backup2.bak", "backup3.bak", "backup4.bak", "backup5.bak",
    "log1.log", "log2.log", "log3.log", "log4.log", "log5.log",
    "temp1.tmp", "temp2.tmp", "temp3.tmp", "temp4.tmp", "temp5.tmp",
    "doc1.doc", "doc2.doc", "doc3.doc", "doc4.doc", "doc5.doc",
    "img1.jpg", "img2.jpg", "img3.jpg", "img4.jpg", "img5.jpg",
    "video1.mp4", "video2.mp4", "video3.mp4", "video4.mp4", "video5.mp4",
    "audio1.mp3", "audio2.mp3", "audio3.mp3", "audio4.mp3", "audio5.mp3"
};

// Función para crear un directorio de prueba con estructura jerárquica
void crearDirectorioPrueba(const std::string& rutaBase, int numDirectorios, int numArchivos) {
    std::filesystem::create_directories(rutaBase);
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> profundidad(1, 5);
    std::uniform_int_distribution<> archivosPorDir(1, 10);
    std::uniform_int_distribution<> nombreIdx(0, NOMBRES_ARCHIVOS.size() - 1);
    
    int archivosCreados = 0;
    int directoriosCreados = 0;
    
    // Crear directorios y archivos
    for (int i = 0; i < numDirectorios && directoriosCreados < numDirectorios; ++i) {
        std::string rutaDir = rutaBase + "/dir_" + std::to_string(i);
        
        // Crear subdirectorios anidados
        int prof = profundidad(gen);
        for (int j = 0; j < prof; ++j) {
            rutaDir += "/subdir_" + std::to_string(j);
            std::filesystem::create_directories(rutaDir);
            directoriosCreados++;
            
            if (directoriosCreados >= numDirectorios) break;
        }
        
        // Crear archivos en el directorio
        int archivosEnEsteDir = archivosPorDir(gen);
        for (int k = 0; k < archivosEnEsteDir && archivosCreados < numArchivos; ++k) {
            std::string nombreArchivo = NOMBRES_ARCHIVOS[nombreIdx(gen)];
            std::string rutaArchivo = rutaDir + "/" + nombreArchivo + "_" + std::to_string(k);
            
            std::ofstream archivo(rutaArchivo);
            if (archivo.is_open()) {
                archivo << "Contenido del archivo " << archivosCreados << std::endl;
                archivo.close();
                archivosCreados++;
            }
        }
    }
    
    std::cout << "Directorio de prueba creado: " << rutaBase << std::endl;
    std::cout << "Directorios creados: " << directoriosCreados << std::endl;
    std::cout << "Archivos creados: " << archivosCreados << std::endl;
}

// Función para medir el tiempo de creación del árbol
double medirTiempoCreacion(const std::string& rutaBase) {
    auto inicio = std::chrono::high_resolution_clock::now();
    
    ArbolSistemaArchivos arbol;
    arbol.cargarDatos(rutaBase);
    
    auto fin = std::chrono::high_resolution_clock::now();
    auto duracion = std::chrono::duration_cast<std::chrono::microseconds>(fin - inicio);
    
    return static_cast<double>(duracion.count()) / 1000.0; // Convertir a milisegundos
}

// Función para medir el tiempo promedio de búsqueda
double medirTiempoBusqueda(ArbolSistemaArchivos& arbol, const std::vector<std::string>& rutas) {
    if (rutas.empty()) return 0.0;
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, rutas.size() - 1);
    
    auto inicio = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < REP; ++i) {
        int idx = dist(gen);
        arbol.buscar(rutas[idx]);
    }
    
    auto fin = std::chrono::high_resolution_clock::now();
    auto duracion = std::chrono::duration_cast<std::chrono::nanoseconds>(fin - inicio);
    
    return static_cast<double>(duracion.count()) / REP; // Promedio en nanosegundos
}

// Función para medir el tiempo promedio de eliminación
double medirTiempoEliminacion(ArbolSistemaArchivos& arbol, const std::vector<std::string>& rutas) {
    if (rutas.empty()) return 0.0;
    
    // Crear una copia del árbol para las pruebas de eliminación
    ArbolSistemaArchivos arbolCopia;
    // Nota: Para una implementación completa, necesitaríamos un constructor de copia
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, rutas.size() - 1);
    
    auto inicio = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < REP; ++i) {
        int idx = dist(gen);
        arbol.eliminar(rutas[idx]);
    }
    
    auto fin = std::chrono::high_resolution_clock::now();
    auto duracion = std::chrono::duration_cast<std::chrono::nanoseconds>(fin - inicio);
    
    return static_cast<double>(duracion.count()) / REP; // Promedio en nanosegundos
}

// Función para medir el tiempo promedio de inserción
double medirTiempoInsercion(ArbolSistemaArchivos& arbol, const std::vector<std::string>& directorios) {
    if (directorios.empty()) return 0.0;
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distDir(0, std::min(static_cast<int>(directorios.size()) - 1, NUM_DIRECTORIOS_INSERCION - 1));
    std::uniform_int_distribution<> distArchivo(0, NOMBRES_ARCHIVOS.size() - 1);
    
    auto inicio = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < REP; ++i) {
        int idxDir = distDir(gen);
        int idxArchivo = distArchivo(gen);
        std::string rutaInsercion = directorios[idxDir] + "/" + NOMBRES_ARCHIVOS[idxArchivo] + "_" + std::to_string(i);
        arbol.insertar(rutaInsercion);
    }
    
    auto fin = std::chrono::high_resolution_clock::now();
    auto duracion = std::chrono::duration_cast<std::chrono::nanoseconds>(fin - inicio);
    
    return static_cast<double>(duracion.count()) / REP; // Promedio en nanosegundos
}

// Función para ejecutar experimentos completos
ResultadoExperimento ejecutarExperimento(const std::string& rutaBase, int numDirectorios, int numArchivos) {
    std::cout << "\n=== Ejecutando experimento ===" << std::endl;
    std::cout << "Configuración: " << numDirectorios << " directorios, " << numArchivos << " archivos" << std::endl;
    
    // Crear directorio de prueba
    std::string rutaPrueba = rutaBase + "_" + std::to_string(numDirectorios) + "_" + std::to_string(numArchivos);
    crearDirectorioPrueba(rutaPrueba, numDirectorios, numArchivos);
    
    ResultadoExperimento resultado;
    resultado.numDirectorios = numDirectorios;
    resultado.numArchivos = numArchivos;
    
    // Medir tiempo de creación
    std::cout << "Midiendo tiempo de creación..." << std::endl;
    resultado.tiempoCreacion = medirTiempoCreacion(rutaPrueba);
    
    // Crear el árbol para las demás pruebas
    ArbolSistemaArchivos arbol;
    arbol.cargarDatos(rutaPrueba);
    
    // Obtener todas las rutas para los experimentos
    std::vector<std::string> todasLasRutas = arbol.obtenerTodasLasRutas();
    std::vector<std::string> todosLosDirectorios = arbol.obtenerTodosLosDirectorios();
    
    std::cout << "Rutas encontradas: " << todasLasRutas.size() << std::endl;
    std::cout << "Directorios encontrados: " << todosLosDirectorios.size() << std::endl;
    
    // Medir tiempo de búsqueda
    std::cout << "Midiendo tiempo de búsqueda..." << std::endl;
    resultado.tiempoBusqueda = medirTiempoBusqueda(arbol, todasLasRutas);
    
    // Medir tiempo de eliminación
    std::cout << "Midiendo tiempo de eliminación..." << std::endl;
    resultado.tiempoEliminacion = medirTiempoEliminacion(arbol, todasLasRutas);
    
    // Medir tiempo de inserción
    std::cout << "Midiendo tiempo de inserción..." << std::endl;
    resultado.tiempoInsercion = medirTiempoInsercion(arbol, todosLosDirectorios);
    
    // Obtener estadísticas del árbol
    resultado.alturaArbol = arbol.obtenerAltura();
    resultado.numeroNodos = arbol.obtenerNumeroNodos();
    
    std::cout << "Experimento completado." << std::endl;
    return resultado;
}

// Función para ejecutar todos los experimentos
void ejecutarTodosLosExperimentos(const std::string& rutaBase) {
    std::vector<std::pair<int, int>> configuraciones = {
        {20000, 200000},    // Configuración pequeña
        {100000, 1000000},  // Configuración mediana
        {1000000, 10000000} // Configuración grande
    };
    
    std::vector<ResultadoExperimento> resultados;
    
    for (const auto& config : configuraciones) {
        ResultadoExperimento resultado = ejecutarExperimento(rutaBase, config.first, config.second);
        resultados.push_back(resultado);
    }
    
    // Guardar resultados en archivo
    guardarResultados(resultados, "resultados_experimentos.csv");
    
    // Mostrar resumen
    mostrarResumen(resultados);
}

// Función para guardar resultados en archivo CSV
void guardarResultados(const std::vector<ResultadoExperimento>& resultados, const std::string& nombreArchivo) {
    std::ofstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        std::cerr << "Error al crear el archivo de resultados" << std::endl;
        return;
    }
    
    // Escribir encabezados
    archivo << "NumDirectorios,NumArchivos,TiempoCreacion(ms),TiempoBusqueda(ns),TiempoEliminacion(ns),TiempoInsercion(ns),AlturaArbol,NumeroNodos" << std::endl;
    
    // Escribir datos
    for (const auto& resultado : resultados) {
        archivo << resultado.numDirectorios << ","
                << resultado.numArchivos << ","
                << std::fixed << std::setprecision(3) << resultado.tiempoCreacion << ","
                << std::fixed << std::setprecision(2) << resultado.tiempoBusqueda << ","
                << std::fixed << std::setprecision(2) << resultado.tiempoEliminacion << ","
                << std::fixed << std::setprecision(2) << resultado.tiempoInsercion << ","
                << resultado.alturaArbol << ","
                << resultado.numeroNodos << std::endl;
    }
    
    archivo.close();
    std::cout << "Resultados guardados en: " << nombreArchivo << std::endl;
}

// Función para mostrar resumen de resultados
void mostrarResumen(const std::vector<ResultadoExperimento>& resultados) {
    std::cout << "\n=== RESUMEN DE RESULTADOS ===" << std::endl;
    std::cout << std::left << std::setw(15) << "Configuración"
              << std::setw(18) << "Creación (ms)"
              << std::setw(16) << "Búsqueda (ns)"
              << std::setw(18) << "Eliminación (ns)"
              << std::setw(17) << "Inserción (ns)"
              << std::setw(10) << "Altura"
              << std::setw(12) << "Nodos" << std::endl;
    
    std::cout << std::string(110, '-') << std::endl;
    
    for (const auto& resultado : resultados) {
        std::string config = std::to_string(resultado.numDirectorios) + "/" + std::to_string(resultado.numArchivos);
        
        std::cout << std::left << std::setw(15) << config
                  << std::setw(18) << std::fixed << std::setprecision(3) << resultado.tiempoCreacion
                  << std::setw(16) << std::fixed << std::setprecision(2) << resultado.tiempoBusqueda
                  << std::setw(18) << std::fixed << std::setprecision(2) << resultado.tiempoEliminacion
                  << std::setw(17) << std::fixed << std::setprecision(2) << resultado.tiempoInsercion
                  << std::setw(10) << resultado.alturaArbol
                  << std::setw(12) << resultado.numeroNodos << std::endl;
    }
    
    std::cout << std::string(110, '-') << std::endl;
}

// Función para limpiar directorios de prueba
void limpiarDirectoriosPrueba(const std::string& rutaBase) {
    std::vector<std::string> sufijos = {"_20000_200000", "_100000_1000000", "_1000000_10000000"};
    
    for (const std::string& sufijo : sufijos) {
        std::string rutaCompleta = rutaBase + sufijo;
        if (std::filesystem::exists(rutaCompleta)) {
            try {
                std::filesystem::remove_all(rutaCompleta);
                std::cout << "Directorio limpiado: " << rutaCompleta << std::endl;
            } catch (const std::filesystem::filesystem_error& e) {
                std::cerr << "Error al limpiar directorio " << rutaCompleta << ": " << e.what() << std::endl;
            }
        }
    }
}

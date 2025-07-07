#include "experimentacion.h"
#include "tree.h"
#include <iostream>
#include <string>
#include <filesystem>
#include <chrono>

void mostrarMenu() {
    std::cout << "\n=== SISTEMA DE ARCHIVOS CON ÁRBOL K-ARIO ===" << std::endl;
    std::cout << "1. Ejecutar experimentos completos" << std::endl;
    std::cout << "2. Crear árbol desde directorio personalizado" << std::endl;
    std::cout << "3. Pruebas básicas de funcionalidad" << std::endl;
    std::cout << "4. Limpiar directorios de prueba" << std::endl;
    std::cout << "5. Salir" << std::endl;
    std::cout << "Seleccione una opción: ";
}

void pruebasBasicas() {
    std::cout << "\n=== PRUEBAS BÁSICAS DE FUNCIONALIDAD ===" << std::endl;
    
    // Crear un árbol de prueba
    ArbolSistemaArchivos arbol;
    
    // Insertar algunas rutas de prueba
    std::cout << "Insertando rutas de prueba..." << std::endl;
    arbol.insertar("docs/readme.txt");
    arbol.insertar("docs/manual.pdf");
    arbol.insertar("src/main.cpp");
    arbol.insertar("src/utils/helper.cpp");
    arbol.insertar("tests/test1.cpp");
    
    // Mostrar estadísticas
    std::cout << "Número de nodos: " << arbol.obtenerNumeroNodos() << std::endl;
    std::cout << "Altura del árbol: " << arbol.obtenerAltura() << std::endl;
    
    // Probar búsquedas
    std::cout << "\nPruebas de búsqueda:" << std::endl;
    std::vector<std::string> rutasPrueba = {
        "docs/readme.txt",
        "docs/manual.pdf", 
        "src/main.cpp",
        "src/utils/helper.cpp",
        "tests/test1.cpp",
        "archivo_inexistente.txt",
        "docs"
    };
    
    for (const std::string& ruta : rutasPrueba) {
        int resultado = arbol.buscar(ruta);
        std::cout << "Búsqueda '" << ruta << "': ";
        switch (resultado) {
            case 0: std::cout << "ARCHIVO ENCONTRADO" << std::endl; break;
            case 1: std::cout << "NO EXISTE" << std::endl; break;
            case 2: std::cout << "DIRECTORIO ENCONTRADO" << std::endl; break;
            default: std::cout << "ERROR" << std::endl; break;
        }
    }
    
    // Probar eliminación
    std::cout << "\nPruebas de eliminación:" << std::endl;
    std::cout << "Eliminando 'docs/readme.txt': " << (arbol.eliminar("docs/readme.txt") ? "ÉXITO" : "FALLO") << std::endl;
    std::cout << "Búsqueda después de eliminación: ";
    int resultado = arbol.buscar("docs/readme.txt");
    std::cout << (resultado == 1 ? "NO EXISTE (correcto)" : "ERROR - aún existe") << std::endl;
    
    // Mostrar todas las rutas
    std::cout << "\nTodas las rutas en el árbol:" << std::endl;
    std::vector<std::string> todasLasRutas = arbol.obtenerTodasLasRutas();
    for (const std::string& ruta : todasLasRutas) {
        std::cout << "  " << ruta << std::endl;
    }
    
    // Mostrar todos los directorios
    std::cout << "\nTodos los directorios en el árbol:" << std::endl;
    std::vector<std::string> todosLosDirectorios = arbol.obtenerTodosLosDirectorios();
    for (const std::string& dir : todosLosDirectorios) {
        std::cout << "  " << dir << std::endl;
    }
}

void crearArbolPersonalizado() {
    std::cout << "\n=== CREAR ÁRBOL DESDE DIRECTORIO PERSONALIZADO ===" << std::endl;
    
    std::string rutaDirectorio;
    std::cout << "Ingrese la ruta del directorio a cargar: ";
    std::getline(std::cin, rutaDirectorio);
    
    if (!std::filesystem::exists(rutaDirectorio)) {
        std::cout << "Error: El directorio no existe." << std::endl;
        return;
    }
    
    if (!std::filesystem::is_directory(rutaDirectorio)) {
        std::cout << "Error: La ruta no es un directorio." << std::endl;
        return;
    }
    
    std::cout << "Cargando árbol desde: " << rutaDirectorio << std::endl;
    
    ArbolSistemaArchivos arbol;
    auto inicio = std::chrono::high_resolution_clock::now();
    arbol.cargarDatos(rutaDirectorio);
    auto fin = std::chrono::high_resolution_clock::now();
    
    auto duracion = std::chrono::duration_cast<std::chrono::milliseconds>(fin - inicio);
    
    std::cout << "Árbol cargado exitosamente!" << std::endl;
    std::cout << "Tiempo de carga: " << duracion.count() << " ms" << std::endl;
    std::cout << "Número de nodos: " << arbol.obtenerNumeroNodos() << std::endl;
    std::cout << "Altura del árbol: " << arbol.obtenerAltura() << std::endl;
    
    // Mostrar algunas estadísticas
    std::vector<std::string> archivos = arbol.obtenerTodasLasRutas();
    std::vector<std::string> directorios = arbol.obtenerTodosLosDirectorios();
    
    std::cout << "Archivos encontrados: " << archivos.size() << std::endl;
    std::cout << "Directorios encontrados: " << directorios.size() << std::endl;
    
    // Opción para mostrar algunos archivos
    char opcion;
    std::cout << "¿Desea ver algunos archivos? (s/n): ";
    std::cin >> opcion;
    std::cin.ignore();
    
    if (opcion == 's' || opcion == 'S') {
        std::cout << "\nPrimeros 20 archivos encontrados:" << std::endl;
        for (size_t i = 0; i < std::min(static_cast<size_t>(20), archivos.size()); ++i) {
            std::cout << "  " << archivos[i] << std::endl;
        }
    }
}

int main() {
    std::cout << "Iniciando programa de experimentos con Árbol K-ario..." << std::endl;
    
    int opcion;
    do {
        mostrarMenu();
        std::cin >> opcion;
        std::cin.ignore(); // Limpiar buffer
        
        switch (opcion) {
            case 1: {
                std::cout << "\n=== EXPERIMENTOS COMPLETOS ===" << std::endl;
                std::string rutaBase;
                std::cout << "Ingrese la ruta base para crear directorios de prueba (ej: /tmp/pruebas): ";
                std::getline(std::cin, rutaBase);
                
                if (rutaBase.empty()) {
                    rutaBase = "/tmp/pruebas_arbol";
                }
                
                std::cout << "Iniciando experimentos en: " << rutaBase << std::endl;
                std::cout << "ADVERTENCIA: Este proceso puede tomar varios minutos..." << std::endl;
                
                char confirmar;
                std::cout << "¿Continuar? (s/n): ";
                std::cin >> confirmar;
                std::cin.ignore();
                
                if (confirmar == 's' || confirmar == 'S') {
                    ejecutarTodosLosExperimentos(rutaBase);
                }
                break;
            }
            
            case 2: {
                crearArbolPersonalizado();
                break;
            }
            
            case 3: {
                pruebasBasicas();
                break;
            }
            
            case 4: {
                std::cout << "\n=== LIMPIAR DIRECTORIOS DE PRUEBA ===" << std::endl;
                std::string rutaBase;
                std::cout << "Ingrese la ruta base de los directorios a limpiar: ";
                std::getline(std::cin, rutaBase);
                
                if (rutaBase.empty()) {
                    rutaBase = "/tmp/pruebas_arbol";
                }
                
                limpiarDirectoriosPrueba(rutaBase);
                break;
            }
            
            case 5: {
                std::cout << "Saliendo del programa..." << std::endl;
                break;
            }
            
            default: {
                std::cout << "Opción inválida. Intente de nuevo." << std::endl;
                break;
            }
        }
        
    } while (opcion != 5);
    
    return 0;
}

#include "tree.h"
#include <algorithm>
#include <filesystem>
#include <iostream>

// Constructor del nodo
NodoArbol::NodoArbol(const std::string& nombre) : nombre(nombre) {}

// Destructor del nodo
NodoArbol::~NodoArbol() {
    for (auto* hijo : hijos) {
        delete hijo;
    }
}

// Constructor del árbol
ArbolSistemaArchivos::ArbolSistemaArchivos() : raiz(nullptr) {}

// Destructor del árbol
ArbolSistemaArchivos::~ArbolSistemaArchivos() {
    delete raiz;
}

// Función para cargar datos del sistema de archivos
void ArbolSistemaArchivos::cargarDatos(const std::string& rutaBase) {
    if (raiz) {
        delete raiz;
    }
    
    // Crear nodo raíz
    raiz = new NodoArbol("raiz");
    
    try {
        // Recorrer el directorio de forma recursiva
        for (const auto& entry : std::filesystem::recursive_directory_iterator(rutaBase)) {
            std::string rutaRelativa = std::filesystem::relative(entry.path(), rutaBase).string();
            insertarRuta(rutaRelativa);
        }
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Error al acceder al sistema de archivos: " << e.what() << std::endl;
    }
}

// Función para insertar una ruta en el árbol
void ArbolSistemaArchivos::insertarRuta(const std::string& ruta) {
    if (!raiz) {
        raiz = new NodoArbol("raiz");
    }
    
    std::vector<std::string> componentes = dividirRuta(ruta);
    NodoArbol* actual = raiz;
    
    for (const std::string& componente : componentes) {
        NodoArbol* hijo = buscarHijo(actual, componente);
        if (!hijo) {
            hijo = new NodoArbol(componente);
            insertarHijoOrdenado(actual, hijo);
        }
        actual = hijo;
    }
}

// Función para dividir una ruta en componentes
std::vector<std::string> ArbolSistemaArchivos::dividirRuta(const std::string& ruta) {
    std::vector<std::string> componentes;
    std::string componente;
    
    for (char c : ruta) {
        if (c == '/' || c == '\\') {
            if (!componente.empty()) {
                componentes.push_back(componente);
                componente.clear();
            }
        } else {
            componente += c;
        }
    }
    
    if (!componente.empty()) {
        componentes.push_back(componente);
    }
    
    return componentes;
}

// Función para buscar un hijo específico por nombre
NodoArbol* ArbolSistemaArchivos::buscarHijo(NodoArbol* nodo, const std::string& nombre) {
    if (!nodo) return nullptr;
    
    // Búsqueda binaria en el vector ordenado
    int izq = 0, der = static_cast<int>(nodo->hijos.size()) - 1;
    
    while (izq <= der) {
        int medio = izq + (der - izq) / 2;
        
        if (nodo->hijos[medio]->nombre == nombre) {
            return nodo->hijos[medio];
        } else if (nodo->hijos[medio]->nombre < nombre) {
            izq = medio + 1;
        } else {
            der = medio - 1;
        }
    }
    
    return nullptr;
}

// Función para insertar un hijo manteniendo el orden lexicográfico
void ArbolSistemaArchivos::insertarHijoOrdenado(NodoArbol* padre, NodoArbol* hijo) {
    if (!padre || !hijo) return;
    
    // Encontrar la posición correcta para mantener el orden
    auto it = std::lower_bound(padre->hijos.begin(), padre->hijos.end(), hijo,
        [](const NodoArbol* a, const NodoArbol* b) {
            return a->nombre < b->nombre;
        });
    
    padre->hijos.insert(it, hijo);
}

// Función de búsqueda por ruta
int ArbolSistemaArchivos::buscar(const std::string& ruta) {
    if (!raiz) return 1; // No existe
    
    std::vector<std::string> componentes = dividirRuta(ruta);
    NodoArbol* actual = raiz;
    
    for (const std::string& componente : componentes) {
        actual = buscarHijo(actual, componente);
        if (!actual) {
            return 1; // No existe
        }
    }
    
    // Si llegamos aquí, el nodo existe
    if (actual->hijos.empty()) {
        return 0; // Es un archivo (nodo hoja)
    } else {
        return 2; // Es un directorio
    }
}

// Función para insertar un nuevo archivo/directorio
int ArbolSistemaArchivos::insertar(const std::string& ruta) {
    if (!raiz) {
        raiz = new NodoArbol("raiz");
    }
    
    std::vector<std::string> componentes = dividirRuta(ruta);
    if (componentes.empty()) return 2; // Ruta inválida
    
    // Verificar si ya existe
    if (buscar(ruta) != 1) {
        return 1; // Ya existe
    }
    
    // Encontrar el directorio padre
    NodoArbol* padre = raiz;
    for (size_t i = 0; i < componentes.size() - 1; ++i) {
        padre = buscarHijo(padre, componentes[i]);
        if (!padre) {
            return 2; // No existe la ruta padre
        }
    }
    
    // Insertar el nuevo nodo
    NodoArbol* nuevoNodo = new NodoArbol(componentes.back());
    insertarHijoOrdenado(padre, nuevoNodo);
    
    return 0; // Éxito
}

// Función para eliminar un archivo/directorio
bool ArbolSistemaArchivos::eliminar(const std::string& ruta) {
    if (!raiz) return false;
    
    std::vector<std::string> componentes = dividirRuta(ruta);
    if (componentes.empty()) return false;
    
    // Encontrar el nodo padre
    NodoArbol* padre = raiz;
    for (size_t i = 0; i < componentes.size() - 1; ++i) {
        padre = buscarHijo(padre, componentes[i]);
        if (!padre) {
            return false; // No existe la ruta padre
        }
    }
    
    // Buscar el nodo a eliminar
    std::string nombreAEliminar = componentes.back();
    auto it = std::find_if(padre->hijos.begin(), padre->hijos.end(),
        [&nombreAEliminar](const NodoArbol* nodo) {
            return nodo->nombre == nombreAEliminar;
        });
    
    if (it == padre->hijos.end()) {
        return false; // No existe el nodo
    }
    
    // Eliminar el nodo (el destructor se encarga de eliminar el subárbol)
    delete *it;
    padre->hijos.erase(it);
    
    return true;
}

// Función para obtener la altura del árbol
int ArbolSistemaArchivos::obtenerAltura() const {
    return obtenerAltura(raiz);
}

// Función auxiliar recursiva para obtener la altura
int ArbolSistemaArchivos::obtenerAltura(NodoArbol* nodo) const {
    if (!nodo) return 0;
    
    int maxAltura = 0;
    for (NodoArbol* hijo : nodo->hijos) {
        maxAltura = std::max(maxAltura, obtenerAltura(hijo));
    }
    
    return maxAltura + 1;
}

// Función para obtener el número total de nodos
int ArbolSistemaArchivos::obtenerNumeroNodos() const {
    return obtenerNumeroNodos(raiz);
}

// Función auxiliar recursiva para contar nodos
int ArbolSistemaArchivos::obtenerNumeroNodos(NodoArbol* nodo) const {
    if (!nodo) return 0;
    
    int contador = 1; // Contar el nodo actual
    for (NodoArbol* hijo : nodo->hijos) {
        contador += obtenerNumeroNodos(hijo);
    }
    
    return contador;
}

// Función para obtener todas las rutas del árbol
std::vector<std::string> ArbolSistemaArchivos::obtenerTodasLasRutas() const {
    std::vector<std::string> rutas;
    if (raiz) {
        obtenerRutasRecursivo(raiz, "", rutas);
    }
    return rutas;
}

// Función auxiliar recursiva para obtener todas las rutas
void ArbolSistemaArchivos::obtenerRutasRecursivo(NodoArbol* nodo, const std::string& rutaActual, std::vector<std::string>& rutas) const {
    if (!nodo) return;
    
    std::string nuevaRuta = rutaActual.empty() ? nodo->nombre : rutaActual + "/" + nodo->nombre;
    
    // Si es un nodo hoja (archivo), agregar la ruta
    if (nodo->hijos.empty() && nodo != raiz) {
        rutas.push_back(nuevaRuta);
    }
    
    // Recorrer recursivamente todos los hijos
    for (NodoArbol* hijo : nodo->hijos) {
        obtenerRutasRecursivo(hijo, nuevaRuta, rutas);
    }
}

// Función para obtener todos los directorios
std::vector<std::string> ArbolSistemaArchivos::obtenerTodosLosDirectorios() const {
    std::vector<std::string> directorios;
    if (raiz) {
        obtenerDirectoriosRecursivo(raiz, "", directorios);
    }
    return directorios;
}

// Función auxiliar recursiva para obtener todos los directorios
void ArbolSistemaArchivos::obtenerDirectoriosRecursivo(NodoArbol* nodo, const std::string& rutaActual, std::vector<std::string>& directorios) const {
    if (!nodo) return;
    
    std::string nuevaRuta = rutaActual.empty() ? nodo->nombre : rutaActual + "/" + nodo->nombre;
    
    // Si no es un nodo hoja y no es la raíz, es un directorio
    if (!nodo->hijos.empty() && nodo != raiz) {
        directorios.push_back(nuevaRuta);
    }
    
    // Recorrer recursivamente todos los hijos
    for (NodoArbol* hijo : nodo->hijos) {
        obtenerDirectoriosRecursivo(hijo, nuevaRuta, directorios);
    }
}

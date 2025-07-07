#ifndef TREE_H
#define TREE_H

#include <string>
#include <vector>

struct NodoArbol {
    std::string nombre;
    std::vector<NodoArbol*> hijos;

    explicit NodoArbol(const std::string& nombre);

    ~NodoArbol();
};

class ArbolSistemaArchivos {
private:
    NodoArbol* raiz;
    int obtenerAltura(NodoArbol* nodo) const;
    int obtenerNumeroNodos(NodoArbol* nodo) const;
    void obtenerRutasRecursivo(NodoArbol* nodo, const std::string& rutaActual, std::vector<std::string>& rutas) const;
    void obtenerDirectoriosRecursivo(NodoArbol* nodo, const std::string& rutaActual, std::vector<std::string>& directorios) const;

public:
    ArbolSistemaArchivos();
    ~ArbolSistemaArchivos();
    void cargarDatos(const std::string& rutaBase);
    void insertarRuta(const std::string& ruta);
    std::vector<std::string> dividirRuta(const std::string& ruta);
    NodoArbol* buscarHijo(NodoArbol* nodo, const std::string& nombre);
    void insertarHijoOrdenado(NodoArbol* padre, NodoArbol* hijo);
    int buscar(const std::string& ruta);
    int insertar(const std::string& ruta);
    bool eliminar(const std::string& ruta);
    int obtenerAltura() const;
    int obtenerNumeroNodos() const;
    std::vector<std::string> obtenerTodasLasRutas() const;
    std::vector<std::string> obtenerTodosLosDirectorios() const;
};

#endif // TREE_H

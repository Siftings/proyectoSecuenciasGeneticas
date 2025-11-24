#ifndef GRAFO_H
#define GRAFO_H

#include <string>
#include <vector>

using namespace std;

// Estructura para representar una posicion en la matriz
struct Posicion {
    int fila;
    int columna;
    
    Posicion() : fila(-1), columna(-1) {}
    Posicion(int f, int c) : fila(f), columna(c) {}
    
    bool operator==(const Posicion& otra) const { // Overload del operador == para comparar si 2 posiciones son iguales
        return fila == otra.fila && columna == otra.columna;
    }
};

// Estructura para representar una arista con su peso
struct Arista {
    Posicion destino;
    double peso;
    
    Arista(Posicion dest, double p) : destino(dest), peso(p) {}
};

// Estructura para representar un nodo del grafo
struct NodoGrafo {
    char base;                    // Base nitrogenada en esta posicion
    Posicion posicion;            // Posición [i,j] en la matriz
    vector<Arista> vecinos;       // Lista de vecinos con sus pesos
    
    NodoGrafo() : base('\0'), posicion() {}
    NodoGrafo(char b, int i, int j) : base(b), posicion(i, j) {}
};

// Estructura para representar el grafo completo de una secuencia
struct GrafoSecuencia {
    vector<vector<NodoGrafo>> matriz;  // Matriz de nodos [fila][columna]
    int filas;
    int columnas; // Teniendo en cuenta el ancho de linea del archivo fasta
    
    GrafoSecuencia() : filas(0), columnas(0) {}
};

// Estructura para representar el resultado de una ruta
struct ResultadoRuta {
    vector<Posicion> camino;      // Secuencia de posiciones en la ruta
    vector<char> bases;           // Bases en cada posicion de la ruta
    double costo_total;           // Costo total de la ruta
    bool existe;                  // Si se encontro una ruta valida
    
    ResultadoRuta() : costo_total(0.0), existe(false) {}
};

// Funciones principales
void ruta_mas_corta(string descripcion, string i_str, string j_str, string x_str, string y_str);
void base_remota(string descripcion, string i_str, string j_str);

// Funciones auxiliares
GrafoSecuencia construir_grafo(const string& secuencia_bases, int ancho_linea);
double calcular_peso_arista(char base1, char base2);
ResultadoRuta dijkstra(const GrafoSecuencia& grafo, Posicion origen, Posicion destino); // Algoritmo de Dijkstra, utilizado para ambas funciones
bool posicion_valida(const GrafoSecuencia& grafo, int i, int j);
Posicion encontrar_base_remota(const GrafoSecuencia& grafo, Posicion origen);

#endif

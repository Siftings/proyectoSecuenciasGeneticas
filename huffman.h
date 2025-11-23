#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <string>
#include <cstdint>

using namespace std;

// Estructura para almacenar frecuencia de un símbolo
struct FrecuenciaSimbolo {
    char simbolo;
    uint64_t frecuencia;
};

// Codigo de huffman (estandar para referirse a la secuencia de 0,1s derivada del arbol)
struct CodigoHuffman {
    char simbolo;
    string codigo;
};

// Nodo del arbol de Huffman
struct NodoHuffman {
    char simbolo;               // Simbolo (En nodos internos y raiz es '\0' nulo) implementacion estandar del arbol
    uint64_t frecuencia;        // Numero de veces que aparece el simbolo en la secuencia
    NodoHuffman* izquierdo;     // Hijo izquierdo (etiqueta "0")
    NodoHuffman* derecho;       // Hijo derecho (etiqueta "1")
    
    // Constructor
    NodoHuffman(char s, uint64_t f) 
        : simbolo(s), frecuencia(f), izquierdo(nullptr), derecho(nullptr) {}
};

// Funciones 
void codificar(string nombreArchivo);
void decodificar(string nombreArchivo);

// Funciones auxiliares para el arbol
NodoHuffman* construir_arbol_huffman(FrecuenciaSimbolo* frecuencias, int num_simbolos);
void generar_tabla_codigos(NodoHuffman* nodo, string codigo_actual, CodigoHuffman* tabla, int& num_codigos);
void liberar_arbol(NodoHuffman* nodo);

#endif

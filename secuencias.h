#ifndef SECUENCIAS_H
#define SECUENCIAS_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

// Estructura para representar una secuencia genética
struct Secuencia {
    string descripcion; // Nombre de la secuencia que viene después de '>'
    string bases;       // Secuencia de letras A, C, G, T, etc (nucleotidos).
    int ancho_linea;    // Ancho de línea original del archivo FASTA 
};

// Declaración extern para la variable global
extern vector<Secuencia> secuencias;

// Declaraciones de funciones para el manejo de secuencias genéticas
void cargar_archivo(string nombreArchivo);
void listar_secuencias();
void histograma(string descripcion);
void subsecuencia(string sub);
void enmascarar(string sub);
void guardar_archivo(string nombreArchivo);

// Funciones auxiliares para manejar códigos ambiguos
bool es_base_valida(char base);
int obtener_bases_minimas(char codigo);
bool son_compatibles(char codigo_secuencia, char codigo_busqueda);

#endif 

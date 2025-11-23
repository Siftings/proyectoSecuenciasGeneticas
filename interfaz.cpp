#include "interfaz.h"
#include <iostream>
#include <sstream>

using namespace std;


// Lista de nombres de comandos aceptados
string comandos[NUM_COMANDOS] = {
    "cargar", "listar_secuencias", "histograma", "es_subsecuencia",
    "enmascarar", "guardar", "codificar", "decodificar",
    "ruta_mas_corta", "base_remota", "ayuda", "salir"
};

// Ayudas asociadas a cada comando (en el mismo orden que el arreglo anterior)
string ayudas[NUM_COMANDOS] = {
    "Uso: cargar <archivo>. Carga un archivo FASTA.",
    "Uso: listar_secuencias. Lista las secuencias en memoria.",
    "Uso: histograma <descripcion>. Muestra el histograma de una secuencia.",
    "Uso: es_subsecuencia <sub>. Verifica si la subsecuencia está presente.",
    "Uso: enmascarar <sub>. Enmascara la subsecuencia encontrada.",
    "Uso: guardar <archivo>. Guarda las secuencias modificadas.",
    "Uso: codificar <archivo.fabin>. Codifica las secuencias.",
    "Uso: decodificar <archivo.fabin>. Decodifica un archivo .fabin.",
    "Uso: ruta_mas_corta <desc> <i> <j> <x> <y>. Ruta más corta en el grafo.",
    "Uso: base_remota <desc> <i> <j>. Busca la base remota más lejana.",
    "Uso: ayuda [comando]. Muestra ayuda general o específica.",
    "Uso: salir. Termina el programa."
};

// Función que divide una línea de texto por espacios y guarda las partes en un array
// Devuelve la cantidad de partes que encontró
int dividir(const string& input, string partes[]) {
    stringstream ss(input); // Permite extraer palabras una por una
    int count = 0;

    // Mientras haya palabras y no se sobrepase el máximo
    while (ss >> partes[count] && count < MAX_PARTES) {
        count++;
    }

    return count;
}

// Muestra la lista general de comandos disponibles
void mostrar_ayuda_general() {
    cout << "Comandos disponibles:\n";
    for (int i = 0; i < NUM_COMANDOS; i++) {
        cout << "  " << comandos[i] << "\n";
    }
}

// Muestra la ayuda específica de un comando si existe
void mostrar_ayuda_comando(const string& comando) {
    for (int i = 0; i < NUM_COMANDOS; i++) {
        if (comando == comandos[i]) {
            cout << ayudas[i] << "\n";
            return;
        }
    }
    cout << "Comando no reconocido. Usa 'ayuda' para ver todos los comandos.\n";
}

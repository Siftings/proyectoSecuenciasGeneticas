#include "huffman.h"
#include "secuencias.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cstdint>
#include <vector>

using namespace std;

const int MAX_SIMBOLOS = 256; // Funciona tanto con las bases nitrogenadas como con cualquier otro caracter ASCII

// Comparador para min-heap 
struct CompararNodos {
    bool operator()(NodoHuffman* a, NodoHuffman* b) const {
        return a->frecuencia > b->frecuencia; // Invertir para min-heap
    }
};

// Función para liberar memoria del árbol de Huffman
void liberar_arbol(NodoHuffman* nodo) {
    if (nodo == nullptr) return;
    liberar_arbol(nodo->izquierdo);
    liberar_arbol(nodo->derecho);
    delete nodo;
}

// Hace el arbol de huffman iniciando desde las hojas, dado el min heap
NodoHuffman* construir_arbol_huffman(FrecuenciaSimbolo* frecuencias, int num_simbolos) {
    // Crear vector para el heap
    vector<NodoHuffman*> heap;
    
    // Insertar todos los símbolos como nodos hoja
    for (int i = 0; i < num_simbolos; i++) {
        heap.push_back(new NodoHuffman(frecuencias[i].simbolo, frecuencias[i].frecuencia));
    }
    
    // Convertir el vector en un min-heap
    make_heap(heap.begin(), heap.end(), CompararNodos());
    
    // En el caso de que solo haya un simbolo
    if (heap.size() == 1) {
        NodoHuffman* unico = heap.front();
        NodoHuffman* raiz = new NodoHuffman('\0', unico->frecuencia);
        raiz->izquierdo = unico;
        return raiz;
    }
    
    // Construir el árbol combinando nodos
    while (heap.size() > 1) {
        // Extraer el nodo con menor frecuencia
        pop_heap(heap.begin(), heap.end(), CompararNodos());
        NodoHuffman* izq = heap.back();
        heap.pop_back();
        
        // Extraer el segundo nodo con menor frecuencia
        pop_heap(heap.begin(), heap.end(), CompararNodos());
        NodoHuffman* der = heap.back();
        heap.pop_back();
        
        // Crear un nodo interno con la suma de frecuencias
        NodoHuffman* interno = new NodoHuffman('\0', izq->frecuencia + der->frecuencia);
        interno->izquierdo = izq;
        interno->derecho = der;
        
        // Insertar el nodo interno de vuelta en el heap
        heap.push_back(interno);
        push_heap(heap.begin(), heap.end(), CompararNodos());
    }
    
    // El último nodo es la raíz del árbol
    return heap.front();
}

// Genera la tabla de codigos de huffman, recorriendo el arbol de manera recursiva
void generar_tabla_codigos(NodoHuffman* nodo, string codigo_actual, CodigoHuffman* tabla, int& num_codigos) {
    if (nodo == nullptr) return;
    
    // Si es un nodo hoja, guardar el código
    if (nodo->izquierdo == nullptr && nodo->derecho == nullptr) {
        tabla[num_codigos].simbolo = nodo->simbolo;
        tabla[num_codigos].codigo = codigo_actual.empty() ? "0" : codigo_actual;
        num_codigos++;
        return;
    }
    // Dado que todos los nodos internos tienen dos hijos, no es necesario verificar nullidad
    // Recorrer subarbol izquierdo con "0"
    generar_tabla_codigos(nodo->izquierdo, codigo_actual + "0", tabla, num_codigos);
    
    // Recorrer subarbol derecho con "1"
    generar_tabla_codigos(nodo->derecho, codigo_actual + "1", tabla, num_codigos);
}

// Escribir en binario 
template<typename T>
void escribir_binario(ofstream& archivo, T valor) {
    archivo.write(reinterpret_cast<const char*>(&valor), sizeof(T));
}

// Funcion para leer de un binario, devuelve si se leyo correctamente
template<typename T>
bool leer_binario(ifstream& archivo, T& valor) {
    archivo.read(reinterpret_cast<char*>(&valor), sizeof(T));
    return archivo.good();
}

// Codifica las secuencias en memoria y las guarda en un archivo binario .fabin
void codificar(string nombreArchivo) {
    // Verificar si hay secuencias cargadas
    if (secuencias.empty()) {
        cout << "No hay secuencias cargadas en memoria.\n";
        return;
    }
    
    // 1. Calcular frecuencias de todas las bases en todas las secuencias usando arreglo
    FrecuenciaSimbolo frecuencias[MAX_SIMBOLOS];
    int num_simbolos = 0;
    
    for (int i = 0; i < secuencias.size(); i++) {
        for (int j = 0; j < secuencias[i].bases.size(); j++) {
            char base = secuencias[i].bases[j];
            
            // Buscar si el simbolo ya existe
            bool encontrado = false;
            for (int k = 0; k < num_simbolos; k++) {
                if (frecuencias[k].simbolo == base) {
                    frecuencias[k].frecuencia++;
                    encontrado = true;
                    break;
                }
            }
            
            // Si no existe, agregarlo
            if (!encontrado) {
                frecuencias[num_simbolos].simbolo = base;
                frecuencias[num_simbolos].frecuencia = 1;
                num_simbolos++;
            }
        }
    }
    
    // Verificar que haya al menos un simbolo
    if (num_simbolos == 0) {
        cout << "No se pueden guardar las secuencias cargadas en " << nombreArchivo << ".\n";
        return;
    }
    
    // 2. Construir el arbol de huffman
    NodoHuffman* raiz = construir_arbol_huffman(frecuencias, num_simbolos);
    
    // 3. Generar la tabla de codigos
    CodigoHuffman tabla_codigos[MAX_SIMBOLOS];
    int num_codigos = 0;
    generar_tabla_codigos(raiz, "", tabla_codigos, num_codigos);
    
    // 4. Abrir el archivo binario para escritura
    ofstream archivo(nombreArchivo, ios::binary);
    if (!archivo.is_open()) {
        cout << "No se pueden guardar las secuencias cargadas en " << nombreArchivo << ".\n";
        liberar_arbol(raiz);
        return;
    }
    
    // 5. Escribir la cantidad de bases diferentes (n: 2 bytes)
    uint16_t n = num_simbolos;
    escribir_binario(archivo, n);
    
    // 6. Escribir cada base y su frecuencia (ci: 1 byte, fi: 8 bytes)
    for (int i = 0; i < num_simbolos; i++) {
        char ci = frecuencias[i].simbolo;
        uint64_t fi = frecuencias[i].frecuencia;
        escribir_binario(archivo, ci);
        escribir_binario(archivo, fi);
    }
    
    // 7. Escribir la cantidad de secuencias (ns: 4 bytes)
    uint32_t ns = secuencias.size();
    escribir_binario(archivo, ns);
    
    // 8. Escribir cada secuencia
    for (int idx = 0; idx < secuencias.size(); idx++) {
        const Secuencia& sec = secuencias[idx];
        
        // 8a. Longitud del nombre (li: 2 bytes)
        uint16_t li = sec.descripcion.size();
        escribir_binario(archivo, li);
        
        // 8b. Nombre de la secuencia (caracteres)
        archivo.write(sec.descripcion.c_str(), li);
        
        // 8c. Longitud de la secuencia (wi: 8 bytes)
        uint64_t wi = sec.bases.size();
        escribir_binario(archivo, wi);
        
        // 8d. justificacion/ancho de linea (xi: 2 bytes) - usar el ancho original
        uint16_t xi = sec.ancho_linea;
        escribir_binario(archivo, xi);
        
        // 8e. Codificar la secuencia en binario
        string codigo_completo = "";
        for (int j = 0; j < sec.bases.size(); j++) {
            char base = sec.bases[j];
            
            // Buscar el código de esta base
            for (int k = 0; k < num_codigos; k++) {
                if (tabla_codigos[k].simbolo == base) {
                    codigo_completo += tabla_codigos[k].codigo;
                    break;
                }
            }
        }
        
        // Relleno de 00 si no es multiplo de 8, para la lectura ya que solo lee bytes
        while (codigo_completo.size() % 8 != 0) {
            codigo_completo += "0";
        }
        
        // Convertir string de bits a bytes y escribir
        // .read solo permite escribir bytes
        for (size_t i = 0; i < codigo_completo.size(); i += 8) {
            uint8_t byte = 0;
            for (int j = 0; j < 8; j++) {
                if (codigo_completo[i + j] == '1') {
                    byte |= (1 << (7 - j)); 
                }
            }
            escribir_binario(archivo, byte);
        }
    }
    
    archivo.close();
    liberar_arbol(raiz);
    
    cout << "Secuencias codificadas y almacenadas en " << nombreArchivo << ".\n";
}

// Decodifica un archivo binario .fabin y carga las secuencias en memoria
void decodificar(string nombreArchivo) {
    // Abrir el archivo binario para lectura
    ifstream archivo(nombreArchivo, ios::binary);
    if (!archivo.is_open()) {
        cout << "No se pueden cargar las secuencias desde " << nombreArchivo << ".\n";
        return;
    }
    
    // 1. Leer la cantidad de bases diferentes (n: 2 bytes)
    uint16_t n;
        if (!leer_binario(archivo, n)) {
            archivo.close();
            cout << "No se pueden cargar las secuencias desde " << nombreArchivo << ".\n";
            return;
        }
        
        // 2. Leer cada base y su frecuencia para reconstruir el arbol
        FrecuenciaSimbolo frecuencias[MAX_SIMBOLOS];
        for (int i = 0; i < n; i++) {
            char ci;
            uint64_t fi;
            if (!leer_binario(archivo, ci)) {
                archivo.close();
                cout << "No se pueden cargar las secuencias desde " << nombreArchivo << ".\n";
                return;
            }
            if (!leer_binario(archivo, fi)) {
                archivo.close();
                cout << "No se pueden cargar las secuencias desde " << nombreArchivo << ".\n";
                return;
            }
            frecuencias[i].simbolo = ci;
            frecuencias[i].frecuencia = fi;
        }
        
        // 3. Reconstruir el arbol de Huffman
        NodoHuffman* raiz = construir_arbol_huffman(frecuencias, n);
        
        // 4. Leer la cantidad de secuencias (ns: 4 bytes)
        uint32_t ns;
        if (!leer_binario(archivo, ns)) {
            liberar_arbol(raiz);
            archivo.close();
            cout << "No se pueden cargar las secuencias desde " << nombreArchivo << ".\n";
            return;
        }
        
        // 5. Limpiar secuencias anteriores en memoria
        secuencias.clear();
        
        // 6. Leer cada secuencia
        for (uint32_t i = 0; i < ns; i++) {
            // 6a. Longitud del nombre (li: 2 bytes)
            uint16_t li;
            if (!leer_binario(archivo, li)) {
                liberar_arbol(raiz);
                archivo.close();
                cout << "No se pueden cargar las secuencias desde " << nombreArchivo << ".\n";
                return;
            }
            
            // 6b. Leer el nombre
            string descripcion(li, '\0');
            archivo.read(&descripcion[0], li);
            if (!archivo.good()) {
                liberar_arbol(raiz);
                archivo.close();
                cout << "No se pueden cargar las secuencias desde " << nombreArchivo << ".\n";
                return;
            }
            
            // 6c. Longitud de la secuencia (wi: 8 bytes)
            uint64_t wi;
            if (!leer_binario(archivo, wi)) {
                liberar_arbol(raiz);
                archivo.close();
                cout << "No se pueden cargar las secuencias desde " << nombreArchivo << ".\n";
                return;
            }
            
            // 6d. Ancho de linae (xi: 2 bytes)
            uint16_t xi;
            if (!leer_binario(archivo, xi)) {
                liberar_arbol(raiz);
                archivo.close();
                cout << "No se pueden cargar las secuencias desde " << nombreArchivo << ".\n";
                return;
            }
            
            // 6e. Decodificar la secuencia binaria
            string bases_decodificadas = "";
            NodoHuffman* nodo_actual = raiz;
            
            // Leer bytes y decodificar hasta obtener wi bases
            while (bases_decodificadas.size() < wi) {
                uint8_t byte;
                if (!leer_binario(archivo, byte)) {
                    liberar_arbol(raiz);
                    archivo.close();
                    cout << "No se pueden cargar las secuencias desde " << nombreArchivo << ".\n";
                    return;
                }
                
                // Procesar cada bit del byte
                for (int bit = 7; bit >= 0 && bases_decodificadas.size() < wi; bit--) {
                    bool es_uno = (byte & (1 << bit)) != 0;
                    
                    // Bajar por el arbol segun el bit
                    if (es_uno) {
                        nodo_actual = nodo_actual->derecho;
                    } else {
                        nodo_actual = nodo_actual->izquierdo;
                    }
                    
                    // Al llegar a una hoja se agrega el simbolo 
                    if (nodo_actual->izquierdo == nullptr && nodo_actual->derecho == nullptr) {
                        bases_decodificadas += nodo_actual->simbolo;
                        nodo_actual = raiz; // Reiniciar desde la raíz
                    }
                }
            }
            
            // Agregar la secuencia decodificada a memoria con su ancho de línea
            secuencias.push_back({descripcion, bases_decodificadas, (int)xi});
        }
        
        liberar_arbol(raiz);
        archivo.close();
        
        cout << "Secuencias decodificadas desde " << nombreArchivo << " y cargadas en memoria.\n";
}
#include "grafo.h"
#include "secuencias.h"
#include <iostream>
#include <cmath>

using namespace std;

const double INFINITO = 1e9;  // Valor grande para representar distancia infinita de Dijkstra

// Funcion para calcular el peso de una arista segun la formula: 1 / (1 + |ASCII_ij - ASCII_xy|)
double calcular_peso_arista(char base1, char base2) { // Retorno en double, dado a que hay un cociente
    int diferencia = abs(base1 - base2); // Hay restar ambos char, toma el valor en ASCII
    return 1.0 / (1.0 + diferencia); // Division con punto flotante (Se fuese 1 y no 1.0 retornaria siempre 0 o 1 aproximando)
}

// Verificar si una posicion es valida en el grafo
bool posicion_valida(const GrafoSecuencia& grafo, int i, int j) {
    return i >= 0 && i < grafo.filas && j >= 0 && j < grafo.columnas;
}

// Construir el grafo a partir de una secuencia de bases
GrafoSecuencia construir_grafo(const string& secuencia_bases, int ancho_linea) {
    GrafoSecuencia grafo;
    
    // Calcular dimensiones de la matriz
    int total_bases = secuencia_bases.size();
    grafo.columnas = ancho_linea;
    grafo.filas = (total_bases + ancho_linea - 1) / ancho_linea; // Redondeo hacia arriba
    
    // Inicializar la matriz de nodos
    grafo.matriz.resize(grafo.filas);
    for (int i = 0; i < grafo.filas; i++) {
        grafo.matriz[i].resize(grafo.columnas);
    }
    
    // Llenar la matriz con las bases
    for (int idx = 0; idx < total_bases; idx++) {
        int fila = idx / ancho_linea;
        int columna = idx % ancho_linea;
        grafo.matriz[fila][columna] = NodoGrafo(secuencia_bases[idx], fila, columna);
    }
    
    // Construir las conexiones (aristas) para cada nodo
    for (int i = 0; i < grafo.filas; i++) {
        for (int j = 0; j < grafo.columnas; j++) {
            // Verificar que este nodo tenga una base valida
            int idx = i * ancho_linea + j;
            if (idx >= total_bases) continue;
            
            char base_actual = grafo.matriz[i][j].base;
            
            // Conectar con vecino superior [i-1, j]
            if (i > 0) {
                int idx_superior = (i - 1) * ancho_linea + j;
                if (idx_superior < total_bases) {
                    char base_vecino = grafo.matriz[i-1][j].base;
                    double peso = calcular_peso_arista(base_actual, base_vecino);
                    grafo.matriz[i][j].vecinos.push_back(Arista(Posicion(i-1, j), peso));
                }
            }
            
            // Conectar con vecino inferior [i+1, j]
            if (i < grafo.filas - 1) {
                int idx_inferior = (i + 1) * ancho_linea + j;
                if (idx_inferior < total_bases) {
                    char base_vecino = grafo.matriz[i+1][j].base;
                    double peso = calcular_peso_arista(base_actual, base_vecino);
                    grafo.matriz[i][j].vecinos.push_back(Arista(Posicion(i+1, j), peso));
                }
            }
            
            // Conectar con vecino derecho [i, j-1] 
            if (j > 0) {
                int idx_derecho = i * ancho_linea + (j - 1);
                if (idx_derecho < total_bases) {
                    char base_vecino = grafo.matriz[i][j-1].base;
                    double peso = calcular_peso_arista(base_actual, base_vecino);
                    grafo.matriz[i][j].vecinos.push_back(Arista(Posicion(i, j-1), peso));
                }
            }
            
            // Conectar con vecino izquierdo [i, j+1] 
            if (j < grafo.columnas - 1) {
                int idx_izquierdo = i * ancho_linea + (j + 1);
                if (idx_izquierdo < total_bases) {
                    char base_vecino = grafo.matriz[i][j+1].base;
                    double peso = calcular_peso_arista(base_actual, base_vecino);
                    grafo.matriz[i][j].vecinos.push_back(Arista(Posicion(i, j+1), peso));
                }
            }
        }
    }
    
    return grafo;
}

// Implementacion del algoritmo de Dijkstra con busqueda lineal
ResultadoRuta dijkstra(const GrafoSecuencia& grafo, Posicion origen, Posicion destino) {
    ResultadoRuta resultado;
    
    // Validar posiciones
    if (!posicion_valida(grafo, origen.fila, origen.columna) ||
        !posicion_valida(grafo, destino.fila, destino.columna)) {
        return resultado;
    }
    
    // Matrices para almacenar distancias y predecesores
    vector<vector<double>> distancia(grafo.filas, vector<double>(grafo.columnas, INFINITO));
    vector<vector<Posicion>> predecesor(grafo.filas, vector<Posicion>(grafo.columnas, Posicion(-1, -1)));
    vector<vector<bool>> visitado(grafo.filas, vector<bool>(grafo.columnas, false));
    
    // Inicializar el nodo origen
    distancia[origen.fila][origen.columna] = 0.0;
    
    // Procesar todos los nodos
    for (int count = 0; count < grafo.filas * grafo.columnas; count++) {
        // Busqueda lineal: Encontrar el nodo no visitado con menor distancia
        Posicion actual(-1, -1);
        double min_distancia = INFINITO;
        
        for (int i = 0; i < grafo.filas; i++) {
            for (int j = 0; j < grafo.columnas; j++) {
                if (!visitado[i][j] && distancia[i][j] < min_distancia) {
                    min_distancia = distancia[i][j];
                    actual = Posicion(i, j);
                }
            }
        }
        
        // Si no hay mas nodos alcanzables, terminar
        if (actual.fila == -1) break;
        
        int i = actual.fila;
        int j = actual.columna;
        visitado[i][j] = true;
        
        // Si llegamos al destino, podemos terminar
        if (actual == destino) break;
        
        // Explorar vecinos
        const NodoGrafo& nodo = grafo.matriz[i][j];
        for (const Arista& arista : nodo.vecinos) {
            int vi = arista.destino.fila;
            int vj = arista.destino.columna;
            
            if (visitado[vi][vj]) continue;
            
            double nueva_distancia = distancia[i][j] + arista.peso;
            
            if (nueva_distancia < distancia[vi][vj]) {
                distancia[vi][vj] = nueva_distancia;
                predecesor[vi][vj] = Posicion(i, j);
            }
        }
    }
    
    // Verificar si existe un camino al destino
    if (distancia[destino.fila][destino.columna] == INFINITO) {
        return resultado;
    }
    
    // Reconstruir el camino desde destino hacia origen
    vector<Posicion> camino_inverso;
    Posicion actual = destino;
    
    while (!(actual == origen)) {
        camino_inverso.push_back(actual);
        actual = predecesor[actual.fila][actual.columna];
        
        // Verificacion de seguridad
        if (actual.fila == -1 && actual.columna == -1) {
            return resultado;
        }
    }
    camino_inverso.push_back(origen);
    
    // Invertir el camino para que vaya de origen a destino
    for (int i = camino_inverso.size() - 1; i >= 0; i--) {
        resultado.camino.push_back(camino_inverso[i]);
        Posicion& pos = camino_inverso[i];
        resultado.bases.push_back(grafo.matriz[pos.fila][pos.columna].base);
    }
    
    resultado.costo_total = distancia[destino.fila][destino.columna];
    resultado.existe = true;
    
    return resultado;
}

// Encontrar la base remota (misma letra, mas lejana)
Posicion encontrar_base_remota(const GrafoSecuencia& grafo, Posicion origen) {
    char base_buscada = grafo.matriz[origen.fila][origen.columna].base;
    Posicion mejor_remota(-1, -1);
    double mayor_distancia = -1.0;
    
    // Calcular distancias desde el origen a todos los nodos usando busqueda lineal
    vector<vector<double>> distancia(grafo.filas, vector<double>(grafo.columnas, INFINITO));
    vector<vector<bool>> visitado(grafo.filas, vector<bool>(grafo.columnas, false));
    
    distancia[origen.fila][origen.columna] = 0.0;
    
    // Procesar todos los nodos
    for (int count = 0; count < grafo.filas * grafo.columnas; count++) {
        // Busqueda lineal: Encontrar el nodo no visitado con menor distancia
        Posicion actual(-1, -1);
        double min_distancia = INFINITO;
        
        for (int i = 0; i < grafo.filas; i++) {
            for (int j = 0; j < grafo.columnas; j++) {
                if (!visitado[i][j] && distancia[i][j] < min_distancia) {
                    min_distancia = distancia[i][j];
                    actual = Posicion(i, j);
                }
            }
        }
        
        // Si no hay mas nodos alcanzables, terminar
        if (actual.fila == -1) break;
        
        int i = actual.fila;
        int j = actual.columna;
        visitado[i][j] = true;
        
        // Si encontramos la misma base y no es el origen
        if (grafo.matriz[i][j].base == base_buscada && !(actual == origen)) {
            if (distancia[i][j] > mayor_distancia) {
                mayor_distancia = distancia[i][j];
                mejor_remota = Posicion(i, j);
            }
        }
        
        // Explorar vecinos
        const NodoGrafo& nodo = grafo.matriz[i][j];
        for (const Arista& arista : nodo.vecinos) {
            int vi = arista.destino.fila;
            int vj = arista.destino.columna;
            
            if (visitado[vi][vj]) continue;
            
            double nueva_distancia = distancia[i][j] + arista.peso;
            
            if (nueva_distancia < distancia[vi][vj]) {
                distancia[vi][vj] = nueva_distancia;
            }
        }
    }
    
    return mejor_remota;
}

// Comando: ruta_mas_corta
void ruta_mas_corta(string descripcion, string i_str, string j_str, string x_str, string y_str) {
    // Verificar que hay secuencias cargadas
    if (secuencias.empty()) {
        cout << "No hay secuencias cargadas en memoria.\n";
        return;
    }
    
    // Buscar la secuencia
    int indice = -1;
    for (int idx = 0; idx < secuencias.size(); idx++) {
        if (secuencias[idx].descripcion == descripcion) {
            indice = idx;
            break;
        }
    }
    
    if (indice == -1) {
        cout << "La secuencia " << descripcion << " no existe.\n";
        return;
    }
    
    // Convertir strings a enteros
    int i, j, x, y;
    try {
        i = stoi(i_str);
        j = stoi(j_str);
        x = stoi(x_str);
        y = stoi(y_str);
    } catch (...) {
        cout << "Error: Las posiciones deben ser numeros enteros.\n";
        return;
    }
    
    // Construir el grafo de la secuencia
    GrafoSecuencia grafo = construir_grafo(secuencias[indice].bases, secuencias[indice].ancho_linea);
    
    // Validar posicion de origen
    if (!posicion_valida(grafo, i, j)) {
        cout << "La base en la posicion [" << i << "," << j << "] no existe.\n";
        return;
    }
    
    // Verificar que la posicion de origen tenga una base valida
    int idx_origen = i * grafo.columnas + j;
    if (idx_origen >= secuencias[indice].bases.size()) {
        cout << "La base en la posicion [" << i << "," << j << "] no existe.\n";
        return;
    }
    
    // Validar posicion de destino
    if (!posicion_valida(grafo, x, y)) {
        cout << "La base en la posicion [" << x << "," << y << "] no existe.\n";
        return;
    }
    
    // Verificar que la posicion de destino tenga una base valida
    int idx_destino = x * grafo.columnas + y;
    if (idx_destino >= secuencias[indice].bases.size()) {
        cout << "La base en la posicion [" << x << "," << y << "] no existe.\n";
        return;
    }
    
    // Calcular la ruta mas corta
    ResultadoRuta resultado = dijkstra(grafo, Posicion(i, j), Posicion(x, y));
    
    if (!resultado.existe) {
        cout << "No existe una ruta entre [" << i << "," << j << "] y [" << x << "," << y << "].\n";
        return;
    }
    
    // Imprimir resultado
    char base_origen = grafo.matriz[i][j].base;
    char base_destino = grafo.matriz[x][y].base;
    
    cout << "Para la secuencia " << descripcion 
         << ", la ruta mas corta entre la base " << base_origen 
         << " en [" << i << "," << j << "] y la base " << base_destino 
         << " en [" << x << "," << y << "] es: \n";
    
    // Imprimir la ruta
    for (int idx = 0; idx < resultado.camino.size(); idx++) {
        cout << resultado.bases[idx] << "[" << resultado.camino[idx].fila 
             << "," << resultado.camino[idx].columna << "]";
        if (idx < resultado.camino.size() - 1) {
            cout << " -> ";
        }
    }
    
    cout << ".\nEl costo total de la ruta es: " << resultado.costo_total << ".\n";
}

// Comando: base_remota
void base_remota(string descripcion, string i_str, string j_str) {
    // Verificar que hay secuencias cargadas
    if (secuencias.empty()) {
        cout << "No hay secuencias cargadas en memoria.\n";
        return;
    }
    
    // Buscar la secuencia
    int indice = -1;
    for (int idx = 0; idx < secuencias.size(); idx++) {
        if (secuencias[idx].descripcion == descripcion) {
            indice = idx;
            break;
        }
    }
    
    if (indice == -1) {
        cout << "La secuencia " << descripcion << " no existe.\n";
        return;
    }
    
    // Convertir strings a enteros
    int i, j;
    try {
        i = stoi(i_str);
        j = stoi(j_str);
    } catch (...) {
        cout << "Error: Las posiciones deben ser numeros enteros.\n";
        return;
    }
    
    // Construir el grafo de la secuencia
    GrafoSecuencia grafo = construir_grafo(secuencias[indice].bases, secuencias[indice].ancho_linea);
    
    // Validar posicion
    if (!posicion_valida(grafo, i, j)) {
        cout << "La base en la posicion [" << i << "," << j << "] no existe.\n";
        return;
    }
    
    // Verificar que la posicion tenga una base valida
    int idx_pos = i * grafo.columnas + j;
    if (idx_pos >= secuencias[indice].bases.size()) {
        cout << "La base en la posicion [" << i << "," << j << "] no existe.\n";
        return;
    }
    
    // Encontrar la base remota
    Posicion remota = encontrar_base_remota(grafo, Posicion(i, j));
    
    if (remota.fila == -1 || remota.columna == -1) {
        cout << "No se encontro otra base " << grafo.matriz[i][j].base 
             << " en la secuencia " << descripcion << ".\n";
        return;
    }
    
    // Calcular la ruta hacia la base remota
    ResultadoRuta resultado = dijkstra(grafo, Posicion(i, j), remota);
    
    if (!resultado.existe) {
        cout << "No existe una ruta hacia la base remota.\n";
        return;
    }
    
    // Imprimir resultado
    cout << "Para la secuencia " << descripcion 
         << ", la base remota esta ubicada en [" << remota.fila << "," << remota.columna 
         << "], y la ruta entre la base en [" << i << "," << j 
         << "] y la base remota en [" << remota.fila << "," << remota.columna << "] es: \n";
    
    // Imprimir la ruta
    for (int idx = 0; idx < resultado.camino.size(); idx++) {
        cout << resultado.bases[idx] << "[" << resultado.camino[idx].fila 
             << "," << resultado.camino[idx].columna << "]";
        if (idx < resultado.camino.size() - 1) {
            cout << " -> ";
        }
    }
    
    cout << ".\nEl costo total de la ruta es: " << resultado.costo_total << ".\n";
}

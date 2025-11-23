#include "secuencias.h"
#include "interfaz.h"
#include "huffman.h"
#include <iostream>

using namespace std;

int main() {
    string input;                  // Línea completa que escribe el usuario
    string partes[MAX_PARTES];    // Partes del comando separadas
    int numPartes;                // Número real de partes del comando

    // Mensaje inicial
    cout << "Bienvenido al sistema de manipulacion de secuencias geneticas.\n";
    cout << "Escribe 'ayuda' para ver los comandos disponibles.\n";

    // Bucle principal de la consola
    while (true) {
        cout << "$ ";              // Indicador de línea de comandos
        getline(cin, input);       // Leer la línea completa que escribe el usuario

        if (input.empty()) continue;  // Ignorar líneas vacías

        numPartes = dividir(input, partes); // Separar el comando en partes
        string comando = partes[0];         // Primer palabra = nombre del comando

        // Comando para salir del programa
        if (comando == "salir") {
            break;

        // Comando de ayuda general o específica
        } else if (comando == "ayuda") {
            if (numPartes == 1) {
                mostrar_ayuda_general();
            } else {
                mostrar_ayuda_comando(partes[1]);
            }

        // Comandos del Componente 1: Resumen de la información de un genoma
        } else if (comando == "cargar") {
            if (numPartes != 2) cout << "Error: Uso correcto -> cargar <archivo>\n";
            else cargar_archivo(partes[1]);

        } else if (comando == "listar_secuencias") {
            listar_secuencias();

        } else if (comando == "histograma") {
            if (numPartes != 2) cout << "Error: Uso correcto -> histograma <descripcion>\n";
            else histograma(partes[1]);

        } else if (comando == "es_subsecuencia") {
            if (numPartes != 2) cout << "Error: Uso correcto -> es_subsecuencia <sub>\n";
            else subsecuencia(partes[1]);

        } else if (comando == "enmascarar") {
            if (numPartes != 2) cout << "Error: Uso correcto -> enmascarar <sub>\n";
            else enmascarar(partes[1]);

        } else if (comando == "guardar") {
            if (numPartes != 2) cout << "Error: Uso correcto -> guardar <archivo>\n";
            else guardar_archivo(partes[1]);

        // Comandos del Componente 2: Codificación y decodificación Huffman
        } else if (comando == "codificar") {
            if (numPartes != 2) cout << "Error: Uso correcto -> codificar <archivo.fabin>\n";
            else codificar(partes[1]);

        } else if (comando == "decodificar") {
            if (numPartes != 2) cout << "Error: Uso correcto -> decodificar <archivo.fabin>\n";
            else decodificar(partes[1]);
            
        // Comandos del Componente 3: 
        } else if (comando == "ruta_mas_corta") {
            if (numPartes != 6) cout << "Error: Uso correcto -> ruta_mas_corta <desc> <i> <j> <x> <y>\n";
            else cout << "Simulación: Ruta más corta desde [" << partes[2] << "," << partes[3]
                      << "] hasta [" << partes[4] << "," << partes[5] << "] en " << partes[1] << "\n";

        } else if (comando == "base_remota") {
            if (numPartes != 4) cout << "Error: Uso correcto -> base_remota <desc> <i> <j>\n";
            else cout << "Simulación: Base remota desde [" << partes[2] << "," << partes[3]
                      << "] en " << partes[1] << "\n";

        // Cualquier otro comando que no se reconozca
        } else {
            cerr << "Error: Comando no reconocido. Escribe 'ayuda' para ver los comandos válidos.\n";
        }
    }

    // Fin del programa
    cout << "Programa finalizado.\n";
    return 0;
}

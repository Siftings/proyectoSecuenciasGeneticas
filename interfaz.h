#ifndef INTERFAZ_H
#define INTERFAZ_H

#include <string>
using namespace std;
// Const partes
const int MAX_PARTES = 10;
const int NUM_COMANDOS = 12; 
// Declaraciones de funciones para la interfaz de usuario
int dividir(const string& input, string partes[]);
void mostrar_ayuda_general();
void mostrar_ayuda_comando(const string& comando);

#endif 

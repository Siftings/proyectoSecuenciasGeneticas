# Proyecto de Estructuras de Datos

Pontificia Universidad Javeriana   
Curso: Estructuras de Datos  
Docente: Alejandro Castro, MSc.  
Autor: Juan David Daza Caro

## Descripcion General

Este proyecto implementa un sistema completo de manipulacion y analisis de secuencias geneticas en formato FASTA, con tres componentes principales:

1. **Componente 1**: Operaciones basicas con secuencias geneticas
2. **Componente 2**: Codificacion y decodificacion usando arboles de Huffman
3. **Componente 3**: Analisis de relaciones entre bases usando grafos

## Compilacion

```bash
make clean
make
```

## Ejecucion

```bash
./programa
```
## Componente 1: Operaciones con Secuencias Geneticas

### Concepto

Carga y manipula archivos FASTA que contienen secuencias de ADN con:
- Soporte para codigos ambiguos 
- Lectura de archivos con formato FASTA estandar
- Almacenamiento en memoria de multiples secuencias
- Preservacion del nombre de la secuencia
- Preservacion del ancho de linea original

## Componente 2: Codificacion con Arbol de Huffman

### Concepto

Utiliza codificacion de Huffman para comprimir informacion de secuencias geneticas de un archivo `.fa` a un `.fabin`:
- Construccion de arbol binario basado en frecuencias
- Asignacion de codigos binarios mas cortos a bases mas frecuentes
- Formato binario personalizado (.fabin)
- Preservacion completa de informacion

### Funcionamiento del Arbol de Huffman

1. **Conteo de frecuencias**: Analiza cuantas veces aparece cada base
2. **Construccion del arbol**: 
   - Crea nodos hoja para cada base
   - Combina nodos de menor frecuencia
   - Construye arbol binario de abajo hacia arriba
3. **Generacion de codigos**:
   - Izquierda = '0', Derecha = '1'
   - Bases frecuentes = codigos cortos
   - Bases raras = codigos largos

## Componente 3: Grafos y Rutas 

### Concepto

Cada secuencia genetica se representa como una matriz donde:
- Cada fila corresponde a una linea del archivo FASTA
- Cada columna corresponde a posicion de la base en una fila (# de bases en fila = # de columnas)
- Las bases estan organizadas desde [0,0] (esquina superior izquierda)

Esta matriz se transforma en un grafo donde:
- Cada base es un vertice
- Cada vertice se conecta con sus 4 vecinos: superior, inferior, izquierdo, derecho
- Cada arista tiene un peso calculado por: **Peso = 1 / (1 + |ASCII_i - ASCII_j|)**

Las funciones princiales utilizan el algoritmo de Dijkstra para encontrar la ruta con mayor y menor peso entre vertices del grafo.


## Comandos Completos del Sistema

### Componente 1 - Secuencias
- `cargar <archivo>`: Carga archivo FASTA
- `listar_secuencias`: Lista secuencias en memoria
- `histograma <descripcion>`: Muestra frecuencias de bases
- `es_subsecuencia <sub>`: Busca subsecuencia
- `enmascarar <sub>`: Enmascara subsecuencia con 'X'
- `guardar <archivo>`: Guarda secuencias modificadas

### Componente 2 - Arbol de Huffman
- `codificar <archivo.fabin>`: Codifica a formato binario
- `decodificar <archivo.fabin>`: Decodifica desde binario

### Componente 3 - Grafos
- `ruta_mas_corta <desc> <i> <j> <x> <y>`: Ruta optima entre bases
- `base_remota <desc> <i> <j>`: Base mas lejana del mismo tipo

### Sistema
- `ayuda [comando]`: Muestra ayuda general o especifica
- `salir`: Termina el programa

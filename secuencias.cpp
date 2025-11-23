#include "secuencias.h"

// Definición de la variable global
vector<Secuencia> secuencias;

// Función auxiliar para verificar si un carácter es válido según la Tabla 1
bool es_base_valida(char base) {
    string bases_validas = "ACGTURYKMSWBDHVNX-";
    for (int i = 0; i < bases_validas.size(); i++) {
        if (base == bases_validas[i]) {
            return true;
        }
    }
    return false;
}

// Función auxiliar para obtener el número mínimo de bases que representa un código
int obtener_bases_minimas(char codigo) {
    switch (codigo) {
        case 'A': case 'C': case 'G': case 'T': case 'U': case 'X':
            return 1;  // Bases específicas y máscara
        case 'R': case 'Y': case 'K': case 'M': case 'S': case 'W':
            return 1;  // Códigos ambiguos de 2 bases, representan 1 posición
        case 'B': case 'D': case 'H': case 'V':
            return 1;  // Códigos ambiguos de 3 bases, representan 1 posición
        case 'N':
            return 1;  // Código ambiguo de todas las bases, representa 1 posición
        case '-':
            return 0;  // Espacio de longitud indeterminada no cuenta
        default:
            return 0;  // Carácter no válido
    }
}

// Función para verificar si un código de secuencia puede coincidir con un código literal de subsecuencia
bool son_compatibles(char codigo_secuencia, char codigo_subsecuencia) {
    // Si son iguales, siempre son compatibles
    if (codigo_secuencia == codigo_subsecuencia) {
        return true;
    }
    
    // La máscara X en la secuencia coincide con cualquier cosa
    if (codigo_secuencia == 'X') {
        return true;
    }
    
    // Los espacios solo coinciden con espacios
    if (codigo_secuencia == '-' || codigo_subsecuencia == '-') {
        return codigo_secuencia == codigo_subsecuencia;
    }
    
    // Si la subsecuencia tiene X, coincide con cualquier cosa
    if (codigo_subsecuencia == 'X') {
        return true;
    }
    
    // REGLA CLAVE: La subsecuencia se trata como LITERAL
    // Solo verificamos si el código de secuencia (que puede ser ambiguo)
    // puede representar el código literal de la subsecuencia
    
    // Obtener las bases que puede representar el código de SECUENCIA
    string bases_secuencia = "";
    
    switch (codigo_secuencia) {
        case 'A': bases_secuencia = "A"; break;
        case 'C': bases_secuencia = "C"; break;
        case 'G': bases_secuencia = "G"; break;
        case 'T': bases_secuencia = "T"; break;
        case 'U': bases_secuencia = "U"; break;
        case 'R': bases_secuencia = "AG"; break;        // A o G
        case 'Y': bases_secuencia = "CTU"; break;       // C, T o U
        case 'K': bases_secuencia = "GTU"; break;       // G, T o U
        case 'M': bases_secuencia = "AC"; break;        // A o C
        case 'S': bases_secuencia = "CG"; break;        // C o G
        case 'W': bases_secuencia = "ATU"; break;       // A, T o U
        case 'B': bases_secuencia = "CGTU"; break;      // C, G, T o U
        case 'D': bases_secuencia = "AGTU"; break;      // A, G, T o U
        case 'H': bases_secuencia = "ACTU"; break;      // A, C, T o U
        case 'V': bases_secuencia = "ACG"; break;       // A, C o G
        case 'N': bases_secuencia = "ACGTU"; break;     // A, C, G, T o U
        default: return false;
    }
    
    // Verificar si el código LITERAL de la subsecuencia está en las bases 
    // que puede representar el código de secuencia
    for (int i = 0; i < bases_secuencia.size(); i++) {
        if (bases_secuencia[i] == codigo_subsecuencia) {
            return true;
        }
    }
    
    return false; // El código de secuencia no puede representar el código literal
}

// Función para cargar un archivo FASTA
void cargar_archivo(string nombreArchivo) {
    ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        cout << nombreArchivo << " no se encuentra o no puede leerse.\n";
        return;
    }

    secuencias.clear(); // Se reemplazan secuencias anteriores

    string linea, descripcion, bases = "";
    int ancho_linea = 80; // Valor por defecto
    bool primera_linea_bases = true; // Para detectar ancho de la primera línea
    
    while (getline(archivo, linea)) {

        if (linea[0] == '>') {
            // Si ya había una secuencia, guardarla antes de iniciar otra
            if (!descripcion.empty()) {
                secuencias.push_back({descripcion, bases, ancho_linea});
                bases.clear();
                primera_linea_bases = true; // Reiniciar para nueva secuencia
                ancho_linea = 80; // Resetear a default
            }
            descripcion = linea.substr(1); // quitar el '>'
        } 
        else {
            // Validar y filtrar caracteres según la Tabla 1
            string linea_filtrada = "";
            for (int i = 0; i < linea.size(); i++) {
                char c = toupper(linea[i]); // Convertir a mayúscula
                if (es_base_valida(c)) {
                    linea_filtrada += c;
                }
                // Ignorar caracteres no válidos (espacios, números, etc.)
            }
            
            // Detectar ancho de línea de la primera línea de bases
            if (primera_linea_bases && !linea_filtrada.empty()) {
                ancho_linea = linea_filtrada.size();
                primera_linea_bases = false;
            }
            
            bases += linea_filtrada;
        }
    }

    // Guardar la última secuencia si existe
    if (!descripcion.empty()) {
        secuencias.push_back({descripcion, bases, ancho_linea});
    }

    if (secuencias.empty()) {
        cout << nombreArchivo << " no contiene ninguna secuencia.\n";
        return;
    }

    if (secuencias.size() == 1) {
        cout << "1 secuencia cargada correctamente desde " << nombreArchivo << ".\n";
        return;
    } else {
        cout << secuencias.size() << " secuencias cargadas correctamente desde " 
             << nombreArchivo << ".\n";
        return;
    }
}

void listar_secuencias() {
    if (secuencias.empty()) {
        cout << "No hay secuencias cargadas en memoria.\n";
        return;
    }
    cout << "Hay " << secuencias.size() << " secuencias cargadas en memoria:\n";

    // Recorremos cada secuencia por índice
    for (int i = 0; i < secuencias.size(); i++) {
        int guiones = 0;
        int bases_minimas = 0;

        // Contar guiones y bases mínimas en la secuencia actual
        for (int j = 0; j < secuencias[i].bases.size(); j++) {
            char base = secuencias[i].bases[j];
            if (base == '-') {
                guiones++;
            } else {
                bases_minimas += obtener_bases_minimas(base);
            }
        }

        if (guiones == 0) {
            // Secuencia completa (sin guiones)
            cout << "Secuencia " << secuencias[i].descripcion
                 << " contiene " << bases_minimas << " bases.\n";
        } else {
            // Secuencia incompleta (con guiones)
            cout << "Secuencia " << secuencias[i].descripcion
                 << " contiene al menos " << bases_minimas << " bases.\n";
        }
    }
}

void histograma(string descripcion) {
    if (secuencias.empty()) {
        cout << "Secuencia inválida.\n";
        return;
    }

    // Buscar la secuencia en memoria
    int indice = -1;
    for (int i = 0; i < secuencias.size(); i++) {
        if (secuencias[i].descripcion == descripcion) {
            indice = i;
            break;
        }
    }

    if (indice == -1) {
        cout << "Secuencia inválida.\n";
        return;
    }

    // Símbolos en el orden de la tabla
    string simbolos = "ACGTURYKMSWBDHVNX-";
    int frecuencia[18] = {0};

    // Contar frecuencias
    for (int j = 0; j < secuencias[indice].bases.size(); j++) {
        char base = secuencias[indice].bases[j];
        for (int k = 0; k < simbolos.size(); k++) {
            if (base == simbolos[k]) {
                frecuencia[k]++;
                break;
            }
        }
    }

    // Imprimir resultados
    for (int k = 0; k < simbolos.size(); k++) {
        cout << simbolos[k] << " : " << frecuencia[k] << "\n";
    }
}

void subsecuencia(string sub) {
    if (secuencias.empty()) {
        cout << "No hay secuencias cargadas en memoria.\n";
        return;
    }

    int total = 0;

    // Recorremos todas las secuencias cargadas
    for (int i = 0; i < secuencias.size(); i++) {
        string texto = secuencias[i].bases;

        // Buscar la subsecuencia usando compatibilidad biológica
        for (int j = 0; j <= (texto.size() - sub.size()); j++) {
            bool coincide = true;

            // Comparar carácter por carácter usando compatibilidad
            for (int k = 0; k < sub.size(); k++) {
                if (!son_compatibles(texto[j + k], sub[k])) {
                    coincide = false;
                    break;
                }
            }

            if (coincide) {
                total++;
            }
        }
    }

    if (total == 0) {
        cout << "La subsecuencia dada no existe dentro de las secuencias cargadas en memoria.\n";
    } else {
        cout << "La subsecuencia dada se repite " << total
             << " veces dentro de las secuencias cargadas en memoria.\n";
    }
}

void enmascarar(string sub) {
    if (secuencias.empty()) {
        cout << "No hay secuencias cargadas en memoria.\n";
        return;
    }

    int total = 0;

    // Recorremos todas las secuencias cargadas
    for (int i = 0; i < secuencias.size(); i++) {
        string& texto = secuencias[i].bases; // Referencia para modificar directamente

        // Buscar y enmascarar todas las ocurrencias usando compatibilidad biológica
        for (int j = 0; j <= (texto.size() - sub.size()); j++) {
            bool coincide = true;

            // Comparar carácter por carácter usando compatibilidad
            for (int k = 0; k < sub.size(); k++) {
                if (!son_compatibles(texto[j + k], sub[k])) {
                    coincide = false;
                    break;
                }
            }

            if (coincide) {
                // Enmascarar reemplazando cada carácter por 'X'
                for (int k = 0; k < sub.size(); k++) {
                    texto[j + k] = 'X';
                }
                total++;
                j += sub.size() - 1; // Saltar los caracteres enmascarados
            }
        }
    }

    if (total == 0) {
        cout << "La subsecuencia dada no existe dentro de las secuencias cargadas en memoria, por tanto no se enmascara nada.\n";
    } else {
        cout << total << " subsecuencias han sido enmascaradas dentro de las secuencias cargadas en memoria.\n";
    }
}

void guardar_archivo(string nombreArchivo) {
    if (secuencias.empty()) {
        cout << "No hay secuencias cargadas en memoria.\n";
        return;
    }

    ofstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        cout << "Error guardando en " << nombreArchivo << ".\n";
        return;
    }

    for (int i = 0; i < secuencias.size(); i++) {
        archivo << ">" << secuencias[i].descripcion << "\n";
        // Escribir las bases en lineas del mismo tamaño
        string bases = secuencias[i].bases;
        int anchoPorLinea = secuencias[i].ancho_linea; // Usar ancho original
        
        for (int j = 0; j < bases.size(); j += anchoPorLinea) {
            string linea = bases.substr(j, anchoPorLinea);
            archivo << linea << "\n";
        }
    }

    archivo.close();
    cout << "Las secuencias han sido guardadas en " << nombreArchivo << ".\n";
}

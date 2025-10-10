#include <iostream>
#include <cstring>  // Para strcpy y manejo de cadenas
using namespace std;

// --- ESTRUCTURA DEL LOTE ---
typedef struct {
    int idLote;
    char nombreComponente[50];
    float pesoUnitario;
    int cantidadTotal;
} LoteProduccion;

// --- VARIABLES GLOBALES ---

// Matriz 2D dinámica (almacén)
LoteProduccion **almacen = nullptr;
int filas, columnas;

// Vectores paralelos dinámicos (maestro de lotes)
LoteProduccion *maestroLotes = nullptr;
int *indicesDisponibles = nullptr;
int capacidadMaestro = 100;
int totalLotes = 0;

// Pila dinámica (historial de inspecciones)
int *pilaIDLote = nullptr;
int *pilaResultado = nullptr;
int top = -1;  // índice superior (último elemento)
int capacidadPila = 10;

// --- FUNCIONES ---

// Inicialización del sistema
void inicializarSistema() {
    cout << "Fila inicial: ";
    cin >> filas;
    cout << "Columna inicial: ";
    cin >> columnas;

    // Crear matriz dinámica 2D
    almacen = new LoteProduccion*[filas];
    for (int i = 0; i < filas; i++) {
        almacen[i] = new LoteProduccion[columnas];
        for (int j = 0; j < columnas; j++) {
            almacen[i][j].idLote = 0; // celda vacía
        }
    }

    // Crear vectores paralelos dinámicos
    maestroLotes = new LoteProduccion[capacidadMaestro];
    indicesDisponibles = new int[capacidadMaestro];
    for (int i = 0; i < capacidadMaestro; i++) {
        indicesDisponibles[i] = 0; // 0 = libre
    }

    // Crear pila dinámica
    pilaIDLote = new int[capacidadPila];
    pilaResultado = new int[capacidadPila];

    cout << "\n--- ALPHATEC---\n";
    cout << "Inicializacion exitosa. Almacén " << filas << "x" << columnas 
         << " creado. Capacidad Maestro: " << capacidadMaestro << ".\n\n";
}

// Colocar lote en el almacén
void colocarLote() {
    int f, c;
    cout << "Posición (F C): ";
    cin >> f >> c;

    if (f < 0 || f >= filas || c < 0 || c >= columnas) {
        cout << "PosiciOn inválida.\n\n";
        return;
    }

    if (almacen[f][c].idLote != 0) {
        cout << "Esa celda ya esta ocupada.\n\n";
        return;
    }

    // Buscar índice libre en el maestro de lotes
    int indiceLibre = -1;
    for (int i = 0; i < capacidadMaestro; i++) {
        if (indicesDisponibles[i] == 0) {
            indiceLibre = i;
            break;
        }
    }

    if (indiceLibre == -1) {
        cout << "Capacidad llena en maestro de lotes.\n\n";
        return;
    }

    // Capturar datos del nuevo lote
    LoteProduccion nuevo;
    cout << "ID: ";
    cin >> nuevo.idLote;
    cout << "Nombre: ";
    cin.ignore();
    cin.getline(nuevo.nombreComponente, 50);
    cout << "Peso Unitario: ";
    cin >> nuevo.pesoUnitario;

    // Guardar en el maestro y marcar como ocupado
    maestroLotes[indiceLibre] = nuevo;
    indicesDisponibles[indiceLibre] = 1;
    totalLotes++;

    // Colocar en el almacén
    almacen[f][c] = nuevo;

    cout << "Lote " << nuevo.idLote << " (" << nuevo.nombreComponente
         << ") registrado y colocado en (" << f << ", " << c << ").\n\n";
}

// Control de calidad (Push a pila)
void controlCalidad() {
    int id, resultado;
    cout << "ID de Lote a inspeccionar: ";
    cin >> id;

    bool encontrado = false;
    for (int i = 0; i < capacidadMaestro; i++) {
        if (indicesDisponibles[i] == 1 && maestroLotes[i].idLote == id) {
            encontrado = true;

            cout << "Resultado (1=Aprobado, 0=Rechazado): ";
            cin >> resultado;

            if (top == capacidadPila - 1) {
                // Si la pila está llena, rotamos (buffer circular)
                for (int j = 1; j < capacidadPila; j++) {
                    pilaIDLote[j - 1] = pilaIDLote[j];
                    pilaResultado[j - 1] = pilaResultado[j];
                }
                top--;
            }

            // Insertar nuevo registro (push)
            pilaIDLote[++top] = id;
            pilaResultado[top] = resultado;

            cout << "Inspeccion Lote " << id << " (" << maestroLotes[i].nombreComponente
                 << "): Resultado " << (resultado == 1 ? "Aprobado (1)" : "Rechazado (0)") << ".\n";
            cout << "Evento PUSH a Pila: Lote " << id << " | Resultado " << resultado << ".\n\n";
            break;
        }
    }

    if (!encontrado) {
        cout << "Lote no encontrado\n\n";
    }
}

// Deshacer (Pop de pila)
void deshacer() {
    if (top == -1) {
        cout << "Pila vacia\n\n";
        return;
    }

    cout << "POP de Pila: Evento Lote " << pilaIDLote[top]
         << " | Resultado " << pilaResultado[top] << ".\n";
    cout << "Historial de inspeccion revertido\n\n";
    top--;
}

// Reporte de una fila
void reporteFila() {
    int f;
    cout << "Fila a reportar: ";
    cin >> f;

    if (f < 0 || f >= filas) {
        cout << "Fila invalida\n\n";
        return;
    }

    cout << "--- Reporte de Fila " << f << " ---\n";
    for (int c = 0; c < columnas; c++) {
        if (almacen[f][c].idLote != 0)
            cout << "(" << f << ", " << c << "): ID: "
                 << almacen[f][c].idLote << ", Nombre: "
                 << almacen[f][c].nombreComponente << "\n";
        else
            cout << "(" << f << ", " << c << "): Vacio\n";
    }
    cout << endl;
}

// Liberar toda la memoria dinámica
void liberarMemoria() {
    cout << "Liberando memoria de estructuras LoteProduccion...\n";
    delete[] maestroLotes;
    delete[] indicesDisponibles;

    cout << "Liberando memoria de Matriz 2D...\n";
    for (int i = 0; i < filas; i++) {
        delete[] almacen[i];
    }
    delete[] almacen;

    cout << "Liberando memoria de Pila y Vectores Paralelos...\n";
    delete[] pilaIDLote;
    delete[] pilaResultado;

    cout << "La memoria de liberó. Sistema terminado\n";
}

// --- FUNCIÓN PRINCIPAL ---
int main() {
    inicializarSistema();

    int opcion;
    do {
        cout << "--- Menu Principal ---\n";
        cout << "1. Inicializar Sistema\n";
        cout << "2. Colocar Lote\n";
        cout << "3. Control de Calidad (Push)\n";
        cout << "4. Deshacer (Pop)\n";
        cout << "5. Reporte por Fila\n";
        cout << "7. Salir\n";
        cout << "Opcion seleccionada: ";
        cin >> opcion;
        cout << endl;

        switch (opcion) {
            case 1: 
            inicializarSistema();
            break;
            case 2: 
            colocarLote(); 
            break;
            case 3: 
            controlCalidad(); 
            break;
            case 4: 
            deshacer(); 
            break;
            case 5: 
            reporteFila();
            break;
            case 7: 
            liberarMemoria();
            break;
            default: 
            cout << "Opcion invalida.\n\n"; 
            break;
        }
    } while (opcion != 7);

    return 0;
}

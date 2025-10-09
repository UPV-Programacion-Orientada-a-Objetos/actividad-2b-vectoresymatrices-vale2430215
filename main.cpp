#include <iostream>

typedef struct {
 int idLote;
 char nombreComponente[50];
 float pesoUnitario;
 int cantidadTotal;
} LoteProduccion;

int main() {
    int filas;
    int columnas;

    std::cout<<"Ingresa el numero de filas: "<<std::endl;
    std::cin>>filas;
    std::cout<<"Ingresa el numero de columnas: "<<std::endl;
    std::cin>>columnas;

    //matriz dinamica de punteros, arreglo de filas
    LoteProduccion*** almacen = new LoteProduccion**[filas];

    for(int i=0; i<filas; i++){
        almacen[i] =new LoteProduccion*[columnas]; //arreglo de columnas
        for(int j=0; j<columnas; j++){
            almacen[i][j]=nullptr;
        }
    }

    int capacidadLotes =100;
    int nLotes=0;

    int* ids = new int[capacidadLotes];
    char (*nombres)[50] = new char[capacidadLotes][50];
    float* pesos = new float[capacidadLotes];
    int* cantidades = new int[capacidadLotes];

    std::cout << "Vectores paralelos creados con capacidad para " << capacidadLotes << " lotes.\n";

    //pila dinamica
    int* pila=new int[capacidadLotes];//capacidad igual a la del maestro
    int vacio=-1;

    //liberar memoria
    for (int i = 0; i < filas; i++){
        delete[] almacen[i];
    }

    delete[] almacen;

    delete[] ids;
    delete[] nombres;
    delete[] pesos;
    delete[] cantidades;
    delete[] pila;
    

    return 0;
}
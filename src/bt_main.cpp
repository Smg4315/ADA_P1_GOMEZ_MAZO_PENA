/*
PSEUDOCODIGO GENERAL DEL MODULO BT (BACKTRACKING)

resolverBacktracking(prefijo, contadores, politica, alfabeto):
    si tamaño(prefijo) == politica.n:
        si contadores cumplen los minimos de la politica:
            guardar prefijo como solucion
        retornar

    si NO esFactible(prefijo, contadores, politica):
        retornar   // PODA: se abandona la rama sin explorar descendientes

    para cada caracter en alfabeto:
        si caracter == ultimoCaracter y politica prohibe repetidos:
            continuar
        actualizar contadores segun el tipo de caracter
        resolverBacktracking(prefijo + caracter, contadores, politica, alfabeto)
*/

#include "bt_policy.hpp"
#include "bt_backtrack.hpp"
#include "bt_sinpoda.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <chrono> // Biblioteca para medir el tiempo de ejecucion
#include <fstream> // Biblioteca para escribir en archivos

using namespace std;

const string alfabetoBT = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%";

// Apellidos del equipo, en orden alfabetico 
const vector<string> apellidosEquipo = {"gomez", "mazo", "pena"};

void casosPrueba() {

    int semilla = calcularSemilla(apellidosEquipo);
    cout << "Semilla generada: " << semilla << endl;

    Policy politicaEquipo = derivarPolitica(semilla, 8);
    cout << "\nPolitica del equipo (n=8): minLower=" << politicaEquipo.minLower
         << " minUpper=" << politicaEquipo.minUpper
         << " minDigit=" << politicaEquipo.minDigit
         << " minSymbol=" << politicaEquipo.minSymbol << endl;

    // Instancia de referencia comun, para validar la implementacion
    Policy referencia;
    referencia.n = 6;
    referencia.minLower = 2;
    referencia.minUpper = 1;
    referencia.minDigit = 1;
    referencia.minSymbol = 1;
    referencia.noRepetidosConsecutivos = true;

    cout << "\nValidando contra la instancia de referencia comun (n=6)..." << endl;

    long long nodosVisitados = 0;
    long long totalSoluciones = contarSoluciones(referencia, alfabetoBT, nodosVisitados);

    cout << "Nodos visitados: " << nodosVisitados << endl;
    cout << "Soluciones totales: " << totalSoluciones << endl;

    return;
}

// Funcion para hacer las pruebas de las 5 variantes de dificultad
// especificadas en la Seccion 9.2, comparando la version con poda contra
// la version sin poda.
void pruebasVariantes() {

    int semilla = calcularSemilla(apellidosEquipo);
    Policy politicaEquipo = derivarPolitica(semilla, 8);

    // Vector de las 5 variantes de dificultad (Seccion 9.2)
    vector<pair<string, Policy>> variantes;

    Policy v1 = politicaEquipo;
    variantes.push_back({"(i) Politica completa del equipo, n=8", v1});

    Policy v2 = politicaEquipo;
    v2.n = 6;
    variantes.push_back({"(ii) Misma politica, n=6", v2});

    Policy v3 = politicaEquipo;
    v3.n = 10;
    variantes.push_back({"(iii) Misma politica, n=10", v3});

    Policy v4;
    v4.n = 8;
    v4.minLower = 1;
    v4.minUpper = 0;
    v4.minDigit = 0;
    v4.minSymbol = 0;
    v4.noRepetidosConsecutivos = true;
    variantes.push_back({"(iv) Politica relajada (solo minLower=1), n=8", v4});

    Policy v5;
    v5.n = 6;
    v5.minLower = 0;
    v5.minUpper = 0;
    v5.minDigit = 0;
    v5.minSymbol = 0;
    v5.noRepetidosConsecutivos = false;
    variantes.push_back({"(v) Sin restricciones, n=6 (poda nula)", v5});

    ofstream archivoSalida("results/bt_resultados.csv");

    if (!archivoSalida.is_open()) {
        cerr << "No se pudo crear el archivo de metricas." << endl;
        return;
    }

    archivoSalida << "variante,n,nodos_con_poda,nodos_sin_poda,soluciones_con_poda,soluciones_sin_poda,tiempo_con_poda_s,tiempo_sin_poda_s" << endl;

    for (const auto& variante : variantes) {
        string nombre = variante.first;
        Policy p = variante.second;

        cout << "\n--- " << nombre << " ---" << endl;

        // Version CON PODA (modo conteo, para no agotar memoria)
        long long nodosConPoda = 0;
        auto inicioConPoda = chrono::high_resolution_clock::now();
        long long solucionesConPoda = contarSoluciones(p, alfabetoBT, nodosConPoda);
        auto finConPoda = chrono::high_resolution_clock::now();
        chrono::duration<double> tiempoConPoda = finConPoda - inicioConPoda;

        cout << "Con poda -> nodos: " << nodosConPoda
             << ", soluciones: " << solucionesConPoda
             << ", tiempo: " << tiempoConPoda.count() << "s" << endl;

        // Version SIN PODA
        long long nodosSinPoda = 0;
        auto inicioSinPoda = chrono::high_resolution_clock::now();
        vector<string> solucionesSinPoda = resolverSinPoda(p, alfabetoBT, nodosSinPoda);
        auto finSinPoda = chrono::high_resolution_clock::now();
        chrono::duration<double> tiempoSinPoda = finSinPoda - inicioSinPoda;

        cout << "Sin poda -> nodos: " << nodosSinPoda
             << ", soluciones: " << solucionesSinPoda.size()
             << ", tiempo: " << tiempoSinPoda.count() << "s" << endl;

        if (solucionesConPoda == (long long)solucionesSinPoda.size()) {
            cout << "Verificacion de correctitud: OK (mismo numero de soluciones)" << endl;
        } else {
            cout << "Verificacion de correctitud: FALLO (numeros distintos)" << endl;
        }

        archivoSalida << nombre << "," << p.n << ","
                      << nodosConPoda << "," << nodosSinPoda << ","
                      << solucionesConPoda << "," << solucionesSinPoda.size() << ","
                      << tiempoConPoda.count() << "," << tiempoSinPoda.count() << endl;
    }

    archivoSalida.close();
    cout << "\nResultados guardados en results/bt_resultados.csv" << endl;

    return;
}

int main() {

    while (true) {
        int opcion;

        cout << "" << endl;
        cout << "Ingrese la opcion que le gustaria hacer" << endl;
        cout << "1. Revisar casos de prueba (instancia de referencia)" << endl;
        cout << "2. Hacer pruebas de las 5 variantes (con poda vs sin poda)" << endl;
        cout << "3. Salir" << endl;

        cin >> opcion;

        if (opcion == 1) {
            casosPrueba();
        } else if (opcion == 2) {
            pruebasVariantes();
            cout << "" << endl;
        } else if (opcion == 3) {
            cout << "Saliendo del programa..." << endl;
            cout << "" << endl;
            return 0;
        } else {
            cout << "Opcion invalida. Por favor, ingrese 1, 2 o 3" << endl;
            cout << "" << endl;
        }
    }
}
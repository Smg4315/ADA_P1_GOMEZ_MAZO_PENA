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

// -> Modelamiento del problema <-
// Alfabeto base del Modulo BT: minusculas, mayusculas,
// digitos, y el conjunto de simbolos {!, @, #, $, %}.

const string alfabetoBT = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%";

// Apellidos del equipo, en orden alfabetico (Seccion 9.1/9.2)
const vector<string> apellidosEquipo = {"gomez", "mazo", "pena"};

// Funcion para hacer el caso de prueba de la instancia de referencia
// comun: minLower=2, minUpper=1, minDigit=1, minSymbol=1,
// sin repetidos consecutivos, n=6, alfabeto base completo.
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

void pruebasVariantes(bool usarLimite, long long maxNodos) {

    int semilla = calcularSemilla(apellidosEquipo);
    Policy politicaEquipo = derivarPolitica(semilla, 8);

    // Vector de las 5 variantes de dificultad
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

    archivoSalida << "variante,n,nodos_con_poda,nodos_sin_poda,soluciones_con_poda,soluciones_sin_poda,tiempo_con_poda_s,tiempo_sin_poda_s,truncado" << endl;

    cout << "\n----------------------------------------------------------------------------------------------------------------------" << endl;
    cout << "|  Variante                                        |  n  |  Nodos con poda  |  Nodos sin poda  |  Tiempo con poda (s)  |  Tiempo sin poda (s)  |" << endl;
    cout << "----------------------------------------------------------------------------------------------------------------------" << endl;

    for (const auto& variante : variantes) {
        string nombre = variante.first;
        Policy p = variante.second;

        // Version CON PODA (modo conteo, para no agotar memoria)
        long long nodosConPoda = 0;
        auto inicioConPoda = chrono::high_resolution_clock::now();
        long long solucionesConPoda = usarLimite
            ? contarSoluciones(p, alfabetoBT, nodosConPoda, maxNodos)
            : contarSoluciones(p, alfabetoBT, nodosConPoda);
        auto finConPoda = chrono::high_resolution_clock::now();
        chrono::duration<double> tiempoConPoda = finConPoda - inicioConPoda;

        // Version SIN PODA
        long long nodosSinPoda = 0;
        auto inicioSinPoda = chrono::high_resolution_clock::now();
        vector<string> solucionesSinPoda = usarLimite
            ? resolverSinPoda(p, alfabetoBT, nodosSinPoda, maxNodos)
            : resolverSinPoda(p, alfabetoBT, nodosSinPoda);
        auto finSinPoda = chrono::high_resolution_clock::now();
        chrono::duration<double> tiempoSinPoda = finSinPoda - inicioSinPoda;

        bool truncado = usarLimite && (nodosConPoda >= maxNodos || nodosSinPoda >= maxNodos);
        bool correcto = (solucionesConPoda == (long long)solucionesSinPoda.size());

        // Fila de la tabla en consola
        cout << "|  " << nombre;
        for (int i = 0; i < (int)(48 - nombre.size()) && i >= 0; i++) cout << " ";
        cout << "|  " << p.n << "  |  " << nodosConPoda;
        for (int i = 0; i < (int)(17 - to_string(nodosConPoda).size()) && i >= 0; i++) cout << " ";
        cout << "|  " << nodosSinPoda;
        for (int i = 0; i < (int)(17 - to_string(nodosSinPoda).size()) && i >= 0; i++) cout << " ";
        cout << "|  " << tiempoConPoda.count();
        for (int i = 0; i < (int)(21 - to_string(tiempoConPoda.count()).size()) && i >= 0; i++) cout << " ";
        cout << "|  " << tiempoSinPoda.count() << "  |" << endl;

        if (truncado) {
            cout << "   AVISO: se alcanzo el limite de " << maxNodos
                 << " nodos, resultados PARCIALES para esta variante." << endl;
        } else if (correcto) {
            cout << "   Soluciones: con poda=" << solucionesConPoda
                 << ", sin poda=" << solucionesSinPoda.size() << " -> OK" << endl;
        } else {
            cout << "   Soluciones: con poda=" << solucionesConPoda
                 << ", sin poda=" << solucionesSinPoda.size()
                 << " -> FALLO EN CORRECTITUD" << endl;
        }

        archivoSalida << nombre << "," << p.n << ","
                      << nodosConPoda << "," << nodosSinPoda << ","
                      << solucionesConPoda << "," << solucionesSinPoda.size() << ","
                      << tiempoConPoda.count() << "," << tiempoSinPoda.count() << ","
                      << (truncado ? "SI" : "NO") << endl;
    }

    cout << "----------------------------------------------------------------------------------------------------------------------" << endl;

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
            char respuesta;
            cout << "\n¿Desea usar un limite de nodos para instancias intratables? (s/n): ";
            cin >> respuesta;

            if (respuesta == 's' || respuesta == 'S') {
                long long limite;
                cout << "Ingrese el limite de nodos (ej. 20000000 para 20 millones): ";
                cin >> limite;
                pruebasVariantes(true, limite);
            } else {
                cout << "Corriendo SIN limite. Algunas variantes pueden tardar mucho "
                     << "o no terminar; use Ctrl+C para cancelar si es necesario." << endl;
                pruebasVariantes(false, 0);
            }

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
// verificar_semilla.cpp
//Uso: g++ -std=c++17 -O2 -o verificar_semilla resources/verificar_semilla.cpp
//     .\verificar_semilla.exe Gomez Mazo Pena

#include <iostream>
#include <string>
#include <vector>
#include "../src/bt_policy.hpp"

using namespace std;

// Alfabeto base del Modulo BT: minusculas, mayusculas,
// digitos, y el conjunto de simbolos {!, @, #, $, %}.

string alfabetoBT() {
    string minusculas = "abcdefghijklmnopqrstuvwxyz";
    string mayusculas = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    string digitos = "0123456789";
    string simbolos = "!@#$%";
    return minusculas + mayusculas + digitos + simbolos;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Uso: ./verificar_semilla apellido1 [apellido2] [apellido3]" << endl;
        return 1;
    }

    vector<string> apellidos;
    for (int i = 1; i < argc; i++) {
        apellidos.push_back(argv[i]);
    }
    cout << "Apellidos leidos: " << apellidos.size() << endl;

    int semilla = calcularSemilla(apellidos);
    cout << "Semilla calculada para el equipo: " << semilla << endl;

    Policy politica = derivarPolitica(semilla, 8);
    cout << "\nParametros de la politica (Modulo BT, Seccion 9.2):" << endl;
    cout << "  n         = " << politica.n << endl;
    cout << "  minLower  = " << politica.minLower << endl;
    cout << "  minUpper  = " << politica.minUpper << endl;
    cout << "  minDigit  = " << politica.minDigit << endl;
    cout << "  minSymbol = " << politica.minSymbol << endl;
    cout << "  prohibicion de repetidos consecutivos: "
         << (politica.noRepetidosConsecutivos ? "Si" : "No") << endl;

    bool coincide = coincideConReferencia(politica);
    cout << "  ¿Coincide con la instancia de referencia comun? "
         << (coincide ? "Si" : "No") << endl;

    return 0;
}
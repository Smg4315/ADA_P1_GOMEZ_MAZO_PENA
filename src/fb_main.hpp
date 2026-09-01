#pragma once

#include <string>
#include <vector>

using namespace std;

// Declaraciones compartidas entre fb_main.cpp y tests/tests.cpp

extern const string alfabetoA1;
extern const string alfabetoA2;
extern const vector<int> longitudes;
extern vector<string> alfabetos;

extern bool encontrado;
extern vector<string> hashesObjetivo;

string generarHash(string psw);
string generarContrasena(string contrasena, int tamanoContrasena, string alfabeto, string hashObjetivo);
vector<string> generarContrasenasObjetivo(int semilla);

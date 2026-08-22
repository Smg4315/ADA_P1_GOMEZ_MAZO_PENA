/* 
PSEUDOCÓDIGO DE LA FUNCIÓN DE GENERACIÓN Y VERIFICACION DE CONTRASEÑAS

alfabeto[a-z]}

generarContraseña(contraseña, tamañoContraseña, alfabeto, hashObjetivo):

si funcionHash(contraseña) == hashObjetivo:
    imprimir "La contraseña es: " + contraseña
    retornar

Si tamañoContraseña == longitud(contraseña):
    retornar

para cada caracter en alfabeto:
    generarContraseña(contraseña+caracter, tamañoContraseña, alfabeto, hashObjetivo) // llamada recursiva para generar todas las combinaciones posibles de contraseñas

*/

#include "third_party/picosha2.cpp"
#include <iostream>
#include <string>

using namespace std;

const string alfabeto = "abcdefghijklmnopqrstuvwxyz123456789"; // Alfabeto de caracteres a utilizar para generar la contraseña
const int tamañoContraseña = 4; // Tamaño de la contraseña a generar
string hashObjetivo = "9b871512327c09ce91dd649b3f96a63b7408ef267c8cc5710114e629730cb61f"; // Hash objetivo a comparar

void generarContraseña(string contraseña, int tamañoContraseña, string alfabeto, string hashObjetivo) {

    if (generarHash(contraseña) == hashObjetivo) {
        cout << "La contraseña es: " << contraseña << endl;
        return ;
    }

    if (contraseña.length() == tamañoContraseña) {
        //cout << "contraseña generada: " << contraseña << endl;
        return;
    }

    for (char caracter : alfabeto) {
        generarContraseña(contraseña + caracter, tamañoContraseña, alfabeto, hashObjetivo);
    }

}

int main() {
    string contraseña = ""; // Contraseña inicial vacía
    string cadena= "abz3"; // Cadena de texto a generar el hash
    generarContraseña(contraseña, tamañoContraseña, alfabeto, generarHash(cadena)); // Llamada a la función para generar y verificar contraseñas
    return 0;

}
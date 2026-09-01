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
#include "fb_main.hpp"
#include "../tests/tests.cpp"

#include <iostream>
#include <string>
#include <vector>

#include <chrono> // Biblioteca para medir el tiempo de ejecución
#include <fstream> // Biblioteca para escribir en archivos

using namespace std;

// -> Modelamiento del problema <-
const string alfabetoA1 = "abcdefghijklmnopqrstuvwxyz"; // Alfabeto de caracteres a utilizar para generar la contraseña
const string alfabetoA2 = "abcdefghijklmnopqrstuvwxyz123456789"; // Alfabeto de caracteres a utilizar para generar la contraseña

// Vector para almacenar las longitudes de las contraseñas a generar -> para ir variandola
const vector<int> longitudes = {4, 4, 5, 5, 6}; 

// Vector para almacenar los alfabetos a utilizar para generar las contraseñas -> para ir variandolos
vector<string> alfabetos = { alfabetoA1, alfabetoA2, alfabetoA1, alfabetoA2, alfabetoA1};

bool encontrado = false; // Variable para indicar si se ha encontrado la contraseña
vector<string> hashesObjetivo; // Vector para almacenar los hashes generados con la semilla

//Función de generación y verificación de contraseñas

// Recibe como parámetros de entrada: la contraseña generada hasta el momento,
// longitud maxima a explorar, el alfabeto y el hash objetivo a comparar
string generarContrasena(string contrasena, int tamanoContrasena, string alfabeto, string hashObjetivo) {

    if (generarHash(contrasena) == hashObjetivo) {
        encontrado = true; // Se ha encontrado la contraseña
       return contrasena; // Retornamos la contraseña encontrada y salimos de la función
    }

    if (contrasena.length() == tamanoContrasena) {
        //cout << "contrasena generada: " << contrasena << endl;
        return "";
    }

    for (char caracter : alfabeto) {
        string resultado =  generarContrasena(contrasena + caracter, tamanoContrasena, alfabeto, hashObjetivo);

          if (encontrado) {
            return resultado; // Retornamos la contraseña encontrada y salimos de la funcion (ciclo especifico)
        }
    }

    return "";
}


// Función para generar las contraseñas a partir de la semilla generada para los casos de prueba
vector<string> generarContrasenasObjetivo(int semilla) {

    // Vector para almacenar las contraseñas generadas
    vector<string> contrasenas; 

    long long x = semilla; // X0 = semilla

    for (int j = 0; j < longitudes.size(); j++) {

        string contrasena = "";

        // Alfabeto correspondiente a esta contraseña
        string alfabetoActual = alfabetos[j];

        for (int i = 0; i < longitudes[j]; i++) {

            // Generar siguiente número pseudoaleatorio
            x = (1103515245LL * x + 12345) % 2147483648LL;

            // Obtener posición dentro del alfabeto actual
            int posicion = x % alfabetoActual.length();

            // Agregar carácter
            contrasena += alfabetoActual[posicion];
        }

        contrasenas.push_back(contrasena);
    }

    return contrasenas;
}


int main() {

    while (true) {
        
        int opcion;    

        cout << "" << endl;
        cout << "Ingrese la opcion que le gustaria hacer" << endl;
        cout << "1. Revisar casos de prueba (semilla) + comparación por diccionario" << endl;
        cout << "2. Hacer pruebas de tamaños de entrada para metricas" << endl;
        cout << "3. salir" << endl;

        cin >> opcion;

        if (opcion == 1) {
            // Llamado a la función de casos de prueba para verificar cumplimiento de la función
            casosPrueba();
        } else if (opcion == 2) {
            // A través de esta opción se pueden hacer pruebas de tamaños de entrada para métricas de tiempo y espacio
            pruebasTamanosEntrada();
            cout << "" << endl;
        } else if (opcion == 3) {
            cout << "Saliendo del programa..." << endl;
            cout << "" << endl;
            return 0;
        } else {
            cout << "Opción inválida. Por favor, ingrese 1, 2 o 3" << endl;
            cout << "" << endl;
        }
    }
}
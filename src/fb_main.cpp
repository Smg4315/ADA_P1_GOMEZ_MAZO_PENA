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
#include <vector>

using namespace std;

// Modelamiento del problema
const string alfabetoA1 = "abcdefghijklmnopqrstuvwxyz"; // Alfabeto de caracteres a utilizar para generar la contraseña
const string alfabetoA2 = "abcdefghijklmnopqrstuvwxyz123456789"; // Alfabeto de caracteres a utilizar para generar la contraseña
const int tamañoContraseña = 5; // Tamaño de la contraseña a generar

bool encontrado = false; // Variable para indicar si se ha encontrado la contraseña
vector<string> hashesObjetivo; // Vector para almacenar los hashes generados con la semilla

//Función de generación y verificación de contraseñas

// Recibe como parámetros de entrada: la contraseña generada hasta el momento,
// longitud maxima a explorar, el alfabeto y el hash objetivo a comparar
string generarContraseña(string contraseña, int tamañoContraseña, string alfabeto, string hashObjetivo) {

    if (generarHash(contraseña) == hashObjetivo) {
        encontrado = true; // Se ha encontrado la contraseña
       return contraseña; // Retornamos la contraseña encontrada y salimos de la función
    }

    if (contraseña.length() == tamañoContraseña) {
        //cout << "contraseña generada: " << contraseña << endl;
        return "";
    }

    for (char caracter : alfabeto) {
        string resultado =  generarContraseña(contraseña + caracter, tamañoContraseña, alfabeto, hashObjetivo);

          if (encontrado) {
            return resultado; // Retornamos la contraseña encontrada y salimos de la funcion (ciclo especifico)
        }
    }

    return "";
}

// Función para generar la semilla de creación de contraseñas
int generarSemilla(){
    string apellidos = "gomezmazopena"; // Apellidos de los estudiantes en orden alfabético
    int sum = 0; // Variable para almacenar la semilla generada

    for (char caracter : apellidos) {
        sum += int(caracter); // Se utiliza el valor ASCII de cada caracter para generar la semilla
    }

    int modulo = 100000;
    int semilla = sum % modulo; // Se utiliza el módulo para limitar el rango de la semilla generada
    return semilla; // Retornamos la semilla generada
}

// Función para generar las contraseñas a partir de la semilla generada
vector<string> generarContraseñasObjetivo(int semilla) {

    // Vector para almacenar las contraseñas generadas
    vector<string> contraseñas; 

    // Vector para almacenar las longitudes de las contraseñas a generar -> para ir variandola
    vector<int> longitudes = {4, 4, 5, 5, 6};

    // Vector para almacenar los alfabetos a utilizar para generar las contraseñas -> para ir variandolos
    vector<string> alfabetos = { alfabetoA1, alfabetoA2, alfabetoA1, alfabetoA2, alfabetoA1};

    long long x = semilla; // X0 = semilla

    for (int j = 0; j < longitudes.size(); j++) {

        string contraseña = "";

        // Alfabeto correspondiente a esta contraseña
        string alfabetoActual = alfabetos[j];

        for (int i = 0; i < longitudes[j]; i++) {

            // Generar siguiente número pseudoaleatorio
            x = (1103515245LL * x + 12345) % 2147483648LL;

            // Obtener posición dentro del alfabeto actual
            int posicion = x % alfabetoActual.length();

            // Agregar carácter
            contraseña += alfabetoActual[posicion];
        }

        contraseñas.push_back(contraseña);
    }

    return contraseñas;
}

int main() {
    string contraseña = ""; // Contraseña inicial vacía (se constrira en la recursión)

    // Creación de contraseñas a encontrar con semilla (1405)
    int semilla = generarSemilla(); // Generación de la semilla para la creación de contraseñas
    cout << "Semilla generada: " << semilla << endl; // Print de la semilla generada

    vector<string> contraseñasObjetivo = generarContraseñasObjetivo(semilla); // Generación de las contraseñas a encontra
    
    // Print de las contraseñas generadas y su respectivo hash
    for (const string& contraseñaObjetivo : contraseñasObjetivo) {
        string hash = generarHash(contraseñaObjetivo); // Generación del hash de la contraseña objetivo
        hashesObjetivo.push_back(hash); // Almacenamiento del hash generado en el vector de hashes
        cout << "Contraseña objetivo generada: " << contraseñaObjetivo << ", Hash -> " << hash << endl; 
    }

    // Encontremos la contraseña objetivo a partir de su hash

    for (const string& hashObjetivo : hashesObjetivo) {
        cout << "Buscando contraseña para el hash: " << hashObjetivo << endl;
        string resultado = generarContraseña(contraseña, tamañoContraseña, alfabetoA1, hashObjetivo); // Llamada a la función para generar y verificar contraseñas
        if (encontrado) {
            cout << "La contraseña encontrada es: " << resultado << endl;
            encontrado = false; // Reiniciamos la variable encontrado para la siguiente búsqueda
        } else {
            cout << "No se encontró la contraseña para el hash: " << hashObjetivo << endl;
        }
    }
    
    return 0;
}
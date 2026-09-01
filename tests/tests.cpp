#include "../src/fb_main.hpp"

#include <iostream>
#include <fstream>
#include <chrono>

using namespace std;

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

// Busca, línea por línea, si algún hash del diccionario coincide con el hash objetivo.
// No se carga el diccionario en memoria como estructura de búsqueda: se calcula el hash al vuelo por cada línea.
string buscarEnDiccionario(const string& hashObjetivo) {

    ifstream archivoDiccionario("../resources/diccionario.txt");

    if (!archivoDiccionario.is_open()) {
        cerr << "No se pudo abrir el archivo de diccionario." << endl;
        return "";
    }

    string candidato;
    while (getline(archivoDiccionario, candidato)) {

        if (candidato.empty()) {
            continue;
        }

        if (generarHash(candidato) == hashObjetivo) {
            return candidato; // Coincidencia encontrada
        }
    }

    return ""; // No se encontró coincidencia en el diccionario
}

// Función para hacer los casos de prueba especificados en la seccion 9.1
void casosPrueba() {

    hashesObjetivo.clear(); // Reiniciamos el vector de hashes por si la función ya se había ejecutado antes

    // Creación de contraseñas a encontrar con semilla (1405)
    int semilla = generarSemilla(); // Generación de la semilla para la creación de contraseñas
    cout << "Semilla generada: " << semilla << endl; // Print de la semilla generada

    // Generación de las contraseñas a encontrar con la semilla de equipo
    vector<string> contrasenasObjetivo = generarContrasenasObjetivo(semilla); 
    
    // creacion de los hashes de las contraseñas.
    for (const string& contrasenaObjetivo : contrasenasObjetivo) {
        string hash = generarHash(contrasenaObjetivo); // Generación del hash de la contraseña objetivo
        hashesObjetivo.push_back(hash); // Almacenamiento del hash generado en el vector de hashes
        cout << "Contrasena objetivo generada: " << contrasenaObjetivo << ", Hash -> " << hash << endl; 
    }

    ofstream archivoSalida("../results/resultadosSemilla_FB.csv"); // Archivo para almacenar los resultados de las pruebas

    if (!archivoSalida.is_open()) {
        cerr << "No se pudo crear el archivo de métricas." << endl;
        return;
    }

    archivoSalida << "<------ Resultados de las pruebas con las contrasenas de la semilla ------>" << endl;
    archivoSalida << " " << endl;

    archivoSalida << "------------------------------------------------------------------------------------------------------------------------------------------" << endl;
    archivoSalida << "|  Alfabeto   |   Longitud  |  Contrasena objetivo  |   Tiempo fuerza bruta (µs)  | resultado fuerza bruta | Tiempo diccionario (µs) | resultado diccionario |" << endl;
    archivoSalida << "------------------------------------------------------------------------------------------------------------------------------------------" << endl;

    // Encontremos la contraseña objetivo a partir de su hash

    for (int i = 0; i < hashesObjetivo.size(); i++) {

        cout << "Buscando contrasena para el hash: "
            << hashesObjetivo[i] << endl;

        encontrado = false; // Reiniciamos la variable encontrado para la siguiente búsqueda

        // Cada contraseña de la semilla tiene su propia longitud y alfabeto (longitudes[i] / alfabetos[i]),
        // por lo que se debe usar el par correspondiente a la posición i, no valores fijos
        auto inicioBruta = chrono::high_resolution_clock::now();
        string resultadoBruta = generarContrasena( "", longitudes[i], alfabetos[i], hashesObjetivo[i]);
        auto finBruta = chrono::high_resolution_clock::now();
        chrono::duration<double, micro> elapsedBruta = finBruta - inicioBruta;

        string estadoBruta = encontrado ? "Encontrado" : "No encontrado";

        if (encontrado) {
            cout << "La contrasena encontrada por fuerza bruta es: " << resultadoBruta << endl;
        } else {
            cout << "No se encontró la contrasena por fuerza bruta para el hash: " << hashesObjetivo[i] << endl;
        }

        encontrado = false; // Reiniciamos la variable encontrado para la siguiente búsqueda

        auto inicioDiccionario = chrono::high_resolution_clock::now();
        string resultadoDiccionario = buscarEnDiccionario(hashesObjetivo[i]);
        auto finDiccionario = chrono::high_resolution_clock::now();
        chrono::duration<double, micro> elapsedDiccionario = finDiccionario - inicioDiccionario;

        string estadoDiccionario = resultadoDiccionario.empty() ? "No encontrado" : "Encontrado";

        if (!resultadoDiccionario.empty()) {
            cout << "La contrasena encontrada por diccionario es: " << resultadoDiccionario << endl;
        } else {
            cout << "No se encontró la contrasena por diccionario para el hash: " << hashesObjetivo[i] << endl;
        }

        archivoSalida << "|  " << alfabetos[i] << "   |   " << longitudes[i] << "  |   " << contrasenasObjetivo[i]
                      << "  |   " << elapsedBruta.count() << " | " << estadoBruta
                      << " | " << elapsedDiccionario.count() << " | " << estadoDiccionario << "  |" << endl;
    }

    archivoSalida << "------------------------------------------------------------------------------------------------------------------------------------------" << endl;

    return;
}

void pruebasTamanosEntrada() {


    // Implementación de pruebas de tamaños de entrada para métricas de tiempo y espacio

    vector<int> longitudesPruebaA1 = {3, 4, 5, 6}; // Longitudes de prueba para el alfabeto 1
    vector<int> longitudesPruebaA2 = {3, 4, 5}; // Longitudes de prueba para el alfabeto 2

    // Definimos un arreglo de contraseñas ya establecidas para hacer un ánalisis uniforme 
    // de los tiempos de ejecución y el espacio utilizado para cada tamaño de entrada. Evitando comparar un 
    // posible mejor caso con uno peor. 
    // Se usa la m pues está alrededor de la mitad del alfabeto
    vector<string> contrasenasObjetivo = {"mmm", "mmmm", "mmmmm", "mmmmmm"};

    ofstream archivoSalida("../results/resultadosTamanoEntrada_FB.csv"); // Archivo para almacenar los resultados de las pruebas

    if (!archivoSalida.is_open()) {
        cerr << "No se pudo crear el archivo de métricas." << endl;
        return;
    }

    archivoSalida << "<------ Resultados de las pruebas de tamaños de entrada ------>" << endl;
    archivoSalida << " " << endl;

    archivoSalida << "------------------------------------------------------------------------------------------------" << endl;
    archivoSalida << "|  Alfabeto   |   Longitud  |   Tiempo de ejecución (µs) | candidato evaluado | resultado |" << endl;
    archivoSalida << "------------------------------------------------------------------------------------------------" << endl;


    for(int i = 0; i < longitudesPruebaA1.size(); i++) {
        
        encontrado = false; // Reiniciamos la variable encontrado para la siguiente búsqueda
        
        string hash = generarHash(contrasenasObjetivo[i]); // Generación del hash de la contraseña objetivo

        auto inicio = chrono::high_resolution_clock::now(); // Inicio de la medición de tiempo
        string candidato = generarContrasena("", longitudesPruebaA1[i], alfabetoA1, hash); // Búsqueda de la contraseña a partir del hash
        auto fin = chrono::high_resolution_clock::now(); // Fin de la medición de tiempo

        chrono::duration<double, micro> elapsed = fin - inicio; // Cálculo del tiempo transcurrido

        string resultado = "";
        if(candidato == "") {
            resultado = "No encontrado";
        } else {
            resultado = "Encontrado";
        }

        archivoSalida << "|  " << alfabetoA1 << "   |   " << longitudesPruebaA1[i] << "  |   " 
                      << elapsed.count() << " | " << contrasenasObjetivo[i] << " | " << resultado  << "  |   " << endl;
    }

    archivoSalida << "------------------------------------------------------------------------------------------------" << endl;

    for(int i = 0; i < longitudesPruebaA2.size(); i++) {

        encontrado = false; // Reiniciamos la variable encontrado para la siguiente búsqueda

        string hash = generarHash(contrasenasObjetivo[i]); // Generación del hash de la contraseña objetivo

        auto inicio = chrono::high_resolution_clock::now(); // Inicio de la medición de tiempo
        string candidato = generarContrasena("", longitudesPruebaA2[i], alfabetoA2, hash); // Búsqueda de la contraseña a partir del hash
        auto fin = chrono::high_resolution_clock::now(); // Fin de la medición de tiempo

        chrono::duration<double, micro> elapsed = fin - inicio; // Cálculo del tiempo transcurrido

        string resultado = "";
        if(candidato == "") {
            resultado = "No encontrado";
        } else {
            resultado = "Encontrado";
        }

        archivoSalida << "|  " << alfabetoA2 << "   |   " << longitudesPruebaA2[i] << "  |   " 
                      << elapsed.count() << " | " << contrasenasObjetivo[i] << " | " << resultado  << "  |   " << endl;
    }

    return;
}
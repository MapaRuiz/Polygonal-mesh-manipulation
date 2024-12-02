//Se llama Nuri
#include "sistema.h"   //Incluye la definicion de la clase sistema (contine las funcionalidades del sistema)
#include <chrono>  //Para tiempo de ejecucion
#include <iostream>  //Entrada y salida estandar
#include <string>  //Manejar cadena de caracetres
#include <vector>  //Manejar arreglos dinamicos
#include <sstream>   //Separa comandos ingresados por el usuario

using namespace std;

vector<string> customInput() { //Para mostrar el simbolo de '$' al inicio de cada linea de comando
  cout << "$ ";
  string input;
  getline(cin, input);

  string s;
  stringstream ss(input);
  vector<string> v;

  while (getline(ss, s, ' ')) {
    v.push_back(s);
  }

  return v;
}

int main() {
  Sistema sistema;  //Instancia de clase sistema
  vector<string> comandos = {   //Comandos aceptador por el sistema
      "cargar",          "listado",    "envolvente",
      "descargar",       "guardar",    "v_cercano",
      "v_cercanos_caja", "ruta_corta", "ruta_corta_centro",
      "salir",           "ayuda"};

  while (true) {  //Ciclo para que el usario seleccione opciones seleccionadas
    vector<string> cin = customInput();
    auto start = chrono::system_clock::now();    //Empezar a contar tiempo

    if (cin[0] == comandos[9]) {
      if (cin.size() == 1){
        exit(1);
      }else {
        cout << "El comando: 'salir', no requiere parametros, revise el comando a ejecutar consultando la guia con el comando: 'ayuda'.\n";
      }

    } else if (cin[0] == comandos[10]) {
      if (cin.size() == 1) {
        sistema.mostrarComandos(); //Muestra los comandos disponibles
      } else if (cin.size() == 2) {
        sistema.ayudaComando(cin[1], comandos); //Recibe string correspondinete al comando que se solicita y vector de comandos, genera la informacion de como usar el comando solicitado
      } else {
        cout << "El comando: 'ayuda comando', requiere menos o mas parametros, revise el comando a ejecutar consultando la guia con el comando: 'ayuda'.\n";
      }

    } else if (cin[0] == comandos[0]) {
      if (cin.size() == 2) {
        sistema.cargar(cin[1]); //Cuando se ingresa cargar + archivo, carga una malla poligonal desde el archivo, cin[1] es el nombre del archivo
      } else {
        cout << "El comando: 'cargar', requiere menos o mas parametros, revise el comando a ejecutar consultando la guia con el comando: 'ayuda'.\n";
      }

    } else if (cin[0] == comandos[1]) {
      if (cin.size() == 1) {
        sistema.listado();  //Muetsra lista de mallas cargadas en el sistema
      } else {
        cout << "El comando: 'listado' no requiere parametros, revise el comando a ejecutar consultando la guia con el comando: 'ayuda'.\n";
      }

    } else if (cin[0] == comandos[2]){
      if (cin.size() == 2) {
        sistema.envolvente(cin[1]);  //Recibe nombre de objeto para calcular envolvente y calcula su envolvente
      } else if (cin.size() == 1) {
        sistema.envolvente();  //Calcula envolvente de todos los objetos en el sistema
      } else {
        cout << "El comando: 'envolvente' requiere menos o mas parametros, revise el comando a ejecutar consultando la guia con el comando: 'ayuda'.\n";
      }

    } else if (cin[0] == comandos[3]){
      if (cin.size() == 2) {
        sistema.descargar(cin[1]);  //Recibe el nombre del objeto a descargar y lo elimina del sistema
      } else {
        cout << "El comando: 'descargar' requiere menos o mas parametros, revise el comando a ejecutar consultando la guia con el comando: 'ayuda'.\n";
      }

    } else if(cin[0] == comandos[4]){
      if (cin.size() == 3) {
        sistema.guardar(cin[1], cin[2]);   //Recibe nombre archivo y formato a guardar, guarda la malla en el archivo especificado
      } else {
        cout << "El comando: 'guardar' requiere menos o mas parametros, revise el comando a ejecutar consultando la guia con el comando: 'ayuda'.\n";
      }

    } else if(cin[0] == comandos[5]){
      if (cin.size() == 5) {
        sistema.vCercanoObj(cin[4], stof(cin[1]), stof(cin[2]), stof(cin[3]));   //Recibe nombre y tres enteros (x,y,z) y encuntra el vertice mas cercano al punto (x,y,z)
      } else if (cin.size() == 4) {
        sistema.vCercano(stof(cin[1]), stof(cin[2]), stof(cin[3]));  //Recibe (x,y,z) del punto de referencia y encuntra el vertice a las corrdenadas dentro de malla cargada
      } else {
        cout << "El comando: 'v-cercano' requiere menos o mas parametros, revise el comando a ejecutar consultando la guia con el comando: 'ayuda'.\n";
      }

    } else if(cin[0] == comandos[6]){     //Vertices cercanos a la caja delimitada por los valores de los ejes
      if (cin.size() == 2) {
        if(sistema.buscarMalla(cin[1]).direcEnvolvente == nullptr){
          sistema.envolvente(cin[1]);
        }
        sistema.vCercano_caja(cin[1]);  //Recibe el nombre de la caja delimitadora y ecnutra los vertices dentro de ella
      } else {
        cout << "El comando: 'v-cercanos-caja' requiere menos o mas parametros, revise el comando a ejecutar consultando la guia con el comando: 'ayuda'.\n";
      }

    } else if(cin[0] == comandos[7]){  //Ruta mas corta entre dos vertices del objeto
      if (cin.size() == 4) {
        sistema.rutaCorta(stoi(cin[1]), stoi(cin[2]), cin[3]);  //Recibe vertices de inicio, fin y nombre, calcula la ruta mas corta de los vertices dentro del objeto
      } else {
        cout << "El comando: 'ruta-corta' requiere menos o mas parametros, revise el comando a ejecutar consultando la guia con el comando: 'ayuda'.\n";
      }

    }else if(cin[0] == comandos[8]){     //Calcula ruta mas corta desde un vertice a un centro en un objeto
      if (cin.size() == 3) {
        sistema.rutaCorta_centro(stoi(cin[1]), cin[2]);  //Recibe vertice de incio y nombre, calcula la ruta mas corta desde el vertice a el centro del objeto
      } else {
        cout << "El comando: 'ruta-corta-centro' requiere menos o mas parametros, revise el comando a ejecutar consultando la guia con el comando: 'ayuda'.\n";
      }

    }else{
      cout<<"Comando no valido"<<endl;
    }

    auto end = chrono::system_clock::now();   //Terminar el tiempo de ejecucion
    chrono::duration<double> elapsed_seconds = end - start;
    cout << "Elapsed time: " << elapsed_seconds.count() << "s\n";
    time_t end_time = chrono::system_clock::to_time_t(end);  //Imprime tiempo de ejecucion
    cout << "Processing finished at " << ctime(&end_time) << endl;
  }
  return 0;
}

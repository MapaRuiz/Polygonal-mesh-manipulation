#include <iostream>
#include <fstream>
#include <vector>
#include <limits>
#include <sstream>
#include <memory>
#include "malla.h"
#include "sistema.h"

#include <algorithm>

using namespace std;

void Sistema::mostrarComandos() {   //Lee el archivo de comandos y meustra su contenido
  ifstream archivo("comandos.txt");
  if (!archivo.is_open()) { 
    cerr << "Error al abrir el archivo." << endl;
    return;
  }

  string linea;  
  cout << endl;
  while (getline(archivo, linea)) {  
    cout << linea << endl; 
  }
  cout << endl;
  archivo.close();  
}

void Sistema::ayudaComando(string comando, vector<string> comandos) {   //Recibe comando y vector de comandos, muestra informacion de como usar el comando solicitado
  if (comando == comandos[0]) {   
    cout << "Usage: cargar nombre_archivo" << endl;
  }
  else if (comando == comandos[1]) {
    cout << "Usage: listado" << endl;
  }
  else if (comando == comandos[2]) {
    cout << "Usage: envolvente OR envolvente nombre_objeto" << endl;
    cout << "Type 'ayuda' for more information" << endl;
  }
  else if (comando == comandos[3]) {
    cout << "Usage: descargar nombre_objeto" << endl;
  }
  else if (comando == comandos[4]) {
    cout << "Usage: guardar nombre_objeto nombre_archivo" << endl;
  }
  else if (comando == comandos[5]) {
    cout << "Usage: v_cercano px py pz OR v_cercano px py pz nombre_objeto"
         << endl;
    cout << "Type 'ayuda' for more information" << endl;
  }
  else if (comando == comandos[6]) {
    cout << "Usage: v_cercanos_caja nombre_objeto" << endl;
  }
  else if (comando == comandos[7]) {
    cout << "Usage: ruta_corta i1 i2 nombre_objeto" << endl;
  }
  else if (comando == comandos[8]) {
    cout << "Usage: ruta_corta_centro i1 nombre_objeto" << endl;
  }
  else
  {
    cout << "Usage: ayuda Comando \n"
            "El comando ingresado no es valido o no pertenece al sistema" << endl;
  }
}

Malla Sistema::leerArchivo(string nombre_archivo) {  //Recibe nombre de archivo y lee sus datos para crear la malla
  ifstream file(nombre_archivo); 
  if (!file.is_open()) {  
    cout << "El archivo " << nombre_archivo << " no existe o es ilegible" << endl;
    return Malla();
  }

  string line;
  string nombre;
  if (getline(file, line)) {
    trim(line);
    nombre = line;
  }else {
    cout << "El archivo " << nombre_archivo << " no contiene un objeto 3D valido." << endl;
    return Malla();
  }

  int n_vertices;
  if (getline(file, line)) {
    trim(line);
    istringstream ss(line);  
    if (!(ss >> n_vertices) || n_vertices <= 0) {   
      cout << "El archivo " << nombre_archivo << " no contiene un objeto 3D valido. Error en numero de vertices." << endl;
      return Malla();
    }
  }else {
    cout << "El archivo " << nombre_archivo << " no contiene un objeto 3D valido. Error en numero de vertices" << endl;
    return Malla();
  }

  map<int, Vertice> vertices;  
  for (int i = 0; i < n_vertices; ++i) {
    if (getline(file, line)) {
      trim(line);
      istringstream ss(line);
      Vertice vertice;
      float tempX, tempY, tempZ;
      if (!(ss >> tempX >> tempY >> tempZ)) {
        cout << "El archivo " << nombre_archivo << " no contiene un objeto 3D valido. Error en creacion de vertices" << endl;
        return Malla();
      }
      vertice.setx(tempX);
      vertice.sety(tempY);
      vertice.setz(tempZ);
      vertice.setid(i);
      vertices[i]=vertice;
    } else {
      cout << "El archivo " << nombre_archivo << " no contiene un objeto 3D valido. Error en creacion de vertices" << endl;
      return Malla();
    }
  }

  vector<vector<int>> caras;  
  while (getline(file, line)) {
    trim(line);
    if (line == "-1") {  
      break;
    }

    istringstream ss(line);
    int cant;
    if (!(ss >> cant) || cant <= 0) {  
      cout << "El archivo " << nombre_archivo << " no contiene un objeto 3D valido. Error en numero de caras" << endl;
      return Malla();
    }

    vector<int> cara(cant);  
    for (int i = 0; i < cant; i++) {
      if (!(ss >> cara[i]) || cara[i] < 0 || cara[i] >= n_vertices) {
        cout << "El archivo " << nombre_archivo << " no contiene un objeto 3D valido. Error en numero de caras" << endl;
        return Malla();
      }
    }
    caras.push_back(cara);
  }

  if (line != "-1") {  
    cout << "El archivo " << nombre_archivo << " no contiene un objeto 3D valido." << endl;
    return Malla();
  }

  file.close();

  Malla malla = Malla(nombre, n_vertices, vertices, caras.size(), caras);
  return malla;   
}

void Sistema::trim (string &s){
  // Eliminar espacios y caracteres especiales al principio y al final
  s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char c) {
    return!std::isspace(c) && c !='\n'&& c !='\t'&& c !='\r';
  }));
  s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char c) {
    return!std::isspace(c) && c !='\n'&& c !='\t'&& c !='\r';
  }).base(), s.end());
}

void Sistema::cargar(string nomarch){  //Recibe nombre de archivo y carga una malla desde un archivo si no esta todavia en memoria
  bool mal = false;
  Malla m = leerArchivo(nomarch);
  for(int i=0;i<mallas.size();i++){
    if(mallas[i].igual(m)){
      cout<<"El objeto "<<m.getNombre()<<" ya ha sido cargado en memoria."<<endl;
      mal=true;
      break;
    }
  }
  if(m.igual(Malla())){
    mal = true;
  }
  if(!mal){
    mallas.push_back(m);
    cout << "El objeto " << m.getNombre() << " ha sido cargado exitosamente desde el archivo " << nomarch << "." << endl;
  }
}

void Sistema::descargar(string nombre) {  //Elimina una malla de la memoria segun el nombre
  bool band = false;  
  vector<Malla>::iterator it = mallas.begin(); 
  while (it != mallas.end() && band == false) {  
    if (it->getNombre() == nombre) {    
      it = mallas.erase(it);
      band = true;    
    } else {
      it++;  
    }
  }
  if(nombre == "env_global"){
    this->env_global = Malla();
    band = true;
  }
  if (band) {   
    cout << "El objeto " << nombre << " ha sido eliminado de la memoria de trabajo." << endl;
  } else {
     cout << "El objeto " << nombre << " no ha sido cargado en memoria." << endl;
  }
}

void Sistema::guardar(string nombre, string nomarch) {  //Recibe nombre de objeto y archivo, guarda la info de la malla en el archivo especificado
  Malla m = buscarMalla(nombre);
  if (m.igual(Malla())) {
    cout << "El objeto " << nombre << " no ha sido cargado en la memoria." << endl;
  } else {
    ofstream archivo(nomarch);
    if (archivo.is_open()) {
      archivo << m.getNombre() << endl;    
      archivo << m.getNumVertices() << endl;
            
      map<int, Vertice> vertices = m.getVertices();
      for (int i=0; i<m.getNumVertices(); i++) {
        archivo << vertices[i].getx() << " " << vertices[i].gety() << " " << vertices[i].getz() << endl;
      }
          
      vector<vector<int>> caras = m.getCaras();  
      vector<vector<int>>::iterator itG2 = caras.begin();
      vector<int>::iterator itP2;
      for( ;itG2!=caras.end();itG2++){      
        itP2 = itG2->begin();
        archivo << itG2->size() << " ";
        for( ;itP2!=itG2->end();itP2++){   
          archivo << *itP2 << " ";
        }
        archivo << endl;
      }
      archivo << "-1\n";
      archivo.close();
    }
    cout << "La informacion del objeto " << nombre << " ha sido guardada exitosamente en el archivo " << nomarch << endl;
    
  }
}

Malla Sistema::buscarMalla(string nombre){  //Recibe nombre de objeto y busca en la memoria si existe su malla
  vector<Malla>::iterator it;
  for(it=mallas.begin(); it!=mallas.end(); it++){  
    if(it->getNombre() == nombre){   
      return *it;
    }
  }
  if(nombre == "env_global"){
    return this->env_global;
  }
  return Malla();   
}

void Sistema::listado(){ //Muestra listado de las mallas cargadas en memoria
  
  if(mallas.empty()){
    cout<<"Ningun objeto ha sido cargado en memoria."<<endl;
  }else{
    if(env_global.igual(Malla())){
      cout<<"Hay "<<mallas.size()<<" objetos en memoria:"<<endl;
      vector<Malla>::iterator it;
      for(it=mallas.begin(); it!=mallas.end(); it++){
        it->info();
      }
    }else{
      cout<<"Hay "<<mallas.size() + 1<<" objetos en memoria:"<<endl;
      vector<Malla>::iterator it;
      for(it=mallas.begin(); it!=mallas.end(); it++){
        it->info();
      }
      this->env_global.info();
    }
    
  }
}

void Sistema::envolvente(string nombre){ //Recibe nombre de objeto y calcula su caja envolvente
  Malla m = buscarMalla(nombre);
  if(m.igual(Malla()))
    cout<<"El objeto "<<nombre<<" no ha sido cargado en memoria."<<endl;
  else{
    Vertice pmin(
        1,
        std::numeric_limits<float>::max(),
        std::numeric_limits<float>::max(),
        std::numeric_limits<float>::max()
    );
    Vertice pmax(
        1,
        std::numeric_limits<float>::lowest(),
        std::numeric_limits<float>::lowest(),
        std::numeric_limits<float>::lowest()
    );

    map<int, Vertice> verticesMalla = m.getVertices();
    for (int i=0; i<m.getNumVertices(); i++) {
      if (verticesMalla[i].getx() < pmin.getx()) pmin.setx(verticesMalla[i].getx());
      if (verticesMalla[i].gety() < pmin.gety()) pmin.sety(verticesMalla[i].gety());
      if (verticesMalla[i].getz() < pmin.getz()) pmin.setz(verticesMalla[i].getz());

      if (verticesMalla[i].getx() > pmax.getx()) pmax.setx(verticesMalla[i].getx());
      if (verticesMalla[i].gety() > pmax.gety()) pmax.sety(verticesMalla[i].gety());
      if (verticesMalla[i].getz() > pmax.getz()) pmax.setz(verticesMalla[i].getz());
    }
    map<int, Vertice> verticesEnv;

    verticesEnv[0]=Vertice(0, pmin.x, pmin.y, pmin.z);
    verticesEnv[4]=Vertice(4, pmin.x, pmin.y, pmax.z);
    verticesEnv[3]=Vertice(3, pmin.x, pmax.y, pmin.z);
    verticesEnv[7]=Vertice(7, pmin.x, pmax.y, pmax.z);
    verticesEnv[1]=Vertice(1, pmax.x, pmin.y, pmin.z);
    verticesEnv[5]=Vertice(5, pmax.x, pmin.y, pmax.z);
    verticesEnv[2]=Vertice(2, pmax.x, pmax.y, pmin.z);
    verticesEnv[6]=Vertice(6, pmax.x, pmax.y, pmax.z);

    vector<vector<int>> carasEnv = {
      {0, 1, 2, 3}, // Cara inferior
      {4, 5, 6, 7}, // Cara superior
      {0, 1, 5, 4}, // eye z Cara frontal
      {1, 2, 6, 5}, // eje z Cara derecha
      {2, 3, 7, 6}, // eje z Cara trasera
      {3, 0, 4, 7}  // eje z Cara izquierda
    };

    string nombreEnvolvente = "env_" + m.nombre;
    int n_ver = 8;
    int n_car = 6;
    Malla mallaEnvolvente(nombreEnvolvente, n_ver,verticesEnv, n_car, carasEnv);
    mallaEnvolvente.setEsEnvolvente(true);
    this->mallas.push_back(mallaEnvolvente);
    
    vector<Malla>::iterator mal;
    vector<Malla>::iterator env = mallas.end();
    env--;

    for(mal=mallas.begin(); mal!=mallas.end(); mal++){
      if(mal->getNombre() == nombre){   
        mal->setDirecEnvolvente(*env);
      }
    }
    
    cout<<"La caja envolvente del objeto " << nombre << " se ha generado con el nombre " << nombreEnvolvente << " y se ha agregado a los objetos en memoria."<<endl;
  }
}

void Sistema::envolvente(){  //Genera la caja envolvente para todos los objetos
  if(mallas.empty()){
    cout<<"Ningun objeto ha sido cargado en memoria."<<endl;
  }else{
    Vertice pmin(
        1,
        std::numeric_limits<float>::max(),
        std::numeric_limits<float>::max(),
        std::numeric_limits<float>::max()
    );
    Vertice pmax(
        1,
        std::numeric_limits<float>::lowest(),
        std::numeric_limits<float>::lowest(),
        std::numeric_limits<float>::lowest()
    );
    vector<Malla>::iterator it;
    for(it=mallas.begin(); it!=mallas.end(); it++){
      map<int, Vertice> verticesMalla = it->getVertices();
      for (int i=0; i<it->getNumVertices(); i++) {
        if (verticesMalla[i].getx() < pmin.getx()) pmin.setx(verticesMalla[i].getx());
        if (verticesMalla[i].gety() < pmin.gety()) pmin.sety(verticesMalla[i].gety());
        if (verticesMalla[i].getz() < pmin.getz()) pmin.setz(verticesMalla[i].getz());

        if (verticesMalla[i].getx() > pmax.getx()) pmax.setx(verticesMalla[i].getx());
        if (verticesMalla[i].gety() > pmax.gety()) pmax.sety(verticesMalla[i].gety());
        if (verticesMalla[i].getz() > pmax.getz()) pmax.setz(verticesMalla[i].getz());
      }
    }
    map<int, Vertice> verticesEnv;

    verticesEnv[0]=Vertice(0, pmin.x, pmin.y, pmin.z);
    verticesEnv[4]=Vertice(4, pmin.x, pmin.y, pmax.z);
    verticesEnv[3]=Vertice(3, pmin.x, pmax.y, pmin.z);
    verticesEnv[7]=Vertice(7, pmin.x, pmax.y, pmax.z);
    verticesEnv[1]=Vertice(1, pmax.x, pmin.y, pmin.z);
    verticesEnv[5]=Vertice(5, pmax.x, pmin.y, pmax.z);
    verticesEnv[2]=Vertice(2, pmax.x, pmax.y, pmin.z);
    verticesEnv[6]=Vertice(6, pmax.x, pmax.y, pmax.z);
    
    vector<vector<int>> carasEnv = {
      {0, 1, 2, 3}, // Cara inferior
      {4, 5, 6, 7}, // Cara superior
      {0, 1, 5, 4}, // eye z Cara frontal
      {1, 2, 6, 5}, // eje z Cara derecha
      {2, 3, 7, 6}, // eje z Cara trasera
      {3, 0, 4, 7}  // eje z Cara izquierda
    };

    string nombreEnvolvente = "env_global";
    int n_ver = 8;
    int n_car = 6;
    Malla mallaEnvolvente(nombreEnvolvente, n_ver,verticesEnv, n_car, carasEnv);
    this->env_global = mallaEnvolvente;

    cout << "La caja envolvente de los objetos en memoria se ha generado con el nombre env_global y se ha agregado a los objetos en memoria."<<endl;
  }
}

void Sistema::vCercanoObj(string nombre, float x, float y, float z){    //Recibe nombre de objeto y coordenadas, calcula vertice mas cercano a punto dado
  Malla m = buscarMalla(nombre);
  if(m.igual(Malla())){
    cout<<"El objeto "<<nombre<<" no ha sido cargado en memoria."<<endl;
  }else{
    Vertice vert(1, x, y, z);
    vector<float> valores = m.vCercano(vert);
    if (!valores.empty()) {
        int indice = static_cast<int>(valores[0]);
        Vertice v = m.getVertices()[indice];
        cout << "El vertice " << indice << " (" << v.getx() << ", " << v.gety() << ", " << v.getz() << ") del objeto " << m.getNombre() << " es el mas cercano al punto (" << x << ", " << y << ", " << z << "), a una distancia de " << valores[1] << endl;
    }
  }
}

void Sistema::vCercano(float x, float y, float z){  //Recibe coordenadas, calcula vertice mas cercano a punto dado
  if(mallas.empty()){ 
    cout<<"Ningun objeto ha sido cargado en memoria."<<endl;
  }else{
    vector<float>valores;
    vector<float>resultado;
    float d_min=numeric_limits<float>::max();  
    int malla=-1, vert =-1;
    Vertice vertice(1, x, y, z);
    vector<Malla>::iterator it;
    int i=0;
    for(it=mallas.begin(); it!=mallas.end(); it++, i++){
      valores = it->vCercano(vertice);
      if(valores[1]<d_min){
        d_min=valores[1];
        vert=valores[0];
        malla=i;
      }
    }
    if (d_min >= 0) {
      resultado.push_back(malla);
      resultado.push_back(vert);
      resultado.push_back(d_min);
    }
    malla = static_cast<int>(resultado[0]);
    vert = static_cast<int>(resultado[1]);
    Malla m = mallas[malla];
    Vertice v = m.getVertices()[vert];
    cout << "El vertice " << vert << " (" << v.getx() << ", " << v.gety() << ", " << v.getz() << ") del objeto " << m.getNombre() << " es el mas cercano al punto (" << vertice.getx() << ", " << vertice.gety() << ", " << vertice.getz() << "), a una distancia de " << resultado[2] << endl;
  }
}

void Sistema::vCercano_caja(string nombre) {  //Recibe nombre de objeto y calcula vertice mas cercanos a las esquinas de la caja envolvente
  Malla m = buscarMalla(nombre);
  if (m.igual(Malla())) {
    cout << "El objeto " << nombre << " no ha sido cargado en memoria." << endl;
  } else {

    vector<float> valores;
    Malla* caja = m.getDirecEnvolvente();
    map<int, Vertice> vCaja = caja->getVertices();
    map<int, Vertice> vertices = m.getVertices();

    cout << "Los vertices del objeto Cube mas cercanos a las esquinas de su caja envolvente son:" << endl;
    cout << setw(9) << "Esquina" << setw(22) << "Vertice" << setw(25) << "Distancia" << endl;
    cout << "------------------------------------------------------------" << endl;

    for (int i = 0; i < 8; i++) {
      valores = m.vCercano(vCaja[i]);

      // Print "Esquina" with separate `setw` for ID and coordinates
      cout << setw(3) << vCaja[i].getid()
           << "(" << setw(3) << vCaja[i].getx() << ","
           << setw(3) << vCaja[i].gety() << ","
           << setw(3) << vCaja[i].getz() << ")";

      // Print "Vertice" with separate `setw` for ID and coordinates
      cout << setw(10)<< valores[0]
           << "(" << setw(3) << vertices[valores[0]].getx() << ","
           << setw(3) << vertices[valores[0]].gety() << ","
           << setw(3) << vertices[valores[0]].getz() << ")";

      // Print "Distancia" with `setw` for alignment
      cout << setw(10) << valores[1] << endl;

      valores.clear();
    }

    cout << "------------------------------------------------------------" << endl;
  }
}

void Sistema::rutaCorta(int i1, int i2, string nombre){ 
  Malla m = buscarMalla(nombre);
  if(m.igual(Malla())){
    cout<<"El objeto "<<nombre<<" no ha sido cargado en memoria."<<endl;
  }else if(i1==i2){
   cout<<"Los indices de los vertices dados son iguales."<<endl;
  }else if(i1>=m.getNumVertices()||i2>=m.getNumVertices()){
    m.rutaCorta(i1, i2);
    cout<<"Algunos de los indices de vertices estan fuera de rango para el objeto " << m.getNombre()<<"."<<endl;
  }else{
    deque<float> ruta = m.rutaCorta(i1, i2);
    cout<<"La ruta mas corta que conecta los vertices "<< i1 << " y " <<i2 << " del objeto "<< m.getNombre() << " pasa por: ";
    deque<float>::iterator it = ruta.begin();
    it++;
    int indice;
    for( ; it!= ruta.end(); it++){
      int indice = *it;
      cout<< indice << ", ";
    }
    cout<<i2 << "; con una longitud de "<<ruta[0]<<endl;
  }
}

void Sistema::rutaCorta_centro(int i1, string nombre){
  Malla m = buscarMalla(nombre);
  if(m.igual(Malla())){
    cout<<"El objeto "<<nombre<<" no ha sido cargado en memoria."<<endl;
  }else if(i1>= m.getNumVertices()){
    cout<<"El indice de vertice esta fuera de rango para el objeto " << m.getNombre() <<"."<<endl;
  }else{
    cout<<"La ruta mas corta que conecta el vertice "<<i1 << " con el centro del objeto "<< m.getNombre()<< " , ubicado en ";
    m.rutaCortaCentro(i1);
  }
}
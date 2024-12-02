#include "malla.h"
#include "Grafo.h"
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <iostream>
#include <string>
#include <cmath>
#include <limits>

using namespace std;

Malla::Malla(string nombre, int n_vertices, map<int, Vertice> vertices, int n_caras, vector<vector<int>> caras){
  this->nombre = nombre;
  this->n_vertices = n_vertices;
  this->vertices = vertices;
  this->n_caras = n_caras;
  this->caras = caras;

  set<vector<int>>aristas;
  vector<int>ar;
  vector<vector<int>>::iterator itG;
  vector<int>::iterator itP;
  for(itG = caras.begin(); itG!=caras.end();itG++){
    itP = itG->begin();
    for(int i=0; i<itG->size()-1; i++, itP++){
      ar.clear();
      ar.push_back(*itP);
      itP++;
      ar.push_back(*itP);
      sort(ar.begin(), ar.end());  
      aristas.insert(ar);
      itP--;
    }
  }
  n_aristas=aristas.size();
  this->grafo = Grafo<Vertice, float>(n_vertices, vertices, n_aristas, aristas);
  //(this->grafo).mostrarMatriz();
}

string Malla::getNombre(){
  return nombre;
}
int Malla::getNumVertices() {
  return n_vertices;
}
int Malla::getNumCaras() {
  return n_caras;
}
map<int, Vertice> Malla::getVertices() {
  return vertices;
}
vector<vector<int>> Malla::getCaras() {
  return caras;
}
Malla* Malla::getDirecEnvolvente() const {
  return direcEnvolvente;
}
bool Malla::getEsEnvolvente() const {
  return esEnvolvente;
}

void Malla::setDirecEnvolvente(Malla& malla) {
  this->direcEnvolvente = &malla;
}
void Malla::setEsEnvolvente(bool esEnvolvente) {
  this->esEnvolvente = esEnvolvente;
}

void Malla::info(){  
  cout<<nombre<<" contiene "<<n_vertices<<" vertices, "<<n_aristas<<" aristas y "<<n_caras<<" caras."<<endl;
}

bool Malla::igual(Malla m){  
  return nombre == m.nombre && n_vertices == m.n_vertices && n_caras == m.n_caras && caras == m.caras && vertices == m.vertices;
}

vector<float> Malla::vCercano(Vertice coords){ 
  vector<float> valores;
  float d_2, d, d_min = numeric_limits<float>::max();
  int vert_min=-1;
  
  for(int i=0; i<n_vertices;i++){
    Vertice vert = vertices[i];
    d_2 = pow(vert.getx() - coords.getx(), 2) + pow(vert.gety() - coords.gety(), 2) + pow(vert.getz() - coords.getz(), 2);
    d=sqrt(d_2);
    if(d<d_min){
      d_min = d;
      vert_min = i;
    }
  }
  if (vert_min != -1) {
    valores.push_back(vert_min);
    valores.push_back(d_min);
  }
  return valores;
}

deque<float> Malla::rutaCorta(int origen, int destino) {
  vector<deque<float>> rutas = this->grafo.dijkstra(vertices[origen]);
  return rutas[destino];
}

void Malla::rutaCortaCentro(int origen) {
  float x=0, y=0, z=0;
  for(int i = 0; i<vertices.size(); i++){
    x+=vertices[i].getx();
    y+=vertices[i].gety();
    z+=vertices[i].getz();
  }  
  x/=vertices.size();
  y/=vertices.size();
  z/=vertices.size();
  Vertice centroide(x, y, z, -1);

  vector<float> verCercano = this->vCercano(centroide);
  int idxCercano = static_cast<int>(verCercano[0]);
  deque<float> ruta = this->grafo.rutaCortaCentro(vertices[origen], centroide, vertices[idxCercano]);
  cout << "centro(" << x << ", " << y << ", " << z << "), pasa por: ";
  deque<float>::iterator it = ruta.begin();
    it++;
    int indice;
    for( ; it!= ruta.end(); it++){
      int indice = *it;
      cout<< indice << ", ";
    }
  cout << "centro; con una longitud de " << ruta[0] << "." << endl;
}
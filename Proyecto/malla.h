#ifndef __MALLA_H__
#define __MALLA_H__

#include "vertice.h"
#include "Grafo.h"
#include <string>
#include <vector>
#include <deque>
#include <map>

using namespace std;

class Malla {

public:
  string nombre;
  int n_vertices;
  int n_caras;
  int n_aristas;
  map<int, Vertice> vertices;
  vector<vector<int>> caras;
  Grafo<Vertice,float> grafo; 
  Malla* direcEnvolvente = nullptr;
  bool esEnvolvente = false;

  Malla(string nombre = "", int n_vertices =0, map<int, Vertice> vertices = {}, int n_caras =0,vector<vector<int>> caras= {});

  void info();  
  bool igual(Malla);
  vector<float> vCercano(Vertice);
  string getNombre();
  int getNumVertices();
  int getNumCaras();
  map<int, Vertice> getVertices();
  vector<vector<int>> getCaras();
  Malla* getDirecEnvolvente() const;
  void setDirecEnvolvente(Malla& malla);
  bool getEsEnvolvente() const;
  void setEsEnvolvente(bool esEnvolvente);
  deque<float> rutaCorta(int origen, int destino);
  void rutaCortaCentro(int origen);
};

#endif // MALLA_H

// eof - Malla.h
#ifndef __SISTEMA_H__
#define __SISTEMA_H__

#include "malla.h"

using namespace std;


class Sistema {

public:
  vector<Malla> mallas;
  Malla env_global;

  void mostrarComandos();  
  void ayudaComando(string, vector<string>);  
  Malla leerArchivo(string);
  void trim(string& s);
  void cargar(string); 
  void descargar(string); 
  void guardar(string, string);  
  Malla buscarMalla(string);  
  void listado();  
  void envolvente(string); 
  void envolvente(); 

  void vCercanoObj(string, float, float, float); 
  void vCercano(float, float, float);  
  void vCercano_caja(string); 

  void rutaCorta(int, int, string);  
  void rutaCorta_centro(int, string); 

};

#endif // SISTEMA_H

// eof - Sistema.h
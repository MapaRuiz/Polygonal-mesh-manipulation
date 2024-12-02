#ifndef __VERTICE_H__
#define __VERTICE_H__

#include <cmath>
using namespace std;

class Vertice {

public:
  int id;
  float x;
  float y;
  float z;

  Vertice(int id = 0, float x = 0, float y = 0, float z = 0): id(id), x(x), y(y), z(z) {}
  Vertice(float x, float y, float z, int id);
  void setx(float);
  void sety(float);
  void setz(float);
  void setid(int);
  float getx();
  float gety();
  float getz();
  int getid() const;

  bool operator==(const Vertice& other) const;
  double calcularDistancia(const Vertice& otro) const;
};

#endif // VERTICE_h

// eof - Vertice.h
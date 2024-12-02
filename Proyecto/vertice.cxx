#include <iostream>
#include "vertice.h"
using namespace std;

Vertice::Vertice(float x, float y, float z, int id){
  this->x = x;
  this->y = y;
  this->z = z;
  this->id = id;
}

void Vertice::setx(float value) {
  x = value;
}
void Vertice::sety(float value) {
  y = value;
}
void Vertice::setz(float value) {
  z = value;
}

void Vertice::setid(int value) {
id = value;
}

float Vertice::getx(){
  return x;
}

float Vertice::gety(){
  return y;
}

float Vertice::getz(){
  return z;
}

int Vertice::getid() const{
  return id;
}

bool Vertice::operator==(const Vertice& other) const{
  return (id == other.id && x == other.x && y == other.y && z == other.z);
}

double Vertice::calcularDistancia(const Vertice& otro) const {
  return sqrt((x - otro.x) * (x - otro.x) + (y - otro.y) * (y - otro.y) + (z - otro.z) * (z - otro.z));
}
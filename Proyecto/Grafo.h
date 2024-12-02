#ifndef __GRAFO_H__
#define __GRAFO_H__

#include <vector>
#include <deque>
#include <map>
#include <set>

template <typename T, typename U>
class Grafo {
public:
    vector<T> vertices;
    deque<deque<U>>aristas;

    Grafo();
    Grafo(int n_vertices, map<int, Vertice> mvertices, int n_aristas, set<vector<int>> aristas);
    ~Grafo();
    int cantidadAristas() const;
    void agregarVertice(const T& vert);
    void agregarArista(const T& origen, const T& destino, U peso);
    bool buscarVertice(const T& vertice) const;
    int indiceVertice(const T& vertice) const;
    bool buscarArista(const T& origen, const T& destino) const;
    U pesoArista(const T& origen, const T& destino) const;
    void eliminarVertice(const T& vertice);
    void eliminarArista(const T& origen, const T& destino);
    void recorridoPlano() const;
    void recorridoDFS(const T& inicio) const;
    void recorridoBFS(const T& inicio) const;
    void mostrarMatriz() const;
    vector<deque<U>> dijkstra(const T& origen) const;
    deque<U> rutaCortaCentro(const T& origen, const T& centroide, const T& vCercano) const;
};

#include "Grafo.hxx"

#endif

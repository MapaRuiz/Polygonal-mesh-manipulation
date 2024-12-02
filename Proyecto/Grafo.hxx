#include "Grafo.h"
#include <iostream>
#include <vector>
#include <queue>
#include <iomanip>
#include <stack>
#include <map>
#include <set>
#include <limits>

template <typename T, typename U>
Grafo<T,U>::Grafo(): aristas(), vertices(){}

template <typename T, typename U>
Grafo<T,U>:: Grafo(int n_vertices, map<int, Vertice> m_vertices, int n_aristas, set<vector<int>> m_aristas){
    for(int i = 0; i < n_vertices; i++){
        agregarVertice(m_vertices.at(i));
    }
    set<vector<int>>::iterator it = m_aristas.begin();
    for( ;it!=m_aristas.end(); it++){
        vector<int> arista = *it;   
        Vertice origen = m_vertices[arista[0]];
        Vertice destino = m_vertices[arista[1]];
        float distancia =  static_cast<float>(origen.calcularDistancia(destino));
        agregarArista(origen, destino, distancia);
    }
}

template <typename T, typename U>
Grafo<T,U>::~Grafo() {}

template <typename T, typename U>
int Grafo<T,U>::cantidadAristas() const {
    int conteo = 0;
    for (int i = 0; i < vertices.size(); ++i) {
        for (int j = 0; j < vertices.size(); ++j) {
            if (aristas[i][j] != std::numeric_limits<U>::max()) {
                conteo++;
            }
        }
    }
    return conteo;
}

template <typename T, typename U>
void Grafo<T,U>::agregarVertice(const T& vert) {
    if (indiceVertice(vert) == -1) {
        vertices.push_back(vert);
        int n = vertices.size();

        for (typename std::deque<std::deque<U>>::iterator itFila = aristas.begin(); itFila != aristas.end(); ++itFila) {
            itFila->resize(n, std::numeric_limits<U>::max());
        }
        aristas.resize(n, std::deque<U>(n, std::numeric_limits<U>::max()));
    }
}


template <typename T, typename U>
void Grafo<T,U>::agregarArista(const T& origen, const T& destino, U peso) {
    int i = indiceVertice(origen);
    int j = indiceVertice(destino);
    if (i != -1 && j != -1) {
        aristas[i][j] = peso;
        aristas[j][i] = peso;
    }
}

template <typename T, typename U>
bool Grafo<T,U>::buscarVertice(const T& vertice) const {
    bool encontrado = false;
    for (int i = 0; i < vertices.size(); ++i) {
        if (vertices[i] == vertice) {
            encontrado = true;
            break;
        }
    }
    return encontrado;
}

template <typename T, typename U>
int Grafo<T,U>::indiceVertice(const T& vertice) const {
    for (int i = 0; i < vertices.size(); ++i) {
        if (vertices[i] == vertice) {
            return i;
        }
    }
    return -1;
}

template <typename T, typename U>
bool Grafo<T,U>::buscarArista(const T& origen, const T& destino) const {
    int i = indiceVertice(origen);
    int j = indiceVertice(destino);
    return (i != -1 && j != -1 && (aristas[i][j] != std::numeric_limits<U>::max() || aristas[j][i] != std::numeric_limits<U>::max()));
}

template <typename T, typename U>
U Grafo<T,U>::pesoArista(const T& origen, const T& destino) const {
    int i = indiceVertice(origen);
    int j = indiceVertice(destino);
    return (i != -1 && j != -1) ? aristas[i][j] : std::numeric_limits<U>::max();
}

template <typename T, typename U>
void Grafo<T,U>::eliminarVertice(const T& vertice) {
    int index = indiceVertice(vertice);
    if (index == -1) {
        std::cout << "Vértice no encontrado." << std::endl;
        return;
    }

    // Eliminamos el vértice de la lista de vértices.
    vertices.erase(vertices.begin() + index);

    aristas.erase(aristas.begin() + index);  // Eliminar la fila

    for (int i=0; i<vertices.size(); i++) {
        aristas[i].erase(aristas.begin() + index);  // Eliminar la columna
    }
}

template <typename T, typename U>
void Grafo<T,U>::eliminarArista(const T& origen, const T& destino) {
    int i = indiceVertice(origen);
    int j = indiceVertice(destino);
    if (i != -1 && j != -1) {
        aristas[i][j] = std::numeric_limits<U>::max();
        aristas[j][i] = std::numeric_limits<U>::max();
    }
}

template <typename T, typename U>
void Grafo<T,U>::recorridoPlano() const {
    for (typename std::vector<T>::const_iterator it = vertices.begin(); it != vertices.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
}

template <typename T, typename U>
void Grafo<T,U>::recorridoDFS(const T& inicio) const {
    std::stack<int> pila;
    std::vector<bool> visitados(vertices.size(), false);
    int idxInicio = indiceVertice(inicio);

    if (idxInicio == -1) return;

    pila.push(idxInicio);
    while (!pila.empty()) {
        int idx = pila.top();
        pila.pop();
        if (!visitados[idx]) {
            std::cout << vertices[idx] << " ";
            visitados[idx] = true;
            for (int j = vertices.size() - 1; j >= 0; --j) {
                if (aristas[idx][j] != std::numeric_limits<U>::max() && !visitados[j]) {
                    pila.push(j);
                }
            }
        }
    }
    std::cout << std::endl;
}

template <typename T, typename U>
void Grafo<T,U>::recorridoBFS(const T& inicio) const {
    std::queue<int> cola;
    std::vector<bool> visitados(vertices.size(), false);
    int idxInicio = indiceVertice(inicio);

    if (idxInicio == -1) return;

    cola.push(idxInicio);
    visitados[idxInicio] = true;

    while (!cola.empty()) {
        int idx = cola.front();
        cola.pop();
        std::cout << vertices[idx] << " ";
        for (int j = 0; j < vertices.size(); ++j) {
            if (aristas[idx][j] != std::numeric_limits<U>::max() && !visitados[j]) {
                visitados[j] = true;
                cola.push(j);
            }
        }
    }
    std::cout << std::endl;
}

template <typename T, typename U>
void Grafo<T,U>::mostrarMatriz() const {
    cout << "           ";
    for (int i=0; i<vertices.size(); i++) {
        cout << setw(10) << vertices[i].getid() << " ";
    }
    cout << endl;

    for (int i = 0; i < vertices.size(); i++) {
        cout << setw(10) << vertices[i].getid() << " ";
        for (int j = 0; j < vertices.size(); j++) {
            if (aristas[i][j] == std::numeric_limits<U>::max()) {
                cout << setw(10) << "-" << " ";
            } else {
                cout << setw(10) << aristas[i][j] << " ";
            }
        }
        cout << endl;
    }
}

template <typename T, typename U>
deque<U> Grafo<T,U>::rutaCortaCentro(const T& origen, const T& centroide, const T& vCercano) const {
    Grafo<T, U> grafoTemporal = *this;
    int idx = indiceVertice(vCercano);
    float distancia = static_cast<float>(centroide.calcularDistancia(vertices[idx]));

    grafoTemporal.agregarVertice(centroide);    
    grafoTemporal.agregarArista(centroide, vertices[idx], distancia);

    vector<deque<U>> rutas = grafoTemporal.dijkstra(origen);
    int idxCentroide = grafoTemporal.indiceVertice(centroide);
    return rutas[idxCentroide];
}

template <typename T, typename U>
vector<deque<U>> Grafo<T,U>::dijkstra(const T& origen) const {
    vector<U> distancias(vertices.size(), std::numeric_limits<U>::max());
    vector<deque<U>> rutas(vertices.size());
    vector<bool> visitados(vertices.size(), false);

    int idx = indiceVertice(origen);
    distancias[idx] = U();

    for(int i=0; i< vertices.size(); i++){
        int u = -1;
        for (int j = 0; j < vertices.size(); j++) {
            if (!visitados[j] && (u == -1 || distancias[j] < distancias[u])) {
                u = j;
            }
        }
        visitados[u] = true;
        for (int v = 0; v < vertices.size(); v++) {
            if (aristas[u][v] != std::numeric_limits<U>::max() && !visitados[v]) {
                U peso = aristas[u][v];
                if (distancias[u] + peso < distancias[v]) {
                    distancias[v] = distancias[u] + peso;
                    rutas[v] = rutas[u];
                    rutas[v].push_back(u);
                }
            }
        }
    }
    for(int i=0; i< vertices.size(); i++){
        rutas[i].push_front(distancias[i]);
    }
    return rutas;
}

#ifndef COMPORTAMIENTOJUGADOR_H
#define COMPORTAMIENTOJUGADOR_H

#include "comportamientos/comportamiento.hpp"

#include <list>
#include <set>
#include <queue>
#include <stack>


struct estado {
  int fila;
  int columna;
  int orientacion;
  list<estado> anteriores;
};

class ComportamientoJugador : public Comportamiento {
public:
  ComportamientoJugador(unsigned int size) : Comportamiento(size) {
    // Inicializar Variables de Estado
    fil = col = 99;
    brujula = 0; // 0: Norte, 1:Este, 2:Sur, 3:Oeste
    destino.fila = -1;
    destino.columna = -1;
    destino.orientacion = -1;
    ultimaAccion = actIDLE;
    hayPlan = false;
    //Inicializar matriz
    visitado = new bool*[TAM];
    for (int i = 0; i < TAM; i++) {
      visitado[i] = new bool[TAM];
    }
  }
  ComportamientoJugador(std::vector< std::vector< unsigned char> > mapaR) : Comportamiento(mapaR) {
    // Inicializar Variables de Estado
    fil = col = 99;
    brujula = 0; // 0: Norte, 1:Este, 2:Sur, 3:Oeste
    destino.fila = -1;
    destino.columna = -1;
    destino.orientacion = -1;
    ultimaAccion = actIDLE;
    hayPlan = false;
    //Inicializar matriz
    visitado = new bool*[TAM];
    for (int i = 0; i < TAM; i++)
      visitado[i] = new bool[TAM];

  }
  ComportamientoJugador(const ComportamientoJugador & comport) : Comportamiento(comport) {}
  ~ComportamientoJugador() {
    //Liberar matriz
    for (int i = 0; i < TAM; i++)
      delete []visitado[i];
    delete []visitado;
  }

  Action think(Sensores sensores);
  int interact(Action accion, int valor);
  void VisualizaPlan(const estado &st, const list<Action> &plan);

  ComportamientoJugador * clone() {return new ComportamientoJugador(*this);}


private:
  // Declarar Variables de Estado
  int fil, col, brujula;
  estado destino;
  list<Action> plan;
  /**Variables y métodos definidos por mi**/
  bool **visitado;  //Matriz de visitados. Sería equivalente a la cola de cerrados, pero así el algoritmo es más eficiente.
  const int TAM = mapaResultado.size(); //Tamaño del mapa
  const set<char> PUEDO_PASAR = {'S', 'K', 'T'};  //Casillas por las que puedo pasar

  list<estado> BusquedaEnAnchura(const estado &origen, const estado &destino);
  list<Action> calcularListaAcciones(const list<estado> &lista);
  void InicializarCerrados();
  estado CrearAdyacente(int f, int c, int o, const estado &actual);
  bool EsViable(int fila, int columna/*, queue<estado> q*/);
  bool Contiene(int fila, int columna, queue<estado> q);

  /****************************************/
  // Nuevas Variables de Estado
  Action ultimaAccion;
  bool hayPlan;
  bool pathFinding(const estado & origen, const estado & destino, list<Action> &plan);
  void PintaPlan(list<Action> plan);




};

#endif

#ifndef COMPORTAMIENTOJUGADOR_H
#define COMPORTAMIENTOJUGADOR_H

#include "comportamientos/comportamiento.hpp"

#include <list>

struct estado {
  int fila;
  int columna;
  int orientacion;
  int d;
  list<estado> anteriores;
};

struct estado2 {
  estado status;
  int distancia;
};
struct estadoConAntecesores {
  estado status;
  list<Action> antecesores;
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
  }
  ComportamientoJugador(const ComportamientoJugador & comport) : Comportamiento(comport) {}
  ~ComportamientoJugador() {}

  Action think(Sensores sensores);
  int interact(Action accion, int valor);
  void VisualizaPlan(const estado &st, const list<Action> &plan);
<<<<<<< HEAD

  ComportamientoJugador * clone() {return new ComportamientoJugador(*this);}


  const set<char> PUEDO_PASAR = {'S', 'K', 'T'};
  list<Action> busquedaEnProfundidad(const estado &origen, const estado &destino) ;
  estadoConAntecesores calcularEstado(const estadoConAntecesores &actual, const Action &accion) ;
=======

  ComportamientoJugador * clone() {return new ComportamientoJugador(*this);}
  list<estado> BFS(const estado &origen, const estado &destino);
>>>>>>> a394d72e8e5b6881c024c10e5d0f48a9d21992bc

private:
  // Declarar Variables de Estado
  int fil, col, brujula;
  estado destino;
  list<Action> plan;
<<<<<<< HEAD
=======

  // Nuevas Variables de Estado
  Action ultimaAccion;
  bool hayPlan;


>>>>>>> a394d72e8e5b6881c024c10e5d0f48a9d21992bc

  // Nuevas Variables de Estado
  Action ultimaAccion;
  bool hayPlan;
  bool pathFinding(const estado &origen, const estado &destino, list<Action> &plan);
  void PintaPlan(list<Action> plan);




};

#endif

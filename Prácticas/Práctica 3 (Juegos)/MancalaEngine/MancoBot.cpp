/*
 * MancoBot.cpp
 *
 *  Created on: 20 mayo 2018
 *      Author: csp98
 */

#include "MancoBot.h"

#include <cstdlib>
#include <iostream>
#include <limits>
#include <string>
using namespace std;

#define DEBUG 0
#if DEBUG
long long int generados = 0;
#endif

MancoBot::MancoBot() {}

MancoBot::~MancoBot() {}

void MancoBot::initialize() {}

string MancoBot::getName() { return NOMBRE; }

/******************************************************************************/

// Devuelve true si el movimiento causa inmolación (pérdida por 48-0)
bool MancoBot::Inmolacion(const GameState &estado, const Move &mov) const {
  Position posicion = (Position)mov;
  return estado.getSeedsAt(yo, posicion) == 0;
}

// Calcula los sucesores válidos de un nodo (no causan inmolación)
list<node> MancoBot::calcularSucesores(const GameState &estado) const {
  list<node> resultado;
  node nuevo;
  for (auto it = MOVIMIENTOS.begin(); it != MOVIMIENTOS.end(); ++it) {
    if (!Inmolacion(estado, *it)) { // Si el movimiento no causa inmolación
      nuevo.estado = estado.simulateMove(*it); // Lo almaceno
      nuevo.movimiento = *it;
      resultado.push_back(nuevo);
#if DEBUG
      generados++;
#endif
    }
  }

  return resultado;
}

// True si hay que podar
bool MancoBot::Podar(int alpha, int beta) const { return alpha >= beta; }

// Obtiene las semillas que tiene un determinado jugador en total
int MancoBot::ObtenerSemillas(const Player &p, const GameState &estado) const {
  int semillas = 0;
  for (auto it = POSICIONES.begin(); it != POSICIONES.end(); ++it) {
    semillas += estado.getSeedsAt(p, *it);
  }
  return semillas;
}

// Devuelve el valor heurístico de un nodo
int MancoBot::CalcularHeuristica(const GameState &estado) const {
  int resultado;
  int mi_heuristica = 0;
  int heuristica_oponente = 0;
  Player ganador = estado.getWinner();

  // Si el movimiento me hace ganador, lo elijo.

  if (ganador == yo)
    mi_heuristica += 500; // Valor grande
  else if (ganador == oponente)
    heuristica_oponente += 500;

  // Sumo las puntuaciones de los graneros.

  mi_heuristica += estado.getScore(yo);
  heuristica_oponente += estado.getScore(oponente);

  // Si puedo volver a tirar, lo hago (siempre que no pierda puntos)

  if (mi_heuristica == heuristica_oponente && estado.getCurrentPlayer() == yo)
    mi_heuristica += 200;

  /****/
  //  int mis_semillas = ObtenerSemillas(yo, estado);
  //  int sus_semillas = ObtenerSemillas(oponente, estado);
  /****/

  resultado = mi_heuristica - heuristica_oponente;
  //  resultado =
  //      (mi_heuristica - heuristica_oponente) + (mis_semillas - sus_semillas);

  return resultado;
}
// Algoritmo minimax alpha-beta. Devuelve el coste heurístico de una rama.
int MancoBot::alphaBeta(const node &nodo, int profundidad, int alpha, int beta,
                        bool esNodoMax) const {
  int resultado;
  if (profundidad == 0) { // Caso base: nodo terminal. Calculamos heurística.
    resultado = CalcularHeuristica(nodo.estado);
  } else { // No es un nodo terminal. Sigo explorando.

    int valor;
    list<node> hijos = calcularSucesores(nodo.estado); // Calculo sus hijos.

    if (esNodoMax) { // Nodo MAX, el siguiente es min y devuelvo alpha
      for (auto it = hijos.begin(); it != hijos.end() && !Podar(alpha, beta);
           ++it) {
        valor = alphaBeta(*it, profundidad - 1, alpha, beta, false);
        if (valor > alpha)
          alpha = valor;
      }
      resultado = alpha;
    } else { // Nodo min. El siguiente es MAX y devuelvo beta
      for (auto it = hijos.begin(); it != hijos.end() && !Podar(alpha, beta);
           ++it) {
        valor = alphaBeta(*it, profundidad - 1, alpha, beta, true);
        if (valor < beta)
          beta = valor;
      }
      resultado = beta;
    }
  }
  return resultado;
}

// Devuelve el siguiente movimiento a realizar tras realizar una exploración.
Move MancoBot::nextMove(const vector<Move> &adversary, const GameState &state) {
  if (primera_vez) { // Si es la primera vez, configuro las variables internas.
    yo = state.getCurrentPlayer();
    if (yo == J1)
      oponente = J2;
    else
      oponente = J1;
    primera_vez = false;
  }

#if DEBUG
  generados = 0;
  auto antes = high_resolution_clock::now();
#endif

  list<node> sucesores = calcularSucesores(state);
  Move movimiento = M_NONE;

  int max = numeric_limits<int>::min();
  int alpha = numeric_limits<int>::min();
  int beta = numeric_limits<int>::max();

  for (auto it = sucesores.begin(); it != sucesores.end(); ++it) {
    it->heuristica = alphaBeta(*it, PROFUNDIDAD_INICIAL, alpha, beta, false);
    if (it->heuristica >= max) {
      max = it->heuristica;
      movimiento = it->movimiento;
    }
  }

#if DEBUG
  auto despues = high_resolution_clock::now();
  auto tiempo = duration_cast<duration<double>>(despues - antes);
  cerr << "Tiempo de cómputo: " << tiempo.count() << endl;
  cerr << " generados " << generados << endl;
#endif
  return movimiento;
}

/*
// ARREGLO TEMPORAL GETSEEDSAT -> PRIMERA_VEZ
// Devuelve el siguiente movimiento a realizar tras realizar una exploración.
Move MancoBot::nextMove(const vector<Move> &adversary, const GameState &state) {
#if DEBUG
  generados = 0;
  auto antes = high_resolution_clock::now();
#endif

  list<node> sucesores = calcularSucesores(state);
  Move movimiento = M_NONE;
  // Ajusto las variables internas y (devuelvo el mejor movimiento).
  if (primera_vez) {
    yo = state.getCurrentPlayer();
    if (yo == J1)
      oponente = J2;
    else
      oponente = J1;
    primera_vez = false;
    movimiento = M4;
  } else {
    int max = numeric_limits<int>::min();
    int alpha = numeric_limits<int>::min();
    int beta = numeric_limits<int>::max();

    for (auto it = sucesores.begin(); it != sucesores.end(); ++it) {
      it->heuristica = alphaBeta(*it, PROFUNDIDAD_INICIAL, alpha, beta, false);
      if (it->heuristica >= max) {
        max = it->heuristica;
        movimiento = it->movimiento;
      }
    }
  }

#if DEBUG
  auto despues = high_resolution_clock::now();
  auto tiempo = duration_cast<duration<double>>(despues - antes);
  cerr << "Tiempo de cómputo: " << tiempo.count() << endl;
  cerr << "Nodos generados: " << generados << endl;
#endif
  return movimiento;
}
*/
/*
int MancoBot::CalcularValorMiniMax(const GameState &estado) const {
  int resultado;
  list<node> sucesores = calcularSucesores(estado);
  if (sucesores.empty()) // Caso base: node terminal
    resultado = CalcularHeuristica(estado);
  else {
    // Calculo valores minimax de los hijos
    if (estado.getCurrentPlayer() == J1) { // Estoy en un node MAX
      //
    } else { // Estoy en un node min
      //
    }
  }
  return resultado;
}sucesores
*/
/*
Move MancoBot::MiniMaxAB(const GameState &estado, int &alpha, int &beta,
                         const Player &p) const {
  Move movimiento = M_NONE;
  return movimiento;
}
*/

/*
int MancoBot::CalcularHeuristica(const GameState &estado) const {
  int resultado;
  if (estado.getWinner() == estado.getCurrentPlayer()) // Gana el juego.
    resultado = 9999999;
  else // Calculo las semillas en mi granero
    resultado = estado.getScore(estado.getCurrentPlayer());
  return resultado;
}
*/
/*
Move MancoBot::nextMove(const vector<Move> &adversary, const GameState &state)
{

  Move movimiento;
  int alpha = numeric_limits<int>::min();
  int beta = numeric_limits<int>::max();
  movimiento = MiniMaxAB(state, alpha, beta, state.getCurrentPlayer());
  // OJO: Recordatorio. NO USAR cin NI cout.
  // Para salidas por consola (debug) utilizar cerr. Ejemplo:
  // cerr<< "Lo que quiero mostrar"<<endl;

  return movimiento;
}
*/

/*
Move MancoBot::Maximizador(const list<node> &sucesores, int profundidad,
                           int &alpha, int beta) const {
  Move resultado = sucesores.front().movimiento;
  Move sucesor;
  int valor_minimax;
  for (auto it = next(sucesores.begin());
       it != sucesores.end() && !CriterioPoda(alpha, beta); ++it) {
    sucesor = MiniMax(it->estado, profundidad - 1, alpha, beta);
    valor_minimax = CalcularValorMiniMax(it->estado);
    if (valor_minimax > alpha) {
      alpha = valor_minimax;
      resultado = sucesor;
    }
  }
  return resultado;
}

Move MancoBot::Minimizador(const list<node> &sucesores, int profundidad,
                           int alpha, int &beta) const {
  Move resultado = sucesores.front().movimiento;
  Move sucesor;
  int valor_minimax;
  for (auto it = next(sucesores.begin());
       it != sucesores.end() && !CriterioPoda(alpha, beta); ++it) {
    sucesor = MiniMax(it->estado, profundidad - 1, alpha, beta);
    valor_minimax = CalcularValorMiniMax(it->estado);
    if (valor_minimax < beta) {
      beta = valor_minimax;
      resultado = sucesor;
    }
  }
  return resultado;
}

Move MancoBot::MiniMax(const GameState &estado, int profundidad, int alpha,
                       int beta) const {
  Move resultado = M_NONE;      // Por si no quedan posibles movimientos.
  if (!estado.isFinalState()) { // Si no estamos en el node final
    list<node> sucesores;
    sucesores = calcularSucesores(estado); // Calculo la lista de sucesores
    if (!sucesores.empty()) { // Si puedo realizar algún movimiento
      if (estado.getCurrentPlayer() == J1) // Si es un node MAX
        resultado = Maximizador(sucesores, profundidad, alpha, beta);
      else // Si es un node min
        resultado = Minimizador(sucesores, profundidad, alpha, beta);
    }
  }

  return resultado;
}
*/

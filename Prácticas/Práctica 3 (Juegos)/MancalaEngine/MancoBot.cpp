/*
 * MancoBot.cpp
 *
 *  Created on: 15 ene. 2018
 *      Author: manupc
 */

#include "MancoBot.h"

#include <cstdlib>
#include <iostream>
#include <limits>
#include <string>
using namespace std;

MancoBot::MancoBot() {
  // Inicializar las variables necesarias para ejecutar la partida
}

MancoBot::~MancoBot() {
  // Liberar los recursos reservados (memoria, ficheros, etc.)
}

void MancoBot::initialize() {
  // Inicializar el bot antes de jugar una partida
}

string MancoBot::getName() {
  return NOMBRE; // Sustituir por el nombre del bot
}

bool MancoBot::MovimientoLegal(const GameState &estado, const Move &mov) const {
  Position posicion;
  switch (mov) {
  case M1:
    posicion = P1;
    break;
  case M2:
    posicion = P2;
    break;
  case M3:
    posicion = P3;
    break;
  case M4:
    posicion = P4;
    break;
  case M5:
    posicion = P5;
    break;
  case M6:
    posicion = P6;
    break;
  }
  return estado.getSeedsAt(estado.getCurrentPlayer(), posicion) != 0;
}

list<node> MancoBot::calcularSucesores(const GameState &estado) const {
  list<node> resultado;
  node nuevo;
  for (auto it = POSIBLES_MOVIMIENTOS.begin(); it != POSIBLES_MOVIMIENTOS.end();
       ++it) {
    nuevo.estado = estado.simulateMove(*it);
    nuevo.movimiento = *it;
    if (MovimientoLegal(nuevo.estado, nuevo.movimiento))
      resultado.push_back(nuevo);
  }
  return resultado;
}

bool MancoBot::CriterioPoda(int alpha, int beta) const { // True si podamos
  return alpha >= beta;
}

int MancoBot::alphaBeta(const node &nodo, int profundidad, int alpha, int beta,
                        bool esNodoMax) const {
  int resultado;
  if (profundidad == 0) { // Caso base: nodo terminal
    resultado = nodo.estado.getScore(nodo.estado.getCurrentPlayer());
    /* V2 -> resultado = nodo.estado.getScore(J1) - nodo.estado.getScore(J2);*/
    //Incorporar mejores heurísticas
  } else {
    int valor;
    list<node> sucesores = calcularSucesores(nodo.estado);

    if (esNodoMax) { // Si estoy en un nodo MAX, el siguiente es min
      for (auto it = sucesores.begin();
           it != sucesores.end() && !CriterioPoda(alpha, beta); ++it) {
        valor = alphaBeta(*it, profundidad - 1, alpha, beta, false);
        if (valor > alpha)
          alpha = valor;
      }
      resultado = alpha;
    } else { // Nodo min. El siguiente es MAX
      for (auto it = sucesores.begin();
           it != sucesores.end() && !CriterioPoda(alpha, beta); ++it) {
        valor = alphaBeta(*it, profundidad - 1, alpha, beta, true);
        if (valor < beta)
          beta = valor;
      }
      resultado = beta;
    }
  }
  return resultado;
}

Move MancoBot::nextMove(const vector<Move> &adversary, const GameState &state) {
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
  return movimiento;
}

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
}
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
Move MancoBot::nextMove(const vector<Move> &adversary, const GameState &state) {

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

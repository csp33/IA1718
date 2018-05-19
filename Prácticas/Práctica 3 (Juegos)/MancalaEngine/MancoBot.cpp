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

int MancoBot::CalcularHeuristica(const GameState &estado) const {
  int resultado;
  if (estado.getWinner() == estado.getCurrentPlayer()) // Gana el juego.
    resultado = 9999999;
  else // Calculo las semillas en mi granero
    resultado = estado.getScore(estado.getCurrentPlayer());
  return resultado;
}

list<nodo> MancoBot::calcularSucesores(const GameState &estado) const {
  list<nodo> resultado;
  nodo nuevo;
  for (auto it = POSIBLES_MOVIMIENTOS.begin(); it != POSIBLES_MOVIMIENTOS.end();
       ++it) {
    nuevo.estado = estado.simulateMove(*it);
    nuevo.movimiento = *it;
    if (nuevo.estado.isValidState())
      resultado.push_back(nuevo);
  }
  return resultado;
}

bool MancoBot::CriterioPoda(int alpha, int beta) const { // True si podamos
  return alpha >= beta;
}

int MancoBot::CalcularValorMiniMax(const GameState &estado) const {
  int resultado;
  list<nodo> sucesores = calcularSucesores(estado);
  if (sucesores.empty()) // Caso base: nodo terminal
    resultado = CalcularHeuristica(estado);
  else {
    // Calculo valores minimax de los hijos
    if (estado.getCurrentPlayer() == J1) { // Estoy en un nodo MAX
      //
    } else { // Estoy en un nodo min
      //
    }
  }
  return resultado;
}

Move MancoBot::MiniMaxAB(const GameState &estado, int &alpha, int &beta,
                         const Player &p) const {
  Move movimiento = M_NONE;
  // TODO
  return movimiento;
}

/*****
Opciones:
  1) Tipo de dato nodo (estado+movimiento que lo llevó a él)
  2) Tipo de dato nodo (heurística (solo en terminales) + movimiento + estado)
**/

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

/*
Move MancoBot::Maximizador(const list<nodo> &sucesores, int profundidad,
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

Move MancoBot::Minimizador(const list<nodo> &sucesores, int profundidad,
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
  if (!estado.isFinalState()) { // Si no estamos en el nodo final
    list<nodo> sucesores;
    sucesores = calcularSucesores(estado); // Calculo la lista de sucesores
    if (!sucesores.empty()) { // Si puedo realizar algún movimiento
      if (estado.getCurrentPlayer() == J1) // Si es un nodo MAX
        resultado = Maximizador(sucesores, profundidad, alpha, beta);
      else // Si es un nodo min
        resultado = Minimizador(sucesores, profundidad, alpha, beta);
    }
  }

  return resultado;
}
*/

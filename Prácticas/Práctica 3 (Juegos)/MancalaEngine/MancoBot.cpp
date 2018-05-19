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

list<GameState> MancoBot::calcularSucesores(const GameState &estado) const {
  list<GameState> resultado;
  for (auto it = POSIBLES_MOVIMIENTOS.begin(); it != POSIBLES_MOVIMIENTOS.end();
       ++it)
    resultado.push_back(estado.simulateMove(*it));
  return resultado;
}

Move MancoBot::CalcularMovimiento(const GameState &estado) const {
  /********TODO********************/
  return M_NONE;
}

bool MancoBot::CriterioPoda(int alpha,
                            int beta) const { // True si hay que podar
  return alpha >= beta;
}

int MancoBot::CalcularValorMiniMax(const GameState &estado) const {
  /* ¿Parámetro estado o movimiento?*/
  /********TODO********************/
  return 0;
}

Move MancoBot::Maximizador(const list<GameState> &sucesores, int profundidad,
                           int alpha, int beta) const {
  Move resultado = CalcularMovimiento(sucesores.front());
  Move sucesor;
  int valor_minimax;
  for (auto it = next(sucesores.begin());
       it != sucesores.end() && !CriterioPoda(alpha, beta); ++it) {
    sucesor = MiniMax(*it, profundidad - 1, alpha, beta);
    valor_minimax = CalcularValorMiniMax(*it /* ¿o sucesor?*/);
    if (valor_minimax > alpha) {
      alpha = valor_minimax;
      resultado = sucesor;
    }
  }
  return resultado;
}

Move MancoBot::Minimizador(const list<GameState> &sucesores, int profundidad,
                           int alpha, int beta) const {
  /********TODO********************/
  return M_NONE;
}

Move MancoBot::MiniMax(const GameState &estado, int profundidad, int alpha,
                       int beta) const {
  Move resultado = M_NONE;      // Por si no quedan posibles movimientos.
  if (!estado.isFinalState()) { // Si no estamos en el nodo final
    list<GameState> sucesores;
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

Move MancoBot::nextMove(const vector<Move> &adversary, const GameState &state) {

  Move movimiento = M_NONE;
  int alpha = numeric_limits<int>::min();
  int beta = numeric_limits<int>::max();
  movimiento = MiniMax(state, 20, alpha, beta);
  // OJO: Recordatorio. NO USAR cin NI cout.
  // Para salidas por consola (debug) utilizar cerr. Ejemplo:
  // cerr<< "Lo que quiero mostrar"<<endl;

  return movimiento;
}

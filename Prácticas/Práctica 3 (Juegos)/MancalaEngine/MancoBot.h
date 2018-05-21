/*
 * MancoBot.h
 *
 *  Created on: 15 ene. 2018
 *      Author: manupc
 */

#include "Bot.h"
#include <chrono>
#include <ctime>
#include <list>
#include <ratio>
#include <vector>

using namespace std::chrono;

#ifndef MANCOBOT_H_
#define MANCOBOT_H_

struct node {
  GameState estado;
  Move movimiento;
};

class MancoBot : Bot {
public:
  MancoBot();
  ~MancoBot();

  void initialize();
  string getName();
  Move nextMove(const vector<Move> &adversary, const GameState &state);

private:
  // Variables

  const string NOMBRE = "MancoBot";
  const vector<Move> MOVIMIENTOS = {M1, M2, M3, M4, M5, M6};
  const vector<Position> POSICIONES = {P1, P2, P3, P4, P5, P6};
  const int PROFUNDIDAD_MAXIMA =6; // Con 6 va perfecto
  Player yo;
  Player oponente;
  bool primera_vez = true;
  const int MIN = numeric_limits<int>::min();
  const int MAX = numeric_limits<int>::max();

  // Métodos

  bool Podar(int alpha, int beta) const;

  bool Inmolacion(const GameState &estado, const Move &mov) const;

  list<node> calcularSucesores(const GameState &estado) const;

  Move obtenerMovimiento(const GameState &estado) const;

  int alphaBeta(const GameState &estado, int profundidad, int alpha, int beta,
                bool mi_turno) const;

  int CalcularHeuristica(const GameState &estado) const;

  int CuantasRoba(const Player &ladron, const GameState &estado,
                  const Move &mov) const;

  int ObtenerSemillas(const Player &p, const GameState &estado) const;
  int Maximo(int a, int b) const;
  int Minimo(int a, int b) const;
};
#endif /* MANCOBOT_H_ */

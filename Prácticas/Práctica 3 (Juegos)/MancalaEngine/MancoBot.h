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
  int heuristica;
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

  // 6
  const int PROFUNDIDAD_INICIAL = 6;
  Player yo;
  Player oponente;
  bool primera_vez = true;
  bool primer_turno = true;

  // Métodos

  bool Podar(int alpha, int beta) const;
  list<node> calcularSucesores(const GameState &estado) const;
  int alphaBeta(const node &nodo, int profundidad, int alpha, int beta,
                bool esNodoMax) const;
  int CalcularHeuristica(const GameState &estado) const;
  bool Inmolacion(const GameState &estado, const Move &mov) const;
  int ObtenerSemillas(const Player &p, const GameState &estado) const;
};

#endif /* MANCOBOT_H_ */

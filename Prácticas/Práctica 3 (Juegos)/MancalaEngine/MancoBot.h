/*
 * MancoBot.h
 *
 *  Created on: 15 ene. 2018
 *      Author: manupc
 */

#include "Bot.h"
#include <list>
#include <vector>

#ifndef MANUPCBOT_H_
#define MANUPCBOT_H_

class MancoBot : Bot {
public:
  MancoBot();
  ~MancoBot();

  void initialize();
  string getName();
  Move nextMove(const vector<Move> &adversary, const GameState &state);

private:
  const string NOMBRE = "MancoBot";
  list<GameState> calcularSucesores(const GameState &estado) const;
  bool CriterioPoda(int alpha, int beta) const;
  Move Minimizador(const list<GameState> &sucesores, int profundidad, int alpha,
                   int beta) const;
  Move Maximizador(const list<GameState> &sucesores, int profundidad, int alpha,
                   int beta) const;
  const vector<Move> POSIBLES_MOVIMIENTOS = {M1, M2, M3, M4, M5, M6};
  Move MiniMax(const GameState &estado, int profunidad, int alpha,
               int beta) const;
  Move CalcularMovimiento(const GameState &estado) const;
  int CalcularValorMiniMax(const GameState &estado) const;
};

#endif /* MANUPCBOT_H_ */

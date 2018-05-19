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

struct nodo {
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
  const vector<Move> POSIBLES_MOVIMIENTOS = {M1, M2, M3, M4, M5, M6};
  // Métoddos
  int CalcularHeuristica(const GameState &estado) const;
  list<nodo> calcularSucesores(const GameState &estado) const;
  bool CriterioPoda(int alpha, int beta) const;
  int CalcularValorMiniMax(const GameState &estado) const;
  Move MiniMaxAB(const GameState &estado, int &alpha, int &beta,
                 const Player &p) const;

  //  Move CalcularMovimiento(const GameState &estado) const;
  // Move Minimizador(const list<nodo> &sucesores, int profundidad, int alpha,
  //                 int &beta) const;
  //  Move Maximizador(const list<nodo> &sucesores, int profundidad, int &alpha,
  //                 int beta) const;
  //  Move MiniMax(const GameState &estado, int profundidad, int alpha,
  //         int beta) const;
};

#endif /* MANUPCBOT_H_ */

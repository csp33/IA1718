/*
 * MancoBot.h
 *
 *  Created on: 15 ene. 2018
 *      Author: manupc
 */

#include "Bot.h"
#include <list>
#include <vector>

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
  const vector<Move> POSIBLES_MOVIMIENTOS = {M1, M2, M3, M4, M5, M6};
  const int PROFUNDIDAD_INICIAL = 5;
  // Métodos

  bool CriterioPoda(int alpha, int beta) const;
  list<node> calcularSucesores(const GameState &estado) const;
  int alphaBeta(const node &nodo, int profundidad, int alpha, int beta,
                bool esNodoMax) const;
  bool MovimientoLegal(const GameState &estado, const Move &mov) const;

  //  int CalcularValorMiniMax(const GameState &estado) const;
  //  Move MiniMaxAB(const GameState &estado, int &alpha, int &beta,
  //                 const Player &p) const;
  //  int CalcularHeuristica(const GameState &estado) const;
  //  Move CalcularMovimiento(const GameState &estado) const;
  // Move Minimizador(const list<nodo> &sucesores, int profundidad, int alpha,
  //                 int &beta) const;
  //  Move Maximizador(const list<nodo> &sucesores, int profundidad, int &alpha,
  //                 int beta) const;
  //  Move MiniMax(const GameState &estado, int profundidad, int alpha,
  //         int beta) const;
};

#endif /* MANCOBOT_H_ */

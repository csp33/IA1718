/***********************
Jugador para Mancala.
Inteligencia Artificial, curso 2017-2018
Carlos Sánchez Páez
***********************/

#include "Bot.h"
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <limits>
#include <list>
#include <ratio>
#include <string>
#include <vector>
using namespace std;

using namespace std::chrono;

#ifndef MANCOBOT_H_
#define MANCOBOT_H_

struct node {
  GameState estado;
  Move movimiento;
};

class MancoBot : Bot {
private:
  /******Constantes********/

  const string NOMBRE = "MancoBot";

  const vector<Move> MOVIMIENTOS = {M1, M2, M3, M4, M5, M6};

  const vector<Position> POSICIONES = {P1, P2, P3, P4, P5, P6};

  const int PROFUNDIDAD_MAXIMA = 8;
  // Profundidad=8 máximos generados=118502 (J1) 135145 (J2)
  // Profundidad=8 máximos generados=148234 (J1) 310635 (J2)
  Player oponente, yo;

  bool primera_vez = true;

  const int MIN = numeric_limits<int>::min();

  const int MAX = numeric_limits<int>::max();

  /******Métodos********/

  // Métodos auxiliares

  int Maximo(int a, int b) const;

  int Minimo(int a, int b) const;

  // Métodos para el cálculo de la heurística

  int CalcularHeuristica(const GameState &estado) const;

  int CuantasRoba(const Player &ladron, const GameState &estado,
                  const Move &mov) const;

  int ObtenerSemillas(const Player &p, const GameState &estado) const;

  // Métodos para el cálculo de los sucesores

  bool Inmolacion(const GameState &estado, const Move &mov) const;

  list<node> calcularSucesores(const GameState &estado) const;

  // Métodos para el algoritmo Minimax con poda alpha-beta

  int alphaBeta(const GameState &estado, int profundidad, int alpha, int beta,
                bool mi_turno) const;

  bool Podar(int alpha, int beta) const;

  Move obtenerMovimiento(const GameState &estado) const;

public:
  MancoBot();

  ~MancoBot();

  void initialize();

  string getName();

  Move nextMove(const vector<Move> &adversary, const GameState &state);
};

#endif

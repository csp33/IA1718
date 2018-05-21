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

#define DEBUG 1
#if DEBUG
long long int generados = 0;
#endif

MancoBot::MancoBot() {}

MancoBot::~MancoBot() {}

void MancoBot::initialize() {}

string MancoBot::getName() { return NOMBRE; }

/******************************************************************************/

// Devuelve el máximo valor
int MancoBot::Maximo(int a, int b) const { return a > b ? a : b; }

int MancoBot::Minimo(int a, int b) const { return a < b ? a : b; }

// Devuelve true si el movimiento causa inmolación (pérdida por 48-0)
bool MancoBot::Inmolacion(const GameState &estado, const Move &mov) const {
  Position posicion = (Position)mov;
  return estado.getSeedsAt(yo, posicion) == 0;
}

// Calcula los sucesores válidos de un nodo (no causan inmolación)
list<node> MancoBot::calcularSucesores(const GameState &estado) const {
  list<node> resultado;
  for (auto it = MOVIMIENTOS.begin(); it != MOVIMIENTOS.end(); ++it) {
    if (!Inmolacion(estado, *it)) { // Si el movimiento no causa inmolación
      node nuevo;
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

// Devuelve el número de semillas que el jugador parámetro puede robar.
int MancoBot::CuantasRoba(const Player &ladron, const GameState &estado,
                          const Move &mov) const {
  int cuantas = 0;
  // Configuro víctima
  Player victima;
  if (ladron == J1)
    victima = J2;
  else // No se puede dar el caso p==NONE
    victima = J1;

  // Calculo datos del estado anterior
  int anterior = estado.getScore(ladron);
  int sus_semillas = ObtenerSemillas(victima, estado);
  GameState nuevo;

  if (!Inmolacion(estado, mov)) { // Si el movimiento es legal
    // Simulo el movimiento
    nuevo = estado.simulateMove(mov);
    int actual = nuevo.getScore(ladron);
    // Si he robado, calculo la diferencia
    if (actual > anterior && sus_semillas < ObtenerSemillas(victima, nuevo))
      cuantas = actual - anterior;
  }
  return cuantas;
}

// Devuelve el valor heurístico de un nodo

int MancoBot::CalcularHeuristica(const GameState &estado) const {
  int resultado;
  double mi_heuristica = 0.0;
  double heuristica_oponente = 0.0;

  // Si el movimiento me hace ganador, lo elijo.

  Player ganador = estado.getWinner();
  if (ganador == yo)
    mi_heuristica += 500; // Valor grande
  else if (ganador == oponente)
    heuristica_oponente += 500;

  // Sumo las puntuaciones de los graneros (constante alta, están garantizadas).

  mi_heuristica += estado.getScore(yo) * 4.5;
  heuristica_oponente += estado.getScore(oponente) * 4.5;

  // Sumo las semillas que hay en el tablero (constante menor, no garantizadas)

  mi_heuristica += ObtenerSemillas(yo, estado) * 0.3;
  heuristica_oponente += ObtenerSemillas(oponente, estado) * 0.3;

  // Sumo los posibles robos
  /*
    for (auto it = MOVIMIENTOS.begin(); it != MOVIMIENTOS.end(); ++it) {
      mi_heuristica += CuantasRoba(yo, estado, *it) * 0.5;
      heuristica_oponente += CuantasRoba(oponente, estado, *it) * 0.5;
    }
  */

  // Si puedo volver a tirar, lo hago (siempre que no evite ganar puntos)

  if (mi_heuristica == heuristica_oponente && estado.getCurrentPlayer() == yo)
    mi_heuristica += 50;

  resultado = (int)(mi_heuristica - heuristica_oponente + 0.5); // Aproximo

  return resultado;
}

// Algoritmo minimax alpha-beta. Devuelve el coste heurístico de una rama.
int MancoBot::alphaBeta(const GameState &estado, int profundidad, int alpha,
                        int beta, bool esNodoMax) const {
  int resultado;
  // Caso base: nodo terminal. Calculamos heurística.
  if (profundidad == PROFUNDIDAD_MAXIMA) {
    resultado = CalcularHeuristica(estado);
  } else { // No es un nodo terminal. Sigo explorando.
    int actual;
    int mejor = esNodoMax ? MIN : MAX; // Si es MAX, maximizo. Si no, minimizo.
    list<node> hijos = calcularSucesores(estado); // Calculo sus hijos.

    if (esNodoMax) { // Nodo MAX, el siguiente es min y tengo que maximizar.
      for (auto it = hijos.begin(); it != hijos.end() && !Podar(alpha, beta);
           ++it) {
        actual = alphaBeta(it->estado, profundidad + 1, alpha, beta, false);
        mejor = Maximo(actual, mejor);
        alpha = Maximo(alpha, mejor);
      }
    } else { // Nodo min. El siguiente es MAX y devuelvo beta
      for (auto it = hijos.begin(); it != hijos.end() && !Podar(alpha, beta);
           ++it) {
        actual = alphaBeta(it->estado, profundidad + 1, alpha, beta, true);
        mejor = Minimo(actual, mejor);
        beta = Minimo(actual, mejor);
      }
    }
    resultado = mejor;
  }
  return resultado;
}

// Devuelve el siguiente movimiento a realizar tras realizar una exploración.
Move MancoBot::nextMove(const vector<Move> &adversary, const GameState &state) {
#if DEBUG
  generados = 0;
  auto antes = high_resolution_clock::now();
#endif

  if (primera_vez) { // Si es la primera vez, configuro las variables internas.
    yo = state.getCurrentPlayer();
    if (yo == J1)
      oponente = J2;
    else
      oponente = J1;
    primera_vez = false;
  }

  list<node> sucesores = calcularSucesores(state);
  Move movimiento = M_NONE;

  int max = MIN;

#if DEBUG
  cerr << "Numero de sucesores: " << sucesores.size() << endl;
#endif

  // Este bucle obtiene el máximo de los hijos (es la raíz)
  for (auto it = sucesores.begin(); it != sucesores.end(); ++it) {
    it->heuristica = alphaBeta(it->estado, 0, MIN, MAX, false);
#if DEBUG
    cerr << "Heuristica " << it->heuristica << " max " << max << endl;
#endif
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

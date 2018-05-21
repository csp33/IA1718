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

  int mia = (int)(mi_heuristica + 0.5);
  int suya = (int)(heuristica_oponente + 0.5);

  if (mia == suya && estado.getCurrentPlayer() == yo)
    mia += 50;

  resultado = mia - suya;

  return resultado;
}

// Algoritmo minimax alpha-beta. Devuelve el coste heurístico de una rama.
/*
int MancoBot::alphaBeta(const GameState &estado, int profundidad, int alpha,
                        int beta, bool mi_turno) const {
  int resultado;
  // Caso base: nodo terminal. Calculamos heurística.
  if (profundidad == PROFUNDIDAD_MAXIMA) {
    resultado = CalcularHeuristica(estado);
  } else { // No es un nodo terminal. Sigo explorando.
    int actual;
    list<node> hijos = calcularSucesores(estado); // Calculo sus hijos.
    for (auto it = hijos.begin(); it != hijos.end() && !Podar(alpha, beta);++it) {
      actual = alphaBeta(it->estado, profundidad + 1, alpha, beta, !mi_turno);
      if (mi_turno) // Nodo MAX. Incremento alpha
        alpha = Maximo(alpha, actual);
      else // Nodo min. Decremento beta
        beta = Minimo(beta, actual);
    }
    resultado = mi_turno ? alpha : beta;
  }
  return resultado;
}
*/

int MancoBot::alphaBeta(const GameState &estado, int profundidad, int alpha,
                        int beta, bool mi_turno) const {
  int resultado;
  // Caso base: nodo terminal. Calculamos heurística.
  if (profundidad == PROFUNDIDAD_MAXIMA) {
    resultado = CalcularHeuristica(estado);
  } else { // No es un nodo terminal. Sigo explorando.
    int actual;
    list<node> hijos = calcularSucesores(estado); // Calculo sus hijos.
    if (mi_turno) {                               // Tengo que maximizar alpha
      for (auto it = hijos.begin(); it != hijos.end() && !Podar(alpha, beta);
           ++it) {
        // Miro de que tipo será el hijo (min/MAX)
        bool sig_turno = it->estado.getCurrentPlayer() == yo;
        actual = alphaBeta(it->estado, profundidad + 1, alpha, beta, sig_turno);
        alpha = Maximo(alpha, actual);
      }
    } else { // Tengo que minimizar beta
      for (auto it = hijos.begin(); it != hijos.end() && !Podar(alpha, beta);
           ++it) {
        bool sig_turno = it->estado.getCurrentPlayer() == yo;
        actual = alphaBeta(it->estado, profundidad + 1, alpha, beta, sig_turno);
        beta = Minimo(beta, actual);
      }
    }
    resultado = mi_turno ? alpha : beta;
  }
  return resultado;
}

/*
Move MancoBot::obtenerMovimiento(const GameState &estado) const {
  // Calculo los hijos del estado actual
  list<node> sucesores = calcularSucesores(estado);
  Move movimiento = M_NONE;

  int max = MIN;
  int alpha = MIN;
  int beta = MAX;
  int actual;

#if DEBUG
  cerr << "Numero de sucesores: " << sucesores.size() << endl;
#endif
  // Me encuentro en un nodo MAX (es mi turno)
  for (auto it = sucesores.begin(); it != sucesores.end(); ++it) {
    actual = alphaBeta(it->estado, 0, alpha, beta, false);
#if DEBUG
    cerr << "Heuristica " << actual << " max " << max << endl;
#endif
    if (actual >= max) {
      max = actual;
      movimiento = it->movimiento;
    }
  }
  return movimiento;
}

*/


// Obtiene el movimiento más óptimo a partir de un estado.
Move MancoBot::obtenerMovimiento(const GameState &estado) const {
  // Calculo los hijos del estado actual
  list<node> sucesores = calcularSucesores(estado);
  Move movimiento = M_NONE;

  int alpha = MIN;
  int beta = MAX;
  int actual;
#if DEBUG
  cerr << "Numero de sucesores: " << sucesores.size() << endl;
#endif
  // Me encuentro en un nodo MAX (es mi turno)
  for (auto it = sucesores.begin(); it != sucesores.end(); ++it) {
    // Compruebo si también será mi turno en el hijo. En ese caso, será MAX.
    bool mi_turno = it->estado.getCurrentPlayer() == yo;
    actual = alphaBeta(it->estado, 0, alpha, beta, mi_turno);
#if DEBUG
    cerr << "Heuristica de " << it->movimiento << ":" << actual
         << " max: " << alpha << endl;
#endif
    if (actual >= alpha) {
      alpha = actual;
      movimiento = it->movimiento;
    }
  }
  return movimiento;
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

  Move movimiento = obtenerMovimiento(state);

#if DEBUG
  auto despues = high_resolution_clock::now();
  auto tiempo = duration_cast<duration<double>>(despues - antes);
  cerr << "Tiempo de cómputo: " << tiempo.count() << endl;
  cerr << " generados " << generados << endl;
#endif

  return movimiento;
}

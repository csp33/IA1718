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

#define DEBUG 0

#define TEST_GENERADOS 0
#if TEST_GENERADOS
int max_generados = numeric_limits<int>::min();
int generados = 0;
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
  // Position posicion = (Position)mov;
  Position posicion;
  switch (mov) {
  case M1:
    posicion = P1;
    break;
  case M2:
    posicion = P2;
    break;
  case M3:
    posicion = P3;
    break;
  case M4:
    posicion = P4;
    break;
  case M5:
    posicion = P5;
    break;
  case M6:
    posicion = P6;
    break;
  default:
    cerr << "LA HEMOS LIADO" << endl;
    break;
  }
  Player jugador = estado.getCurrentPlayer();
  return estado.getSeedsAt(jugador, posicion) == 0;
}

// Calcula los sucesores válidos de un nodo (no causan inmolación)
list<node> MancoBot::calcularSucesores(const GameState &estado) const {
  list<node> resultado;
  bool todos_inmolan = false;
  for (auto it = MOVIMIENTOS.begin(); it != MOVIMIENTOS.end(); ++it) {
    if (!Inmolacion(estado, *it)) { // Si el movimiento no causa inmolación
      todos_inmolan = true;
      node nuevo;
      nuevo.estado = estado.simulateMove(*it); // Lo almaceno
      nuevo.movimiento = *it;
      resultado.push_back(nuevo);
#if TEST_GENERADOS
      generados++;
#endif
    }
  }
  if (!todos_inmolan) {
    cerr << "INMOLA. Jugador actual " << estado.getCurrentPlayer() << endl;
    // Mías
    cerr << "YO:" << endl;
    for (auto it = POSICIONES.begin(); it != POSICIONES.end(); ++it)
      cerr << "Casilla " << *it << ": " << (int)estado.getSeedsAt(yo, *it)
           << endl;

    cerr << "OPONENTE:" << endl;
    for (auto it = POSICIONES.begin(); it != POSICIONES.end(); ++it)
      cerr << "Casilla " << *it << ": " << (int)estado.getSeedsAt(oponente, *it)
           << endl;
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

  // Sumo las puntuaciones de los graneros (constante alta, están
  // garantizadas).
  mi_heuristica += estado.getScore(yo) * 4.5;
  heuristica_oponente += estado.getScore(oponente) * 4.5;

  // Sumo las semillas que hay en el tablero (constante menor, no
  // garantizadas)
  mi_heuristica += ObtenerSemillas(yo, estado) * 0.3;
  heuristica_oponente += ObtenerSemillas(oponente, estado) * 0.3;

  // Sumo los posibles robos
  //  for (auto it = MOVIMIENTOS.begin(); it != MOVIMIENTOS.end(); ++it) {
  //    mi_heuristica += CuantasRoba(yo, estado, *it) * 0.5;
  //    heuristica_oponente += CuantasRoba(oponente, estado, *it) * 0.5;
  //  }

  // Si puedo volver a tirar, lo hago (siempre que no evite ganar puntos)

  int mia = (int)mi_heuristica;
  int suya = (int)heuristica_oponente;

  if (mia == suya && estado.getCurrentPlayer() == yo)
    mia += 50;

#if DEBUG
  cerr << "Jugador actual "
       << (estado.getCurrentPlayer() == yo ? "yo " : "oponente") << endl;
  cerr << " Mi heurística: " << mia << ". Su heurística: " << suya << endl;
#endif
  resultado = mia - suya;

  return resultado;
}

int MancoBot::alphaBeta(const GameState &estado, int profundidad, int alpha,
                        int beta, bool mi_turno) const {
  int resultado;
  // Caso base: nodo terminal. Calculamos heurística.
  if (profundidad == PROFUNDIDAD_MAXIMA || estado.isFinalState()) {
    resultado = CalcularHeuristica(estado);
  } else { // No es un nodo terminal. Sigo explorando.
    int actual;
    list<node> hijos = calcularSucesores(estado); // Calculo sus hijos.
#if DEBUG
    cerr << "Sucesores (rec): " << hijos.size() << endl;
#endif
    for (auto it = hijos.begin(); it != hijos.end() && !Podar(alpha, beta);
         ++it) {
      // Evalúo quién jugará el siguiente turno.
      bool sig_turno = it->estado.getCurrentPlayer() == yo;
      actual = alphaBeta(it->estado, profundidad + 1, alpha, beta, sig_turno);
      if (mi_turno) // Nodo MAX. Incremento alpha
        alpha = Maximo(alpha, actual);
      else // Nodo min. Decremento beta
        beta = Minimo(beta, actual);
    }
    resultado = mi_turno ? alpha : beta;
  }
  return resultado;
}

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
    actual = alphaBeta(it->estado, 1, alpha, beta, mi_turno);
#if DEBUG
    cerr << "Heuristica de " << it->movimiento << ":" << actual
         << " alpha: " << alpha << endl;
#endif
    if (actual > alpha) {
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

  if (primera_vez) { // Si es la primera vez, configuro las variables
                     // internas.
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
#endif
#if TEST_GENERADOS
  if (generados > max_generados)
    max_generados = generados;
  cerr << "Maximos generados: " << max_generados << endl;
#endif

  return movimiento;
}

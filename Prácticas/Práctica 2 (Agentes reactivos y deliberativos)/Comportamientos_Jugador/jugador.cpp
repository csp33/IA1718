#include "../Comportamientos_Jugador/jugador.hpp"
#include "motorlib/util.h"


#include <iostream>
#include <cmath>
#include<queue>

void ComportamientoJugador::PintaPlan(list<Action> plan) {
	auto it = plan.begin();
	while (it != plan.end()) {
		if (*it == actFORWARD) {
			cout << "A ";
		}
		else if (*it == actTURN_R) {
			cout << "D ";
		}
		else if (*it == actTURN_L) {
			cout << "I ";
		}
		else {
			cout << "- ";
		}
		it++;
	}
	cout << endl;
}
/*
void AniadirHijos(estado &estado, queue<estadoConAntecesores> &cerrados) {
	for (Action accion : posiblesAcciones) {
		estadoConAntecesores aux;
		aux.status = actual.status;
		aux.antecesores.push_back(accion);
		cerrados.push_back(aux);
	}
}

bool Contiene(const estadoConAntecesores &estado, const queue<estadoConAntecesores> &cola) {
	int fila = estado.fila;
	int col = estado.col;
	for (estadoConAntecesores i : cola)
		if (i.status.fila == fila && i.status.col == col)
			return true;
	return false;
}

estadoConAntecesores ComportamientoJugador::calcularEstado(const estadoConAntecesores &actual, const Action &accion) {
	estadoConAntecesores resultado;
	//Actualizo el estado.
	switch (accion) {
	case actTURN_R: resultado.status.orientacion = (resultado.status.orientacion + 1) % 4; break;
	case actTURN_L: resultado.status.orientacion = (resultado.status.orientacion + 3) % 4; break;
	case actFORWARD:
		switch (resultado.status.orientacion) {
		case 0: fil--; break;
		case 1: col++; break;
		case 2: fil++; break;
		case 3: col--; break;
		}
	}
	resultado.antecerores.push_back(accion);
	return resultado;
}

bool ComportamientoJugador::puedoPasar(const estado &estado){
	return estado.fila >= 0 && estado.fila<=99 &
}

list<Action> ComportamientoJugador::busquedaEnProfundidad(const estado &origen, const estado &destino) {
	queue<estadoConAntecesores> abiertos;	//Cola de abiertos
	queue<estadoConAntecesores> cerrados;	//Cola de cerrados (ya visitados)
	bool encontrado = false;	//Variable que indica si hemos encontrado el camino
	estadoConAntecesores primero;
	primero.status = origen;
	abiertos.push(primero);	//Añado el primero a la lista de abiertos
	estadoConAntecesores actual;
	while (!encontrado) {
		actual = abiertos.front();		//Saco el primer elemento de abiertos
		abiertos.pop();
		if (actual.status.fil == destino.fil && actual.status.col == destino.col)
			encontrado = true;
		else {
			cerrados.push(actual);		//Lo coloco en cerrados
			if (puedoPasar() && actual.antecesores.front() == actFORWARD) {
				//	AniadirHijos(actual, abiertos);	//Meto hijos al final de abiertos
				estadoConAntecesores hijo_avanzar, hijo_gira_l, hijo_gira_r;
				hijo_avanzar = calcularEstado(actual, actFORWARD);
				hijo_gira_r = calcularEstado(actual, actTURN_R);
				hijo_gira_l = calcularEstado(actual, actTURN_L);
				abiertos.push(hijo_avanzar);
				abiertos.push(hijo_gira_l);
				abiertos.push(hijo_gira_r);
			}
			cerrados.push(actual);
		}
	}
	return actual.antecesores;
}
*/
bool ComportamientoJugador::pathFinding(const estado & origen, const estado & destino, list<Action> &plan) {
	bool visitado[99][99];	//True si he visitado
	queue<estado2> q;
	queue<estado> cola_estados;
	estado2 inicio;
	inicio.status = origen;
	inicio.distancia = 0;
	q.push(inicio);
	cola_estados.push(inicio.status);
	for (int i = 0; i < 99; i++)
		for (int j = 0; j < 99; j++)
			visitado[i][j] = false;
	int dx[4] = {0, 0, 1, -1};
	int dy[4] = {1, -1, 0, 0};
	while (!q.empty()) {
		estado2 actual = q.front();
		q.pop();
		if (actual.status.fila == destino.fila && actual.status.columna == destino.columna) {
			cola_estados.push(actual.status);
			cout << "(" << actual.status.fila << "," << actual.status.columna << ") ->";
			return actual.distancia;
		}
		visitado[actual.status.fila][actual.status.columna] = true;
		for (int i = 0; i < 4; i++) {
			int nx = dx[i] + actual.status.fila;
			int ny = dy[i] + actual.status.columna;
			if (nx >= 0 && nx <= 99 && ny >= 0 && ny <= 99 && (mapaResultado[nx][ny] == 'S' || mapaResultado[nx][ny] == 'T' || mapaResultado[nx][ny] == 'K')) {
				estado2 adyacente;
				adyacente.status.columna = ny;
				adyacente.status.fila = nx;
				adyacente.distancia = actual.distancia + 1;
				q.push(adyacente);
				visitado[nx][ny] = true;
			}
		}
	}
	cout << "estados:" << endl;
	for (int i = 0; i < cola_estados.size(); i++) {
		cout << "(" << cola_estados.front().fila << "," << cola_estados.front().columna << ") ->";
		cola_estados.pop();
	}
	cout << endl;

	return true;
}

Action ComportamientoJugador::think(Sensores sensores) {
	if (sensores.mensajeF != -1) {
		fil = sensores.mensajeF;
		col = sensores.mensajeC;
	}

	// Actualizar el efecto de la ultima accion
	switch (ultimaAccion) {
	case actTURN_R: brujula = (brujula + 1) % 4; break;
	case actTURN_L: brujula = (brujula + 3) % 4; break;
	case actFORWARD:
		switch (brujula) {
		case 0: fil--; break;
		case 1: col++; break;
		case 2: fil++; break;
		case 3: col--; break;
		}
		cout << "fil: " << fil << "  col: " << col << " Or: " << brujula << endl;
	}



	// Determinar si ha cambiado el destino desde la ultima planificacion
	if (hayPlan and (sensores.destinoF != destino.fila or sensores.destinoC != destino.columna)) {
		cout << "El destino ha cambiado\n";
		hayPlan = false;
	}

	// Determinar si tengo que construir un plan
	if (!hayPlan) {
		estado origen;
		origen.fila = fil;
		origen.columna = col;
		origen.orientacion = brujula;

		destino.fila = sensores.destinoF;
		destino.columna = sensores.destinoC;

		hayPlan = pathFinding(origen, destino, plan);
	}


	// Ejecutar el plan
	Action sigAccion;
	if (hayPlan and plan.size() > 0) {
		sigAccion = plan.front();
		plan.erase(plan.begin());
	}
	else {
		sigAccion = actIDLE;
	}

	ultimaAccion = sigAccion;
	return sigAccion;
}


void AnularMatriz(vector<vector<unsigned char> > &m) {
	for (int i = 0; i < m[0].size(); i++) {
		for (int j = 0; j < m.size(); j++) {
			m[i][j] = 0;
		}
	}
}

void ComportamientoJugador::VisualizaPlan(const estado & st, const list<Action> &plan) {
	AnularMatriz(mapaConPlan);
	estado cst = st;

	auto it = plan.begin();
	while (it != plan.end()) {
		if (*it == actFORWARD) {
			switch (cst.orientacion) {
			case 0: cst.fila--; break;
			case 1: cst.columna++; break;
			case 2: cst.fila++; break;
			case 3: cst.columna--; break;
			}
			mapaConPlan[cst.fila][cst.columna] = 1;
		}
		else if (*it == actTURN_R) {
			cst.orientacion = (cst.orientacion + 1) % 4;
		}
		else {
			cst.orientacion = (cst.orientacion + 3) % 4;
		}
		it++;
	}
}

int ComportamientoJugador::interact(Action accion, int valor) {
	return false;
}

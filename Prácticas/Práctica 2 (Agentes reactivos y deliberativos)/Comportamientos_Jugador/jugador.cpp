#include "../Comportamientos_Jugador/jugador.hpp"
#include "motorlib/util.h"

#include <iostream>
#include <cmath>

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

void AnularMatriz(vector<vector<unsigned char> > &m) {
	for (int i = 0; i < m[0].size(); i++) {
		for (int j = 0; j < m.size(); j++) {
			m[i][j] = 0;
		}
	}
}

void ComportamientoJugador::VisualizaPlan(const estado &st, const list<Action> &plan) {
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


bool ComportamientoJugador::pathFinding(const estado &origen, const estado &destino, list<Action> &plan) {
	//Algoritmo: buscar el camino correcto
	bool posible = false;
	estado temp = origen;
	while (temp.fil > destino.fil) {	//Tengo que ir al norte
		switch (brujula) {
		case 1: plan.push_back(actTURN_L); break;			//Estoy orientado al este
		case 2: plan.push_back(actTURN_L); plan.push_back(actTURN_L); break;	//Estoy orientado al sur
		case 3: plan.push_back(actTURN_R); break;			//Estoy orientado al oeste
		}
		//En este momento ya estoy bien orientado. Ahora debo avanzar
		plan.push_back(actFORWARD);
		temp.fil--;
	}
	while (temp.fil < destino.fil) {	//Tengo que ir al sur
		switch (brujula) {
		case 0: plan.push_back(actTURN_L); plan.push_back(actTURN_L); break;	//Estoy orientado al sur
		case 1: plan.push_back(actTURN_R); break;			//Estoy orientado al este
		case 3: plan.push_back(actTURN_L); break;			//Estoy orientado al oeste
		}
		//En este momento ya estoy bien orientado. Ahora debo avanzar
		plan.push_back(actFORWARD);
		temp.fil--;
	}
	//Estoy en la fila correcta. Ahora necesito situarme en la columna
	return posible;
}

bool ComportamientoJugador::puedoAvanzar(const Sensores & sensores) {
	char contenido = obtenerContenidoCasilla();
	return PUEDO_PASAR.count(contenido) &&
	       sensores.superficie[ENFRENTE] != ALDEANO;	//No tengo aldeano delante y el terreno es óptimo.
}

void ComportamientoJugador::actualizarEstado() {
	switch (ultimaAccion) {
	case actTURN_R : brujula = (brujula + 1) % 4; break;
	case actTURN_L : brujula = (brujula + 3) % 4; break;
	case actFORWARD:
		switch (brujula) {
		case 0: fil--; break;
		case 1: col++; break;
		case 2: fil++; break;
		case 3: col--; break;
		}
	}
}
char ComportamientoJugador::obtenerContenidoCasilla() {
	char contenido;
	switch (brujula) {
	case 0 : contenido = mapaResultado[fil - 1][col]; break;
	case 1 : contenido = mapaResultado[fil][col + 1]; break;
	case 2 : contenido = mapaResultado[fil + 1][col]; break;
	case 3 : contenido = mapaResultado[fil][col - 1]; break;
	}
	return contenido;
}
Action ComportamientoJugador::think(Sensores sensores) {
	Action accion;
	//Captamos la fila y la columna
	if (sensores.mensajeF != -1  || estoyBienSituado) {
		fil = sensores.mensajeF;
		col = sensores.mensajeC;
	}
	//Actualizamos el efecto de la última acción
	actualizarEstado();
	/*****Test*****/
	cout << "Fila: " << fil << " columna: " << col << " orientación: " << brujula << endl;
	/**************/

//Movimiento
	if (puedoAvanzar(sensores)) {		//Puedo avanzar
		accion = actFORWARD;
	}
	else
		accion = actTURN_R;

	ultimaAccion = accion;	//Recordamos última acción
	return accion;
}

int ComportamientoJugador::interact(Action accion, int valor) {
	return false;
}

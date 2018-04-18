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

ostream &operator<<(ostream &flujo, const list<estado> &lista) {
	for (list<estado>::const_iterator it = lista.begin(); it != lista.end(); ++it) {
		flujo << "(" << it->fila << "," << it->columna << ")\t";
	}
	return flujo;
}


list<estado> ComportamientoJugador::BFS(const estado &origen, const estado &destino) {
	bool visitado[99][99];	//Matriz de visitados.
	queue<estado> q;		// Cola de estados.
	q.push(origen);			//Introducimos el origen
	for (int i = 0; i < 99; i++)		//Inicializamos los visitados a false
		for (int j = 0; j < 99; j++)
			visitado[i][j] = false;
	int dx[4] = {0, 0, 1 , -1};			//Para calcular la adyacencia
	int dy[4] = {1, -1, 0, 0};
	while (!q.empty()) {
		estado actual = q.front();			//Sacamos un estado de la cola
		q.pop();
		if (actual.fila == destino.fila && actual.columna == destino.columna)	//Si hemos llegado, devolvemos los predecesores
			return actual.anteriores;
		visitado[actual.fila][actual.columna] = true;		//Configuramos como visitado
		for (int i = 0; i < 4; i++) {
			int nx = dx[i] + actual.fila;
			int ny = dy[i] + actual.columna;
			if (nx >= 0 && nx < 99 && ny >= 0 && ny < 99 && !visitado[nx][ny]		//Si puedo pasar por el adyacente, lo añado a la cola
			        && (mapaResultado[nx][ny] == 'S' || mapaResultado[nx][ny] == 'T' ||  mapaResultado[nx][ny] == 'K')) {
				estado adyacente;
				adyacente.fila = nx;
				adyacente.columna = ny;
				adyacente.d = actual.d + 1;
				cout << adyacente.anteriores;
				adyacente.anteriores.push_back(actual);
				q.push(adyacente);
			}
		}
	}
	list<estado> vacia;
	return vacia;
}
bool ComportamientoJugador::pathFinding(const estado &origen, const estado &destino, list<Action> &plan) {
	list<estado> lista = BFS(origen, destino);
	int i = 0;
	cout << lista;
	cout << endl;

	return true;

}
/*
bool ComportamientoJugador::pathFinding(const estado &origen, const estado &destino, list<Action> &plan) {
	//Borro la lista
	plan.clear();

	estado st = origen;

	int difF = origen.fila - destino.fila;
	int difC = origen.columna - destino.columna;

	// Reduzco la distancia en columnas
	if (difC < 0){
		if (st.orientacion == 0){
			plan.push_back(actTURN_R);
			st.orientacion = 1;
		}
		else if (st.orientacion == 3){
					plan.push_back(actTURN_R);
					plan.push_back(actTURN_R);
					st.orientacion = 1;
		}
		else if (st.orientacion == 2){
					plan.push_back(actTURN_L);
					st.orientacion = 1;
		}
	}
	else if (difC > 0){
		if (st.orientacion == 0){
			plan.push_back(actTURN_L);
			st.orientacion = 3;
		}
		else if (st.orientacion == 1){
					plan.push_back(actTURN_R);
					plan.push_back(actTURN_R);
					st.orientacion = 3;
		}
		else if (st.orientacion == 2){
					plan.push_back(actTURN_R);
					st.orientacion = 3;
		}
	}

	// Avanzo la diferencia entre columnas
	if (difC<0)
	  difC = -difC;

  for (int i=0; i < difC; i++){
		plan.push_back(actFORWARD);
	}

	// Reduzco la distancia en filas
	if (difF < 0){
		if (st.orientacion == 1){
			plan.push_back(actTURN_R);
			st.orientacion = 2;
		}
		else if (st.orientacion == 3){
					plan.push_back(actTURN_L);
					st.orientacion = 2;
		}
	}
	else if (difF > 0){
		if (st.orientacion == 1){
			plan.push_back(actTURN_L);
			st.orientacion = 0;
		}
		else if (st.orientacion == 3){
					plan.push_back(actTURN_R);
					st.orientacion = 0;
		}
	}


	// Avanzo la diferencia entre columnas
	if (difF<0)
	  difF = -difF;

  for (int i=0; i < difF; i++){
		plan.push_back(actFORWARD);
	}

	// Descomentar para ver el plan en el mapa
	VisualizaPlan(origen, plan);

	return true;
}
*/
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

int ComportamientoJugador::interact(Action accion, int valor) {
	return false;
}

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

<<<<<<< HEAD
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
=======

bool Contiene(const estadoConAntecesores &estado, queue<estadoConAntecesores> cola) {
	int fila = estado.status.fila;
	int col = estado.status.columna;
	bool encontrado=false;
	estadoConAntecesores temp;
	for(int i=0;i<cola.size() && !encontrado;i++){
		temp=cola.front();
		cola.pop();
		if(temp.status.fila==fila && temp.status.columna==col)
			encontrado=true;
	}
	return encontrado;
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
	resultado.antecesores=actual.antecesores;
	resultado.antecesores.push_back(accion);
	return resultado;
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
		if (actual.status.fila == destino.fila && actual.status.columna == destino.columna)	//Si es el que buscaba
			encontrado = true;
		else {
			cerrados.push(actual);		//Lo coloco en cerrados
			if (PUEDO_PASAR.count(mapaResultado[actual.status.fila-1][actual.status.columna]) && actual.antecesores.front() == actFORWARD) {
				estadoConAntecesores hijo_avanzar, hijo_gira_l, hijo_gira_r;		//Añado sus hijos al final de la cola de abiertos.
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
>>>>>>> ea9780c0d8f8668f36ea0b33286dbf06f77b465a
}
bool ComportamientoJugador::pathFinding(const estado &origen, const estado &destino, list<Action> &plan) {
	list<estado> lista = BFS(origen, destino);
	int i = 0;
	cout << lista;
	cout << endl;

<<<<<<< HEAD
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

=======
bool ComportamientoJugador::pathFinding(const estado & origen, const estado & destino, list<Action> &plan) {
	plan=busquedaEnProfundidad(origen,destino);


}
/*
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

>>>>>>> ea9780c0d8f8668f36ea0b33286dbf06f77b465a
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
<<<<<<< HEAD
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

=======
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

>>>>>>> ea9780c0d8f8668f36ea0b33286dbf06f77b465a
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

<<<<<<< HEAD
void ComportamientoJugador::VisualizaPlan(const estado &st, const list<Action> &plan) {
=======
void ComportamientoJugador::VisualizaPlan(const estado & st, const list<Action> &plan) {
>>>>>>> ea9780c0d8f8668f36ea0b33286dbf06f77b465a
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

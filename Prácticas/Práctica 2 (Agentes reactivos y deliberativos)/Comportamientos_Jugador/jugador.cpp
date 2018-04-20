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
		flujo << " -> (" << it->fila << "," << it->columna << ")";
	}
	return flujo;
}

/**Funciona (a saber por qué)
list<estado> ComportamientoJugador::BFS(const estado & origen, const estado & destino) {
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
		if (actual.fila == destino.fila && actual.columna == destino.columna) {	//Si hemos llegado, devolvemos los pasos que hemos seguido
			actual.anteriores.push_back(actual);
			return actual.anteriores;
		}
		visitado[actual.fila][actual.columna] = true;		//Configuramos como visitado
		for (int i = 0; i < 4; i++) {
			int nx = dx[i] + actual.fila;
			int ny = dy[i] + actual.columna;
			if (!visitado[nx][ny]	&& PUEDO_PASAR.count(mapaResultado[nx][ny])){	//Si puedo pasar por el adyacente, lo añado a la cola
				estado adyacente;
				adyacente.fila = nx;
				adyacente.columna = ny;
				//ESTABLECER ORIENTACIÓN DE ADYACENTE
				switch (i) {
				case 0: adyacente.orientacion = actual.orientacion; break;	//Adyacente de enfrente. Misma orientación
				case 1:	adyacente.orientacion = actual.orientacion + 3 % 4; break;	//Adyacente de la izquierda.
				case 2:	adyacente.orientacion = actual.orientacion + 2 % 4; break;	//Adyacente de abajo
				case 3:	adyacente.orientacion = actual.orientacion + 1 % 4; break;	//Adyacente de la derecha
				}
				adyacente.d = actual.d + 1;
				for (list<estado>::const_iterator it = actual.anteriores.begin(); it != actual.anteriores.end(); ++it)
					adyacente.anteriores.push_back(*it);
				adyacente.anteriores.push_back(actual);
				q.push(adyacente);
			}
		}
	}
	list<estado> vacia;
	return vacia;
}

list<Action> ComportamientoJugador::calcularListaAcciones(const list<estado> &lista) {
	list<Action> resultado;
	list<estado>::const_iterator it_anterior = lista.begin();
	list<estado>::const_iterator it_siguiente = it_anterior++;
	for (it_anterior = lista.begin(); it_siguiente != lista.end() ; ++it_anterior,++it_siguiente) {
		//Tres casos: avanzo, giro izquierda o giro derecha
		if (it_anterior->fila > it_siguiente->fila)	{	//Me he movido a la derecha
			switch (it_anterior->orientacion) {
			case 0: 		//Estoy mirando al norte
				resultado.push_back(actTURN_L);
				break;
			case 1:			//Este
				break;
			case 2:			//Sur
				resultado.push_back(actTURN_R);
				break;
			case 3:			//Oeste
				resultado.push_back(actTURN_R);
				resultado.push_back(actTURN_R);
				break;
			}
		}
		else if (it_anterior->fila < it_siguiente->fila)	{	//Me he movido a la izquierda
			switch (it_anterior->orientacion) {
			case 0: 		//Estoy mirando al norte
				resultado.push_back(actTURN_R);
				break;
			case 1:			//Este
				resultado.push_back(actTURN_R);
				resultado.push_back(actTURN_R);
				break;
			case 2:			//Sur
				resultado.push_back(actTURN_L);
				break;
			case 3:			//Oeste
				break;
			}
		}
		else if (it_anterior->columna > it_siguiente->columna) {	//Me he movido hacia abajo
			switch (it_anterior->orientacion) {
			case 0: 		//Estoy mirando al norte
				resultado.push_back(actTURN_R);
				resultado.push_back(actTURN_R);
				break;
			case 1:			//Este
				resultado.push_back(actTURN_L);
				break;
			case 2:			//Sur
				break;
			case 3:			//Oeste
				resultado.push_back(actTURN_R);
				break;
			}
		}
		else {		//He ido hacia arriba
			switch (it_anterior->orientacion) {
			case 0: 		//Estoy mirando al norte
				break;
			case 1:			//Este
				resultado.push_back(actTURN_R);
				break;
			case 2:			//Sur
				resultado.push_back(actTURN_L);
				resultado.push_back(actTURN_L);
				break;
			case 3:			//Oeste
				resultado.push_back(actTURN_L);
				break;
			}
		}
		resultado.push_back(actFORWARD);
	}
	return resultado;
}
**/

list<estado> ComportamientoJugador::BFS(const estado & origen, const estado & destino) {
	bool visitado[99][99];	//Matriz de visitados.
	queue<estado> q;		// Cola de estados.
	q.push(origen);			//Introducimos el origen
	for (int i = 0; i < 99; i++)		//Inicializamos los visitados a false
		for (int j = 0; j < 99; j++)
			visitado[i][j] = false;
	int dx[4] = { -1, 0, 1 , 0};			//Para calcular la adyacencia
	int dy[4] = {0, 1, 0, -1};
	while (!q.empty()) {
		estado actual = q.front();			//Sacamos un estado de la cola
		q.pop();
		if (actual.fila == destino.fila && actual.columna == destino.columna) {	//Si hemos llegado, devolvemos los pasos que hemos seguido
			actual.anteriores.push_back(actual);
			return actual.anteriores;
		}
		visitado[actual.fila][actual.columna] = true;		//Configuramos como visitado
		for (int i = 0; i < 4; i++) {
			int nx = dx[i] + actual.fila;
			int ny = dy[i] + actual.columna;
			if (!visitado[nx][ny] && PUEDO_PASAR.count(mapaResultado[nx][ny])) {	//Si puedo pasar por el adyacente, lo añado a la cola
				estado adyacente;
				adyacente.fila = nx;
				adyacente.columna = ny;
				adyacente.orientacion = i;
				adyacente.d = actual.d + 1;
				for (list<estado>::const_iterator it = actual.anteriores.begin(); it != actual.anteriores.end(); ++it)
					adyacente.anteriores.push_back(*it);
				adyacente.anteriores.push_back(actual);
				q.push(adyacente);
			}
		}
	}
	list<estado> vacia;
	return vacia;
}

list<Action> ComportamientoJugador::calcularListaAcciones(const list<estado> &lista) {
	list<Action> resultado;
	list<estado>::const_iterator it_anterior ;
	list<estado>::const_iterator it_siguiente;
	for (it_siguiente = it_anterior = lista.begin(); it_siguiente != lista.end() ; ++it_anterior) {
		++it_siguiente;
		//Cuatro  casos: avanzo, giro izquierda , giro derecha o retrocedo
		//cerr << "----------------------------------------------" << endl;
		//cerr << "Anterior: (" << it_anterior->fila << "," << it_anterior->columna << "). O=" << it_anterior->orientacion << endl;
		//cerr << "siguiente: (" << it_siguiente->fila << "," << it_siguiente->columna << "). O=" << it_siguiente->orientacion << endl;

		if (it_anterior->fila > it_siguiente->fila)	{	//Me he movido ARRIBA
			switch (it_anterior->orientacion) {
			case 0: 		//Estoy mirando al norte
				//cerr << "He ido hacia arriba y miro al norte." << endl;
				break;
			case 1:			//Este
				//cerr << "He ido hacia arriba y miro al este. Giro izq" << endl;
				resultado.push_back(actTURN_L);
				break;
			case 2:			//Sur
				//cerr << "He ido hacia arriba y miro al sur. Media vuelta" << endl;
				resultado.push_back(actTURN_R);
				resultado.push_back(actTURN_R);
				break;
			case 3:			//Oeste
				//cerr << "He ido hacia arriba y miro al oeste. Giro der" << endl;
				resultado.push_back(actTURN_R);
				break;
			}
		}
		else if (it_anterior->fila < it_siguiente->fila)	{	//Me he movido ABAJO
			switch (it_anterior->orientacion) {
			case 0: 		//Estoy mirando al norte
				//cerr << "He ido hacia abajo y miro al norte. Media vuelta" << endl;
				resultado.push_back(actTURN_R);
				resultado.push_back(actTURN_R);
				break;
			case 1:			//Este
				//cerr << "He ido hacia abajo y miro al este. Giro der" << endl;
				resultado.push_back(actTURN_R);
				break;
			case 2:			//Sur
				//cerr << "He ido hacia abajo y miro al sur." << endl;
				break;
			case 3:			//Oeste
				//cerr << "He ido hacia abajo y miro al oeste. Giro izq" << endl;
				resultado.push_back(actTURN_L);
				break;
			}
		}
		else if (it_anterior->columna > it_siguiente->columna) {	//Me he movido IZQUIERDA
			switch (it_anterior->orientacion) {
			case 0: 		//Estoy mirando al norte
				//cerr << "He ido hacia izquierda y miro al norte. Giro izq" << endl;
				resultado.push_back(actTURN_L);
				break;
			case 1:			//Este
				//cerr << "He ido hacia izquierda y miro al este. Media vuelta" << endl;
				resultado.push_back(actTURN_R);
				resultado.push_back(actTURN_R);
				break;
			case 2:			//Sur
				//cerr << "He ido hacia izquierda y miro al sur. Giro der" << endl;
				resultado.push_back(actTURN_R);
				break;
			case 3:			//Oeste
				//cerr << "He ido hacia izquierda y miro al oeste." << endl;
				break;
			}
		}
		else {		//He ido DERECHA
			switch (it_anterior->orientacion) {
			case 0: 		//Estoy mirando al norte
				//cerr << "He ido hacia derecha y miro al norte. Giro der" << endl;
				resultado.push_back(actTURN_R);
				break;
			case 1:			//Este
				//cerr << "He ido hacia derecha y miro al este." << endl;
				break;
			case 2:			//Sur
				//cerr << "He ido hacia derecha y miro al sur. Giro izq" << endl;
				resultado.push_back(actTURN_L);
				break;
			case 3:			//Oeste
				//cerr << "He ido hacia derecha y miro al oeste. Media vuelta" << endl;
				resultado.push_back(actTURN_R);
				resultado.push_back(actTURN_R);
				break;
			}
		}
		resultado.push_back(actFORWARD);
		//cerr << "----------------------------------------------" << endl;
	}
	return resultado;
}

bool ComportamientoJugador::pathFinding(const estado & origen, const estado & destino, list<Action> &plan) {
	plan.clear();
	list<estado> lista = BFS(origen, destino);
	//cerr << "Posición origen: (" << origen.fila << "," << origen.columna << "). O=" << origen.orientacion << endl;
	//cerr << "Posición destino: (" << destino.fila << "," << destino.columna << "). O=" << destino.orientacion << endl;
	//cerr << "Los estados por los que hay que pasar son:" << endl;
	//cerr << lista << endl;
	plan = calcularListaAcciones(lista);
	//cerr << "La lista de acciones es:" << endl;
	//VisualizaPlan(origen, plan);
	return !lista.empty();	//True si no está vacía
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
	if (!hayPlan || sensores.colision) {	//No hay plan o he chocado. Recalculo.
		estado origen;
		origen.fila = fil;
		origen.columna = col;
		origen.orientacion = brujula;

		destino.fila = sensores.destinoF;
		destino.columna = sensores.destinoC;
		cout << "Calculando plan..." << endl;
		hayPlan = pathFinding(origen, destino, plan);
		PintaPlan(plan);
		cout << "Plan calculado." << endl;
		if (!hayPlan)
			cout << "El objetivo es inalcanzable." << endl;
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

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

/**********************************************/
// Para hacer pruebas
ostream &operator<<(ostream &flujo, const list<estado> &lista) {
	for (list<estado>::const_iterator it = lista.begin(); it != lista.end(); ++it) {
		flujo << " -> (" << it->fila << "," << it->columna << ")";
	}
	return flujo;
}
/**********************************************/

void ComportamientoJugador::InicializarCerrados() {
	for (int i = 0; i < TAM; i++)		//Inicializamos los visitados a false
		for (int j = 0; j < TAM; j++)
			visitado[i][j] = false;
}

estado ComportamientoJugador::CrearAdyacente(int f, int c, int o, const estado &actual) {
	estado adyacente;
	adyacente.fila = f;
	adyacente.columna = c;
	adyacente.orientacion = o;
	for (list<estado>::const_iterator it = actual.anteriores.begin(); it != actual.anteriores.end(); ++it)
		adyacente.anteriores.push_back(*it);
	adyacente.anteriores.push_back(actual);
	return adyacente;
}

bool ComportamientoJugador::Contiene(int fila, int columna, queue<estado> q) {
	bool encontrado = false;
	for (int i = 0; i < q.size() && !encontrado; i++) {
		estado actual = q.front();
		q.pop();
		if (actual.fila == fila && actual.columna == columna)
			encontrado = true;
	}
	return encontrado;
}

bool ComportamientoJugador::EsViable(int fila, int columna/*, queue<estado> q*/) {
	//No está en abiertos ni en cerrados y puedo pasar
	return /*!Contiene(fila, columna, q) &&*/ !visitado[fila][columna] && PUEDO_PASAR.count(mapaResultado[fila][columna]);
}

/**Redefino los operadores para que solo comparen fila y columna**/
bool operator==(const estado &uno, const estado &otro) {
	return uno.fila == otro.fila && uno.columna == otro.columna;
}

bool operator!=(const estado &uno, const estado &otro) {
	return !(uno == otro);
}

/**Prueba: operadores de comparación para almacenar los abiertos en un set y que la búsqueda sea logarítmica*/
bool operator<(const estado &uno,const estado &otro){
	return uno.fila<otro.fila;
}
/****************************************************************/

list<estado> ComportamientoJugador::BusquedaEnAnchura(const estado & origen, const estado & destino) {
	queue<estado> abiertos;				// Cola de abiertos.
	abiertos.push(origen);				//Introducimos el origen
	InicializarCerrados();				// Ponemos los visitados (cerrados) a false.
	int dx[4] = { -1, 0, 1 , 0};		//Para calcular la adyacencia (i=0 -> DELANTE i=1 -> DERECHA i=2 -> ATRÁS i=3 -> IZQUIERDA)
	int dy[4] = {0, 1, 0, -1};
	bool encontrado = false;			// Variable que indica si hemos encontrado el camino.
	list<estado> resultado;				//Resultado a devolver

	while (!abiertos.empty() && !encontrado) {			//Mientras queden estados por comprobar o no encontremos el destino
		estado actual = abiertos.front();			//Sacamos un estado de la cola
		abiertos.pop();
		if ( actual == destino) {	//Si hemos llegado, devolvemos los pasos que hemos seguido
			resultado = actual.anteriores;
			resultado.push_back(actual);
			encontrado = true;
		}
		else {
			visitado[actual.fila][actual.columna] = true;		//Configuramos como visitado
			for (int i = 0; i < 4; i++) {						//Adjustamos adyacentes
				int fila_ady = dx[i] + actual.fila;
				int col_ady = dy[i] + actual.columna;
				if (EsViable(fila_ady, col_ady/*,abiertos*/)) {	//Si puedo pasar por el adyacente, lo añado a la cola de abiertos
					estado adyacente = CrearAdyacente(fila_ady, col_ady, i, actual);
					abiertos.push(adyacente);
				}
			}
		}
	}
	return resultado;
}

list<Action> ComportamientoJugador::calcularListaAcciones(const list<estado> &lista) {
	list<Action> resultado;
	list<estado>::const_iterator it_anterior;
	list<estado>::const_iterator it_siguiente;
	for (it_siguiente = it_anterior = lista.begin(); it_siguiente != lista.end() ; ++it_anterior) {		//Dos iteradores: anterior y siguiente.
		++it_siguiente;
		//Cuatro  casos: avanzo, giro izquierda , giro derecha o retrocedo
		if (it_anterior->fila > it_siguiente->fila)	{	//Me he movido hacia ARRIBA
			switch (it_anterior->orientacion) {
			case 0: 		//Estoy mirando al norte
				break;
			case 1:			//Este
				resultado.push_back(actTURN_L);
				break;
			case 2:			//Sur
				resultado.push_back(actTURN_R);
				resultado.push_back(actTURN_R);
				break;
			case 3:			//Oeste
				resultado.push_back(actTURN_R);
				break;
			}
		}
		else if (it_anterior->fila < it_siguiente->fila)	{	//Me he movido hacia ABAJO
			switch (it_anterior->orientacion) {
			case 0: 		//Estoy mirando al norte
				resultado.push_back(actTURN_R);
				resultado.push_back(actTURN_R);
				break;
			case 1:			//Este
				resultado.push_back(actTURN_R);
				break;
			case 2:			//Sur
				break;
			case 3:			//Oeste
				resultado.push_back(actTURN_L);
				break;
			}
		}
		else if (it_anterior->columna > it_siguiente->columna) {	//Me he movido hacia la IZQUIERDA
			switch (it_anterior->orientacion) {
			case 0: 		//Estoy mirando al norte
				resultado.push_back(actTURN_L);
				break;
			case 1:			//Este
				resultado.push_back(actTURN_R);
				resultado.push_back(actTURN_R);
				break;
			case 2:			//Sur
				resultado.push_back(actTURN_R);
				break;
			case 3:			//Oeste
				break;
			}
		}
		else {		//Me he movido hacia la DERECHA
			switch (it_anterior->orientacion) {
			case 0: 		//Estoy mirando al norte
				resultado.push_back(actTURN_R);
				break;
			case 1:			//Este
				break;
			case 2:			//Sur
				resultado.push_back(actTURN_L);
				break;
			case 3:			//Oeste
				resultado.push_back(actTURN_R);
				resultado.push_back(actTURN_R);
				break;
			}
		}
		resultado.push_back(actFORWARD);
	}
	return resultado;
}

bool ComportamientoJugador::pathFinding(const estado & origen, const estado & destino, list<Action> &plan) {
	plan.clear();
	list<estado> lista = BusquedaEnAnchura(origen, destino);
	plan = calcularListaAcciones(lista);
	VisualizaPlan(origen, plan);
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
	if (!hayPlan || sensores.colision || plan.empty()) {	//No hay plan o he chocado. Recalculo.
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

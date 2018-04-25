#include "../Comportamientos_Jugador/jugador.hpp"
#include "motorlib/util.h"


#include <iostream>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <chrono>
using namespace std::chrono;


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

void ComportamientoJugador::InicializarMatrices() {
	for (int i = 0; i < TAM; i++) {		//Inicializamos las matrices a false
		for (int j = 0; j < TAM; j++) {
			m_cerrados[i][j] = false;
			m_abiertos[i][j] = false;
		}
	}
}
/*
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
*/
/***V2. Elimino anteriores para ahorrar memoria.***/
estado ComportamientoJugador::CrearAdyacente(int f, int c, int o, estado &actual) {
	estado adyacente;
	adyacente.fila = f;
	adyacente.columna = c;
	adyacente.orientacion = o;
	for (list<estado>::iterator it = actual.anteriores.begin(); it != actual.anteriores.end(); ++it) {
		it->anteriores.clear();		//Elimino los padres, ya que sólo necesito conservar fila, columna y orientación.
		adyacente.anteriores.push_back(*it);
	}
	adyacente.anteriores.push_back(actual);
	return adyacente;
}

/*************************************/
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

bool ComportamientoJugador::EsViable(int fila, int columna) {
	//No está en abiertos ni en cerrados y puedo pasar.
	return !m_abiertos[fila][columna] && !m_cerrados[fila][columna] && PUEDO_PASAR.count(mapaResultado[fila][columna]);
}

/**Redefino los operadores de comparación para que solo comparen fila y columna**/
bool operator==(const estado &uno, const estado &otro) {
	return uno.fila == otro.fila && uno.columna == otro.columna;
}

bool operator!=(const estado &uno, const estado &otro) {
	return !(uno == otro);
}
/********************************************************************/

/**
Búsqueda en anchura. Pseudocódigo:
	Introducimos el origen en la cola de abiertos (por visitar)
	Mientras que queden elementos en abiertos o no hayamos encontrado el camino:
			Saco el primer elemento de abiertos.
			Si es la solución:
				Añado el nodo actual al histórico y salgo del bucle.
			En caso contrario:
				Lo meto en cerrados (ya visitados).
				Para cada uno de sus adyacentes (delante,detrás,izquierda y derecha)
					Si es viable (no está ni en abiertos ni en cerrados y es transitable):
						Lo meto en abiertos.
	Devuelvo la solución
	----------------
	Aclaraciones:
		-> m_abiertos[][] y m_cerrados[][]. Son estructuras auxiliares para que la comprobación abiertos.contains(estado) y
			cerrados.contains(estado) sea más eficiente: en una cola para comprobar si un elemento existe habría que realizar
			una copia del objeto y explorarlo elemento a elemento (O(n)). Sin embargo, el acceso a una matriz se realiza en tiempo
			constante (O(1)), por lo que el algoritmo es mucho más eficiente.
		-> Si no encontramos el camino, devolveremos una lista vacía.
		-> En cada iteración elimino los predecesores del estado actual, ya que esta información está almacenada en los adyacentes viables,
			que habrán sido pasados a la cola de abiertos. Si no lo hago, el programa consume demasiada memoria.
*/

list<estado> ComportamientoJugador::BusquedaEnAnchura(const estado & origen, const estado & destino) {
	queue<estado> abiertos;				// Cola de abiertos.
	int generados = 0;
	abiertos.push(origen);				//Introducimos el origen
	m_abiertos[origen.fila][origen.columna] = true;
	InicializarMatrices();				// Ponemos las matrices a false.
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
			m_cerrados[actual.fila][actual.columna] = true;		//Configuramos como m_cerrados
			for (int i = 0; i < 4; i++) {						//Adjustamos adyacentes
				int fila_ady = dx[i] + actual.fila;
				int col_ady = dy[i] + actual.columna;
				if (EsViable(fila_ady, col_ady)) {	//Si puedo pasar por el adyacente, lo añado a la cola de abiertos
					estado adyacente = CrearAdyacente(fila_ady, col_ady, i, actual);
					abiertos.push(adyacente);
					m_abiertos[adyacente.fila][adyacente.columna] = true;
				}
			}
		}
		actual.anteriores.clear();		//Libero memoria, ya que esta información estará en el adyacente.
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
	high_resolution_clock::time_point tantes;
	high_resolution_clock::time_point tdespues;
	duration<double> tiempo;

	plan.clear();
	tantes = high_resolution_clock::now();
	list<estado> lista = BusquedaEnAnchura(origen, destino);
	tdespues = high_resolution_clock::now();
	tiempo = duration_cast<duration<double>>(tdespues - tantes);
	cout << "Tiempo empleado en el cálculo del plan: " << tiempo.count() << "s." << endl;

	tantes = high_resolution_clock::now();
	plan = calcularListaAcciones(lista);
	tdespues = high_resolution_clock::now();
	tiempo = duration_cast<duration<double>>(tdespues - tantes);
	cout << "Tiempo empleado en la transcripción a acciones: " << tiempo.count() << "s." << endl;

	VisualizaPlan(origen, plan);
	return !lista.empty();	//True si no está vacía
}


/**Otra versión: incorporar lista de acciones. No funciona.********* /

bool ComportamientoJugador::pathFinding(const estado & origen, const estado & destino, list<Action> &plan) {
	high_resolution_clock::time_point tantes;
	high_resolution_clock::time_point tdespues;
	duration<double> tiempo;

	plan.clear();

	queue<estado> abiertos;
	InicializarMatrices();
	abiertos.push(origen);
	m_abiertos[origen.fila][origen.columna] = true;
	while (!abiertos.empty()) {
		estado actual = abiertos.front();
		abiertos.pop();
		if (actual == destino) {
			cerr << "Solución encontrada ("<<actual.fila<<","<<actual.columna<<")." << endl;
			plan = actual.antecesores;
			return true;
		}
		else {
				m_cerrados[actual.fila][actual.columna] = true;
			int d_fila[] = { -1, 0, 0};
			int d_col[] = {0, -1, 1};
			int d_or[] = {0, 3, 1};
			Action ac[] = {actIDLE, actTURN_L, actTURN_R};
			for (int i = 0; i < 3; i++) {
				estado adyacente;
				adyacente.fila = actual.fila + d_fila[i];
				adyacente.columna = actual.columna + d_col[i];
				adyacente.orientacion = (actual.orientacion + d_or[i]) % 4;

				if (EsViable(adyacente.fila, adyacente.columna)) {
					for (auto it = actual.antecesores.begin(); it != actual.antecesores.end(); ++it)
						adyacente.antecesores.push_back(*it);
					if (i != 0)
						adyacente.antecesores.push_back(ac[i]);
					adyacente.antecesores.push_back(actFORWARD);
					abiertos.push(adyacente);
					m_abiertos[adyacente.fila][adyacente.columna] = true;
				}
			}
		}
	}
	return false;
}

/**********************************************/
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
	if (!hayPlan) {	//No hay plan o tengo un aldeano enfrente. Recalculo.
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
		if (sigAccion == actFORWARD and sensores.superficie[2] == 'a') {
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

void ComportamientoJugador::Reservar() {
	m_cerrados = new bool*[TAM];
	m_abiertos = new bool*[TAM];
	for (int i = 0; i < TAM; i++) {
		m_cerrados[i] = new bool[TAM];
		m_abiertos[i] = new bool[TAM];
	}
}

void ComportamientoJugador::Liberar() {
	for (int i = 0; i < TAM; i++) {
		delete []m_abiertos[i];
		delete []m_cerrados[i];
	}
	delete []m_abiertos;
	delete []m_cerrados;
}

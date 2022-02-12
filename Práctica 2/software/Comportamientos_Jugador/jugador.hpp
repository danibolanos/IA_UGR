#ifndef COMPORTAMIENTOJUGADOR_H
#define COMPORTAMIENTOJUGADOR_H

#include "comportamientos/comportamiento.hpp"

#include <list>

struct estado {
  int fila;
  int columna;
  int orientacion;

	bool operator ==(const estado &e){
		bool igual=false;
		if(fila==e.fila && columna==e.columna && orientacion==e.orientacion)
			igual = true;
		return igual;
	}

	estado& operator =(const estado &e){
		fila=e.fila;
		columna=e.columna; 
		orientacion=e.orientacion;
		return *this;
	}
};

struct Nodo{
	Nodo *padre;
	Action accion;
	estado status;
	int coste;
	int coste_estimado;

	Nodo(){
		padre = nullptr;
	}
	Nodo(estado e, int c, int ce){
		padre = nullptr;
		status=e;
		coste=c;
		coste_estimado=ce;
	}
	Nodo(Nodo *n,estado e, int c, int ce){
		padre=n;
		status=e;
		coste=c;
		coste_estimado=ce;
	}
	Nodo(Nodo *n,estado e, int c, int ce, Action a){
		padre=n;
		status=e;
		coste=c;
		coste_estimado=ce;
		accion = a;
	}

	bool operator ==(const Nodo &n){
		bool igual=false;
		if(this->status==n.status)
			igual = true;
		return igual;
	}

	Nodo& operator =(const Nodo &n){
		status=n.status;
		padre=n.padre;
		accion=n.accion;
		coste=n.coste;
		coste_estimado=n.coste_estimado;
		return *this;
	}

	int costeTotal(){
		return coste+coste_estimado;
	}
	
};

class ComportamientoJugador : public Comportamiento {
  public:
    ComportamientoJugador(unsigned int size) : Comportamiento(size) {
      // Inicializar Variables de Estado
      fil = col = 99;
      brujula = 0; // 0: Norte, 1:Este, 2:Sur, 3:Oeste
      destino.fila = -1;
      destino.columna = -1;
      destino.orientacion = -1;
    }
    ComportamientoJugador(std::vector< std::vector< unsigned char> > mapaR) : Comportamiento(mapaR) {
      // Inicializar Variables de Estado
      fil = col = 99;
      brujula = 0; // 0: Norte, 1:Este, 2:Sur, 3:Oeste
      destino.fila = -1;
      destino.columna = -1;
      destino.orientacion = -1;
    }
    ComportamientoJugador(const ComportamientoJugador & comport) : Comportamiento(comport){}
    ~ComportamientoJugador(){}

    Action think(Sensores sensores);
    int interact(Action accion, int valor);
    void VisualizaPlan(const estado &st, const list<Action> &plan);
    ComportamientoJugador * clone(){return new ComportamientoJugador(*this);}

	bool terrenoValido(estado e);
	int valorEstimado();
	estado consecuenciaAccion(Action act, estado est);
	bool  terrenoValido(unsigned char terr);
	bool rellenarMapa(Sensores &sensores, estado &e);
	void mostrarMapa();
	void puntoPK(Sensores &sensores);
	void pasarMapa(int f, int c);
	bool choque(Sensores &sensores);
	bool expandirNodo(Nodo n);
	void expandir(Nodo n);
	void nuevoHijo(Nodo &n, estado e, Action a);
	bool estadoDuplicado(estado e);
	int numeroNodos();
	bool estadoValido(estado e);
	bool estadoValido3(estado e);
	int valorEstimado(estado e);
	void mostrarCamino(list<Action> &plan);

  private:
    // Declarar Variables de Estado
	 int fil, col, brujula;
    estado destino;
	 estado mi_estado;
    list<Action> plan;
	 bool localizado=false;
	 int movimiento=1;
	 vector< vector< unsigned char> > mi_mapa;
	 bool nivel3 = false;
	 bool pk = false;
	 int principio_f;
	 int principio_c;
	 bool imposible = false;
	 bool ocupada[100][100][4];
	 list<Nodo> abiertos;
	 list<Nodo> cerrados;
	 Nodo *objetivo = nullptr;
	 int paciencia = 2;
	 int espera = 0;

    bool pathFinding(const estado &origen, const estado &destino, list<Action> &plan);
    void PintaPlan(list<Action> plan);
};

#endif

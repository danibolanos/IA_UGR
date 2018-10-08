#include "../Comportamientos_Jugador/jugador.hpp"
#include "motorlib/util.h"

#include <iostream>
#include <cmath>

void ComportamientoJugador::PintaPlan(list<Action> plan) {
	auto it = plan.begin();
	while (it!=plan.end()){
		if (*it == actFORWARD){
			cout << "A ";
		}
		else if (*it == actTURN_R){
			cout << "D ";
		}
		else if (*it == actTURN_L){
			cout << "I ";
		}
		else {
			cout << "- ";
		}
		it++;
	}
	cout << endl;
}

void AnularMatriz(vector<vector<unsigned char> > &m){
	for (int i=0; i<m[0].size(); i++){
		for (int j=0; j<m.size(); j++){
			m[i][j]=0;
		}
	}
}

void ComportamientoJugador::VisualizaPlan(const estado &st, const list<Action> &plan){
  AnularMatriz(mapaConPlan);
	estado cst = st;

	auto it = plan.begin();
	while (it!=plan.end()){
		if (*it == actFORWARD){
			switch (cst.orientacion) {
				case 0: cst.fila--; break;
				case 1: cst.columna++; break;
				case 2: cst.fila++; break;
				case 3: cst.columna--; break;
			}
			mapaConPlan[cst.fila][cst.columna]=1;
		}
		else if (*it == actTURN_R){
			cst.orientacion = (cst.orientacion+1)%4;
		}
		else {
			cst.orientacion = (cst.orientacion+3)%4;
		}
		it++;
	}
}

bool ComportamientoJugador::expandirNodo(Nodo n){
	Nodo aux;
	if(localizado){
		bool exp=false;
		expandir(n);
		while(abiertos.size()>0 && objetivo==nullptr){
			aux=abiertos.front();
			expandir(aux);
		}
		if(objetivo!=nullptr)
			exp=true;
		return exp;
	}
	else{
		if(!pk){
			bool exp=false;
			expandir(n);
			int tam;
			while(objetivo==nullptr && abiertos.size()>0){
				tam=abiertos.size();
				for(int i=0; i<tam; i++){
					aux=abiertos.back();
					expandir(aux);
				}
			}
			if(objetivo!=nullptr)
				exp=true;
			return exp;
		}
		else{
			bool exp=false;
			expandir(n);
			int tam;
			for(int j=0; j<8; j++){
				tam=abiertos.size();
				for(int i=0; i<tam; i++){
					aux=abiertos.back();
					expandir(aux);
				}
			}
			if(objetivo!=nullptr)
				exp=true;
			return exp;
		}
	}
}
		
void ComportamientoJugador::expandir(Nodo n){
	abiertos.remove(n);			
	cerrados.push_back(n);
	estado nuevo_estado=n.status;
	Action nueva_accion;
	nueva_accion=actFORWARD;
	nuevoHijo(cerrados.back(), consecuenciaAccion(nueva_accion,nuevo_estado),nueva_accion);
	nueva_accion=actTURN_R;
	nuevoHijo(cerrados.back(), consecuenciaAccion(nueva_accion,nuevo_estado),nueva_accion);	
	nueva_accion=actTURN_L;
	nuevoHijo(cerrados.back(), consecuenciaAccion(nueva_accion,nuevo_estado),nueva_accion);
}

void ComportamientoJugador::nuevoHijo(Nodo &n, estado e, Action a){
	if(localizado){
		bool colocado=false;
		int lugar;
		Nodo nuevo(&n,e,n.coste+1,valorEstimado(e),a);
		if(estadoValido(e))
		if(!estadoDuplicado(e)){
			list<Nodo>::iterator it2;
			for(list<Nodo>::iterator it=abiertos.begin(); it!=abiertos.end() && !colocado; it++){
				if(it->costeTotal() >= n.coste+1+valorEstimado(e)){
							it2=abiertos.insert(it,nuevo);
							colocado=true;
				}
			}
			if(!colocado){
				abiertos.push_back(nuevo);
			}
			if(valorEstimado(e) == 0){
				objetivo = &abiertos.front();
			}
				ocupada[e.fila][e.columna][e.orientacion]=true;
			}
		}
		else{
			unsigned char buscar;
			if(pk)
				buscar='K';
			else
				buscar='?';
			int lugar;
			Nodo nuevo(&n,e,n.coste+1,valorEstimado(e),a);
			if(estadoValido3(e))
			if(!estadoDuplicado(e)){
				abiertos.push_front(nuevo);
				if(mi_mapa[e.fila][e.columna]==buscar)
						objetivo = &abiertos.front();
		}
	}
}
		
bool ComportamientoJugador::estadoDuplicado(estado e){
	bool duplicado=false;
	if(localizado)
		duplicado=ocupada[e.fila][e.columna][e.orientacion];
	else{
		for(list<Nodo>::iterator it=abiertos.begin(); it!=abiertos.end() && !duplicado; it++){
			if(it->status == e)
				duplicado = true;
		}
		for(list<Nodo>::iterator it=cerrados.begin(); it!=cerrados.end() && !duplicado; it++){
			if(it->status == e)
				duplicado = true;
		}
	}
	return duplicado;
}

int ComportamientoJugador::numeroNodos(){
	int num=abiertos.size()+cerrados.size();
	return num;
}

bool ComportamientoJugador::estadoValido(estado e){
	bool valido = false;
	if(e.fila<100 && e.fila>=0 && e.columna<100 && e.columna>=0)
	if(terrenoValido(e))
			valido = true;
	return valido;
}

bool ComportamientoJugador::estadoValido3(estado e){
	bool valido = false;
	if(e.fila<200 && e.fila>=0 && e.columna<200 && e.columna>=0)
	if(terrenoValido(mi_mapa[e.fila][e.columna]))
			valido = true;
	return valido;
}

int ComportamientoJugador::valorEstimado(estado e){
	int valor=0;
	int f,c;
	f = destino.fila-e.fila;
	c = destino.columna-e.columna;
	if(f>0)
		valor+=f;
	else
		valor-=f;
	if(c>0)
		valor+=c;
	else
		valor-=c;
	if(e.orientacion==0){
		if(f==0){
			if(c!=0)
				valor++;
		}
		if(f>0){
			valor+=2;
		}
		if(f<0){
			if(c!=0)
				valor++;
		}
	}
	if(e.orientacion==1){
		if(c==0){
			if(f!=0)
				valor++;
		}
		if(c<0){
			valor+=2;
		}
		if(c>0){
			if(f!=0)
				valor++;
		}
	}
	if(e.orientacion==2){
		if(f==0){
			if(c!=0)
				valor++;
		}
		if(f<0){
			valor+=2;
		}
		if(f>0){
			if(c!=0)
				valor++;
		}
	}
	
	if(e.orientacion==3){
		if(c==0){
			if(f!=0)
				valor++;
		}
		if(c>0){
			valor+=2;
		}
		if(c<0){
			if(f!=0)
				valor++;
		}
	}
	return valor;
}

void ComportamientoJugador::mostrarCamino(list<Action> &plan){
	if(objetivo!=nullptr){
	Nodo *aux;
	aux = objetivo;
	while(aux->padre!=nullptr){
		plan.push_front(aux->accion);
		aux = aux->padre;
	}
	}
}

bool ComportamientoJugador::pathFinding(const estado &origen, const estado &destino, list<Action> &plan) {
	objetivo=nullptr;
	plan.clear();
	Nodo n(origen,0,valorEstimado(origen));
	bool exito;
	if(localizado){
		for(int i=0; i<mapaResultado.size(); i++)
			for(int j=0; j<mapaResultado[i].size(); j++)
				for(int k=0; k<4; k++)
					ocupada[i][j][k]=false;
	}
	exito=expandirNodo(n);
	if(!localizado){
		if(pk){
			if(!exito){
				pk=false;
				exito=expandirNodo(n);
			}
		}	
	}
	if(exito){
		mostrarCamino(plan);
	}
	else
		imposible=true;
		objetivo=nullptr;
		abiertos.clear();
		cerrados.clear();
  return exito;
}

Action ComportamientoJugador::think(Sensores sensores) {
	Action accion=actIDLE;

	if(terrenoValido(mapaResultado[sensores.destinoF][sensores.destinoC]) && !imposible){

		if(!nivel3){
			if(mapaResultado[0][0] == '?'){
				nivel3 = true;	
				mi_estado.fila = 100;
				mi_estado.columna = 100;
				mi_estado.orientacion=0;
				mi_mapa.resize(200);
				for(int i=0; i<200; i++)
					mi_mapa[i].resize(200,'?');
			}
		}

		if(sensores.mensajeF != -1 && sensores.mensajeC != -1){
			if(!localizado && nivel3){
				pk = false;
				pasarMapa(sensores.mensajeF, sensores.mensajeC);
				plan.clear();
			}
			localizado=true;
			mi_estado.fila = sensores.mensajeF;
			mi_estado.columna = sensores.mensajeC;
			if(!nivel3)
				mi_estado.orientacion = 0;
			destino.fila = sensores.destinoF;
			destino.columna = sensores.destinoC;
			if(plan.size()==0){	
				pathFinding(mi_estado,destino,plan);
			}
		}
	
		if(nivel3){		
			bool nueva_info;
			nueva_info=rellenarMapa(sensores,mi_estado);

			if(destino.fila != sensores.destinoF || destino.columna != sensores.destinoC){
				destino.fila = sensores.destinoF;
				destino.columna = sensores.destinoC;
			}

			if(localizado){

				if(plan.size()==0)
					pathFinding(mi_estado,destino,plan);
				if(plan.size()!=0){
					accion = plan.front();
					plan.pop_front();
				}
			}
			
			else{
				puntoPK(sensores);

				if(pk){
					plan.clear();
				}
				else if(nueva_info)
					plan.clear();
	
				if(plan.size() == 0){
					pathFinding(mi_estado,destino,plan);
				}
				if(plan.size() == 0){
					pathFinding(mi_estado,destino,plan);
				}		

				if(plan.size()!=0){
					accion = plan.front();
					plan.pop_front();
				}
				if(sensores.terreno[2]=='K'){
					plan.clear();
					accion=actFORWARD;
				}				
			}
		}

		else{
			if(plan.size()!=0){
				accion = plan.front();
				plan.pop_front();
			}
		}

		if(accion == actFORWARD){
		
			if(choque(sensores)){
				pathFinding(mi_estado,destino,plan);
				if(!imposible){
					if(plan.size()!=0){
						accion = plan.front();			
						plan.pop_front();
					}
					else
						accion=actIDLE;
				}
			}
			
		}
	}
		if(sensores.superficie[2] == 'a'){
			espera++;
			if(espera>paciencia && nivel3){
				espera=0;
				int f=mi_estado.fila, c=mi_estado.columna;
				unsigned char aux;
				espera=0;
				if(mi_estado.orientacion==0)
					f--;
				if(mi_estado.orientacion==1)
					c++;
				if(mi_estado.orientacion==2)
					f++;
				if(mi_estado.orientacion==3)
					c--;
				if(localizado){
					aux = mapaResultado[f][c];
					mapaResultado[f][c]='P';
					pathFinding(mi_estado,destino,plan);
					mapaResultado[f][c]=aux;
				}
				else{
					aux = mi_mapa[f][c];
					mi_mapa[f][c]='P';
					pathFinding(mi_estado,destino,plan);
					mi_mapa[f][c]=aux;
				}
				if(plan.size()!=0){
					accion = plan.front();							
					plan.pop_front();
				}
				else{ 
				}
			}
			else{
				if(plan.size()!=0){
					plan.push_front(accion);
					accion = actIDLE;
				}
			}
		}

		if(accion == actFORWARD)
		if(!terrenoValido(sensores.terreno[2]) || sensores.superficie[2] == 'a'){
			accion = actIDLE;
			if(plan.size()!=0){
				plan.clear();
			}
		}
	
	mi_estado = consecuenciaAccion(accion, mi_estado);
	
	return accion;
	
}

bool ComportamientoJugador::choque(Sensores &sensores){
	bool chocar=false;
	list<Action>::iterator it;
	if(!chocar && plan.size()>1 && !terrenoValido(sensores.terreno[1])){
		it=plan.begin();
			if(*it==actTURN_L){
				it++;
				if(*it==actFORWARD){
					chocar=true;
				}
			}
		
	}
	if(!chocar && !terrenoValido(sensores.terreno[2])){
			chocar=true;
	}
	if(!chocar && plan.size()>1 && !terrenoValido(sensores.terreno[3])){
		it=plan.begin();
			if(*it==actTURN_R){
				it++;
				if(*it==actFORWARD){
					chocar=true;
				}
			}
		
	}
	if(!chocar && plan.size()>3 && !terrenoValido(sensores.terreno[4])){
		it=plan.begin();
			if(*it==actFORWARD){
				it++;
				if(*it==actTURN_L){
					it++;
					if(*it==actFORWARD){
						it++;
						if(*it==actFORWARD){
							chocar=true;
						}
					}
				}
			}
		
	}	
	if(!chocar && plan.size()>3 && !terrenoValido(sensores.terreno[5])){
		it=plan.begin();
			if(*it==actFORWARD){
				it++;
				if(*it==actTURN_L){
					it++;
					if(*it==actFORWARD){
						chocar=true;
					}
				}
			}
		

	}
	if(!chocar && plan.size()>0 && !terrenoValido(sensores.terreno[6])){
		it=plan.begin();
			if(*it==actFORWARD){
				chocar=true;
			}

	}
	if(!chocar && plan.size()>2 && !terrenoValido(sensores.terreno[7])){
		it=plan.begin();
			if(*it==actFORWARD){
				it++;
				if(*it==actTURN_R){
					it++;
					if(*it==actFORWARD){
						chocar=true;
					}
				}
			}
		

	}
	if(!chocar && plan.size()>3 && !terrenoValido(sensores.terreno[8])){
		it=plan.begin();
			if(*it==actFORWARD){
				it++;
				if(*it==actTURN_R){
					it++;
					if(*it==actFORWARD){
						it++;
						if(*it==actFORWARD){
							chocar=true;
						}
					}
				}
			}
		

	}
	
	if(!chocar && plan.size()>1 && !terrenoValido(sensores.terreno[12])){
		it=plan.begin();
			if(*it==actFORWARD){
				it++;
				if(*it==actFORWARD){
					chocar=true;
				}
			}

	}

	if(!chocar && plan.size()>5 && !terrenoValido(sensores.terreno[9])){
		it=plan.begin();
			if(*it==actFORWARD){
				it++;
				if(*it==actFORWARD){
					it++;
					if(*it==actTURN_L){
						it++;
						if(*it==actFORWARD){
							it++;
							if(*it==actFORWARD){
								it++;
								if(*it==actFORWARD)
									chocar=true;
							}
						}
					}
				}
			}
		

	}
	if(!chocar && plan.size()>4&& !terrenoValido(sensores.terreno[10])){
		it=plan.begin();
			if(*it==actFORWARD){
				it++;
				if(*it==actFORWARD){
					it++;
					if(*it==actTURN_L){
						it++;
						if(*it==actFORWARD){
							it++;
							if(*it==actFORWARD){
								chocar=true;
							}
						}
					}
				}
			}

	}
	if(!chocar && plan.size()>3 && !terrenoValido(sensores.terreno[11])){
		it=plan.begin();
			if(*it==actFORWARD){
				it++;
				if(*it==actFORWARD){
					it++;
					if(*it==actTURN_L){
						it++;
						if(*it==actFORWARD){
							chocar=true;
						}
					}
				}
			}
		

	}
	if(!chocar && plan.size()>3 && !terrenoValido(sensores.terreno[13])){
		it=plan.begin();
			if(*it==actFORWARD){
				it++;
				if(*it==actFORWARD){
					it++;
					if(*it==actTURN_R){
						it++;
						if(*it==actFORWARD){
							chocar=true;
						}
					}
				}
			}

	}
	if(!chocar && plan.size()>4 && !terrenoValido(sensores.terreno[14])){
		it=plan.begin();
			if(*it==actFORWARD){
				it++;
				if(*it==actFORWARD){
					it++;
					if(*it==actTURN_R){
						it++;
						if(*it==actFORWARD){
							it++;
							if(*it==actFORWARD){
								chocar=true;
							}
						}
					}
				}
			}

	}
	if(!chocar && plan.size()>5 && !terrenoValido(sensores.terreno[15])){
		it=plan.begin();
			if(*it==actFORWARD){
				it++;
				if(*it==actFORWARD){
					it++;
					if(*it==actTURN_R){
						it++;
						if(*it==actFORWARD){
							it++;
							if(*it==actFORWARD){
								it++;
								if(*it==actFORWARD)
									chocar=true;
							}
						}
					}
				}
			}
		

	}
	
	return chocar;
}

void ComportamientoJugador::puntoPK(Sensores &sensores){
	for(int i=0; i<16; i++){
		if(sensores.terreno[i] == 'K')
			pk=true;
	}
}

void ComportamientoJugador::pasarMapa(int f, int c){
	for(int i=0; i<mapaResultado.size(); i++)
		for(int j=0; j<mapaResultado[i].size(); j++)
			mapaResultado[i][j] = mi_mapa[mi_estado.fila -f +i][mi_estado.columna -c +j];
	for(int i=0; i<mi_mapa.size(); i++){
		mi_mapa[i].clear();
	}

	mi_mapa.clear();
}

bool ComportamientoJugador::rellenarMapa(Sensores &sensores, estado &e){
	bool nuevo=false;
	int k=0;
	if(localizado) mapaResultado[e.fila][e.columna] = sensores.terreno[k];
	else mi_mapa[e.fila][e.columna] = sensores.terreno[k];
	if(e.orientacion == 0){
		for(int i=1; i<4; i++){
			for(int j=e.columna-i; j<=e.columna+i; j++){
				k++;
				if(localizado && mapaResultado[e.fila-i][j] == '?'){
					nuevo=true;
					mapaResultado[e.fila-i][j] = sensores.terreno[k];
				}
				else{
					if(mi_mapa[e.fila-i][j] == '?'){
						nuevo=true;
						mi_mapa[e.fila-i][j] = sensores.terreno[k];
					}
				}
			}
		}
	}
	if(e.orientacion == 1){
		for(int i=1; i<4; i++){
			for(int j=e.fila-i; j<=e.fila+i; j++){
				k++;
				if(localizado && mapaResultado[j][e.columna+i] == '?'){
					nuevo=true;
					mapaResultado[j][e.columna+i] = sensores.terreno[k];
				}
				else if(mi_mapa[j][e.columna+i] == '?'){
					nuevo=true;
					mi_mapa[j][e.columna+i] = sensores.terreno[k];
				}
			}
		}
	}
	if(e.orientacion == 2){
		for(int i=1; i<4; i++){
			for(int j=e.columna+i; j>=e.columna-i; j--){
				k++;
				if(localizado && mapaResultado[e.fila+i][j]=='?'){
					nuevo=true;
					mapaResultado[e.fila+i][j] = sensores.terreno[k];
				}
				else if(mi_mapa[e.fila+i][j]=='?'){
					nuevo=true;
					mi_mapa[e.fila+i][j] = sensores.terreno[k];
				}
			}
		}
	}
	if(e.orientacion == 3){
		for(int i=1; i<4; i++){
			for(int j=e.fila+i; j>=e.fila-i; j--){
				k++;
				if(localizado && mapaResultado[j][e.columna-i]=='?'){
					nuevo=true;
					mapaResultado[j][e.columna-i] = sensores.terreno[k];
				}
				else if(mi_mapa[j][e.columna-i]=='?'){
					nuevo=true;
					mi_mapa[j][e.columna-i] = sensores.terreno[k];
				}
			}
		}
	}
	return nuevo;
}

void ComportamientoJugador::mostrarMapa(){
	for(int i=0; i<mi_mapa.size(); i++){
		for(int j=0; j<mi_mapa[i].size(); j++)
			if(mi_mapa[i][j] != '?')
				cout << mi_mapa[i][j] << " ";
	}
	cout << endl;
}

bool ComportamientoJugador::terrenoValido(unsigned char terr) {
	bool valido = true;
	unsigned char no[5]={'B','D','M','A','P'};

	for(int i=0; i<5; i++)
		if (terr==no[i])
			valido=false;

	return valido;
}


bool ComportamientoJugador::terrenoValido(estado e) {
	bool valido = true;
	unsigned char terr= mapaResultado[e.fila][e.columna];
	unsigned char no[5]={'B','D','M','A','P'};

	for(int i=0; i<5; i++)
		if (terr==no[i])
			valido=false;
	
	return valido;
}

int ComportamientoJugador::valorEstimado(){
	int valor=0;
	int f,c;
	f = destino.fila-fil;
	c = destino.columna-col;
	if(f>0)
		valor+=f;
	else
		valor-=f;
	if(c>0)
		valor+=c;
	else
		valor-=c;
	return valor;
}

estado ComportamientoJugador::consecuenciaAccion(Action act, estado est){
	switch (act){
		case actFORWARD:
			if(est.orientacion == 0)
				est.fila--;
			if(est.orientacion == 1)
				est.columna++;
			if(est.orientacion == 2)
				est.fila++;
			if(est.orientacion == 3)
				est.columna--;
			break;
		case actTURN_R:
			if(est.orientacion == 3)
				est.orientacion=0;
			else
				est.orientacion++;
			break;			
		case actTURN_L:
			if(est.orientacion == 0)
				est.orientacion=3;
			else
				est.orientacion--;
			break;
		case actIDLE:
			break;
	}
	return est;
}

int ComportamientoJugador::interact(Action accion, int valor){
  return false;
}

/*
 * DanniBot.cpp
 *
 *  Created on: 15 may. 2018
 *      Author: daniel
 */

#include "DanniBot.h"

#include <string>
#include <cstdlib>
#include <iostream>
#include <cmath>

using namespace std;

DanniBot::DanniBot() {
	// Inicializar las variables necesarias para ejecutar la partida

}

DanniBot::~DanniBot() {
	// Liberar los recursos reservados (memoria, ficheros, etc.)
}

void DanniBot::initialize() {
	// Inicializar el bot antes de jugar una partida
}

string DanniBot::getName() {
	return "DanniBot"; // Sustituir por el nombre del bot
}

Move DanniBot::nextMove(const vector<Move> &adversary, const GameState &state) {

	Move movimiento;
	int move = 1;
	int mejorMov = 1;
	int profundidad = 11;
	int alpha = -999;
	int beta = 999;
	jugadorBot = state.getCurrentPlayer();
	
	GameState hijo = state.simulateMove((Move) move);
	int maxAlpha = alpha_beta(hijo, profundidad-1, alpha , beta, jugadorBot);

	for(int i=2; i <= 6; ++i){
			GameState hijo = state.simulateMove((Move) i);
			alpha = alpha_beta(hijo, profundidad-1, alpha , beta, jugadorBot);

			if (alpha > maxAlpha){
				maxAlpha = alpha;
				mejorMov = i;
			}
	}

	movimiento = (Move) mejorMov;

	return movimiento;
}

int DanniBot::alpha_beta(GameState &nodo, int depth, int alpha, int beta, Player turn){

	if(depth == 0 || nodo.isFinalState())
		return heuristica(nodo);

	if(nodo.getCurrentPlayer() == turn){
		int v = -999;
		for(int i = 1; i <= 6; ++i){
			GameState hijo = nodo.simulateMove((Move) i);
			v = max(v, alpha_beta(hijo, depth-1, alpha, beta, turn));
			alpha = max(alpha, v);
			if(beta <= alpha)
				break;
		}
		return v;
	}
	
	else{
		int v = 999;
		for(int i = 1; i <= 6; ++i){
			GameState hijo = nodo.simulateMove((Move) i);
			v = min(v, alpha_beta(hijo, depth-1, alpha, beta, turn));
			beta = min(beta, v);
			if(beta <= alpha)
				break;
		}		
		return v;
	}
}

int DanniBot::heuristica(GameState &state){
	int heuristicaFavor = 0;
	int heuristicaContra = 0;
	int ganador = 0;

	if(jugadorBot==J1){
		if(state.getWinner() == J1)
			ganador = 500;
		heuristicaFavor = ganador + 10*state.getScore(J1) + 10*casillasVacias(state, J2);
		heuristicaContra = 10*state.getScore(J2) + 10*casillasVacias(state, J1);
	}

	else{
		if(state.getWinner() == J2)
			ganador = 500;
		heuristicaFavor = ganador + 10*semillasTotales(state, J2);
		heuristicaContra = 10*semillasTotales(state, J1);
	}

	return heuristicaFavor - heuristicaContra;
}

int DanniBot::semillasTotales(GameState &status, Player turn){
	int total = 0;
	int vacios = 0;

	for(int i = 0; i <= 6; i++){
		if(status.getSeedsAt(turn, (Position) i) == 0)
			vacios++;

		total += status.getSeedsAt(turn, (Position) i);
	}

	return total;
}

int DanniBot::casillasVacias(GameState &status, Player turn){
	int vacios = 0;

	for(int i = 0; i <= 6; i++)
		if(status.getSeedsAt(turn, (Position) i) == 0)
			vacios++;

	return vacios;
}

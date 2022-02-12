/*
 * DanniBot.h
 *
 *  Created on: 15 may. 2018
 *      Author: daniel
 */

#include "Bot.h"

#ifndef MANUPCBOT_H_
#define MANUPCBOT_H_

class DanniBot:Bot {
public:
	DanniBot();
	~DanniBot();


	void initialize();
	string getName();
	Move nextMove(const vector<Move> &adversary, const GameState &state);
	int alpha_beta(GameState &nodo, int depth, int alpha, int beta, Player turn);
	int heuristica(GameState &state);
	int semillasTotales(GameState &status, Player turn);
	int casillasVacias(GameState &status, Player turn);

private:
	Player jugadorBot;
};

#endif /* MANUPCBOT_H_ */

#ifndef ALGORITHM_H
#define	ALGORITHM_H


#include "graph.h"
#include "tournament.h"

typedef std::shared_ptr<Tournament> tournament_ptr;

tournament_ptr fix_tournament(TournamentGraph&, int);

#endif	/* ALGORITHM_H */


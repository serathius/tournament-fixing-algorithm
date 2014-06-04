#ifndef TOURNAMENT_H
#define	TOURNAMENT_H

#include <vector>
#include <memory>

#include "graph.h"

class Match 
{
    int winner; 
    int looser;

public:
    Match(TournamentGraph&, int, int);
    int get_looser();
    int get_winner();
};

typedef std::shared_ptr<Match> match_ptr;

class Level
{
    std::vector<match_ptr> matches;
    
private:
    Level(Level*, Level*);
    friend class Tournament;
    
public:
    Level(TournamentGraph&, int, int);
    Level(const Level&);
    ~Level();
    int size();
};

class Tournament
{
    std::vector<Level*> levels;
    TournamentGraph * graph;
    int winner;
    
private:
    Tournament(TournamentGraph*, const std::vector<Level*>&, int);
    
public:
    Tournament(TournamentGraph*, int, int);
    Tournament(const Tournament&, const Tournament&);
    ~Tournament();
    void print();
    int get_winner();
    int get_level_count();
};

#endif	/* TOURNAMENT_H */


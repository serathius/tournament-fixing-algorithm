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
    void print();
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
    void print();
    int size();
};

class Tournament
{
    std::vector<Level*> levels;
    std::vector<int> compatitors;
    TournamentGraph * graph;
    int winner;
    
private:
    Tournament(TournamentGraph*, const std::vector<Level*>&, 
               const std::vector<int>&, int);
    
public:
    Tournament(TournamentGraph*, int);
    Tournament(TournamentGraph*, int, int);
    Tournament(const Tournament&, const Tournament&);
    ~Tournament();
    void print();
    int get_winner();
    int get_level_count();
};

#endif	/* TOURNAMENT_H */


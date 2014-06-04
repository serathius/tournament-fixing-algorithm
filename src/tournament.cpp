#include <cassert> 

#include <algorithm>

#include "../include/tournament.h"


Match::Match(TournamentGraph& graph, 
             int compatitor1, int compatitor2)
{
    if (graph.wins(compatitor2, compatitor1))
        std::swap(compatitor1, compatitor2);
    this->winner = compatitor1;
    this->looser = compatitor2;
}

int Match::get_winner()
{
    return this->winner;
}

int Match::get_looser()
{
    return this->looser;
}

Level::Level(Level* level1, Level* level2)
{
    assert(level1->matches.size() == level2->matches.size());
    this->matches.reserve(level1->matches.size() + level2->matches.size());
    std::merge(level1->matches.begin(), level1->matches.end(), 
               level2->matches.begin(), level2->matches.end(), 
               std::back_inserter(this->matches));
}

Level::Level(TournamentGraph& graph, 
             int compatitor1, int compatitor2)
{
    auto match = new Match(graph, compatitor1, compatitor2);
    this->matches.push_back(match_ptr(match));
}

Level::Level(const Level& orig)
{
    this->matches = orig.matches;
}

Level::~Level()
{
    
}

int Level::size()
{
    return this->matches.size();
}

Tournament::Tournament(TournamentGraph* graph, 
                      const std::vector<Level*>& levels,
                      int winner)
{
    this->levels = levels;
    this->level_count = levels.size();
    this->graph = graph;
    this->winner = winner;
}

Tournament::Tournament(TournamentGraph* graph, 
                       int compatitor1, int compatitor2) 
{
    auto level = new Level(*graph, compatitor1, compatitor2);
    this->levels.push_back(level);
    this->level_count = 1;
    this->graph = graph;
    if(graph->wins(compatitor1, compatitor2))
        this->winner = compatitor1;
    else
        this->winner = compatitor2;
}

Tournament::~Tournament()
{
    for (auto level: this->levels)
    {
        delete level;
    }
}

Tournament Tournament::operator+(const Tournament& other)
{
    assert(this->level_count == other.level_count);
    assert(this->graph == other.graph);
    std::vector<Level*> levels;
    for (int i=0; i< other.level_count; ++i)
        levels.push_back(new Level(this->levels[i], other.levels[i]));
    levels.push_back(new Level(*this->graph, this->winner, other.winner));
    if(this->graph->wins(this->winner, other.winner))
        return Tournament(this->graph, levels, this->winner);
    else
        return Tournament(this->graph, levels, other.winner);
    
}

int Tournament::get_winner()
{
    return this->winner;
}

int Tournament::get_level_count()
{
    return this->level_count;
}
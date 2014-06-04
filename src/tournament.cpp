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
                      const std::vector<int>& compatitors, int winner)
{
    this->levels = levels;
    this->compatitors = compatitors;
    this->graph = graph;
    this->winner = winner;
}

Tournament::Tournament(TournamentGraph* graph, int compatitor) 
{
    this->compatitors.reserve(1);
    this->compatitors.push_back(compatitor);
    this->graph = graph;
    this->winner = compatitor;
}

Tournament::Tournament(TournamentGraph* graph, int compatitor1, int compatitor2) 
{
    this->levels.reserve(1);
    this->levels.push_back(new Level(*graph, compatitor1, compatitor2));
    this->compatitors.reserve(2);
    this->compatitors.push_back(compatitor1);
    this->compatitors.push_back(compatitor2);
    this->graph = graph;
    if(graph->wins(compatitor1, compatitor2))
        this->winner = compatitor1;
    else
        this->winner = compatitor2;
}

Tournament::Tournament(const Tournament& first, const Tournament& second)
{
    assert(first.levels.size() == second.levels.size());
    assert(first.graph == second.graph);
    this->levels.reserve(first.levels.size() + 1);
    for (int i=0; i<first.levels.size(); ++i)
        this->levels.push_back(new Level(first.levels[i], second.levels[i]));
    this->levels.push_back(new Level(*first.graph, first.winner, second.winner));
    this->graph = first.graph;
    if(first.graph->wins(first.winner, second.winner))
        this->winner = first.winner;
    else
        this->winner = second.winner;
}

Tournament::~Tournament()
{
    for (auto level: this->levels)
    {
        delete level;
    }
}

int Tournament::get_winner()
{
    return this->winner;
}

int Tournament::get_level_count()
{
    return this->levels.size();
}
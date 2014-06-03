#include "../include/graph.h"

#include <cassert>


TournamentGraph::TournamentGraph(int size) 
{
    this->edge = new bool*[size]();
    for (int i=0; i<size; ++i)
        this->edge[i] = new bool[size - i - 1]();
    this->size = size;
}

TournamentGraph::~TournamentGraph()
{
    for (int i=0; i<this->size;++i)
        delete [] this->edge[i];
    delete [] this->edge;    
}

bool TournamentGraph::wins(int edge1, int edge2)
{
    assert(edge1 != edge2);
    if (edge1 < edge2)
        return this->edge[edge1][edge2 - edge1 - 1];
    else
        return !this->edge[edge2][edge1 - edge2 - 1];
}

void TournamentGraph::set_winner(int edge1, int edge2) 
{
    assert(edge1 != edge2);
    if (edge1 < edge2)
        this->edge[edge1][edge2 - edge1 - 1] = true;
    else
        this->edge[edge2][edge1 - edge2 - 1] = false;
}

int TournamentGraph::get_size()
{
    return this->size;
}
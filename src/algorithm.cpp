#include "../include/graph.h"
#include "../include/tournament.h"
#include <cmath>

bool check_if_fixable(TournamentGraph& graph, int node)
{
    int wins = 0;
    for (int i=0; i<graph.get_size(); ++i)
    {
        if(i != node && graph.wins(node, i))
            wins++;
    }
    return wins >= log2(graph.get_size());
}

void fix_tournament(TournamentGraph& graph, int node)
{
    if(check_if_fixable(graph, node))
        printf("Solvable\n");
}

#include "../include/graph.h"
#include "../include/tournament.h"
#include <vector>
#include <cmath>
#include <set>
#include <cassert>

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

bool check_case_A(TournamentGraph& graph, int node)
{
    int wins = 0;
    int max_wins_from_losses = 0;
    int wins_from_losses;  
            
    for (int i=0; i<graph.get_size(); ++i)
    {
        if(i != node && graph.wins(node, i))
            wins++;
        else
        {
            wins_from_losses = 0;
            for (int j=0; j<graph.get_size(); ++j)
            {
                if(i == j)
                    break;
                if(graph.wins(i,j))
                    wins_from_losses++;
            }
            if(wins_from_losses > max_wins_from_losses)
            {
                max_wins_from_losses = wins_from_losses;
            }
        }
    }
    return wins > max_wins_from_losses;
}

void fix_tournament_A(TournamentGraph& graph, int node)
{
    std::set<int> won_with, lost_with;
    std::vector<std::shared_ptr<Tournament>> tournaments;
    for (int i=0; i<graph.get_size(); ++i)
    {
        if(node == i)
            continue;
        if(graph.wins(node, i))
            won_with.insert(i);
        else
            lost_with.insert(i);
    }
    std::set<int> won_copy(won_with), lost_copy(lost_with);
    for(auto lose: lost_copy)
    {
        for(auto win: won_copy)
        {
            if(won_with.count(win) && graph.wins(win, lose))
            {
                won_with.erase(win);
                lost_with.erase(lose);
                tournaments.push_back(
                    std::shared_ptr<Tournament>(
                        new Tournament(&graph, win, lose)));
                break;
            }
        }
    }
    assert(lost_with.size() == 0);
    
    for (auto i = won_with.begin(); i!=won_with.end(); ++i)
    {
        auto compatitor1 = *i;
        if (++i==won_with.end())
            break;
        auto compatitor2 = *i;
        tournaments.push_back(
            std::shared_ptr<Tournament>(
                new Tournament(&graph, compatitor1, compatitor2)));
    }
    auto last_element = *won_with.begin();
    tournaments.push_back(
        std::shared_ptr<Tournament>(
            new Tournament(&graph, node, last_element)));
    
    std::vector<std::shared_ptr<Tournament>> tournaments_temp;
    while(tournaments.size() > 1)
    {
        for (auto i = tournaments.begin(); i!=tournaments.end(); ++i)
        {
            auto tournament1 = *i;
            i++;
            auto tournament2 = *i;
            tournaments_temp.push_back(std::unique_ptr<Tournament>(
                new Tournament(*tournament1, *tournament2)));
        }
        tournaments = tournaments_temp;
    }
    printf("%d\n",tournaments[0]->get_winner());
 }

void fix_tournament(TournamentGraph& graph, int node)
{
    if(check_if_fixable(graph, node))
        printf("Solvable\n");
    if (check_case_A(graph, node))
    {
        printf("Case A\n");    
        fix_tournament_A(graph, node);
    }
}

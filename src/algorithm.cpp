#include "../include/graph.h"
#include "../include/tournament.h"
#include <vector>
#include <cmath>
#include <set>
#include <cassert>

typedef std::shared_ptr<Tournament> tournament_ptr;

bool check_if_fixable(TournamentGraph& graph, int node)
{
    int wins = 0;
    for (int i=0; i<graph.get_size(); ++i)
    {
        if(i != node && graph.wins(node, i))
            wins++;
    }
    if (wins < log2(graph.get_size()))
        return false;
    else
    {
        for (int i=0; i<graph.get_size(); ++i)
        {
            if(i == node)
                continue;
            wins = 0;
            for (int j=0; j<graph.get_size(); ++j)
            {
                if(i == j)
                    continue;
                if(graph.wins(i, j))
                    wins++;
                else
                    break;
            }
            if(wins == graph.get_size() - 1)
                return false;
        }
        return true;
    }
}

bool check_if_case_A(TournamentGraph& graph, int node)
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

std::shared_ptr<Tournament> fix_tournament_A(TournamentGraph& graph, int node)
{
    std::set<tournament_ptr> won_tournaments, lost_tournaments;
    for (int i=0; i<graph.get_size(); ++i)
    {
        if(node == i)
            continue;
        if(graph.wins(node, i))
            won_tournaments.insert(tournament_ptr(new Tournament(&graph, i)));
        else
            lost_tournaments.insert(tournament_ptr(new Tournament(&graph, i)));    
    }
    std::set<tournament_ptr> new_won_tournaments;
    std::set<tournament_ptr> won_copy(won_tournaments);
    std::set<tournament_ptr> lost_copy(lost_tournaments);
    
    for(auto lose: lost_copy)
    {
        for(auto win: won_copy)
        {
            if(won_tournaments.count(win) && graph.wins(win->get_winner(), 
                                                        lose->get_winner()))
            {
                won_tournaments.erase(win);
                lost_tournaments.erase(lose);
                new_won_tournaments.insert(tournament_ptr(
                    new Tournament(*win, *lose)));
                break;
            }
        }
    }
    
    assert(lost_tournaments.size() == 0);
    while(won_tournaments.size() > 1)
    {
        for (auto i = won_tournaments.begin(); i!=won_tournaments.end(); ++i)
        {
            auto compatitor1 = *i;
            if (++i==won_tournaments.end())
            {
                new_won_tournaments.insert(tournament_ptr(
                    new Tournament(*compatitor1, Tournament(&graph, node))));
                break;
            }
            auto compatitor2 = *i;
            new_won_tournaments.insert(tournament_ptr(
                new Tournament(*compatitor1, *compatitor2)));
        }
        won_tournaments = new_won_tournaments;
        new_won_tournaments.clear();
    }
    
    return *won_tournaments.begin();
 }

bool check_if_case_B(TournamentGraph& graph, int node)
{
    int wins = 0;
    for (int i=0; i<graph.get_size(); ++i)
    {
        if(i != node && graph.wins(node, i))
            wins++;
    }
    return wins >= graph.get_size() / 2;
}

std::shared_ptr<Tournament> fix_tournament_B(TournamentGraph &graph, int node)
{
    std::set<tournament_ptr> won_tournaments, lost_tournaments;
    std::set<tournament_ptr> new_won_tournaments, new_lost_tournaments;
    for (int i=0; i<graph.get_size(); ++i)
    {
        if(node == i)
            continue;
        if(graph.wins(node, i))
            won_tournaments.insert(tournament_ptr(new Tournament(&graph, i)));
        else
            lost_tournaments.insert(tournament_ptr(new Tournament(&graph, i)));    
    }
    while(lost_tournaments.size() > 0)
    {
        std::set<tournament_ptr> won_copy(won_tournaments);
        std::set<tournament_ptr> lost_copy(lost_tournaments);

        for(auto lose: lost_copy)
        {
            for(auto win: won_copy)
            {
                if(won_tournaments.count(win) && graph.wins(win->get_winner(), 
                                                            lose->get_winner()))
                {
                    won_tournaments.erase(win);
                    lost_tournaments.erase(lose);
                    new_won_tournaments.insert(tournament_ptr(
                        new Tournament(*win, *lose)));
                    break;
                }
            }
        }
        for (auto i=lost_tournaments.begin(); i!=lost_tournaments.end(); ++i)
        {
            auto compatitor1 = *i;
            if (++i==lost_tournaments.end())
            {
                auto won_example = *won_tournaments.begin();
                won_tournaments.erase(won_example);
                new_lost_tournaments.insert(tournament_ptr(
                    new Tournament(*compatitor1, *won_example)));
                break;
            }
            auto compatitor2 = *i;
            new_lost_tournaments.insert(tournament_ptr(
                new Tournament(*compatitor1, *compatitor2)));
        }
        for (auto i=won_tournaments.begin(); i!=won_tournaments.end(); ++i)
        {
            auto compatitor1 = *i;
            if (++i==won_tournaments.end())
            {
                new_won_tournaments.insert(tournament_ptr(
                    new Tournament(*compatitor1, Tournament(&graph, node))));
                break;
            }
            auto compatitor2 = *i;
            new_won_tournaments.insert(tournament_ptr(
                new Tournament(*compatitor1, *compatitor2)));
        }
        won_tournaments = new_won_tournaments;
        new_won_tournaments.clear();
        lost_tournaments = new_lost_tournaments;
        new_lost_tournaments.clear();
    }
    
    while(won_tournaments.size() > 1)
    {
        for (auto i = won_tournaments.begin(); i!=won_tournaments.end(); ++i)
        {
            auto compatitor1 = *i;
            ++i;
            auto compatitor2 = *i;
            new_won_tournaments.insert(
                std::shared_ptr<Tournament>(
                    new Tournament(*compatitor1, *compatitor2)));
        }
        won_tournaments = new_won_tournaments;
        new_won_tournaments.clear();
    }
    return *won_tournaments.begin();
}

void fix_tournament(TournamentGraph& graph, int node)
{
    if(check_if_fixable(graph, node))
    {
        std::shared_ptr<Tournament> tournament;
        printf("Fixable\n");
        if (check_if_case_A(graph, node))
        {
            printf("Case A\n");    
            tournament = fix_tournament_A(graph, node);
        }
        else if (check_if_case_B(graph, node))
        {
            printf("Case B\n");
            tournament = fix_tournament_B(graph, node);
        }
        else
        {
            printf("Worst Case");
        }
    }
    else
    {
        printf("Unfixable\n");
    }
    
}

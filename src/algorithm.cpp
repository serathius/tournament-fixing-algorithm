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

typedef std::shared_ptr<Tournament> tournament_ptr;

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
    std::set<int> won_with, lost_with;
    std::set<std::shared_ptr<Tournament>> won_subtournaments;
    std::set<std::shared_ptr<Tournament>> lost_subtournaments;
    
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
                won_subtournaments.insert(
                    std::shared_ptr<Tournament>(
                        new Tournament(&graph, win, lose)));
                break;
            }
        }
    }    
    for (auto i = lost_with.begin(); i!=lost_with.end(); ++i)
    {
        auto compatitor1 = *i;
        if (++i==lost_with.end())
            break;
        auto compatitor2 = *i;
        lost_subtournaments.insert(
            std::shared_ptr<Tournament>(
                new Tournament(&graph, compatitor1, compatitor2)));
    }
    for (auto i = won_with.begin(); i!=won_with.end(); ++i)
    {
        auto compatitor1 = *i;
        if (++i==won_with.end())
        {
            won_subtournaments.insert(
                std::shared_ptr<Tournament>(
                    new Tournament(&graph, node, compatitor1)));
            break;
        }
        auto compatitor2 = *i;
        won_subtournaments.insert(
            std::shared_ptr<Tournament>(
                new Tournament(&graph, compatitor1, compatitor2)));
    }
    
    while(lost_subtournaments.size() > 0)
    {
        auto lost_subtournaments_copy = lost_subtournaments;
        auto won_subtournaments_copy = won_subtournaments;
        std::set<std::shared_ptr<Tournament>> new_lost_subtournaments;
        std::set<std::shared_ptr<Tournament>> new_won_subtournaments;
        for(auto lose: lost_subtournaments_copy)
        {
            for(auto win: won_subtournaments_copy)
            {
                if(won_subtournaments.count(win) && graph.wins(
                    win->get_winner(), lose->get_winner()))
                {
                    won_subtournaments.erase(win);
                    lost_subtournaments.erase(lose);
                    new_won_subtournaments.insert(
                        std::shared_ptr<Tournament>(
                            new Tournament(*win, *lose)));
                    break;
                }
            }
        }
        for (auto i = lost_subtournaments.begin(); 
             i!=lost_subtournaments.end(); ++i)
        {
            auto compatitor1 = *i;
            if (++i==lost_subtournaments.end())
            {
                new_lost_subtournaments.insert(compatitor1);
                break;
            }
            auto compatitor2 = *i;
            new_lost_subtournaments.insert(
                std::shared_ptr<Tournament>(
                    new Tournament(*compatitor1, *compatitor2)));
        }
        for (auto i = won_subtournaments.begin(); i!=won_subtournaments.end(); ++i)
        {
            auto compatitor1 = *i;
            if (++i==won_subtournaments.end())
            {
                new_won_subtournaments.insert(compatitor1);
                break;
            }
            auto compatitor2 = *i;
            new_won_subtournaments.insert(
                std::shared_ptr<Tournament>(
                    new Tournament(*compatitor1, *compatitor2)));
        }
        lost_subtournaments = new_lost_subtournaments;
        won_subtournaments = new_won_subtournaments;
    }
    while(won_subtournaments.size() > 1)
    {
        std::set<std::shared_ptr<Tournament>> new_won_subtournaments;
        for (auto i = won_subtournaments.begin(); i!=won_subtournaments.end(); ++i)
        {
            auto compatitor1 = *i;
            if (++i==won_subtournaments.end())
            {
                new_won_subtournaments.insert(compatitor1);
                break;
            }
            auto compatitor2 = *i;
            new_won_subtournaments.insert(
                std::shared_ptr<Tournament>(
                    new Tournament(*compatitor1, *compatitor2)));
        }
        won_subtournaments = new_won_subtournaments;
    }
    return *won_subtournaments.begin();
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

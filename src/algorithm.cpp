#include <vector>
#include <cmath>
#include <set>
#include <cassert>

#include "../include/graph.h"
#include "../include/tournament.h"
#include "../include/exceptions.h"
#include "../include/iterators.h"


typedef std::shared_ptr<Tournament> tournament_ptr;


bool check_if_fixable(TournamentGraph& graph, std::set<int>& nodes, int winner)
{
    int wins = 0;
    for(auto node: nodes)
    {
        if(node != winner && graph.wins(winner, node))
            wins++;
    }
    if (wins < log2(nodes.size()))
        return false;
    else
    {
        for (auto node1: nodes)
        {
            if(node1 == winner)
                continue;
            wins = 0;
            for (auto node2: nodes)
            {
                if(node1 == node2)
                    continue;
                if(graph.wins(node1, node2))
                    wins++;
                else
                    break;
            }
            if(wins == nodes.size() - 1)
                return false;
        }
        return true;
    }
}


bool check_if_case_A(TournamentGraph& graph, std::set<int>& nodes, int winner)
{
    int wins = 0;
    int max_wins_from_losses = 0;
    int wins_from_losses;  
    for(auto node1: nodes)
    {
        if(node1 == winner)
            continue;
        if(graph.wins(winner, node1))
            wins++;
        else
        {
            wins_from_losses = 0;
            for (auto node2: nodes)
            {
                if(node1 == node2)
                    continue;
                if(graph.wins(node1, node2))
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


tournament_ptr fix_tournament_A(TournamentGraph& graph, 
                                std::set<int>& nodes, int winner)
{
    std::set<tournament_ptr> won_tournaments, lost_tournaments;
    for(auto node: nodes)
    {
        if(winner == node)
            continue;
        if(graph.wins(winner, node))
            won_tournaments.insert(
                tournament_ptr(new Tournament(&graph, node)));
        else
            lost_tournaments.insert(
                tournament_ptr(new Tournament(&graph, node)));    
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
    do
    {
        for (auto i = won_tournaments.begin(); i!=won_tournaments.end(); ++i)
        {
            auto compatitor1 = *i;
            if (++i==won_tournaments.end())
            {
                new_won_tournaments.insert(tournament_ptr(
                    new Tournament(*compatitor1, Tournament(&graph, winner))));
                break;
            }
            auto compatitor2 = *i;
            new_won_tournaments.insert(tournament_ptr(
                new Tournament(*compatitor1, *compatitor2)));
        }
        won_tournaments = new_won_tournaments;
        new_won_tournaments.clear();
    }
    while (won_tournaments.size() > 1);
    return *won_tournaments.begin();
 }


bool check_if_case_B(TournamentGraph& graph, std::set<int>& nodes, int winner)
{
    int wins = 0;
    for(auto node: nodes)
    {
        if(node != winner && graph.wins(winner, node))
            wins++;
    }
    return wins >= nodes.size() / 2;
}


tournament_ptr fix_tournament_B(TournamentGraph &graph, 
                                std::set<int>& nodes, int winner)
{
    std::set<tournament_ptr> won_tournaments, lost_tournaments;
    std::set<tournament_ptr> new_won_tournaments, new_lost_tournaments;
    for (auto node: nodes)
    {
        if(winner == node)
            continue;
        if(graph.wins(winner, node))
            won_tournaments.insert(
                tournament_ptr(new Tournament(&graph, node)));
        else
            lost_tournaments.insert(
                tournament_ptr(new Tournament(&graph, node)));    
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
                    new Tournament(*compatitor1, Tournament(&graph, winner))));
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


bool check_if_case_C(TournamentGraph& graph, std::set<int>& nodes, int winner)
{
    std::set<int> won_with, lost_with;
    int log2n = std::log2(nodes.size());
    for (auto node: nodes)
    {
        if(node == winner)
            continue;
        if(graph.wins(winner, node))
            won_with.insert(node);
        else
            lost_with.insert(node);
    }
    if (won_with.size() < log2n)
        return false;
    for (auto lose: lost_with)
    {
        int lose_lost_with_won_count = 0;
        for (auto win: won_with)
        {
            if(graph.wins(win, lose))
                lose_lost_with_won_count++;
        }
        if(lose_lost_with_won_count < log2n)
            return false;
    }
    return true;
}


tournament_ptr fix_tournament_C(TournamentGraph& graph, 
                                std::set<int>& nodes, int winner)
{
    std::set<tournament_ptr> won_tournaments, lost_tournaments;
    std::set<tournament_ptr> new_won_tournaments, new_lost_tournaments;
    tournament_ptr winner_tournament(new Tournament(&graph, winner));
    for (auto node: nodes)
    {
        if(winner == node)
            continue;
        if(graph.wins(winner, node))
            won_tournaments.insert(
                tournament_ptr(new Tournament(&graph, node)));
        else
            lost_tournaments.insert(
                tournament_ptr(new Tournament(&graph, node)));    
    }
    
    while(won_tournaments.size() > 0 || lost_tournaments.size() > 0)
    {
        auto first_won = *won_tournaments.begin();
        won_tournaments.erase(first_won);
        winner_tournament = tournament_ptr(
            new Tournament(*winner_tournament, *first_won));
        
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
                    new Tournament(*compatitor1, Tournament(&graph, winner))));
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
    return winner_tournament;
}


tournament_ptr _fix_tournament(TournamentGraph& graph, 
                               std::set<int>& nodes, int winner)
{
    if(check_if_fixable(graph, nodes, winner))
    {
        if (check_if_case_A(graph, nodes, winner)) 
            return fix_tournament_A(graph, nodes, winner);
        else if (check_if_case_B(graph, nodes, winner))
            return fix_tournament_B(graph, nodes, winner);
        else if (check_if_case_C(graph, nodes, winner))
            return fix_tournament_C(graph, nodes, winner);
        else
        {   
            int log2n = log2(nodes.size());
            std::set<int> won_with;
            
            nodes.erase(winner);
            for(auto node: nodes)
            {
                if(graph.wins(winner, node))
                    won_with.insert(node);
            }

            CombinationIterator won_iter(won_with, log2n);
            while(true)
            {
                try
                {
                    int size = 2;
                    std::set<int> nodes_copy(nodes);
                    std::vector<int> wins(won_iter.next());
                    for (auto win: wins)
                    {
                        nodes_copy.erase(win);
                    }
                    int first_win = wins.back();
                    wins.pop_back();
                    tournament_ptr tournament_copy(tournament_ptr(
                        new Tournament(&graph, winner, first_win)));
                    for(auto win: wins)
                    {
                        CombinationIterator combinations_iter(
                            nodes_copy, size - 1);
                        while(true)
                        {
                            try
                            {
                                std::set<int> rest(
                                    combinations_iter.next_set());
                                rest.insert(win);
                                tournament_ptr t = _fix_tournament(graph, rest, win);
                                tournament_copy = tournament_ptr(new Tournament(
                                    *t, *tournament_copy));
                                break;
                                 
                            }
                            catch(TournamentUnfixableError)
                            {
                                
                            }
                            catch(StopIterationError)
                            {
                                throw TournamentUnfixableError();
                            }
                        }
                        size *= 2;
                    }
                    return tournament_copy;
                    
                }
                catch(TournamentUnfixableError)
                {
                    
                }
                catch(StopIterationError)
                {
                    throw TournamentUnfixableError();
                }
            }
        }
    }
    else
    {
        throw TournamentUnfixableError();
    }
}


void fix_tournament(TournamentGraph& graph, int winner)
{
    std::set<int> nodes;
    for (int i=0; i<graph.get_size(); ++i)
        nodes.insert(i);
    try
    {
        tournament_ptr tournament = _fix_tournament(graph, nodes, winner);
        tournament->print();
    }
    catch(TournamentUnfixableError)
    {
        printf("Unfixable\n");
    }
}
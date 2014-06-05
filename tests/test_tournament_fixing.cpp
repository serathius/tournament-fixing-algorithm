#include "gtest/gtest.h"
#include "../include/algorithm.h"


TEST (TournamentFixingTest, test_simple_fixing)
{
    TournamentGraph graph(2);
    graph.set_winner(0, 1);
    fix_tournament(graph, 0);
}


TEST (TournamentFixingTest, test_case_A)
{
    TournamentGraph graph(4);
    graph.set_winner(0, 1);
    graph.set_winner(0, 2);
    graph.set_winner(1, 2);
    graph.set_winner(2, 3);
    fix_tournament(graph, 0);
}

TEST (TournamentFixingTest, test_case_B)
{
    TournamentGraph graph(8);
    graph.set_winner(0, 1);
    graph.set_winner(0, 2);
    graph.set_winner(0, 3);
    graph.set_winner(0, 4);
    graph.set_winner(1, 5);
    graph.set_winner(2, 6);
    graph.set_winner(3, 7);

    fix_tournament(graph, 0);
}

TEST (TournamentFixingTest, test_case_C)
{
    TournamentGraph graph(8);
    graph.set_winner(0, 1);
    graph.set_winner(0, 2);
    graph.set_winner(0, 3);
    graph.set_winner(1, 4);
    graph.set_winner(2, 4);
    graph.set_winner(3, 4);
    graph.set_winner(1, 5);
    graph.set_winner(2, 5);
    graph.set_winner(3, 5);
    graph.set_winner(1, 6);
    graph.set_winner(2, 6);
    graph.set_winner(3, 6);
    graph.set_winner(1, 7);
    graph.set_winner(2, 7);
    graph.set_winner(3, 7);
    fix_tournament(graph, 0);
}

TEST (TournamentFixingTest, test_case_D)
{
    TournamentGraph graph(8);
    graph.set_winner(0, 1);
    graph.set_winner(2, 3);
    graph.set_winner(0, 2);
    graph.set_winner(4, 5);
    graph.set_winner(6, 7);
    graph.set_winner(4, 6);
    graph.set_winner(0, 4);

    fix_tournament(graph, 0);
}
#include <cstdio>


#include "../include/tournament.h"
#include "../include/algorithm.h"
#include "../include/exceptions.h"


int main(int argc, const char** argv)
{
    if(argc != 2)
    {
        printf("Arguments: file name.\n");
        return 1;
    }
    else
    {
        FILE * file = fopen(argv[1], "r");
        int size, winner, win, lose;
        fscanf(file, "players %d;", &size);
        if(size == 0 || ((size - 1) & size))
            return 1;
        fscanf(file, "winner %d;", &winner);
        if(winner < 0 || winner >= size)
            return 2;

        TournamentGraph graph(size);
        while(fscanf(file, "%d > %d;", &win, &lose) > 0)
        {
            graph.set_winner(win, lose);
        }
        if(!feof(file))
            return 3;
        try
        {
            const clock_t begin_time = clock();
            tournament_ptr tournament = fix_tournament(graph, winner);
            printf("%f\n", float( clock () - begin_time ) /  CLOCKS_PER_SEC);
            
            tournament->print();
        }
        catch(TournamentUnfixableError)
        {
            printf("Unsolvable\n");
        }
        return 0; 
    }
    
}

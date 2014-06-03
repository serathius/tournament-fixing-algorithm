#ifndef GRAPH_H
#define	GRAPH_H

class TournamentGraph
{
    bool ** edge;
    int size;
    
public:        
    TournamentGraph(int size);
    ~TournamentGraph();
    bool wins(int, int);
    void set_winner(int, int);
    int get_size();
};

#endif	/* GRAPH_H */


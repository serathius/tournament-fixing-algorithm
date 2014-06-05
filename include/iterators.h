#ifndef ITERATORS_H
#define	ITERATORS_H

#include <vector>
#include <set>
#include <memory>

class CombinationIterator
{
    std::unique_ptr<CombinationIterator> next_iter;
    int index;
    std::vector<int> items;
    int size;
    
public:   
    CombinationIterator(std::vector<int> nodes, int size);
    CombinationIterator(std::set<int>& nodes, int size);
    ~CombinationIterator();
    std::vector<int> next();
    std::set<int> next_set();
};

#endif	/* ITERATORS_H */


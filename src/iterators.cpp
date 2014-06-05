#include "../include/iterators.h"
#include "../include/exceptions.h"

#include <cstdio>

CombinationIterator::CombinationIterator(std::vector<int> items, int size) 
{
    this->index = 0;
    this->items = items;
    this->size = size;
    if (size == 1)
        this->next_iter = nullptr;
    else {
        std::vector<int> items_copy;
        for (int i = 0; i < items.size(); i++) {
            if (i != this->index)
                items_copy.push_back(items[i]);
        }
        this->next_iter = std::unique_ptr<CombinationIterator>(
            new CombinationIterator(items_copy, size - 1));
    }
}

CombinationIterator::CombinationIterator(std::set<int>& nodes_set, int size) 
{

    this->index = 0;
    this->items = std::vector<int>(nodes_set.begin(), nodes_set.end());
    this->size = size;
    if (size == 1)
        this->next_iter = nullptr;
    else {
        std::vector<int> items_copy;
        for (int i = 0; i < this->items.size(); i++) {
            if (i != this->index)
                items_copy.push_back(this->items[i]);
        }
        this->next_iter = std::unique_ptr<CombinationIterator>(
            new CombinationIterator(items_copy, size - 1));
    }
}

CombinationIterator::~CombinationIterator()
{
}

std::vector<int> CombinationIterator::next()
{
    std::vector<int> result;
    if(this->next_iter == nullptr)
    {
        if(this->index < this->items.size())
        {
            result.push_back(this->items[this->index++]);
            return result;
        }
        else
        {
            throw StopIterationError();
        }
    }
    else
    {
        try
        {
            std::vector<int> result;
            result.push_back(this->items[this->index]);
            std::vector<int> result2(this->next_iter->next());
            result.insert(result.end(), result2.begin(), result2.end());
            return result;
        }
        catch(StopIterationError)
        {
            if (++this->index < this->items.size())
            {
                std::vector<int> items_copy;
                for(int i=0; i<this->items.size(); i++)
                {
                    if(i != this->index)
                        items_copy.push_back(this->items[i]);
                }
                this->next_iter = std::unique_ptr<CombinationIterator>(
                    new CombinationIterator(items_copy, size - 1));
                return this->next();
            }
            else
                throw StopIterationError();
        }
    }
}


std::set<int> CombinationIterator::next_set()
{
    std::vector<int> combination(this->next());
    return std::set<int>(combination.begin(), combination.end());
}
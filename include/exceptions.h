#ifndef EXCEPTIONS_H
#define	EXCEPTIONS_H

#include <exception>

class TournamentUnfixableError : public std::exception
{
    
};


class StopIterationError : public std::exception
{
    
};

#endif	/* EXCEPTIONS_H */


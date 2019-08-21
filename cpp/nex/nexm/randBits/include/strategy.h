#ifndef NEX_ACTION_GEN_H_
#define NEX_ACTION_GEN_H_

#include "game.h"

class NexExpansionStrategy  :   public ExpansionStrategy<State, string>{
public:
    NexExpansionStrategy(State* state);
    ~NexExpansionStrategy()  override    {}
    string* generateNext();
    bool canGenerateNext();
    void searchNextPossibleMoves();
};

class NexPlayoutStrategy    :   public PlayoutStrategy<State, string>{
public:
    NexPlayoutStrategy(State* state);
    void generateRandom(string* action)   override;
    ~NexPlayoutStrategy()   override    {}
};

#endif  /* NEX_ACTION_GEN_H_ */
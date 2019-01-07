#ifndef ENUMCLASSES_H_
#define ENUMCLASSES_H_

enum class AgentState
{
    FOLLOW,
    ATTACKFRENZY,
    CHARGE,
    FLEE,
    GIVEUP,
    EXIT,
    DONE,
    FAILED,
    IDLE
};
enum class Tag
{
    AGENT,
    PRODUCT,
    PURCHAED,
    ENTRANCE,
    EXIT,
    TWOWAY
};
enum class Direction
{
    N,
    E,
    S,
    W
};

#endif //ENUMCLASSES_H_

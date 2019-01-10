#ifndef CROWDLIB_ENUMCLASSES_H_
#define CROWDLIB_ENUMCLASSES_H_

enum class AgentState
{
    FOLLOW,
    ATTACKFRENZY,
    CHARGE,
    FLEE,
    GIVEUP,
    BUY,
    FAILED,
    SUCCESS,
    IDLE
};

enum class MoveType
{
    PATH,
    OUT,
    TARGET,
    CUSTOM
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

#endif //CROWDLIB_ENUMCLASSES_H_

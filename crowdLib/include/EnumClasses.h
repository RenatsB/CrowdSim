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
    CURRENT,
    N,
    NE,
    E,
    SE,
    S,
    SW,
    W,
    NW
};

#endif //CROWDLIB_ENUMCLASSES_H_

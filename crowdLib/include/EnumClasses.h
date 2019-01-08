#ifndef CROWDLIB_ENUMCLASSES_H_
#define CROWDLIB_ENUMCLASSES_H_

enum class AgentState
{
    FOLLOW,
    ATTACKFRENZY,
    CHARGE,
    FLEE,
    GIVEUP,
    EXIT,
    ENTER,
    WALKAWAY,
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

#endif //CROWDLIB_ENUMCLASSES_H_

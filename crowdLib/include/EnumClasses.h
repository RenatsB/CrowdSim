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
    WALL,
    ENTRANCE,
    EXIT
};

#endif //ENUMCLASSES_H_
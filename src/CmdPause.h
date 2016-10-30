#include "Event.h"

class CmdPause : public Event
{
    public:
        CmdPause()
            : Event(CMD_PAUSE)
        {
        }
};

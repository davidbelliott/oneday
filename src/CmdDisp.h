#include "Event.h"

class CmdDisp : public Event
{
    public:
        std::string str;

        CmdDisp(std::string str_in)
        :   Event(CMD_DISP),
            str(str_in)
        {
        }
};

#include "Char.h"
#include "Terminal.h"

Char::Char(char c_in, int spread)
    : x((spread == 0 ? 0 : rand() % spread - spread / 2)),
      y((spread == 0 ? 0 : rand() % spread - spread / 2)),
      c(c_in),
      lerp_coefficient(0.1),
      snap_threshold(0.5),
      fg(config::colors[config::BASE_6]),
      bg(config::colors[config::BASE_0])
{
    /*if(cur_x > 49)
        cur_x += config::screen_w_chars;
    else
        cur_x -= config::screen_w_chars;
    if(cur_y > 49)
        cur_y += config::screen_h_chars;
    else
        cur_y -= config::screen_h_chars;*/
}

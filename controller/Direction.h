#ifndef _DIRECTION_H_
#define _DIRECTION_H_

namespace Driver {
  enum Movement {
    Front,
    Back,
    Left,
    Right,
    Up,
    Down,
    RotateLeft,
    RotateRight,
    Rest, // stays in position
    Nothing // motors aren't running
  }
}

#endif
#pragma once

#include "cocos2d.h"

namespace blocks {

class BrickInfo {
public:
  int gameX;
  int gameY;

  cocos2d::Node* node;
};

enum MoveType : int { MT_Down = 0, MT_Left, MT_Right };

}

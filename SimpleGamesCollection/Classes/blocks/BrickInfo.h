#pragma once

#include "cocos2d.h"

namespace blocks {

class BrickInfo {
public:
  int gameX;
  int gameY;

  cocos2d::Node* node;
  std::string fileName;
};

enum MoveType : int { MT_Down = 0, MT_Left, MT_Right, MT_Rotate, kDrop, kDown = MT_Down };

enum FigureType : int {
  kQuad = 0,
  kZR1, kZR2,
  kZMirroredR1,  kZMirroredR2,
  kTR1,  kTR2, kTR3,  kTR4,
  kLR1,  kLR2, kLR3,  kLR4,
  kLMirroredR1,  kLMirroredR2, kLMirroredR3,  kLMirroredR4,
  kIR1, kIR2
};

}

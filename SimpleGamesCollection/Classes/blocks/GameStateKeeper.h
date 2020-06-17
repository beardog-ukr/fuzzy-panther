#pragma once

#include <memory>
#include <list>

#include "cocos2d.h"
#include "SixCatsLoggerLoggable.h"

namespace blocks {

class GameStateKeeper : virtual public SixCatsLoggerLoggable {
public:
  GameStateKeeper();
  virtual ~GameStateKeeper();






protected:
  int wallWidth;
  int wallHeight;
  int* currentMap;
};

}


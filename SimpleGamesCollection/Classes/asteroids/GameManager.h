#pragma once

#include "cocos2d.h"
#include "SixCatsLoggerLoggable.h"

namespace asteroids {

class AsteroidNode;

class GameManager final : virtual public SixCatsLoggerLoggable {
public:
  GameManager();
  ~GameManager();

  // returns nullptr if init failed
  cocos2d::Node* prepareGameNode();

  void startGame();

protected:

  bool initAsteroids();
  bool initGameNode();

  cocos2d::Node* gameNode;
  AsteroidNode* asteroidNode;
};

};
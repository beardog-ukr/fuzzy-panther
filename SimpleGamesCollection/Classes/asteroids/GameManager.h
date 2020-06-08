#pragma once

#include "cocos2d.h"
#include "SixCatsLoggerLoggable.h"

namespace asteroids {

class AsteroidNode;
class SpaceshipNode;

class GameManager final : virtual public SixCatsLoggerLoggable {
public:
  GameManager();
  ~GameManager();

  // returns nullptr if init failed
  cocos2d::Node* prepareGameNode();

  void startGame();

  // returns true if key code was processed, false if it should be processed somewhere else
  bool processKeyCode(const cocos2d::EventKeyboard::KeyCode keyCode);

  bool processContact(cocos2d::PhysicsContact& contact);

protected:

  bool initAsteroids();
  bool initGameNode();
  bool initSpaceship();

  void addLaser();
  void addSplashAt(const cocos2d::Vec2& pointA, const cocos2d::Vec2& pointB);
  void rotateSpaceship(const cocos2d::EventKeyboard::KeyCode keyCode);

  cocos2d::Node* gameNode;
  AsteroidNode* asteroidNode;
  SpaceshipNode* spaceshipNode;
};

};
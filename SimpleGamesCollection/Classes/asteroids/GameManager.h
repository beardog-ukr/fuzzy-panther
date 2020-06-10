#pragma once

#include <map>

#include "cocos2d.h"
#include "SixCatsLoggerLoggable.h"

namespace asteroids {

class AsteroidNode;
enum RockType : unsigned int;
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

  void processGameOver(const bool victory);
  void processLaserVsAsteroid(cocos2d::Node*const laserNode, cocos2d::Node*const asteroidNode );
  void addTwoAsteroids(const double currentAngle, const cocos2d::Vec2& position,
                       const RockType type);

  bool evaluateContactAgents(cocos2d::Node * nodeA, cocos2d::Node * nodeB,
                             cocos2d::Node*& laserAgent, cocos2d::Node*& asteroidAgent,
                             cocos2d::Node*& shipAgent);

  void processGameOverStage1(cocos2d::Node* asteroidAgent);

  cocos2d::Node* gameNode;
  std::map <std::string, AsteroidNode*> asteroidNodes;
  SpaceshipNode* spaceshipNode;
};

};
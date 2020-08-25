#pragma once

#include "cocos2d.h"

#include "SixCatsLoggerLoggable.h"

namespace sokoban {

class ActorNode : public cocos2d::Sprite, virtual public SixCatsLoggerLoggable {
public:
  static ActorNode* create(const float iterationDuration, std::shared_ptr<SixCatsLogger> c6);

  void setGamePosition(const int gameX, const int gameY);
  void doChangePositionTo(const int gameX, const int gameY);

  int getValue() const;

  static void unloadAnimations();

protected:
  ActorNode(const float iterationDuration);
  virtual ~ActorNode();

  bool initSelf();

  int gameX;
  int gameY;

  float iterationDuration;

  cocos2d::Vec2 calculatePosition(const int x, const int y) const;
};

}

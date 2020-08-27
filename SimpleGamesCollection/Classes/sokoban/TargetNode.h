#pragma once

#include "cocos2d.h"

#include "SixCatsLoggerLoggable.h"

namespace sokoban {

class TargetNode : public cocos2d::Sprite, virtual public SixCatsLoggerLoggable {
public:
  static TargetNode* create(std::shared_ptr<SixCatsLogger> c6);

  void setGamePosition(const int gameX, const int gameY);
  void setFilled(const bool filled);

protected:
  TargetNode();
  virtual ~TargetNode();

  bool initSelf();

  int gameX;
  int gameY;

  bool currentFilledState;

  cocos2d::Vec2 calculatePosition(const int x, const int y) const;
};

}

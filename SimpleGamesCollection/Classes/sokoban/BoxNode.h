#pragma once

#include "cocos2d.h"

#include "SixCatsLoggerLoggable.h"

namespace sokoban {

class BoxNode : public cocos2d::Sprite, virtual public SixCatsLoggerLoggable {
public:
  static BoxNode* create(std::shared_ptr<SixCatsLogger> c6);

  void setGamePosition(const int gameX, const int gameY);
  void doChangePositionTo(const int gameX, const int gameY);

  void doAlignAgainst(const int gameX, const int gameY);

protected:
  BoxNode();
  virtual ~BoxNode();

  bool initSelf();

  int gameX;
  int gameY;

  cocos2d::Vec2 calculatePosition(const int x, const int y) const;
};

}

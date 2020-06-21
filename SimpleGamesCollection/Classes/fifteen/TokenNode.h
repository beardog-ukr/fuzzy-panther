#pragma once

#include "cocos2d.h"

#include "SixCatsLoggerLoggable.h"

namespace fifteen {

class TokenNode : public cocos2d::Sprite, virtual public SixCatsLoggerLoggable {
public:
  static TokenNode* create(const int number, std::shared_ptr<SixCatsLogger> c6);

  void setGamePosition(const int gameX, const int gameY);
  void doChangePositionTo(const int gameX, const int gameY);

  int getValue() const;

protected:
  TokenNode(const int value);
  virtual ~TokenNode();

  bool initSelf();
  bool initLabelNumber();

  int value;
  int gameX;
  int gameY;

  float tileSize;

  cocos2d::Vec2 calculatePosition(const int x, const int y) const;

  cocos2d::Label* labelNumber;
};

}
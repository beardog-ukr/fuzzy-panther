#pragma once

#include "cocos2d.h"

#include "SixCatsLoggerLoggable.h"

namespace repeat {

class DigitNode : public cocos2d::Sprite, virtual public SixCatsLoggerLoggable {
public:
  static DigitNode* create(const int number, std::shared_ptr<SixCatsLogger> c6);

  void showSelf(const float duration);

  static const std::string kFireRingAnimationName;

protected:
  DigitNode(const int value);
  virtual ~DigitNode();

  bool initSelf();
  bool initLabelNumber();

  int value;

  cocos2d::Sprite* labelNumber;
};

}

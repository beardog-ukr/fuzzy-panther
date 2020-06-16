#pragma once

#include "cocos2d.h"

#include "SixCatsLoggerLoggable.h"

namespace blackjack {

class BlackboardNode : public cocos2d::Node,
  virtual public SixCatsLoggerLoggable
{
public:
  static BlackboardNode* create(std::shared_ptr<SixCatsLogger> c6);

  void setDealerCount(const int count);
  void setPlayerCount(const int count);

  void showGameResult(const int dealerCount, const int playerCount);

  void reset();

protected:
  BlackboardNode();
  virtual ~BlackboardNode();

  bool initSelf();
  bool initBoardImage();
  bool initLabels();

  bool showDealerCount;

  cocos2d::Label* dealerLabel;
  cocos2d::Label* playerLabel;

  cocos2d::Label* gameResultLabel;
};

}
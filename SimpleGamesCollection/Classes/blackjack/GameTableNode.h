#pragma once

#include "cocos2d.h"
#include <vector>
#include <list>

#include "SixCatsLoggerLoggable.h"

namespace blackjack {

class GameTableNode : public cocos2d::Node,
  virtual public SixCatsLoggerLoggable {
public:
  static GameTableNode* create(const cocos2d::Size& expectedSize,
                               std::shared_ptr<SixCatsLogger> c6);

  float dealToDealer(const std::string& cardName);
  float dealToPlayer(const std::string& cardName);

  float revealFirst();

  void reset();

protected:
  GameTableNode();
  virtual ~GameTableNode();

  bool initSelf(const cocos2d::Size& expectedSize);
  bool initScaleFactor();

  void calculateCardsPositions(std::vector<float>& positions,
                               const float yPosition,const int maxCount) const;
  void repositionCards(std::list<cocos2d::Node*>& cards, std::vector<float>& positions,
                       const float yPos);

  int dealerCardsCount;
  std::list<Node*> dealerCards;
  std::string hiddenCardName;
  float dealerRowPosition;
  std::vector<float> dealerCardPositions;

  int playerCardsCount;
  std::list<Node*> playerCards;
  float playerRowPosition;
  std::vector<float> playerCardPositions;

  cocos2d::Vec2 cardFlyStartPosition;

  cocos2d::Node* createCard(const std::string& cardName);

  float cardScaleFactor;
};

}
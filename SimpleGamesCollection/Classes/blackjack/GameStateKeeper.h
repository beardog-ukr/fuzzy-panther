#pragma once
#pragma once

#include "cocos2d.h"
#include <vector>
#include <list>

#include "SixCatsLoggerLoggable.h"

namespace blackjack {

class GameStateKeeper : virtual public SixCatsLoggerLoggable {
public:
  GameStateKeeper();
  virtual ~GameStateKeeper();

  std::list<std::string> prepareInitialDeal();
  std::string hit();
  bool playerGotBust() const;

  int getDealerCount() const;
  int getPlayerCount() const;

  std::list<std::string> prepareFullDealer();

  static bool loadCardsToCache();
  static void unloadCardsFromCache();

  void reset();

protected:
  int getNextRandom();
  std::list<int> currentRandomSeq;

  int getTotal(const std::list<int>& cards) const;

  std::list<int> dealerCards;
  std::list<int> playerCards;

  void initRandomSeq();



};

}
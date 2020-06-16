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

  int getDealerCount() const;
  int getPlayerCount() const;

  static bool loadCardsToCache();
  static void unloadCardsFromCache();

protected:
  int getNextRandom();
  std::list<int> currentRandomSeq;

  std::list<int> dealerCards;
  std::list<int> playerCards;

  void initRandomSeq();



};

}
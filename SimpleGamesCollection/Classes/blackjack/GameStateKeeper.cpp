#include "blackjack/GameStateKeeper.h"
using namespace blackjack;

#include "SixCatsLogger.h"
#include "SixCatsLoggerMacro.h"
#include <sstream>


USING_NS_CC;
using namespace std;


enum CardType {
  CT_clubs_ace,
  CT_clubs_2,
  CT_clubs_3,
  CT_clubs_4,
  CT_clubs_5,
  CT_clubs_6
};

static const string cardNames[] = {
  "Clubs/AofClubs.png",
  "Clubs/2ofClubs.png",
  "Clubs/3ofClubs.png",
  "Clubs/4ofClubs.png",
  "Clubs/5ofClubs.png",
  "Clubs/6ofClubs.png"
};

static const string clubsImagesPlistFN = "blackjack/cards_clubs.plist";

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

GameStateKeeper::GameStateKeeper() {
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

GameStateKeeper::~GameStateKeeper() {
  C6_D1(c6, "here");
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

int GameStateKeeper::getDealerCount() const {
  return 5;
}
// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

int GameStateKeeper::getPlayerCount() const {
  return 10;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool GameStateKeeper::loadCardsToCache() {
  SpriteFrameCache* const sfc = SpriteFrameCache::getInstance();

  sfc->addSpriteFramesWithFile(clubsImagesPlistFN);
  if (!sfc->isSpriteFramesWithFileLoaded(clubsImagesPlistFN)) {
    return false;
  }

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

std::list<std::string> GameStateKeeper::prepareInitialDeal() {
  list<string> result;

  dealerCards.clear();
  playerCards.clear();

  int orv; //one random value
  orv = getNextRandom();
  dealerCards.push_back(orv);
  result.push_back(cardNames[orv]);

  orv = getNextRandom();
  dealerCards.push_back(orv);
  result.push_back(cardNames[orv]);

  orv = getNextRandom();
  playerCards.push_back(orv);
  result.push_back(cardNames[orv]);

  orv = getNextRandom();
  playerCards.push_back(orv);
  result.push_back(cardNames[orv]);

  return result;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void GameStateKeeper::unloadCardsFromCache() {
  SpriteFrameCache::getInstance()->removeSpriteFramesFromFile(clubsImagesPlistFN);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void GameStateKeeper::initRandomSeq() {
  currentRandomSeq.clear();

  currentRandomSeq.push_back(CT_clubs_ace);
  currentRandomSeq.push_back(CT_clubs_2);
  currentRandomSeq.push_back(CT_clubs_3);
  currentRandomSeq.push_back(CT_clubs_4);
  currentRandomSeq.push_back(CT_clubs_5);
  currentRandomSeq.push_back(CT_clubs_6);
  //
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

int GameStateKeeper::getNextRandom() {
  if (currentRandomSeq.empty()) {
    initRandomSeq();
    if (currentRandomSeq.empty()) {
      C6_C1(c6, "internal error");
      return 0;
    }
  }

  const int result = currentRandomSeq.front();
  currentRandomSeq.pop_front();

  return result;
}


// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
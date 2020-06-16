#include "blackjack/GameStateKeeper.h"
using namespace blackjack;

#include "SixCatsLogger.h"
#include "SixCatsLoggerMacro.h"
#include <sstream>


USING_NS_CC;
using namespace std;


enum CardType {
  CT_clubs_ace =0,
  CT_clubs_2,
  CT_clubs_3,
  CT_clubs_4,
  CT_clubs_5,
  CT_clubs_6,
  CT_clubs_7,
  CT_clubs_8,
  CT_clubs_9,
  CT_clubs_10,
  CT_clubs_j,
  CT_clubs_q,
  CT_clubs_k, //12
  //
  CT_diamonds_ace,
  CT_diamonds_2,
  CT_diamonds_3,
  CT_diamonds_4,
  CT_diamonds_5,
  CT_diamonds_6,
  CT_diamonds_7,
  CT_diamonds_8,
  CT_diamonds_9,
  CT_diamonds_10,
  CT_diamonds_j,
  CT_diamonds_q,
  CT_diamonds_k, //25

  CT_hearts_ace,
  CT_hearts_2,
  CT_hearts_3,
  CT_hearts_4,
  CT_hearts_5,
  CT_hearts_6,
  CT_hearts_7,
  CT_hearts_8,
  CT_hearts_9,
  CT_hearts_10,
  CT_hearts_j,
  CT_hearts_q,
  CT_hearts_k,

  CT_spades_ace,
  CT_spades_2,
  CT_spades_3,
  CT_spades_4,
  CT_spades_5,
  CT_spades_6,
  CT_spades_7,
  CT_spades_8,
  CT_spades_9,
  CT_spades_10,
  CT_spades_j,
  CT_spades_q,
  CT_spades_k,

  CT_cards_count
};

static const string cardNames[CT_cards_count] = {
  "blackjack/Clubs/AofClubs.png",
  "blackjack/Clubs/2ofClubs.png",
  "blackjack/Clubs/3ofClubs.png",
  "blackjack/Clubs/4ofClubs.png",

  "blackjack/Clubs/5ofClubs.png",
  "blackjack/Clubs/6ofClubs.png",
  "blackjack/Clubs/7ofClubs.png",
  "blackjack/Clubs/8ofClubs.png",

  "blackjack/Clubs/9ofClubs.png",
  "blackjack/Clubs/10ofClubs.png",
  "blackjack/Clubs/JofClubs.png",
  "blackjack/Clubs/QofClubs.png",
  "blackjack/Clubs/KofClubs.png", //12
  // Diamonds
  "blackjack/Dmnds/AofDmnds.png",
  "blackjack/Dmnds/2ofDmnds.png",
  "blackjack/Dmnds/3ofDmnds.png",
  "blackjack/Dmnds/4ofDmnds.png",

  "blackjack/Dmnds/5ofDmnds.png",
  "blackjack/Dmnds/6ofDmnds.png",
  "blackjack/Dmnds/7ofDmnds.png",
  "blackjack/Dmnds/8ofDmnds.png",

  "blackjack/Dmnds/9ofDmnds.png",
  "blackjack/Dmnds/10ofDmnds.png",
  "blackjack/Dmnds/JofDmnds.png",
  "blackjack/Dmnds/QofDmnds.png",
  "blackjack/Dmnds/KofDmnds.png", //25
  //
  "blackjack/Hearts/AofHearts.png",
  "blackjack/Hearts/2ofHearts.png",
  "blackjack/Hearts/3ofHearts.png",
  "blackjack/Hearts/4ofHearts.png",

  "blackjack/Hearts/5ofHearts.png",
  "blackjack/Hearts/6ofHearts.png",
  "blackjack/Hearts/7ofHearts.png",
  "blackjack/Hearts/8ofHearts.png",

  "blackjack/Hearts/9ofHearts.png",
  "blackjack/Hearts/10ofHearts.png",
  "blackjack/Hearts/JofHearts.png",
  "blackjack/Hearts/QofHearts.png",
  "blackjack/Hearts/KofHearts.png", //38
  //
  "blackjack/Spades/AofSpades.png",
  "blackjack/Spades/2ofSpades.png",
  "blackjack/Spades/3ofSpades.png",
  "blackjack/Spades/4ofSpades.png",

  "blackjack/Spades/5ofSpades.png",
  "blackjack/Spades/6ofSpades.png",
  "blackjack/Spades/7ofSpades.png",
  "blackjack/Spades/8ofSpades.png",

  "blackjack/Spades/9ofSpades.png",
  "blackjack/Spades/10ofSpades.png",
  "blackjack/Spades/JofSpades.png",
  "blackjack/Spades/QofSpades.png",
  "blackjack/Spades/KofSpades.png"
};

static const int cardValues[CT_cards_count] = {
  0,2,3,4, 5,6,7,8, 9,10, 10,10,10, //0 for aces, also there are some special rules
  0,2,3,4, 5,6,7,8, 9,10, 10,10,10,
  0,2,3,4, 5,6,7,8, 9,10, 10,10,10,
  0,2,3,4, 5,6,7,8, 9,10, 10,10,10
};

static const string clubsImagesPlistFN =    "blackjack/cards/cards_clubs.plist";
static const string diamondsImagesPlistFN = "blackjack/cards/cards_diamonds.plist";
static const string heartsImagesPlistFN =   "blackjack/cards/cards_hearts.plist";
static const string spadesImagesPlistFN =   "blackjack/cards/cards_spades.plist";

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

GameStateKeeper::GameStateKeeper() {
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

GameStateKeeper::~GameStateKeeper() {
  C6_D1(c6, "here");
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

int GameStateKeeper::getDealerCount() const {
  return getTotal(dealerCards);
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

int GameStateKeeper::getPlayerCount() const {
  return getTotal(playerCards);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

int GameStateKeeper::getTotal(const std::list<int>& cards) const {
  // C6_D1(c6, "here");
  int acesCount = 0; //amount of aces in the hand
  int total = 0;
  for(const int cardType: cards) {
    C6_F4(c6, "adding ", cardValues[cardType], " for ", cardType);

    total = total + cardValues[cardType];
    if (cardValues[cardType] == 0) {
      acesCount++;
    }
  }

  // " aces have a value of 11 unless this would make the total value of the hand go above 21,
  // in which case they have a value of 1."
  for (int i = 0; i< acesCount; i++ ) {
    int tmpsum = total + 11;
    if (tmpsum > 21) {
      tmpsum = total + 1;
    }
    total = tmpsum;
  }

  C6_F2(c6, "result total ", total);
  return total;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

string GameStateKeeper::hit() {
  int cardCode = getNextRandom();
  playerCards.push_back(cardCode);
  return cardNames[cardCode];
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void GameStateKeeper::initRandomSeq() {
  currentRandomSeq.clear();

  bool taken[CT_cards_count];
  for (int i = 0; i< CT_cards_count; i++) {
    taken[i] = false;
  }

  ostringstream ss;
  for (int shuffleCount = 0; shuffleCount< CT_cards_count; shuffleCount++) {
    int pos = RandomHelper::random_int((int)0, (int)(CT_cards_count-1));

    int wdi = 0;
    while (taken[pos]==true) {
      pos++;
      if (pos>= CT_cards_count) {
        pos = 0;
      }

      if (wdi > 100) {
        C6_D2(c6, "Emergency exit: ", pos);
        break;
      }
    }

    taken[pos] = true;
    currentRandomSeq.push_back(pos);

    ss << pos << " ";
  }

  C6_D2(c6, "Random seq is: ", ss.str());

  // C6_D4(c6, "Card #", CT_spades_2, " is ", cardNames[CT_spades_2]);
  // C6_D4(c6, "Card #", 38, " is ", cardNames[38]);
  // C6_D4(c6, "Card #", 22, " is ", cardNames[22]);
  // C6_D4(c6, "Card #", 25, " is ", cardNames[25]);

  // for (int i = 0; i< CT_cards_count; i++) {
  //   C6_D6(c6, "Card #", i, " is ", cardNames[i], " value ", cardValues[i]);
  // }

  // currentRandomSeq.push_back(CT_clubs_5);
  // currentRandomSeq.push_back(CT_clubs_2);
  // currentRandomSeq.push_back(CT_clubs_ace);
  // currentRandomSeq.push_back(CT_diamonds_4);
  // currentRandomSeq.push_back(CT_spades_10);
  // currentRandomSeq.push_back(CT_hearts_6);

  // currentRandomSeq.push_back(CT_clubs_5);
  // currentRandomSeq.push_back(CT_clubs_6);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool GameStateKeeper::loadCardsToCache() {
  SpriteFrameCache* const sfc = SpriteFrameCache::getInstance();

  sfc->addSpriteFramesWithFile(clubsImagesPlistFN);
  if (!sfc->isSpriteFramesWithFileLoaded(clubsImagesPlistFN)) {
    // C6_D2(c6, "Failed to load: ", clubsImagesPlistFN);
    return false;
  }

  sfc->addSpriteFramesWithFile(diamondsImagesPlistFN);
  if (!sfc->isSpriteFramesWithFileLoaded(diamondsImagesPlistFN)) {
    // C6_D2(c6, "Failed to load: ", diamondsImagesPlistFN);
    return false;
  }

  sfc->addSpriteFramesWithFile(heartsImagesPlistFN);
  if (!sfc->isSpriteFramesWithFileLoaded(heartsImagesPlistFN)) {
    // C6_D2(c6, "Failed to load: ", heartsImagesPlistFN);
    return false;
  }

  sfc->addSpriteFramesWithFile(spadesImagesPlistFN);
  if (!sfc->isSpriteFramesWithFileLoaded(spadesImagesPlistFN)) {
    // C6_D2(c6, "Failed to load: ", spadesImagesPlistFN);
    return false;
  }

  // C6_D1(c6, "all good");
  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool GameStateKeeper::playerGotBust() const {
  return getTotal(playerCards) >21;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

list<string> GameStateKeeper::prepareFullDealer() {
  C6_D2(c6, "here ", getTotal(dealerCards));
  list<string> result;
  while (getTotal(dealerCards) < 17 ) {

    int orv = getNextRandom();
    C6_D2(c6, "Adding to 17: ", orv);
    result.push_back(cardNames[orv]);
    dealerCards.push_back(orv);
  }

  return result;
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

void GameStateKeeper::reset() {
  dealerCards.clear();
  playerCards.clear();
  currentRandomSeq.clear();
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void GameStateKeeper::unloadCardsFromCache() {
  SpriteFrameCache::getInstance()->removeSpriteFramesFromFile(clubsImagesPlistFN);
  SpriteFrameCache::getInstance()->removeSpriteFramesFromFile(diamondsImagesPlistFN);
  SpriteFrameCache::getInstance()->removeSpriteFramesFromFile(heartsImagesPlistFN);
  SpriteFrameCache::getInstance()->removeSpriteFramesFromFile(spadesImagesPlistFN);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#include "blackjack/GameTableNode.h"
using namespace blackjack;

#include "SixCatsLogger.h"
#include "SixCatsLoggerMacro.h"
#include <sstream>

#include <cmath> // round
//#include <cstdlib> //div

USING_NS_CC;
using namespace std;


static const float cardFlyDuration = 1.0;

static const string closedCardFrameName = "blackjack/CardBack1.png";

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

GameTableNode::GameTableNode() : cardScaleFactor(1.0) {
  dealerCardsCount = 0;
  playerCardsCount = 0;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

GameTableNode::~GameTableNode() {
  C6_D1(c6, "here");
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

GameTableNode* GameTableNode::create(const cocos2d::Size& expectedSize,
                                     shared_ptr<SixCatsLogger> c6) {

  GameTableNode *pRet = new(std::nothrow) GameTableNode();
  if (pRet ==nullptr) {
    return nullptr;
  }

  pRet->setLogger(c6);

  if (!pRet->initSelf(expectedSize)) {
    delete pRet;
    pRet = nullptr;
    return nullptr;
  }

  pRet->autorelease();
  return pRet;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void GameTableNode::calculateCardsPositions(vector<float>& positions, const float yPosition,
                                            const int maxCount) const {
  positions.resize(maxCount);

  const Size cs = getContentSize();
  const float xStep = (cs.width*0.9)/maxCount;

  int xAcc = xStep/2 + cs.width*0.05;
  for (int i = 0; i<maxCount; i++) {
    positions[i] = xAcc;
    xAcc += xStep;
  }
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void GameTableNode::repositionCards(list<Node*>& cards,
                                    vector<float>& positions, const float yPosition) {
  int i = 0;
  for(const auto cn:cards) {
    if (i>= positions.size()) {
      break;
    }

    cn->setPosition(positions[i], yPosition);
    i++;
  }
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

float GameTableNode::dealToDealer(const std::string& cardName) {
  string correctedCardName = cardName;
  if (dealerCardsCount==0) {
    correctedCardName = closedCardFrameName;
    hiddenCardName = cardName;
  }

  dealerCardsCount++;
  if (dealerCardsCount>dealerCardPositions.size()) {
    C6_D2(c6, "Need to reevaluate dealers positions: ", dealerCardsCount);
    calculateCardsPositions(dealerCardPositions, dealerRowPosition, dealerCardPositions.size() +4);
    repositionCards(dealerCards, dealerCardPositions, dealerRowPosition);
  }

  Node* newCard = createCard(correctedCardName);
  if (newCard==nullptr) {
    return 0;
  }
  newCard->setPosition(cardFlyStartPosition);

  const Vec2 newCardPos = {.x = dealerCardPositions[dealerCardsCount-1], .y = dealerRowPosition};
  MoveTo* mta = MoveTo::create(cardFlyDuration,newCardPos);
  newCard->runAction(mta);

  dealerCards.push_back(newCard);

  return cardFlyDuration;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

float GameTableNode::dealToPlayer(const std::string& cardName) {
  playerCardsCount++;
  if (playerCardsCount>playerCardPositions.size()) {
    C6_D2(c6, "Need to reevaluate players positions: ", playerCardsCount);
    calculateCardsPositions(playerCardPositions, playerRowPosition, playerCardPositions.size() +4);
    repositionCards(playerCards, playerCardPositions, playerRowPosition);
  }

  // const Vec2 newCardPos = {.x = playerCardPositions[playerCardsCount-1], .y = playerRowPosition};
  // Node* ncn = addCard(newCardPos);
  // if (ncn != nullptr) {
  //   playerCards.push_back(ncn);
  // }
  Node* newCard = createCard(cardName);
  if (newCard==nullptr) {
    return 0;
  }

  newCard->setPosition(cardFlyStartPosition);

  const Vec2 newCardPos = {.x = playerCardPositions[playerCardsCount-1],  .y = playerRowPosition};
  MoveTo* mta = MoveTo::create(cardFlyDuration, newCardPos);
  newCard->runAction(mta);


  // Node* ncn = addCard(newCardPos);
  // if (ncn != nullptr) {
  playerCards.push_back(newCard);

  return cardFlyDuration;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool GameTableNode::initSelf(const cocos2d::Size& expectedSize) {
  setContentSize(expectedSize);

  dealerRowPosition = (expectedSize.height/4)*3;
  calculateCardsPositions(dealerCardPositions, dealerRowPosition, 4);

  playerRowPosition = (expectedSize.height/4);
  calculateCardsPositions(playerCardPositions, playerRowPosition, 4);

  cardFlyStartPosition.x = expectedSize.width/2;
  cardFlyStartPosition.y = expectedSize.height*1.6;

  if (!initScaleFactor()) {
    return false;
  }

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool GameTableNode::initScaleFactor() {
  // const char fn[] = "blackjack/CardBack1.png";
  Sprite* sprite = Sprite::createWithSpriteFrameName(closedCardFrameName);
  // Sprite* sprite;
  // sprite = Sprite::create(fn);
  if (sprite == nullptr) {
    C6_C2(c6, "Error while loading: ", closedCardFrameName);
    return false;
  }

  const Size currentSize = getContentSize();
  const Size spriteSize = sprite->getContentSize();
  //card takes 45% of widget heidgt
  cardScaleFactor = (currentSize.height*0.45) / spriteSize.height;

  //sprite has no parent and will get deleted

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

Node* GameTableNode::createCard(const string& cardName) {
  // const char fn1[] = "blackjack/2ofClubs.png";
  // baseSprite = Sprite::createWithSpriteFrameName(frameName);

  //const char frameName[] = "Clubs/10ofClubs.png";
  Sprite* sprite = Sprite::createWithSpriteFrameName(cardName);

  // Sprite* sprite;
  // sprite = Sprite::create(fn1);
  if (sprite == nullptr) {
    C6_C2(c6, "Error while loading: ", cardName);
    return nullptr;
  }

  sprite->setScale(cardScaleFactor);

  addChild(sprite);

  return sprite;

}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void GameTableNode::reset() {
  dealerCardsCount = 0;
  hiddenCardName = "";
  dealerCards.clear();
  dealerCardPositions.resize(0);
  calculateCardsPositions(dealerCardPositions, dealerRowPosition, 4);

  playerCardsCount = 0;
  playerCards.clear();
  playerCardPositions.resize(0);
  calculateCardsPositions(playerCardPositions, playerRowPosition, 4);

  removeAllChildrenWithCleanup(true);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .


float GameTableNode::revealFirst() {
  Node* revealedCard = createCard(hiddenCardName);
  if (revealedCard == nullptr) {
    return cardFlyDuration;
  }
  revealedCard->setPosition(cardFlyStartPosition);
  const Vec2 rcp = {.x = dealerCardPositions[0], .y = dealerRowPosition};
  MoveTo* revealAction = MoveTo::create(cardFlyDuration/2, rcp);
  revealedCard->runAction(revealAction);

  Node* hiddenCard = dealerCards.front();
  dealerCards.pop_front();
  dealerCards.push_front(revealedCard);

  MoveTo* hideAction = MoveTo::create(cardFlyDuration/2, cardFlyStartPosition);
  Sequence* hideSeq = Sequence::create(hideAction, RemoveSelf::create(), nullptr);
  hiddenCard->runAction(hideSeq);

  return cardFlyDuration;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
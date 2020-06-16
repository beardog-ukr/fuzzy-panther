#include "blackjack/BlackboardNode.h"
using namespace blackjack;

#include "SixCatsLogger.h"
#include "SixCatsLoggerMacro.h"
#include <sstream>

#include <cmath> // round
//#include <cstdlib> //div

USING_NS_CC;
using namespace std;


// static const float rotationDurarionOneStep = 2;
// static const float rotationValueOneStep = 85;
// static const float moveDurarionOneStep = 1.0;


// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

BlackboardNode::BlackboardNode() {
  // ntdh
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

BlackboardNode::~BlackboardNode() {
  C6_D1(c6, "here");
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

BlackboardNode* BlackboardNode::create(shared_ptr<SixCatsLogger> c6) {

  BlackboardNode *pRet = new(std::nothrow) BlackboardNode();
  if (pRet ==nullptr) {
    return nullptr;
  }

  pRet->setLogger(c6);

  if (!pRet->initSelf()) {
    delete pRet;
    pRet = nullptr;
    return nullptr;
  }

  pRet->autorelease();
  return pRet;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool BlackboardNode::initSelf() {

  if (!initBoardImage()) {
    return false;
  }

  if (!initLabels() ) {
    return false;
  }

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool BlackboardNode::initBoardImage() {
  // const char frameName[] = "asteroids/playerShip2_blue.png";
  const char fn1[] = "blackjack/board.png";
  Sprite* sprite;
  sprite = Sprite::create(fn1);
  if (sprite == nullptr) {
    C6_C2(c6, "Error while loading: ", fn1);
    return false;
  }

  // sprite->setScale(expectedSize.width/sprite->getContentSize().width);
  // sprite->setPosition(expectedSize.width/2,expectedSize.height/2);
  const Size spriteSize = sprite->getContentSize();
  setContentSize(spriteSize);
  sprite->setPosition(spriteSize.width/2,spriteSize.height/2);
  addChild(sprite);

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool BlackboardNode::initLabels() {
  const Vec2 dealerCountLabelPosition = {.x = 75, .y = 500};
  const Vec2 playerCountLabelPosition = {.x = 75, .y = 450};

  const string countLabelsFontFileName = "blackjack/fonts/vinizinho.ttf";

  dealerLabel = Label::createWithTTF("", countLabelsFontFileName, 48);
  dealerLabel->setTextColor(Color4B(250,250,250,255));
  dealerLabel->setAnchorPoint(Vec2(0,0));
  dealerLabel->setPosition(dealerCountLabelPosition);
  addChild(dealerLabel);
  setDealerCount(0);

  playerLabel = Label::createWithTTF("", countLabelsFontFileName, 48);
  playerLabel->setTextColor(Color4B(250,250,250,255));
  playerLabel->setAnchorPoint(Vec2(0,0));
  playerLabel->setPosition(playerCountLabelPosition);
  addChild(playerLabel);
  setPlayerCount(0);

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void BlackboardNode::setDealerCount(const int count) {
  if (dealerLabel == nullptr) {
    return;
  }

  ostringstream ss;
  ss << "Dealer total: " << count;
  dealerLabel->setString(ss.str());
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void BlackboardNode::setPlayerCount(const int count) {
  if (playerLabel == nullptr) {
    return;
  }

  ostringstream ss;
  ss << "Player total: " << count;
  playerLabel->setString(ss.str());
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

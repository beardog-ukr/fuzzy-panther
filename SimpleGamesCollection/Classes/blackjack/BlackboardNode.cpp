#include "blackjack/BlackboardNode.h"
using namespace blackjack;

#include "SixCatsLogger.h"
#include "SixCatsLoggerMacro.h"
#include <sstream>

#include <cmath> // round

USING_NS_CC;
using namespace std;

static const string dealerLabelText = "Dealer total: ";
static const string dealerLabelTextHidden = "Dealer total: ??";
static const string playerLabelText = "Player total: ";

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

BlackboardNode::BlackboardNode() {
  showDealerCount = false;
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
  const char fn1[] = "blackjack/board.png";
  Sprite* sprite;
  sprite = Sprite::create(fn1);
  if (sprite == nullptr) {
    C6_C2(c6, "Error while loading: ", fn1);
    return false;
  }

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

  dealerLabel = Label::createWithTTF(dealerLabelTextHidden, countLabelsFontFileName, 48);
  dealerLabel->setTextColor(Color4B(250,250,250,255));
  dealerLabel->setAnchorPoint(Vec2(0,0));
  dealerLabel->setPosition(dealerCountLabelPosition);
  addChild(dealerLabel);

  playerLabel = Label::createWithTTF(playerLabelText, countLabelsFontFileName, 48);
  playerLabel->setTextColor(Color4B(250,250,250,255));
  playerLabel->setAnchorPoint(Vec2(0,0));
  playerLabel->setPosition(playerCountLabelPosition);
  addChild(playerLabel);
  setPlayerCount(0);

  const string resultLabelFontFileName = "blackjack/fonts/vinivicious.ttf";

  gameResultLabel = Label::createWithTTF("", resultLabelFontFileName, 64);
  gameResultLabel->setTextColor(Color4B(250,250,250,255));
  gameResultLabel->setAnchorPoint(Vec2(0,0));
  gameResultLabel->setPosition({.x = 175, .y = 250});
  addChild(gameResultLabel);
  gameResultLabel->setVisible(false);


  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void BlackboardNode::reset() {
  gameResultLabel->setVisible(false);
  dealerLabel->setString(dealerLabelTextHidden);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void BlackboardNode::showGameResult(const int dealerCount, const int playerCount) {
  gameResultLabel->setVisible(true);

  if ((playerCount <= 21)&&
      ((dealerCount>21)||(playerCount > dealerCount))) {
    gameResultLabel->setString("Player wins");
  }
  else if ((dealerCount <= 21)&&
           ((playerCount>21)||(dealerCount > playerCount))) {
    gameResultLabel->setString("Dealer wins");
  }
  else {
    gameResultLabel->setString("DRAW");
  }
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void BlackboardNode::setDealerCount(const int count) {
  if (dealerLabel == nullptr) {
    return;
  }

  ostringstream ss;
  ss << dealerLabelText << count;
  dealerLabel->setString(ss.str());
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void BlackboardNode::setPlayerCount(const int count) {
  if (playerLabel == nullptr) {
    return;
  }

  ostringstream ss;
  ss << playerLabelText << count;
  playerLabel->setString(ss.str());
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

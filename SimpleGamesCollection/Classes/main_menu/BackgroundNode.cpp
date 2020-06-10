#include "BackgroundNode.h"

#include "SixCatsLogger.h"
#include "SixCatsLoggerMacro.h"
#include <sstream>

USING_NS_CC;
using namespace std;

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

BackgroundNode* BackgroundNode::create(const Size& expectedSize, const string& filename,
                                       shared_ptr<SixCatsLogger> c6) {

  BackgroundNode *pRet = new(std::nothrow) BackgroundNode();
  if (pRet ==nullptr) {
    return nullptr;
  }

  pRet->setLogger(c6);

  if (!pRet->init(expectedSize, filename)) {
    delete pRet;
    pRet = nullptr;
    return nullptr;
  }

  pRet->autorelease();
  return pRet;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

BackgroundNode::BackgroundNode() {
  // ntdh
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

BackgroundNode::~BackgroundNode() {
  //
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool BackgroundNode::init(const Size& expectedSize, const string& filename) {
  Sprite* baseSprite = Sprite::create(filename);
  if (baseSprite == nullptr) {
    C6_C2(c6, "Error while loading: ", filename);
    return false;
  }

  int heightFilled = 0;
  while (heightFilled < expectedSize.height) {
    int widthFilled = 0;
    while (widthFilled < expectedSize.width) {
      Sprite* sprite = Sprite::createWithSpriteFrame(baseSprite->getSpriteFrame());
      sprite->setAnchorPoint(Vec2(0,0));
      sprite->setPosition(widthFilled, heightFilled);

      C6_F4(c6, "Adding sprite at: ", widthFilled, ":", heightFilled);

      this->addChild(sprite);

      widthFilled += baseSprite->getContentSize().width;
    }

    C6_F2(c6, "Iteration: ", heightFilled);
    heightFilled += baseSprite->getContentSize().height;
  }

  return true;
}
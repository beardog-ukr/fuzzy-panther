#include "AsteroidsGameNode.h"

#include "SixCatsLogger.h"
#include "SixCatsLoggerMacro.h"
#include <sstream>

USING_NS_CC;
using namespace std;

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

AsteroidsGameNode* AsteroidsGameNode::create(shared_ptr<SixCatsLogger> c6) {

  AsteroidsGameNode *pRet = new(std::nothrow) AsteroidsGameNode();
  if (pRet ==nullptr) {
    return nullptr;
  }

  pRet->setLogger(c6);

  if (!pRet->init()) {
    delete pRet;
    pRet = nullptr;
    return nullptr;
  }

  pRet->autorelease();
  return pRet;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

AsteroidsGameNode::AsteroidsGameNode() {
  // ntdh
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

AsteroidsGameNode::~AsteroidsGameNode() {
  //
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool AsteroidsGameNode::init() {
  const char filename[] = "asteroids/space_background.jpg";
  Sprite* baseSprite = Sprite::create(filename);
  if (baseSprite == nullptr) {
    C6_C2(c6, "Error while loading: ", filename);
    return false;
  }

  addChild(baseSprite);

  return true;
}
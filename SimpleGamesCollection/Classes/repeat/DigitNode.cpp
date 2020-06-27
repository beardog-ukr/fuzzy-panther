#include "repeat/DigitNode.h"
using namespace repeat;

#include "SixCatsLogger.h"
#include "SixCatsLoggerMacro.h"
#include <sstream>

USING_NS_CC;
using namespace std;

const string DigitNode::kFireRingAnimationName = "fire_ring_splash";

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

DigitNode::DigitNode(const int v) {
  value = v;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

DigitNode::~DigitNode() {
  C6_F1(c6, "here");
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

DigitNode* DigitNode::create(const int number, shared_ptr<SixCatsLogger> c6) {

  DigitNode *pRet = new(std::nothrow) DigitNode(number);
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

bool DigitNode::initSelf() {
  const string fn = "repeat/fire_ring/fire_ring_splash_21.png";
  if (!initWithSpriteFrameName(fn)) {
    C6_C2(c6, "Failed to init with ", fn);
    return false;    //
  }

  if (!initLabelNumber()) {
    return false;
  }

  setVisible(false);

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool DigitNode::initLabelNumber() {
  ostringstream ss;
  ss << "repeat/number_" << value << ".png";

  const string fn = ss.str();
  labelNumber = Sprite::createWithSpriteFrameName(fn);
  if (labelNumber == nullptr) {
    C6_D2(c6, "Failed to add file: ", fn);
    return false;
  }

  const Size cs = getContentSize();
  labelNumber->setPosition(cs.width/2, cs.height/2);
  addChild(labelNumber);

  labelNumber->setOpacity(0);

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void DigitNode::showSelf(const float duration) {
  setVisible(true);

  FadeIn* fin = FadeIn::create(duration/2);
  FadeOut* fout = FadeOut::create(duration/2);

  labelNumber->runAction(Sequence::create(fin, fout, nullptr));

  Animation* animation = AnimationCache::getInstance()->getAnimation(kFireRingAnimationName);
  float tdu = animation->getTotalDelayUnits();
//  C6_D3(c6, "There are ", tdu, " delay units" );
  animation->setDelayPerUnit(duration/tdu);
  Animate* animate = Animate::create(animation);

  CallFunc *cf = CallFunc::create([this]() {
    this->setVisible(false);
  });

  runAction(Sequence::create(animate, cf, nullptr));
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .


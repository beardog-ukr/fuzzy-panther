#include "asteroids/SpaceshipNode.h"
using namespace asteroids;
#include "asteroids/ZOrderValues.h"

#include "SixCatsLogger.h"
#include "SixCatsLoggerMacro.h"
#include <sstream>

#include <cmath>

USING_NS_CC;
using namespace std;

enum SpaceshipNodeAnimationTags {
  SNAT_Rotation,
  SNAT_Move
};

static const float rotationDutarionOneStep = 1.0;

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

SpaceshipNode::SpaceshipNode() {
  // ntdh
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

SpaceshipNode::~SpaceshipNode() {
  C6_D1(c6, "here");
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

SpaceshipNode* SpaceshipNode::create(shared_ptr<SixCatsLogger> c6) {

  SpaceshipNode *pRet = new(std::nothrow) SpaceshipNode();
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

void SpaceshipNode::doRotate(const int changeValue) {

  stopAllActionsByTag(SNAT_Rotation);

  float currentValue = getRotation();

  float newValue = currentValue + changeValue;
  if (newValue>360) {
    newValue -= 360;
  }
  if (newValue <0) {
    newValue = newValue +360;
  }

  RotateTo* ra = RotateTo::create(rotationDutarionOneStep, newValue);
  ra->setTag(SNAT_Rotation);
  runAction(ra);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool SpaceshipNode::init() {
  const char filename[] = "asteroids/playerShip2_blue.png";
  baseSprite = Sprite::create(filename);
  if (baseSprite == nullptr) {
    C6_C2(c6, "Error while loading: ", filename);
    return false;
  }

  addChild(baseSprite, ZO_ss);

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void SpaceshipNode::rotateClockwice() {
  doRotate(10);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void SpaceshipNode::rotateCounterClockwice() {
  doRotate(-10);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
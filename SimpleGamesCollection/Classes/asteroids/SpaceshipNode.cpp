#include "asteroids/SpaceshipNode.h"
using namespace asteroids;
#include "asteroids/ZOrderValues.h"

#include "SixCatsLogger.h"
#include "SixCatsLoggerMacro.h"
#include <sstream>

#include <cmath>
static const double pi = acos(-1);

USING_NS_CC;
using namespace std;

enum SpaceshipNodeAnimationTags {
  SNAT_Rotation,
  SNAT_Move
};

static const float rotationDurarionOneStep = 0.5;
static const float moveDurarionOneStep = 1.0;

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
  else if (newValue <0) {
    newValue = newValue +360;
  }

  RotateTo* ra = RotateTo::create(rotationDurarionOneStep, newValue);
  ra->setTag(SNAT_Rotation);
  runAction(ra);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

double SpaceshipNode::getAngle() const {
  float result = getRotation();
  if (result>360) {
    result -= 360;
  }
  else if (result <0) {
    result = result +360;
  }

  return result;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool SpaceshipNode::init() {
  // const char frameName[] = "asteroids/playerShip2_blue.png";
  const char frameName[] = "ship_blue_2.png";
  baseSprite = Sprite::createWithSpriteFrameName(frameName);
  // baseSprite = Sprite::create(frameName);
  if (baseSprite == nullptr) {
    C6_C2(c6, "Error while loading: ", frameName);
    return false;
  }

  const int pointsCount = 12;
  const Vec2 pointsArr[pointsCount] = {
    {.x =   -56, .y =    -5  },
    {.x =   -14, .y =    18  },
    {.x =   -4, .y =    38  },
    {.x =   4, .y =    38  },
    {.x =   12, .y =    19  },
    {.x =   54, .y =    -5  },
    {.x =   36, .y =    -33 },
    {.x =   16, .y =    -29 },
    {.x =   11, .y =    -36 },
    {.x =   -13, .y =    -36 },
    {.x =   -17, .y =    -29 },
    {.x =   -37, .y =    -33 }
  };

  PhysicsBody* physicsBody = PhysicsBody::createPolygon(pointsArr, pointsCount,
                                                        PhysicsMaterial(0.1f, 1.0f, 0.0f));
  physicsBody->setDynamic(false);
  physicsBody->setCategoryBitmask(CM_ship);
  physicsBody->setCollisionBitmask(CM_asteroid);
  physicsBody->setContactTestBitmask(0xFFFFFFFF);

  baseSprite->addComponent(physicsBody);
  baseSprite->setTag(IT_ship);


  addChild(baseSprite, ZO_ss);

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

Vec2 SpaceshipNode::calculateNewForwardPosition() {
  const Vec2 currentPosition = getPosition();
  const float currentRotation = getRotation();

  Vec2 result = currentPosition;

  const float flightLength = 60;

  double angle;

  if ((currentRotation>=0)&&(currentRotation<90)) {
    angle = 90 - currentRotation;
    angle = (angle*pi)/180;

    result.x = currentPosition.x + flightLength*cos(angle);
    result.y = currentPosition.y + flightLength*sin(angle);
  }
  else if ((currentRotation>=90)&&(currentRotation<180)) {
    angle = currentRotation -90;
    angle = (angle*pi)/180;

    result.x = currentPosition.x + flightLength*cos(angle);
    result.y = currentPosition.y - flightLength*sin(angle);
  }
  else if ((currentRotation>=180)&&(currentRotation<270)) {
    angle = 270 - currentRotation;
    angle = (angle*pi)/180;

    result.x = currentPosition.x - flightLength*cos(angle);
    result.y = currentPosition.y - flightLength*sin(angle);
  }
  else if ((currentRotation>=270)&&(currentRotation<360)) {
    angle = currentRotation - 270;
    angle = (angle*pi)/180;

    result.x = currentPosition.x - flightLength*cos(angle);
    result.y = currentPosition.y + flightLength*sin(angle);
  }
  else {
    C6_D2(c6, "Bad rotation value ", currentRotation);
  }

  return result;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void SpaceshipNode::moveForward() {
  const int nra = getNumberOfRunningActionsByTag(SNAT_Move);
  if (nra>0) {
    C6_T3(c6, "Move skipped because it's already running ", nra, " times");
    return;
  }

  Vec2 newPos = calculateNewForwardPosition();


  MoveTo* ma = MoveTo::create(moveDurarionOneStep, newPos);
  ma->setTag(SNAT_Move);

  runAction(ma);
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
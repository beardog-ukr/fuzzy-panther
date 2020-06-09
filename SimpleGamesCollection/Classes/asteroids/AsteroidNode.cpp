#include "asteroids/AsteroidNode.h"
using namespace asteroids;
#include "asteroids/ZOrderValues.h"


#include "SixCatsLogger.h"
#include "SixCatsLoggerMacro.h"
#include <sstream>

#include <cmath>

USING_NS_CC;
using namespace std;

static const double pi = acos(-1);

// static const int categoryMaskAsteroid = 0x02;
// static const int categoryMaskShip = 0x01;

static const int smallPointsCount = 6;
static const Vec2 smallPoints[smallPointsCount] = {
  {.x =   -13, .y = -2  },
  {.x =   -9, .y =  10  },
  {.x =   7, .y =   12  },
  {.x =   15, .y =  2  },
  {.x =   6, .y =   -11 },
  {.x =   -4, .y =  -11 }
};

static const int mediumPointsCount = 6;
static const Vec2 mediumPoints[mediumPointsCount] = {
  {.x = -20, .y =  4},
  {.x = -8, .y =   21},
  {.x = 18, .y =   19},
  {.x = 21, .y =   -9},
  {.x = 1, .y =  -19},
  {.x = -13, .y =  -14}
};

static const int bigPointsCount = 8;
static const Vec2 bigPoints[bigPointsCount] = {
  {.x =   6, .y =    47  },
  {.x =   59, .y =    29  },
  {.x =   45, .y =    -17 },
  {.x =   -9, .y =    -32 },
  {.x =   -26, .y =    -48 },
  {.x =   -53, .y =    -26 },
  {.x =   -60, .y =    4 },
  {.x =   -40, .y =    41  }
};



// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

double calculateDistance(const Vec2& one, const Vec2& two) {
  double xDiff = one.x - two.x;
  double yDiff = one.y - two.y;
  double result = sqrt(xDiff*xDiff + yDiff*yDiff);

  return result;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

AsteroidNode::AsteroidNode(const double inAngle, const RockType inType) {
  basicAngle = inAngle;
  type = inType;

  baseSprite = nullptr;

  upMargin = 680+50;
  downMargin = -50;
  leftMargin = -50;
  rightMargin = 1200 + 50;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

AsteroidNode::~AsteroidNode() {
  C6_D1(c6, "here");
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

Vec2 AsteroidNode::calculatePointA(const Vec2& currentPos, const double rotationAngle) const {
  Vec2 result;

  if ((rotationAngle>0)&&(rotationAngle<pi/2)) {
    result = calculatePointAupright(currentPos, rotationAngle);
  }
  else if ((rotationAngle>=pi/2)&&(rotationAngle<pi)) {
    result = calculatePointAdownright(currentPos, rotationAngle);
  }
  else if ((rotationAngle>=pi)&&(rotationAngle<(pi*1.5))) {
    result = calculatePointAdownleft(currentPos, rotationAngle);
  }
  else if ((rotationAngle>=(pi*1.5))&&(rotationAngle<(2*pi))) {
    result = calculatePointAupleft(currentPos, rotationAngle);
  }
  else {
    C6_D2(c6, "Bad angle value: ", rotationAngle);
  }

  return result;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

Vec2 AsteroidNode::calculatePointAupright(const cocos2d::Vec2& currentPos,
                                          const double rotationAngle) const {
  Vec2 pointA2;
  double alpha;
  double cathetus;
  Vec2 result;

  alpha = pi/2 - rotationAngle;

  Vec2 pointA1;
  pointA1.x = rightMargin;
  cathetus = rightMargin - currentPos.x;
  pointA1.y = currentPos.y + (cathetus*tan(alpha));

  //calculate A2 point
  cathetus = upMargin - currentPos.y;
  pointA2.x = currentPos.x + (cathetus/tan(alpha));
  pointA2.y = upMargin;

  if (calculateDistance(currentPos, pointA1) < calculateDistance(currentPos, pointA2)) {
    result = pointA1;
  }
  else {
    result = pointA2;
  }

  return result;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

Vec2 AsteroidNode::calculatePointAdownright(const cocos2d::Vec2& currentPos,
                                            const double rotationAngle) const {
  Vec2 pointA2;
  double alpha;
  double cathetus;
  Vec2 result;

  alpha = rotationAngle - pi/2;

  Vec2 pointA1;
  pointA1.x = rightMargin;
  cathetus = rightMargin - currentPos.x;
  pointA1.y = currentPos.y - (cathetus*tan(alpha));

  //calculate A2 point
  cathetus = currentPos.y - downMargin;
  pointA2.x = currentPos.x + (cathetus/tan(alpha));
  pointA2.y = downMargin;

  if (calculateDistance(currentPos, pointA1) < calculateDistance(currentPos, pointA2)) {
    result = pointA1;
  }
  else {
    result = pointA2;
  }

  return result;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

Vec2 AsteroidNode::calculatePointAdownleft(const cocos2d::Vec2& currentPos,
                                           const double rotationAngle) const {
  Vec2 pointA2;
  double alpha;
  double cathetus;
  Vec2 result;

  alpha = (1.5*pi) - rotationAngle;

  Vec2 pointA1;
  pointA1.x = leftMargin;
  cathetus = currentPos.x - leftMargin;
  pointA1.y = currentPos.y - (cathetus*tan(alpha));

  //calculate A2 point
  cathetus = currentPos.y - downMargin;
  pointA2.x = currentPos.x - (cathetus/tan(alpha));
  pointA2.y = downMargin;

  if (calculateDistance(currentPos, pointA1) < calculateDistance(currentPos, pointA2)) {
    result = pointA1;
  }
  else {
    result = pointA2;
  }

  return result;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

Vec2 AsteroidNode::calculatePointAupleft(const cocos2d::Vec2& currentPos,
                                         const double rotationAngle) const {
  Vec2 pointA2;
  double alpha;
  double cathetus;
  Vec2 result;

  alpha = rotationAngle - (1.5*pi);

  Vec2 pointA1;
  pointA1.x = leftMargin;
  cathetus = currentPos.x - leftMargin;
  pointA1.y = currentPos.y + (cathetus*tan(alpha));

  //calculate A2 point
  cathetus = upMargin - currentPos.y;
  pointA2.x = currentPos.x - (cathetus/tan(alpha));
  pointA2.y = upMargin;

  if (calculateDistance(currentPos, pointA1) < calculateDistance(currentPos, pointA2)) {
    result = pointA1;
  }
  else {
    result = pointA2;
  }

  return result;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

Vec2 AsteroidNode::calculatePointB(const Vec2& currentPos, const double rotationAngle) const {
  Vec2 result;

  if ((rotationAngle>0)&&(rotationAngle<pi/2)) {
    result = calculatePointAdownleft(currentPos, rotationAngle+pi);
  }
  else if ((rotationAngle>=pi/2)&&(rotationAngle<pi)) {
    result = calculatePointAupleft(currentPos, rotationAngle+pi);
  }
  else if ((rotationAngle>=pi)&&(rotationAngle<(pi*1.5))) {
    result = calculatePointAupright(currentPos, rotationAngle-pi);
  }
  else if ((rotationAngle>=(pi*1.5))&&(rotationAngle<(2*pi))) {
    result = calculatePointAdownright(currentPos, rotationAngle-pi);
  }

  else {
    C6_D2(c6, "Bad angle value: ", rotationAngle);
  }

  return result;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

AsteroidNode* AsteroidNode::create(const double angle, const RockType type,
                                   shared_ptr<SixCatsLogger> c6) {

  AsteroidNode *pRet = new(std::nothrow) AsteroidNode(angle,type);
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

bool AsteroidNode::init() {

  if (!initBaseSprite()) {
    return false;
  }

  if (!initPhysicsBody()) {
    return false;
  }

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool AsteroidNode::initBaseSprite() {
  const string frameNames[3] = {"rock_brown_small_2.png", "rock_brown_medium_1.png",
                                "rock_brown_big_2.png"};

  baseSprite = Sprite::createWithSpriteFrameName(frameNames[(int)type]);
  if (baseSprite == nullptr) {
    C6_C2(c6, "Error while loading: ", frameNames[(int)type]);
    return false;
  }

  baseSprite->setTag(IT_asteroid);

  // baseSprite->setOpacity(60);

  addChild(baseSprite, ZO_asteroid);

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool AsteroidNode::initPhysicsBody() {
  if (baseSprite == nullptr) {
    C6_C1(c6, "Bad call");
    return false;
  }


  int polygonPointCount = 0;
  const Vec2*  polygonPointsArr = nullptr;
  switch(type) {
  case RT_small:
    polygonPointCount = smallPointsCount;
    polygonPointsArr = smallPoints;
    break;
  case RT_medium:
    polygonPointCount = mediumPointsCount;
    polygonPointsArr = mediumPoints;
    break;
  case RT_big:
    // C6_D1(c6, "called for big");
    polygonPointCount = bigPointsCount;
    polygonPointsArr = bigPoints;
    break;

  default:
    C6_C1(c6, "Bad call");
    return false;
  }

  PhysicsBody* physicsBody = PhysicsBody::createPolygon(polygonPointsArr, polygonPointCount,
                                                        PhysicsMaterial(0.1f, 1.0f, 0.0f));
  physicsBody->setDynamic(false);
  physicsBody->setCategoryBitmask(CM_asteroid);
  physicsBody->setCollisionBitmask(CM_ship | CM_laser);
  physicsBody->setContactTestBitmask(0xFFFFFFFF);

  baseSprite->addComponent(physicsBody);
  // baseSprite->setTag(IT_asteroid);

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void AsteroidNode::setAngle(const double rotationAngle) {
  double alpha = (rotationAngle*pi)/180;

  const Vec2 currentPos = getPosition();

  pointA = calculatePointA(currentPos, alpha);
  pointB = calculatePointB(currentPos, alpha);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void AsteroidNode::start() {
  setAngle(basicAngle);// calcu;ation is performed hee because only at this point
  //position of the node is defined
  resetMove();
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void AsteroidNode::resetMove() {

  stopAllActions();

  MoveTo* mt = MoveTo::create(6, pointA);

  CallFunc *cf = CallFunc::create([this]() {
    this->setPosition(this->pointB);
    this->resetMove();
  });

  Sequence* seq = Sequence::create(mt, cf, nullptr);
  runAction(seq);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .


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

// enum MoveType {
//   MT_up_right,
//   MT_down_right,
//   MT_down_left,
//   MT_up_left
// }

// MoveType AsteroidNode::calculateMoveType(const double rotationAngle) {
//   MoveType result = MT_up_right;

//   if ((rotationAngle>0)&&(rotationAngle<pi/2)) {
//     result = MT_up_right;
//   }
//   else if ((rotationAngle>=pi/2)&&(rotationAngle<pi)) {
//     result = MT_down_right;
//   }
//   else if ((rotationAngle>=pi)&&(rotationAngle<(pi*1.5))) {
//     result = MT_down_left;
//   }
//   else if ((rotationAngle>=(pi*1.5))&&(rotationAngle<(2*pi))) {
//     result = MT_up_left;
//   }
//   else {
//     C6_D2(c6, "Bad angle value: ", rotationAngle);
//   }

//   return result;
// }

// double calculateAlphaFromRotation(const double rotationAngle, shared_ptr<SixCatsLogger> c6) {
//   double alpha = rotationAngle;
//   if ((rotationAngle>0)&&(rotationAngle<pi/2)) {
//     alpha = pi/2 - rotationAngle;
//   }
//   else if ((rotationAngle>=pi/2)&&(rotationAngle<pi)) {
//     alpha = pi - rotationAngle;
//   }
//   else if ((rotationAngle>=pi)&&(rotationAngle<(pi*1.5))) {
//     alpha = rotationAngle -pi;
//   }
//   else if ((rotationAngle>=(pi*1.5))&&(rotationAngle<(2*pi))) {
//     alpha = rotationAngle - (1.5*pi);
//   }
//   else {
//     C6_D2(c6, "Bad angle value: ", rotationAngle);
//   }

//   return alpha;
// }

// MoveType AsteroidNode::calculateVerticalDistance() {
//   double cathetus = 0;
//   double alpha = rotationAngle;

//   const Vec2 currentPos = getPosition();

//   if ((rotationAngle>0)&&(rotationAngle<pi/2)) {
//     cathetus  = rightMargin - currentPos.x;
//     alpha = pi/2 - rotationAngle;
//   }
//   else if ((rotationAngle>=pi/2)&&(rotationAngle<pi)) {
//     cathetus  = rightMargin - currentPos.x;
//     alpha = pi - rotationAngle;
//   }
//   else if ((rotationAngle>=pi)&&(rotationAngle<(pi*1.5))) {
//     cathetus  = currentPos.x - leftMargin;
//     alpha = rotationAngle -pi;
//   }
//   else if ((rotationAngle>=(pi*1.5))&&(rotationAngle<(2*pi))) {
//     cathetus  = currentPos.x - leftMargin;
//     alpha = rotationAngle - (1.5*pi);
//   }
//   else {
//     C6_D2(c6, "Bad angle value: ", rotationAngle);
//   }

//   double result = cathetus / cos(alpha);

//   return result;
// }

double calculateDistance(const Vec2& one, const Vec2& two) {
  double xDiff = one.x - two.x;
  double yDiff = one.y - two.y;
  double result = sqrt(xDiff*xDiff + yDiff*yDiff);

  return result;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

AsteroidNode::AsteroidNode() {
  // ntdh
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
  // Vec2 pointB1;
  // Vec2 pointB2;
  // double alpha;
  // double cathetus;

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
  // alpha = pi/2 - rotationAngle;

  // cathetus = currentPos.y - downMargin;
  // pointB1.x = currentPos.x - (cathetus/tan(alpha));
  // pointB1.y = downMargin;

  // pointB2.x = leftMargin;
  // cathetus = currentPos.x - leftMargin;
  // pointB2.y = currentPos.y - (cathetus*tan(alpha));

  // if (calculateDistance(currentPos, pointB1) < calculateDistance(currentPos, pointB2)) {
  //   result = pointB1;
  // }
  // else {
  //   result = pointB2;
  // }

  else {
    C6_D2(c6, "Bad angle value: ", rotationAngle);
  }

  return result;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

AsteroidNode* AsteroidNode::create(shared_ptr<SixCatsLogger> c6) {

  AsteroidNode *pRet = new(std::nothrow) AsteroidNode();
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
  const char filename[] = "asteroids/meteorBrown_med1.png";
  baseSprite = Sprite::create(filename);
  if (baseSprite == nullptr) {
    C6_C2(c6, "Error while loading: ", filename);
    return false;
  }

  addChild(baseSprite, ZO_asteroids);

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void AsteroidNode::setAngle(const double rotationAngle) {
  double alpha = (rotationAngle*pi)/180;

  const Vec2 currentPos = getPosition();

  pointA = calculatePointA(currentPos, alpha);
  pointB = calculatePointB(currentPos, alpha);
  goToPointA = true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void AsteroidNode::start() {
  goToAnotherPoint();
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void AsteroidNode::goToAnotherPoint() {

  stopAllActions();

  MoveTo* mt = MoveTo::create(6, pointA);

  CallFunc *cf = CallFunc::create([this]() {
    this->setPosition(this->pointB);
    this->goToAnotherPoint();
  });

  Sequence* seq = Sequence::create(mt, cf, nullptr);
  runAction(seq);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .


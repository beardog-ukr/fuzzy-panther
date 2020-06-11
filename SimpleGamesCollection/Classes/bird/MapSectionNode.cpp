#include "bird/MapSectionNode.h"

#include "SixCatsLogger.h"
#include "SixCatsLoggerMacro.h"
#include <sstream>

USING_NS_CC;
using namespace std;

using namespace bird;
//static int objectIdGenerator = 0;

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

MapSectionNode::MapSectionNode() {
  //objectId = objectIdGenerator++;

}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

MapSectionNode::~MapSectionNode() {
  // ntdh
  C6_D1(c6, "here");
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .


MapSectionNode* MapSectionNode::create(const std::string& filename,
                                       std::shared_ptr<SixCatsLogger> inc6) {
  MapSectionNode *result = new (std::nothrow) MapSectionNode();

  if (result != nullptr) {
    result->setLogger(inc6);
  }


  if (result->initWithFile(filename)) {
    result->autorelease();
    return result;
  }
  //else
  CC_SAFE_DELETE(result);
  return nullptr;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
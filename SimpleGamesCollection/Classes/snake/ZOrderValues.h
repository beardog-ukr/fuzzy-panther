#pragma once

namespace snake {

enum ZOrderValues {
  kSceneBackgroundZOrder = 10,

  kGameBackgroundZOrder = 20,

  kSnakeZOrder = 30,
  kFireworksZOrder = 31,

  kGameOverImageZOrder = 40,
  kGameOverMenuZOrder = 41,

  kSceneBorderZOrder = 60
};

}

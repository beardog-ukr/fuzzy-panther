#pragma once

namespace bird {

enum ZOrderValues {
  ZO_scene_background = 10,

  ZO_game_background = 20,

  ZO_wizard = 30,

  ZO_scene_border = 60,
};

enum CategoryMasks {
  CM_cliff = 0x01,
  CM_wizard = 0x02
};

// enum ItemsTags {
//   IT_wizard = 1,
//   IT_cliff
// };

}
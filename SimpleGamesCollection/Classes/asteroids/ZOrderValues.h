#pragma once

namespace asteroids {

enum ZOrderValues {
  ZO_scene_background = 10,

  ZO_game_background = 20,

  ZO_asteroid = 30,

  ZO_laser = 40,

  ZO_ss = 50
};

enum CategoryMasks {
  CM_asteroid = 0x01,
  CM_ship = 0x02
};

enum ItemsTags {
  IT_asteroid,
  IT_ship,
  IT_laser
};

}
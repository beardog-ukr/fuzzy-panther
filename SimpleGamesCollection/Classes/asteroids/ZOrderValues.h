#pragma once

namespace asteroids {

enum ZOrderValues {
  ZO_scene_background = 10,
  ZO_menu_background = 11,

  ZO_game_background = 20,
  ZO_menu = ZO_game_background, // same as ZO_game_background

  ZO_asteroid = 30,
  ZO_menu_elements = ZO_asteroid,

  ZO_laser = 40,

  ZO_ss = 50,

  ZO_scene_border_corner = 60,
  ZO_scene_border_frame,
  ZO_scene_border_center
};

enum CategoryMasks {
  CM_asteroid = 0x01,
  CM_ship = 0x02,
  CM_laser = 0x04
};

enum ItemsTags {
  IT_asteroid = 1,
  IT_ship,
  IT_laser
};

enum RockType : unsigned int {
  RT_small = 0,
  RT_medium,
  RT_big
};

}
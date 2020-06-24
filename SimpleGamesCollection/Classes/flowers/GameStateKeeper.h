#pragma once

#include <list>
//#include "cocos2d.h"
#include "SixCatsLoggerLoggable.h"

namespace flowers {


enum TileType : int;
// class FlowersGameNode;

enum GameFieldSize {
  kFlowersGameWidth = 15,
  kFlowersGameHeight = 10
};

class GameStateKeeper : virtual public SixCatsLoggerLoggable {
public:
  GameStateKeeper();
  virtual ~GameStateKeeper();

  TileType getTileStatus(const int x, const int y) const;
  int getTileCounter(const int x, const int y) const;

  //returns true if game is over
  bool getGameIsOver() const;

  // returns true if this was victory
  bool getGameResult() const;

  void reset();

  // returns coordinated of tiles that were changed and need to be redrawn
  std::list< std::pair<int, int> > processClick(const int row, const int column);

  static std::string generateTileNodeName(const int x, const int y);//std::shared_ptr<SixCatsLogger> c6

protected:
  void initMap();

  void generateRandomMap(int * const map);

//  std::list< std::pair<int, int> > changedNodes;

  // returns true if all tiles are either visible or contain mine
  bool checkIfAllRevealed();

  void reevaluateTilesVisual(std::list<std::pair<int, int> > &changedNodes);


  int minesCount[kFlowersGameWidth*kFlowersGameHeight];
  bool minesPresence[kFlowersGameWidth*kFlowersGameHeight];
  bool tilesVisibility[kFlowersGameWidth*kFlowersGameHeight];
  TileType tilesVisual[kFlowersGameWidth*kFlowersGameHeight];

  bool gameIsOver;
  bool gameWasFailed;

};

}


#pragma once

#include "scenes/3D_graphics/Projet/Dinosaurs/Dinosaur.hpp"
#include "scenes/3D_graphics/Projet/Utilities/Utilities.hpp"
#include <list>

class Grid {
  protected:
  int m_resolution_grid;
  int m_resolution_grid_2; 
  float m_r; 
  unsigned int nb_dino=0;

  std::vector<std::vector<std::vector<std::list<Dinosaur*>>>> m_grid; // matrix 3D of lists of dinosaurs
  public:
  // functions to call depending on the m_localUp -> which is here necessarily in +X,-X, +Y, -Y, +Z, -Z
  Grid(unsigned int resolution_grid);

  std::list<Dinosaur> getGrid(vcl::vec3 position); // returns a list of dinosaurs - the ones that are close enough
  void updateGrid(Dinosaur *dino); // update grid
  void addToGrid(Dinosaur *dino); // init grid

  /*
  Possible upgrade : when a Chilesaurus sees a Trex, it just runs from it - no group bahavior. If we always add the Trex in front of the list
  Then we can stop looking to the other dinosaurs. We would need polymorphism to do that.
  */

  vcl::vec3 getPertinentCoord(vcl::vec3 Pos, unsigned int faceIdx);
  GridPosition computePositionInGrid(vcl::vec3 Pos, unsigned int faceIdx);
  std::list<Dinosaur*> getGrid(Dinosaur *dino); // returns a list of dinosaurs - the ones that are close enough
  void remove(Dinosaur *dino);
};


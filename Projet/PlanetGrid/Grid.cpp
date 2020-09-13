
#include "Grid.hpp"
#include "math.h"

// --------------- Grid ------------------- //

Grid::Grid(unsigned int resolution_grid): m_resolution_grid(resolution_grid){
    m_r = m_resolution_grid / M_PI_2;
    m_resolution_grid_2 = m_resolution_grid/2;
    std::vector<std::vector<std::vector<std::list<Dinosaur*>>>> grid(6, std::vector<std::vector<std::list<Dinosaur*>>>(m_resolution_grid,std::vector<std::list<Dinosaur*>>(m_resolution_grid)));
    m_grid = grid;
    for( int k(0); k<6; k++){
         for( int i(0); i<resolution_grid; i++){
            for( int j(0); j<resolution_grid; j++){
                std::list<Dinosaur*> list;
                m_grid[k][i][j]= list;
            }
        }
    }
}

vcl::vec3 Grid::getPertinentCoord(vcl::vec3 pos, unsigned int faceIdx){
    vcl::vec3 newPos;
    if(faceIdx == 0){
        newPos = pos;
    }
    else if (faceIdx == 1){
        newPos = -pos;
    }
    else if (faceIdx == 2){
        newPos = {pos.y,pos.z,pos.x};
    }
    else if (faceIdx == 3){
        newPos = {-pos.y,-pos.z,-pos.x};
    }
    else if (faceIdx == 4){
        newPos = {pos.z,pos.x,pos.y};
    }
    else{
        newPos = {-pos.z,-pos.x,-pos.y};
    }
    return newPos;
}

GridPosition Grid::computePositionInGrid(vcl::vec3 Pos, unsigned int faceIdx){
    
    GridPosition posInGrid;

    vcl::vec3 pos(Pos);
    posInGrid.faceIdx = faceIdx;
    vcl::vec3 pos_ = getPertinentCoord(pos,faceIdx);
    float x2 = pos_.x*pos_.x;
    posInGrid.n_1 = (unsigned int)(m_r * asin(pos_.y/sqrt(x2+pos_.y*pos_.y))+m_resolution_grid_2);
    posInGrid.n_2 = (unsigned int)(m_r * asin(pos_.z/sqrt(x2+pos_.z*pos_.z))+m_resolution_grid_2);
    if(posInGrid.n_1>m_resolution_grid-1){
        posInGrid.n_1 = m_resolution_grid-1;
    }
    if(posInGrid.n_2>m_resolution_grid-1){
        posInGrid.n_2 = m_resolution_grid-1;
    }
    
    return posInGrid;
}

void Grid::addToGrid(Dinosaur *dino){
    // to init the dinosaur
    vcl::vec3 pos(dino->getPositionOnSphereUnit());
    unsigned int faceIdx(dino->getFaceIdx());;

    GridPosition posInGrid = computePositionInGrid(pos, faceIdx);
    m_grid[posInGrid.faceIdx][posInGrid.n_1][posInGrid.n_2].push_back(dino);

    dino->setPositionInGrid(posInGrid);
    nb_dino+=1;


}

void Grid::updateGrid(Dinosaur *dino){
    
    GridPosition oldPosIngrid = dino -> getPositionInGrid();

    vcl::vec3 pos(dino->getPositionOnSphereUnit());
    unsigned int faceIdx(dino->getFaceIdx());
    GridPosition posInGrid = computePositionInGrid(pos, faceIdx);

    if(!posInGrid.equal(oldPosIngrid)){ // we check if the dino is in the same grid as before, if not we move it
        // we changed of area
        m_grid[oldPosIngrid.faceIdx][oldPosIngrid.n_1][oldPosIngrid.n_2].remove(dino);
       // std::cout << posInGrid.n_2 << std::endl;
        m_grid[posInGrid.faceIdx][posInGrid.n_1][posInGrid.n_2].push_back(dino);
        dino->setPositionInGrid(posInGrid);
    }

    
}

std::list<Dinosaur*> Grid::getGrid(Dinosaur *dino){
    GridPosition posIngrid = dino -> getPositionInGrid();
    return m_grid[posIngrid.faceIdx][posIngrid.n_1][posIngrid.n_2];
}

void Grid::remove(Dinosaur *dino){
    nb_dino -= 1;
   //std::cout <<  nb_dino << std::endl;
    GridPosition posIngrid = dino -> getPositionInGrid();
    m_grid[posIngrid.faceIdx][posIngrid.n_1][posIngrid.n_2].remove(dino);
}
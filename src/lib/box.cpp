#include "box.hpp"

Box::~Box(){
  bottomLeftY = 0;
  bottomLeftX = 0;
  width = 0;
  height = 0;
}

Box::Box(int bottomLeftX, int BottomLeftY, int width, int height){
    this->bottomLeftX = bottomLeftX;
    this->bottomLeftY = bottomLeftY;
    this->width = width;
    this->height = height;
}


void Box::MoveObject(int x, int y, int windowWidth, int windowHeight){
    this->bottomLeftX = this->bottomLeftX + x;
    if(this->bottomLeftX > windowWidth){
        this->bottomLeftX = windowWidth;
    }

    this->bottomLeftY = this->bottomLeftY + y;
    if(this->bottomLeftY > windowHeight){
        this->bottomLeftY = windowHeight;
    }
}



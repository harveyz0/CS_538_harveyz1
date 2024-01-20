#ifndef BOX_H
#define BOX_H

class Box {
    public:
         int bottomLeftX = 0;
         int bottomLeftY = 0;
         int width = 0;
         int height = 0;
        Box(int bottomLeftX, int BottomLeftY, int width, int height);
        ~Box();
        void MoveObject(int x, int y, int windowWidth, int windowHeight);

};

#endif

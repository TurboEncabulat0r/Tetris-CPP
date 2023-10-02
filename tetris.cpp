#include "tetris.h"
#include <string>
#include <iostream>

namespace tetris {

    void getNewShape();
    void gameOver();

    std::string shp[7][4][5] = { {{".....",
                           "..0..",
                           ".00..",
                           ".0...",
                           "....."},
                           {".....",
                           ".00..",
                           "..00.",
                           "......",
                           "....."}},


                           {{".....",
                           ".0...",
                           ".00..",
                           "..0..",
                           "....."},
                           {".....",
                           "..00.",
                           ".00..",
                           "......",
                           "....."}},

                         {{".....",
                           "..0..",
                           "..0..",
                           "..0..",
                           "..0.."},
                           {".....",
                           ".....",
                           ".....",
                           "0000.",
                           "....."}},

                         {{".....",
                           ".00..",
                           ".00..",
                           ".....",
                           "....."}},

                         {{".....",
                           "..0..",
                           ".000.",
                           ".....",
                           "....."},
                           {".....",
                           "..0..",
                           ".00..",
                           "..0..",
                           "....."},
                           {".....",
                           ".....",
                           ".000.",
                           "..0..",
                           "....."},
                           {".....",
                           "..0..",
                           "..00.",
                           "..0..",
                           "....."}},


                         {{".....",
                           "..0..",
                           "..0..",
                           "..00.",
                           ".....",},
                           {".....",
                           "...0.",
                           ".000.",
                           ".....",
                           ".....",},
                           {".....",
                           ".00..",
                           "..0..",
                           "..0..",
                           ".....",},
                           {".....",
                           "....",
                           ".000.",
                           ".0...",
                           ".....",}},


                           {{".....",
                           "..0..",
                           "..0..",
                           ".00..",
                           ".....",},
                           {".....",
                           ".....",
                           ".000.",
                           "...0.",
                           ".....",},
                           {".....",
                           "..00.",
                           "..0..",
                           "..0..",
                           ".....",},
                           {".....",
                           ".0...",
                           ".000.",
                           ".....",
                           ".....",}} };

    int rots[7] = { 2, 2, 2, 1, 4, 4, 4 };
    

    const int blockWidth = 45;

    int* getWindowSize(int arr[]) {
       
        arr[0] = blockWidth * 15;
        arr[1] = blockWidth * 21;


        return arr;
    }

    struct color {
        int hex;
        int r;
        int b;
        int g;
        char label;

        color(int c, char l) {
            hex = c;
            this->r = ((this->hex >> 16) & 0xFF) / 255.0;
            this->g = ((this->hex >> 8) & 0xFF) / 255.0;
            this->b = ((this->hex) & 0xFF) / 255.0;
            label = l;
        } 
    };

    color colors[7] = {color(0x00ff00, 'I'), color(0xff0000, 'O'), color(0x00ffff, 'S'), color(0xffff00, 'Z'), color(0x800080, 'T'), color(0xff7f00, 'J'), color(0x0000ff, 'L')};
    color bgColor = color(0x2B2B2B, ' ');

    color getColor(char c) {
        for (int i = 0; i < 7; i++) {
            if (colors[i].label == c)
                return colors[i];
        }
        return color(0, ' ');
    }

    class object {
    public:
        int x = 0;
        int y = 0;

        virtual void draw() = 0;
    };

    class grid : public object {
    public:
        std::string matrix[20];

        const char* str = "..........";

        grid() {
            makeMatrix();
            this->x = 0;
            this->y = 0;
        }

        bool isInBounds(int x, int y) {
            int realx = x - this->x; 
            int realy = y - this->y;

            if (realx < 0 || realx > 9)
                return false;
            if (realy > 19)
                return false;
            if (realy < 0)
                realy = 0;
            if (matrix[realy][realx] != '.')
                return false;
            return true;
        }

        void set(int x, int y, char c) {
            int ry = y - this->y;
            int rx = x - this->x;

            if (y < 0)
                return;
            if (isInBounds(x, y))
                matrix[ry][rx] = c;
            else if (ry < 0)
                gameOver();
        }

        void makeMatrix() {
            for (int i = 0; i < 20; i++) {
                matrix[i] = this->str;
            }
        }

        void draw() {
            for (int y = 0; y < 20; y++) {
                for (int x = 0; x < 10; x++) {
                    color c = getColor(matrix[y][x]);
                    if (c.label == ' ')
                        c.hex = bgColor.hex;
                    DrawRectangle((this->x + x) * blockWidth, (this->y + y) * blockWidth, blockWidth, blockWidth, c.hex);
                }
            }
        }

        void checkLines() {
            for (int i = 0; i < 20; i++) {
                bool full = true;
                for (int j = 0; j < 10; j++) {
                    if (matrix[i][j] == '.') {
                        full = false;
                        break;
                    }
                }
                if (full) {
                    matrix[i] = str;

                    for (int y = i; y > 0; y--) {
                        this->matrix[y] = this->matrix[y - 1];
                    }
                }
            }
        }


    };
    grid* g = new grid();


    class shape : public object {
    public:
        int piece = 4;
        int rot = 0;

        shape(int x, int y, int p) {
            this->x = x;
            this->y = y;
            this->piece = p;
            this->rot = 0;
        }

        shape(shape* s) {
            this->x = s->x;
            this->y = s->y;
            this->piece = s->piece;
            this->rot = s->rot;
        }

        ~shape() {

        }

        void copy(shape* shape) {
            this->x = shape->x;
            this->y = shape->y;
            this->piece = shape->piece;
            this->rot = shape->rot;
        }

        void rotate() {
            this->rot--;
            if (this->rot < 0)
            {
                this->rot = rots[piece] - 1;
                
            }

            int newx = this->x;
            int newy = this->y;

            for (int i = 0; i < 5; i++) {
                for (int j = 0; j < 5; j++) {
                    if (shp[piece][rot][i][j] == '0') {

                        if (!g->isInBounds(newx + j, newy + i))
                            this->rot = rots[piece] - 1;
                    }
                }
            }
        }

        void drop() {
            while (this->move(0, 1))
                continue;

            this->placeShape();
        }

        bool move(int x, int y) {
            int newx = x + this->x;
            int newy = y + this->y;
            
            for (int i = 0; i < 5;i++) {
                for (int j = 0; j < 5; j++) {
                    if (shp[piece][rot][i][j] == '0') {

                        if (!g->isInBounds(newx + j, newy + i))
                            return false;
                    }
                }
            }
            

            this->x = newx;
            this->y = newy;

            return true;
        }

        void placeShape(){
            for (int y = 0; y < 5;y++) {
                for (int x = 0; x < 5; x++) {
                    if (shp[piece][rot][y][x] == '0') {
                        g->set(this->x + x, this->y + y, colors[piece].label);
                    }
                }
            }
            getNewShape();
            g->checkLines();
        }

        void draw() {
            for (int i = 0; i < 5; i++) {
                for (int j = 0; j < 5; j++) {
                    try {
                        if (shp[piece][rot][i][j] == '0') {
                            DrawRectangle((x + j) * blockWidth, (y + i) * blockWidth, blockWidth, blockWidth, colors[piece].hex);
                        }
                    }
                    catch(std::exception){
                        this->rot = 0;
                    }
                }
            }

        }



        void drawGhost() {
            while (this->move(0, 1))
                continue;


            for (int i = 0; i < 5; i++) {
                for (int j = 0; j < 5; j++) {
                    try {
                        if (shp[piece][rot][i][j] == '0') {
                            DrawRectangle((x + j) * blockWidth, (y + i) * blockWidth, blockWidth, blockWidth, (u32)(colors[piece].hex * 0.4) + bgColor.hex);
                        }
                    }
                    catch (std::exception) {
                        this->rot = 0;
                    }
                }
            }
        }


    };

    void gameOver() {
        std::cout << "Game Over!" << std::endl;
    }
    
    shape* s = new shape(2, -3, random(0, 6));
    shape* held = NULL;
    shape* next = new shape(10, 1, random(0, 6));
    shape* ghost = new shape(2, -3, 0);
    void getNewShape() {
        s->copy(next);
        s->x = 2;
        s->y = -3;
        delete next;
        next = new shape(10, 1, random(0, 6));

    }

    void hold() {
        if (held == NULL) {
            held = new shape(0, 0, 0);
            held->copy(s);
            held->x = 10;
            held->y = 6;
            getNewShape();
            return;
        }
        shape* temp = new shape(0, 0, 0);
        temp->copy(held);
        held->copy(s);
        held->x = 10;
        held->y = 6;

        s->copy(temp);
        s->x = 2;
        s->y = -3;
        delete temp;
    }

    void keyDown(char k) {
        if (k == 'W') {
            s->rotate();
        }
        
        // 0x20 == spacebar
        if (k == 0x20) {
            s->drop();
        }
        
        // shift
        if (k == 0x10) {
            hold();
        }
    }



    float shapeMoveTs = 0;
    const float moveCooldown = 0.13f;
    void doMovement() {
        if (shapeMoveTs < getTime()) {
            if (isKeyDown('D')) {
                s->move(1, 0);
                shapeMoveTs = getTime() + moveCooldown;
            }

            if (isKeyDown('A')) {
                s->move(-1, 0);
                shapeMoveTs = getTime() + moveCooldown;
            }
            if (isKeyDown('S')) {
                s->move(0, 1);
                shapeMoveTs = getTime() + moveCooldown / 2;
            }
        }
    }

    float gravityTs = 0;
    void update() {
        doMovement();

        if (gravityTs < getTime()) {
            if (!s->move(0, 1)){
                s->placeShape();
            }
            gravityTs = getTime() + 0.4f;
        }
        
        g->draw();
        ghost->copy(s);
        ghost->drawGhost();
        next->draw();

        if (held != NULL)
            held->draw();
        s->draw();

    }

    void setup() {

        onUpdate(*update);
        onKeyDown(*keyDown);

    }

}
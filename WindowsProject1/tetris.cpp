#include "tetris.h"
#include <string>
#include <iostream>

namespace tetris {
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
    int j = 0;

    const int blockWidth = 20;

    struct color {
        int hex;
        char label;

        color(int c, char l) {
            hex = c;
            label = l;
        }
    };


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
            this->x = 5;
            this->y = 5;
        }

        bool isInBounds(int x, int y) {
            int realx = x - this->x; 
            int realy = y - this->y;
            if (realx < -1 || realx > 11)
                return false;
            if (realy > 20)
                return false;
            return true;
        }

        void makeMatrix() {
            for (int i = 0; i < 20; i++) {
                matrix[i] = this->str;
            }
        }

        void draw() {
            for (int y = 0; y < 20; y++) {
                for (int x = 0; x < 10; x++) {
                    DrawRectangle((this->x + x) * blockWidth, (this->y + y) * blockWidth, blockWidth, blockWidth, 0xff0000);
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
                    matrix[i] = "";
                    for (int j = i; j < 19; j++) {
                        matrix[j] = matrix[j + 1];
                    }
                }
            }
        }


    };
    grid* g = new grid();

    class shape : public object {
    public:
        int index = 4;
        int rot = 0;

        shape(int x, int y) {
            this->x = x;
            this->y = y;
        }

        void rotate() {
            this->rot++;
            if (this->rot > (sizeof(shp[index]) / sizeof(shp[0])))
                this->rot = 0;
        }

        bool move(int x, int y) {
            int newx = x + this->x;
            int newy = y + this->y;
            
            for (int y = 0; y < 5;y++) {
                for (int x = 0; x < 5; x++) {
                    if (!g->isInBounds(newx + x, newy + y))
                        return false;
                }
            }
            

            this->x = newx;
            this->y = newy;

            return true;
        }

        void draw() {
            for (int i = 0; i < 5; i++) {
                for (int j = 0; j < 5; j++) {
                    try {
                        if (shp[0][rot][i][j] == '0') {
                            DrawRectangle((x + j) * blockWidth, (y + i) * blockWidth, blockWidth, blockWidth, 0x0000ff);
                        }
                    }
                    catch(std::exception){
                        this->rot = 0;
                    }
                }
            }

        }


    };
    
    shape* s = new shape(5, 5);
    

    void keyDown(char k) {
        if (k == 'W') {
            //s->rotate();
            s->move(0, -1);
        }
        if (k == 'S')
            s->move(0, 1);
        if (k == 'D')
            s->move(1, 0);

        if (k == 'A')
            s->move(-1, 0);
    }

    float gravityTs = 0;
    void update() {
        

        if (gravityTs < getTime()) {
            s->move(0, 1);
            gravityTs = getTime() + 0.4f;
        }


        g->draw();
        s->draw();
        

    }

    void setup() {

        onUpdate(*update);
        onKeyDown(*keyDown);

    }

}
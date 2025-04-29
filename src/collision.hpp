#include <cstddef>
#include <cmath>

#define HORIZONTAL 0x0
#define VERTICAL 0x1

namespace collision{
    bool detect(int x, int y, int x2, int y2) {
        struct {
            int w = 12;
            int h = 21;
        } player;
        int block = 48;
        if (x <= x2 + block && x >= x2 || x + player.w <= x2 + block && x + player.w >= x2) {
            if (y <= y2 + block && y >= y2 || y + player.h <= y2 + block && y + player.h >= y2) {
                return(1);
            }
        }
        return(0);
    }
    char dir(int x, int y, int x2, int y2, int vectX, int vectY) {
        struct {
            int x;
            int x2;
            int y;
            int y2;
        } overlap;
        overlap.x = abs((x + 21 - x2)); // * (abs(vectX) + 1));
        overlap.x2 = abs((x2 + 48 - x)); // * (abs(vectX) + 1));
        overlap.y = abs((y + 12 - y2)); // * (abs(vectY) + 1));
        overlap.y2 = abs((y2 + 48 - y)); // * (abs(vectY) + 1));
        if ((overlap.x > overlap.y && overlap.x > overlap.y2) || (overlap.x2 > overlap.y && overlap.x2 > overlap.y2)) {
            return(HORIZONTAL);
        }
        else {
            return(VERTICAL);
        }
    }
}


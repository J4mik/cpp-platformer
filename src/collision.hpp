#define LEFTdir 0x0
#define RIGHTdir 0x1
#define UPdir 0x2
#define DOWNdir 0x3


char direction = NULL;

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
        direction = LEFTdir;
        return(0);
    }
}


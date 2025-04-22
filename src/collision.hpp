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
}


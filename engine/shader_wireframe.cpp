void render_pixel(color_t *fb, uint32_t x, uint32_t y) {
    fb[y * SCREEN_WIDTH + x] = 0xFFFF;
}

//Line code modified from picosystem sdk (Copyright (c) 2021 Pimoroni Ltd)
//might need further modifying/rewrite (TODO)
void render_line(color_t *fb, int32_t x1, int32_t y1, int32_t x2, int32_t y2) {
    //early exit if line goes beyond any screen points
    if (x1 >= SCREEN_WIDTH || x1 < 0 || x2 >= SCREEN_WIDTH || x2 < 0 || y1 >= SCREEN_HEIGHT || y1 < 0 || y2 >= SCREEN_HEIGHT || y2 < 0) {
        return;
    }
    int32_t	x = x1, y = y1, dx, dy, incx, incy, balance;

    if(x2 >= x1) {dx = x2 - x1; incx = 1;} else {dx = x1 - x2; incx = -1;}
    if(y2 >= y1) {dy = y2 - y1; incy = 1;} else {dy = y1 - y2; incy = -1;}

    if(dx >= dy) {
        dy <<= 1; balance = dy - dx; dx <<= 1;
        while(x != x2) {
        render_pixel(fb, x, y);
        if (balance >= 0) {y += incy; balance -= dx;}
        balance += dy; x += incx;
        }
    } else {
        dx <<= 1; balance = dx - dy; dy <<= 1;
        while(y != y2) {
        render_pixel(fb, x, y);
        if(balance >= 0) {x += incx; balance -= dy;}
        balance += dx; y += incy;
        }
    }
}
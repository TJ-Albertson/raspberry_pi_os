void fb_init();

// Draw pixel by specifying position and color index from palette. Will use x and y to get memory offset of framebuffer start pointer and assign value of color
void drawPixel(int x, int y, unsigned char color_index);
void drawChar(unsigned char ch, int x, int y, unsigned char attr);
void drawString(int x, int y, char *s, unsigned char attr);
void drawRect(int x1, int y1, int x2, int y2, unsigned char attr, int fill);
void drawCircle(int x0, int y0, int radius, unsigned char attr, int fill);
void drawLine(int x1, int y1, int x2, int y2, unsigned char attr);

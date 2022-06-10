#define TAG_MASK 0b11000000
#define VALUE_MASK 0b00111111

typedef struct {
    int w, h, len;
    unsigned char* data;
} fast_comp_im;

typedef struct {
    unsigned char r, g, b;
} pixel;
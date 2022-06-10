#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "fast_compress.h"
#include "../image.h"

fast_comp_im encode_image(image im) {
    assert(im.w != 0 && im.h != 0 && im.c == 3);
    pixel last_pixel;
    int run_length, index = 0;
    // bytestream to store encoded data, will be resized later
    unsigned char* data = (unsigned char *) malloc(im.w * im.h * 4);
    // lookback array for recent pixels, max size for index to be stored in 6 bits
    pixel cache[64];
    // keeps track of methods used to encode each pixel
    int run_total = 0, new_run_total = 0, cache_total = 0, diff_total = 0, small_diff_total = 0, original_total = 0;
    unsigned char r, g, b;
    for (int y = 0; y < im.h; y++) {
        for (int x = 0; x < im.w; x++) {
            // get rgb values from image
            r = *(im.data + (im.w * im.h * 0) + (im.w * y) + x);
            g = *(im.data + (im.w * im.h * 1) + (im.w * y) + x);
            b = *(im.data + (im.w * im.h * 2) + (im.w * y) + x);
            // check if run continues
            if (!((x == 0) && (y == 0)) && (r == last_pixel.r) && (g == last_pixel.g) && (b == last_pixel.b)) {
                if (run_length == 0) {
                    new_run_total++;
                }
                run_length++;
                // maximum run length for 6 bits with bias of 1 (00111111 tag is reserved so can't use 64), or last pixel
                if ((x == im.w - 1 && y == im.h - 1) || run_length == 63) {
                    // set first two tag bits to 00 and run with bias of 1
                    data[index++] = 0b00000000 | (run_length - 1);
                    run_total += run_length - 1;
                    run_length = 0;
                }
            } else {
                // end and write run
                if (run_length > 0) {
                    data[index++] = 0b00000000 | (run_length - 1);
                    run_total += run_length - 1;
                    run_length = 0;
                }
                unsigned int pixel_hash = (r * 7) + (g * 11) + (b * 13);
                unsigned int pixel_index = (unsigned int) (pixel_hash % 64);
                pixel cache_pixel = cache[pixel_index];
                if (cache_pixel.r == r && cache_pixel.g == g && cache_pixel.b == b) {
                    data[index++] = 0b01000000 | pixel_index;
                    cache_total += 1;
                } else {
                    // method from QOI
                     signed char r_diff_signed = r - last_pixel.r;
                     signed char g_diff_signed = g - last_pixel.g;
                     signed char b_diff_signed = b - last_pixel.b;
                     if (r_diff_signed >= -2 && r_diff_signed <= 1 && g_diff_signed >= -2 && g_diff_signed <= 1 && b_diff_signed >= -2 && b_diff_signed <= 1) {
                        data[index++] = 0b11000000 | (r_diff_signed + 2) << 4 | (g_diff_signed + 2) << 2 | (b_diff_signed + 2);
                        small_diff_total++;
                     }
                     // method from QOI
                     else if (r_diff_signed >= -32 && r_diff_signed <= 31 &&
                             ((g_diff_signed - r_diff_signed) >= -8) && ((g_diff_signed - r_diff_signed) <= 7) &&
                             ((b_diff_signed - r_diff_signed)) >= -8 && ((b_diff_signed - r_diff_signed) <= 7)) {
                        data[index++] = 0b10000000 | (r_diff_signed + 32);
                        data[index++] = (g_diff_signed - r_diff_signed + 8) << 4 | (b_diff_signed - r_diff_signed + 8);
                        diff_total++;
                     } else {
                        data[index++] = 0b00111111;
                        data[index++] = r;
                        data[index++] = g;
                        data[index++] = b;
                        original_total++;
                    }
                    cache[pixel_index] = (pixel) {r, g, b};
                }
            }
            last_pixel = (pixel) {r, g, b};
        }
    }
    unsigned char* bounded_data = realloc(data, index);
    fast_comp_im return_im = {im.w, im.h, index, bounded_data};
    printf("Original Size: %d bytes\n", im.w * im.h * 3);
    printf("Compressed Size: %d bytes\n", index);
    printf("Percent Size: %0.2f %%\n\n", ((double) index + 3 * sizeof(int)) / (im.w * im.h * 3) * 100);
    printf("Run Pixel Total: %d (~%d bytes saved) \n", run_total + new_run_total, (new_run_total * 2) + (run_total) * 3);
    printf("Cache Pixel Total: %d (~%d bytes saved)\n", cache_total, cache_total * 2);
    printf("Small Delta Pixel Total: %d (~%d bytes saved)\n", small_diff_total, small_diff_total * 2);
    printf("Delta Pixel Total: %d (~%d bytes saved)\n", diff_total, diff_total);
    printf("Original Pixel Total: %d (~%d bytes gained)\n\n", original_total, original_total / 3);

    return return_im;
}

image decode_image(fast_comp_im im) {
    assert(im.w != 0 && im.h != 0 && im.len != 0);
    image return_im = make_image(im.w, im.h, 3);
    int pixel_num = 0;
    unsigned char r, g, b, run_write_len;
    pixel cache[64];
    for (int i = 0; i < im.len; i++) {
        run_write_len = 0;
        unsigned char tag = TAG_MASK & im.data[i];
        if (tag == 0b00000000) {
            // original value tag
            if (im.data[i] == 0b00111111) {
                r = im.data[++i];
                g = im.data[++i];
                b = im.data[++i];
                long pixel_hash = (r * 7) + (g * 11) + (b * 13);
                int pixel_index = (int) (pixel_hash % 64);
                cache[pixel_index] = (pixel) {r, g, b};
                // run length encoded tag
            } else {
                run_write_len = (im.data[i] & VALUE_MASK);
            }
            // small diff encoded tag
        } else if (tag == 0b11000000) {
            int r_diff = ((im.data[i] & 0b00110000) >> 4) - 2;
            int g_diff = ((im.data[i] & (0b00001100)) >> 2) - 2;
            int b_diff = (im.data[i] & (0b00000011)) - 2;
            r += r_diff;
            g += g_diff;
            b += b_diff;
            long pixel_hash = (r * 7) + (g * 11) + (b * 13);
            unsigned int pixel_index = (unsigned int) (pixel_hash % 64);
            cache[pixel_index] = (pixel) {r, g, b};
        } else if (tag == 0b01000000) {
            unsigned int cache_index = im.data[i] & VALUE_MASK;
            pixel cached_pixel = cache[cache_index];
            r = cached_pixel.r;
            g = cached_pixel.g;
            b = cached_pixel.b;
        } else if (tag == 0b10000000) {
            unsigned char r_diff = (im.data[i] & VALUE_MASK) - 32;
            unsigned char g_diff = (im.data[++i] >> 4) + r_diff - 8;
            unsigned char b_diff = (im.data[i] & 0b00001111) + r_diff - 8;
            r += r_diff;
            g += g_diff;
            b += b_diff;
            long pixel_hash = (r * 7) + (g * 11) + (b * 13);
            int pixel_index = (int) (pixel_hash % 64);
            cache[pixel_index] = (pixel) {r, g, b};
        }
        for (unsigned char j = 0; j <= run_write_len; j++) {
            // not sure why but need to subtract value from 256?? saves as negative otherwise
            *(return_im.data + (im.w * im.h * 0) + pixel_num) = 256 - r;
            *(return_im.data + (im.w * im.h * 1) + pixel_num) = 256 - g;
            *(return_im.data + (im.w * im.h * 2) + pixel_num) = 256 - b;
            pixel_num++;
        }
    }
    return return_im;
}
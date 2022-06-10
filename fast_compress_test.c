#include "fast_compress.c"
#include "../load_image.c"
#include "../hw0/process_image.c"
#include <math.h>
#include <time.h>
#include <sys/time.h>


int main(int argc, char **argv) {
    struct timeval tv, tv2, tv3, tv4;

    image im = load_image("raw/test/data/kodim01.png");
    image im2 = load_image("raw/test/data/kodim02.png");
    image im3 = load_image("raw/test/data/kodim03.png");
    image im4 = load_image("raw/test/data/kodim04.png");
    image im5 = load_image("raw/test/data/kodim05.png");
    image im6 = load_image("raw/test/data/kodim06.png");
    image im7 = load_image("raw/test/data/kodim07.png");
    image im8 = load_image("raw/test/data/kodim08.png");
    image im9 = load_image("raw/test/data/kodim09.png");
    image im10 = load_image("raw/test/data/kodim10.png");
    image im11 = load_image("raw/test/data/kodim11.png");
    image im12 = load_image("raw/test/data/kodim12.png");
    image im13 = load_image("raw/test/data/kodim13.png");
    image im14 = load_image("raw/test/data/kodim14.png");
    image im15 = load_image("raw/test/data/kodim15.png");
    image im16 = load_image("raw/test/data/kodim16.png");
    image im17 = load_image("raw/test/data/kodim17.png");
    image im18 = load_image("raw/test/data/kodim18.png");
    image im19 = load_image("raw/test/data/kodim19.png");
    image im20 = load_image("raw/test/data/kodim20.png");
    image im21 = load_image("raw/test/data/kodim21.png");
    image im22 = load_image("raw/test/data/kodim22.png");
    image im23 = load_image("raw/test/data/kodim23.png");
    image im24 = load_image("raw/test/data/kodim24.png");

    gettimeofday(&tv, NULL);
    printf("kodim01\n");
    fast_comp_im comp_im = encode_image(im);
    printf("kodim02\n");
    fast_comp_im comp_im2 = encode_image(im2);
    printf("kodim03\n");
    fast_comp_im comp_im3 = encode_image(im3);
    printf("kodim04\n");
    fast_comp_im comp_im4 = encode_image(im4);
    printf("kodim05\n");
    fast_comp_im comp_im5 = encode_image(im5);
    printf("kodim06\n");
    fast_comp_im comp_im6 = encode_image(im6);
    printf("kodim07\n");
    fast_comp_im comp_im7 = encode_image(im7);
    printf("kodim08\n");
    fast_comp_im comp_im8 = encode_image(im8);
    printf("kodim09\n");
    fast_comp_im comp_im9 = encode_image(im9);
    printf("kodim10\n");
    fast_comp_im comp_im10 = encode_image(im10);
    printf("kodim11\n");
    fast_comp_im comp_im11 = encode_image(im11);
    printf("kodim12\n");
    fast_comp_im comp_im12 = encode_image(im12);
    printf("kodim13\n");
    fast_comp_im comp_im13 = encode_image(im13);
    printf("kodim14\n");
    fast_comp_im comp_im14 = encode_image(im14);
    printf("kodim15\n");
    fast_comp_im comp_im15 = encode_image(im15);
    printf("kodim16\n");
    fast_comp_im comp_im16 = encode_image(im16);
    printf("kodim17\n");
    fast_comp_im comp_im17 = encode_image(im17);
    printf("kodim18\n");
    fast_comp_im comp_im18 = encode_image(im18);
    printf("kodim19\n");
    fast_comp_im comp_im19 = encode_image(im19);
    printf("kodim20\n");
    fast_comp_im comp_im20 = encode_image(im20);
    printf("kodim21\n");
    fast_comp_im comp_im21 = encode_image(im21);
    printf("kodim22\n");
    fast_comp_im comp_im22 = encode_image(im22);
    printf("kodim23\n");
    fast_comp_im comp_im23 = encode_image(im23);
    printf("kodim24\n");
    fast_comp_im comp_im24 = encode_image(im24);

    gettimeofday(&tv2, NULL);
    gettimeofday(&tv3, NULL);
    image decoded_im = decode_image(comp_im);
    image decoded_im2 = decode_image(comp_im2);
    image decoded_im3 = decode_image(comp_im3);
    image decoded_im4 = decode_image(comp_im4);
    image decoded_im5 = decode_image(comp_im5);
    image decoded_im6 = decode_image(comp_im6);
    image decoded_im7 = decode_image(comp_im7);
    image decoded_im8 = decode_image(comp_im8);
    image decoded_im9 = decode_image(comp_im9);
    image decoded_im10 = decode_image(comp_im10);
    image decoded_im11 = decode_image(comp_im11);
    image decoded_im12 = decode_image(comp_im12);
    image decoded_im13 = decode_image(comp_im13);
    image decoded_im14 = decode_image(comp_im14);
    image decoded_im15 = decode_image(comp_im15);
    image decoded_im16 = decode_image(comp_im16);
    image decoded_im17 = decode_image(comp_im17);
    image decoded_im18 = decode_image(comp_im18);
    image decoded_im19 = decode_image(comp_im19);
    image decoded_im20 = decode_image(comp_im20);
    image decoded_im21 = decode_image(comp_im21);
    image decoded_im22 = decode_image(comp_im22);
    image decoded_im23 = decode_image(comp_im23);
    image decoded_im24 = decode_image(comp_im24);
    gettimeofday(&tv4, NULL);



    save_png(decode_image(comp_im), "./raw/test/decoded/decoded_im");
    save_png(decode_image(comp_im2), "./raw/test/decoded/decoded_im2");
    save_png(decode_image(comp_im3), "./raw/test/decoded/decoded_im3");
    save_png(decode_image(comp_im4), "./raw/test/decoded/decoded_im4");
    save_png(decode_image(comp_im5), "./raw/test/decoded/decoded_im5");
    save_png(decode_image(comp_im6), "./raw/test/decoded/decoded_im6");
    save_png(decode_image(comp_im7), "./raw/test/decoded/decoded_im7");
    save_png(decode_image(comp_im8), "./raw/test/decoded/decoded_im8");
    save_png(decode_image(comp_im9), "./raw/test/decoded/decoded_im9");
    save_png(decode_image(comp_im10), "./raw/test/decoded/decoded_im10");
    save_png(decode_image(comp_im11), "./raw/test/decoded/decoded_im11");
    save_png(decode_image(comp_im12), "./raw/test/decoded/decoded_im12");
    save_png(decode_image(comp_im13), "./raw/test/decoded/decoded_im13");
    save_png(decode_image(comp_im14), "./raw/test/decoded/decoded_im14");
    save_png(decode_image(comp_im15), "./raw/test/decoded/decoded_im15");
    save_png(decode_image(comp_im16), "./raw/test/decoded/decoded_im16");
    save_png(decode_image(comp_im17), "./raw/test/decoded/decoded_im17");
    save_png(decode_image(comp_im18), "./raw/test/decoded/decoded_im18");
    save_png(decode_image(comp_im19), "./raw/test/decoded/decoded_im19");
    save_png(decode_image(comp_im20), "./raw/test/decoded/decoded_im20");
    save_png(decode_image(comp_im21), "./raw/test/decoded/decoded_im21");
    save_png(decode_image(comp_im22), "./raw/test/decoded/decoded_im22");
    save_png(decode_image(comp_im23), "./raw/test/decoded/decoded_im23");
    save_png(decode_image(comp_im24), "./raw/test/decoded/decoded_im24");

    double elapsedTime = (tv2.tv_sec - tv.tv_sec) * 1000.0;      // sec to ms
    elapsedTime += (double) (tv2.tv_usec - tv.tv_usec) / 1000.0;
    printf("Encode took %f milliseconds \n", elapsedTime);
    double elapsedTimeDecode = (tv4.tv_sec - tv3.tv_sec) * 1000.0;      // sec to ms
    elapsedTimeDecode += (double) (tv4.tv_usec - tv3.tv_usec) / 1000.0;
    printf("Decode took %f milliseconds \n", elapsedTimeDecode);
}



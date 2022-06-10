# Simple Speedy Single-Pass Lossless Image Compression

_Note: * marks a citation is included for more details_
## Overview

- Some computer vision applications utilize cloud computing to handle large amounts of data*
- One bottleneck of this utilization is upload speed
- Lossless image compression is a tradeoff between speed (of encoding and decoding) and compression ratio
- Most popular current methods (PNG, WebP, FLIF) focus on the latter at expense of speed
- Real-time applications may prioritize low-latency over optimized compression
- Need for a fast image compression algorithm
- Aim to build a significantly quicker image compression method with decent/comparable compression ability

## Approach
In order to achieve significant speedup, I tried to combine multiple lossless encoding methods that can all be done in one pass of the image. As a result,
both encoding and decoding the image are in linear time.
## Compression Methods
The original size in bytes of an rgb (24-bit depth) image is the number of pixels in the image multiplied by 3 (one byte for each of r,g,b). In order to losslessly compress an image, we must find ways to store pixels in a fewer number of bytes than 3. Below are the methods I used to encode these pixels. Each encoding method is tagged with a 2-bit code in the compressed format for decompression purposes.
### Run-Length Encoding
Subsequent pixels with the same r,g,b values as the last pixel are saved as a "run". For example, (255,255,255), (255,255,255), (255,255,255) is encoded as (255,255,255), 2, where 2 is the number of repeat values. In order to maximize compression potential, A maximum run value of 64 is used as it can be stored in one byte of data, as 2 bits are used for the tag leaving us with 6 bits remaining to store data. Thus, any pixels that are stored as runs instead of the original pixel can each compress either two or three bytes of data. (two for the first pixel of the run, three for each subsequent pixel up to 64).

### Delta Compression
Rather than storing the original pixel, we can store pixels as a delta from the previous pixel. This can save space when the difference between subsequent pixels is relatively small, as small differences can be stored in fewer bits than the original color value (8 bits). If the difference in each of the r,g,b values on a subsequent pixel is within a total of a range of 4 (2 bits), we can encode that pixel in a single byte (2 bits for each delta), resulting in a savings of two bytes for each pixel where this is the case. Alternatively, if the difference is larger than a range of 4, we can try to use two bytes to store the deltas, which will still result in a savings of one byte.

### Hash-indexed lookback array
We can store a "cache" of the most recently encountered 64 unique pixel values, and use the 6 non-tag bits of one byte to store an index in the cache. During compression, the index at which a pixel is stored is computed as a hash function such that decompression is quick and cache lookups are in constant time. Since each pixel encoded this way will take a total of one byte, we will save 2 bytes for each pixel.

### What if a pixel can't be stored in one of the above methods?
If none of the above encoding methods fit a pixel value, we will need to store the original rgb data in our encoding. However, it is important to note that since we are using tag bits to determine the method of encoding, we will now need to store the pixel values in more than 3 bytes since we need extra bits for the tag. Thus, we use an 8-bit tag (to stay within word boundaries) to mark that a pixel in the encoding is encoded as the original pixel data.

## Benchmark
I ran benchmarks using Kodak's PNG image library*. All images were either 768x512 or 512x768.
![image](http://r0k.us/graphics/kodak/kodim01.html)
![image](https://user-images.githubusercontent.com/37307088/173007085-167b481b-45ce-4fbf-8e97-921a60eb7852.png)
**PNG Size: 736,501 bytes

![image](https://user-images.githubusercontent.com/37307088/173007371-a736f754-d861-4d5b-9ed7-45eaa5826b1d.png)
![image](https://user-images.githubusercontent.com/37307088/173007434-e969dc7e-aede-48b5-9d9e-55ab1a0c34d8.png)
**PNG Size: 637,432 bytes



## Potential Optimizations
**Significant**
- One potential optimization that could result in significant compression ratio gains is encoding and decoding our image array in a Modified Hilbert curve* order rather than row-wise. This is because adjacent pixels in images tend to have similar rgb values, so if we can traverse our image in a "group-like" fashion while encoding, we may see more pixels that can be encoded as a run, as a delta, or as a member of the lookback array, as compared to our current row-wise traversal. Attached below is an image that represents an example of a modified Hilbert traversal path.
![image](https://user-images.githubusercontent.com/37307088/172988953-072b47c3-bf0a-4a7e-852c-e4e262f1996b.png)

**Minor**
- Likely many logic optimizations possible in the current code. Also, code is written on top of our CSE455 uwimg library which has functions (like extra checks in set_pixel()) which are not optimized for our use case.


### Notes

Many encoding method details taken from existing compression utilities including qoi* and zlib*, but code is written myself on top of uwimg library

Outside of real-time computer vision applications, speed-optimized image compression also has other niche use cases such as 3D Graphics where textures can be compressed* in memory since decompression can be done almost instantaneously

### Citations
[Cloud computing for CV](https://www.automate.org/blogs/machine-vision-users-begin-adopting-cloud-computing)

[Cloud computing for CV #2](https://www.automate.org/industry-insights/cloud-computing-machine-vision-s-newest-workhorse)

[Delta encoding for image compression](http://www.diva-portal.org/smash/get/diva2:817831/FULLTEXT01.pdf)
[QOI](https://qoiformat.org/qoi-specification.pdf)
[zlib](https://datatracker.ietf.org/doc/html/rfc1950#section-2.2 - zlib)
[Modified Hilbert curve for image compression](https://www.mdpi.com/1099-4300/23/7/836/pdf)
[Texture Compression](https://en.wikipedia.org/wiki/Texture_compression)
[Kodak PNG image library for benchmarks](http://r0k.us/graphics/kodak/)


![Uploading image.png…]()

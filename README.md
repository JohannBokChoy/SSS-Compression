# Simple Speedy Single-Pass Lossless Image Compression

## Overview

- Some computer vision applications utilize cloud computing to handle large amounts of data
- One bottleneck of this utilization is upload speed
- Lossless image compression is a tradeoff between speed (of encoding and decoding) and compression ratio
- Most popular current methods (PNG, WebP, FLIF) focus on the latter at expense of speed
- Real-time applications may prioritize low-latency over optimized compression
- Need for a fast image compression algorithm
- Aim to build a significantly quicker image compression method with decent/comparable compression ability

## Problem
In recent years, some large-scale real-time computer vision applications such as [automated quality inspection on production lines](https://www.qualitydigest.com/inside/quality-insider-article/using-computer-vision-ai-automate-inspection-031622.html) and facial recognition software have [turned to the cloud](https://www.automate.org/blogs/machine-vision-users-begin-adopting-cloud-computing) in search of  a solution to process and store large amounts of sensor data, such as Google Cloud's [Visual Inspection AI](https://cloud.google.com/blog/products/ai-machine-learning/improve-manufacturing-quality-control-with-visual-inspection-ai) and [Cloud Vision API](https://cloud.google.com/vision/docs/detecting-faces).

However, some applications may be limited by bandwidth when uploading these large amounts of data to the cloud. In order to ease this bottleneck, compression can be done on the data. However, for some real-time systems which require instantaneous analysis such as on production lines or [object detection for unmanned aerial vehicles
](https://www.researchgate.net/publication/316950367_Real-Time_Cloud-Based_Object_Detection_for_Unmanned_Aerial_Vehicles), latency is important and many popular compression methods, while optimized for size, are relatively slow. Thus, the need is there for a low-latency compression method for these real-time systems utilizing the cloud.

## Approach
In order to achieve significant speedup, I tried to combine multiple lossless encoding methods that can all be done in one pass of the image, in order to maximize compression while staying fast. As a result,
both encoding and decoding the image are in linear time.

## Compression Methods
The original size in bytes of an rgb (24-bit depth) image is the number of pixels in the image multiplied by 3 (one byte for each of r,g,b). In order to losslessly compress an image, we must find ways to store pixels in a fewer number of bytes than 3. Below are the methods I used to encode these pixels. Each encoding method is tagged with a 2-bit code in the compressed format for decompression purposes.

### Run-Length Encoding
Subsequent pixels with the same r,g,b values as the last pixel are saved as a "run". For example, (255,255,255), (255,255,255), (255,255,255) is encoded as (255,255,255), 2, where 2 is the number of repeat values (will be stored with a bias of 1 though). In order to maximize compression potential, A maximum run value of 63 is used as it can be stored in one byte of data, as 2 bits are used for the tag leaving us with 6 bits remaining to store data. Thus, any pixels that are stored as runs instead of the original pixel can each compress either two or three bytes of data. (two for the first pixel of the run, three for each subsequent pixel up to 63).

### Hash-indexed lookback array
We can store a "cache" of the most recently encountered 64 unique pixel values. Upon reaching a pixel that can't be stored as a run of a previous pixel, we can instead check if the pixel has been recently encountered in our traversal. If so, we use the 6 non-tag bits of one byte to store the pixel as an index in the cache, which is computed as a hash function  of the r,g,b values. This is such that decompression is quick and cache lookups are in constant time, preserving linearity. Since each pixel encoded this way will take a total of one byte, we will save 2 bytes for each pixel.

### [Delta Encoding](http://www.diva-portal.org/smash/get/diva2:817831/FULLTEXT01.pdf)
Rather than storing the original pixel, we can store pixels as a delta from the previous pixel. This can save space when the difference between subsequent pixels is relatively small, as small differences can be stored in fewer bits than the original color value (8 bits). If the difference in each of the r,g,b values on a subsequent pixel is within a total of a range of 4 (2 bits, either -2...1 or 0...3), we can encode that entire pixel in a single byte (2 bits for each delta), resulting in a savings of two bytes for each pixel where this is the case. Alternatively, if the difference is larger than a range of 4, we can try to use two bytes to store the deltas (with 16 - 2 = 14 bits total to store deltas, albeit unevenly), which will still result in a savings of one byte. The exact bit representation to store these deltas was taken from [QOI](https://qoiformat.org/qoi-specification.pdf). Note that for the larger deltas, the difference between red values is stored in 6 bits (covers -32....-31), but the difference between the green and blue values are encoded in just 4 bits as an offset of the red difference. My testing indicated that storing these as an offset allowed for many more pixels to be encoded this way, perhaps because it is more common for the r,g,b values in pixels t o differ by similar amounts  than it is for one value to change significantly while others stay the same.

### What if a pixel can't be stored in one of the above methods?
If none of the above encoding methods fit a pixel value, we will need to store the original rgb data in our encoding. However, it is important to note that since we are using tag bits to determine the method of encoding, we will now need to store the pixel values in more than 3 bytes since we need extra bits for the tag. Thus, we use an 8-bit tag (to stay within word boundaries) to mark that a pixel in the encoding is encoded as the original pixel data. Note that for the above methods, we must check them in order of bytes taken to maximize space-saving potential (i.e. so that a pixel that can be stored in one byte as a cache index isn't stored in two bytes as a diff)

## Benchmark
I ran benchmarks using a [Kodak PNG image library](http://r0k.us/graphics/kodak/). All images were either 768x512 or 512x768. Below are some examples from the benchmarks. Full benchmark is included in benchmark text file in github repo. Note that statistics are included on how many pixels are encoded using each of the above methods.


**kodim01** 

**PNG Size: 736,501 bytes: 1.60x compression**

**Our size: 745,676 bytes: 1.58x compression**

![image](https://user-images.githubusercontent.com/37307088/173007684-cf8ec9bc-1eb5-4fad-a5c5-92e1dd46a19e.png)

![image](https://user-images.githubusercontent.com/37307088/173160582-769045f4-c805-4027-ad4c-03a0bd0af284.png)


**kodim04** 

**PNG Size: 634,432 bytes: 1.86x compression**

**Our size: 722,254 bytes: 1.63x compression**

![image](https://user-images.githubusercontent.com/37307088/173007371-a736f754-d861-4d5b-9ed7-45eaa5826b1d.png)

![image](https://user-images.githubusercontent.com/37307088/173160468-7f583258-0438-4deb-bef0-22bc75681fce.png)

**PNG Size: 637,432 bytes**
**Our size: 835,703 bytes**

On this dataset, our method seemed to consistently compress similarly but not quite as efficiently as PNG. The compression ratio seemed to be closer for images with higher redundancy, and farther for images with lesser redundancy (second image), which makes sense, as most of our encoding methods focus on repeated (similar) pixels.
In total, encoding of these 24 images took 203.956 ms while decoding took 199.109 ms, for an average of 8.5ms per image. 
I was unable to get extensive PNG compression time benchmarks on my system using C, but a single 768x512 image were about ~100 ms to encode, which means that our preliminary data suggests that method encodes about 11-12x faster for images of this size.




## Potential Optimizations

**Significant**
- One potential optimization that could result in significant compression ratio gains is encoding and decoding our image array in a [Modified Hilbert curve](https://www.mdpi.com/1099-4300/23/7/836/pdf) or [Z-order curve](https://en.wikipedia.org/wiki/Z-order_curve) order rather than row-wise. This would be done by first mapping our 2d image data to a 1d stream in Hilbert or Z-order before encoding. Note that this would add some latency to process the image but would likely result in some compression gains, especially in "blocky" images. This is because adjacent pixels in images tend to have similar rgb values, so if we can traverse our image in a "group-like" fashion while encoding, we may see more pixels that can be encoded as a run, as a delta, or as a member of the lookback array, as compared to our current row-wise traversal, which has some issues like jumping from the end of one row to the start of the next. Note that for real-world use cases, if the images being uploaded are of the same resolution, the mapping can be stored in memory and done quickly instead of processing them for every image.More simply, we could also choose to traverse in a simpler row-wise zig-zag order.

![image](https://user-images.githubusercontent.com/37307088/172988953-072b47c3-bf0a-4a7e-852c-e4e262f1996b.png)
*Hilbert curve*
![image](https://user-images.githubusercontent.com/37307088/173155160-363d4ce4-83e5-404b-b00a-a5e442eb6692.png)
*Z-order curve*


**Minor**
- Some tweaking of the existing hash function could also reduce the number of hash collisions in our lookback array that might result in slight gains.

- Likely many logic optimizations possible in the current code. Also, code is written on top of our CSE455 uwimg library which has functions (like extra checks in set_pixel()) which are not optimized for our use case.


## Notes

- Many encoding method details inspired from existing compression utilities including [QOI](https://qoiformat.org/qoi-specification.pdf) and [zlib](https://datatracker.ietf.org/doc/html/rfc1950#section-2.2), but code and benchmarks are written myself on top of uwimg library

- In order to compile and run files, add them to the CSE455 uwimg/src directory. Also, note that load_image and image.h have been modified to store r,g,b values as unsigned chars instead of floats from 0 to 1.0 for speed purposes.

- Outside of real-time computer vision applications, speed-optimized image compression also has other niche use cases such as 3D Graphics where [textures can be compressed](https://en.wikipedia.org/wiki/Texture_compression) in memory if decompression can be done quickly


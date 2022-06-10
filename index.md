# Simple Speedy Single-Pass Lossless Image Compression

## Overview

- Some computer vision applications utilize cloud computing to handle large amounts of data
- One bottleneck of this utilization is upload speed
- Lossless image compression is a tradeoff between speed (of encoding and decoding) and compression ratio
- Most popular current methods (PNG, WebP, FLIF) focus on the latter at expense of speed
- Real-time applications may prioritize low-latency over  
- Need for a fast image compression algorithm
- Aim to build a significantly quicker image compression method with comparable compression ability

## Approach
In order to achieve significant speedup, I tried to combine multiple lossless compression methods that can all be done in one pass of the image. As a result,
both encoding and decoding the image are in linear time.
## Compression Methods
### Run-Length Encoding
### Delta Compression

## Potential Optimizations
- One potential optimization that could result in significant compression ratio gains is encoding and decoding our image array in a Modified Hilbert curve (see citations) order rather than row-wise. This is because adjacent pixels in images tend to have similar rgb values, so if we can traverse our image in a "group-like" fashion while encoding, we may see more pixels that can be encoded as a run, as a delta, or as a member of the lookback array, as compared to our current row-wise traversal.
![image](https://user-images.githubusercontent.com/37307088/172988953-072b47c3-bf0a-4a7e-852c-e4e262f1996b.png)


```markdown
Syntax highlighted code block

# Header 1
## Header 2
### Header 3

- Bulleted
- List

1. Numbered
2. List

**Bold** and _Italic_ and `Code` text

[Link](url) and ![Image](src)
```

For more details see [Basic writing and formatting syntax](https://docs.github.com/en/github/writing-on-github/getting-started-with-writing-and-formatting-on-github/basic-writing-and-formatting-syntax).

### Notes

Many encoding method details taken from existing file formats including qoi and zlib, but code is written myself

Outside of real-time computer vision applications, fast but slight image compression also has other niche use cases such as 3D Graphics where decompression can be done instantaneously which enables the utilization of compression

### Citations
Articles regarding computer vision applications utilizing cloud computing
https://www.automate.org/blogs/machine-vision-users-begin-adopting-cloud-computing
https://www.automate.org/industry-insights/cloud-computing-machine-vision-s-newest-workhorse

Compression Methods
http://www.diva-portal.org/smash/get/diva2:817831/FULLTEXT01.pdf - Delta encoding for image compression
https://qoiformat.org/qoi-specification.pdf - QOI
https://datatracker.ietf.org/doc/html/rfc1950#section-2.2 - zlib
[https://en.wikipedia.org/wiki/Hilbert_curve](https://www.mdpi.com/1099-4300/23/7/836/pdf) - Modified Hilbert curve for image compression



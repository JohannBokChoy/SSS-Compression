# Simple Speedy Single-Pass Lossless Image Compression

## Overview

- Lossless image compression is a tradeoff between speed (of encoding and decoding) and compression ratio
- Most popular current methods (PNG, WebP, FLIF) focus on the latter at expense of speed
- Some niche use cases might value significant speedup over gains in compression ratio
--“Slightly compressed” is heavily used in 3D graphics -- see ETC2, DXT, etc.
It’s useful because the size savings apply in memory, because decompression is so cheap it can be done on the fly.

It has to be lossy because you want a guaranteed fixed-rate compression ratio (eg 4x) and there’s no way to do that losslessly.
- Aim to build a significantly quicker image compression method with comparable compression ability

## Approach
In order to achieve significant speedup, I tried to combine multiple lossless compression methods that can all be done in one pass of the image. As a result,
both encoding and decoding the image are in linear time.
## Compression Methods
### Run-Length Encoding
### Delta Compression


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

### Jekyll Themes

Your Pages site will use the layout and styles from the Jekyll theme you have selected in your [repository settings](https://github.com/JohannBokChoy/SSS-Compression/settings/pages). The name of this theme is saved in the Jekyll `_config.yml` configuration file.

### Support or Contact

Having trouble with Pages? Check out our [documentation](https://docs.github.com/categories/github-pages-basics/) or [contact support](https://support.github.com/contact) and we’ll help you sort it out.

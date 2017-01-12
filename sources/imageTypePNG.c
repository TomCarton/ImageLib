// imageTypePNG.c
//
// written by Thomas CARTON
//

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "image.h"


static const unsigned int kBufferSize = 64;

struct pngout
{
    FILE *outputFile;

    uint8_t output[kBufferSize];   /* output buffer */
    size_t nout;          /* number of bytes to output */

    uint32_t crc32;       /* running CRC32 */
    uint16_t s1, s2;      /* Adler CRC sums */
};


/* CRC32 computation */
static const uint32_t crc_table[16] =
{
    0x00000000, 0x1db71064, 0x3b6e20c8, 0x26d930ac,
    0x76dc4190, 0x6b6b51f4, 0x4db26158, 0x5005713c,
    0xedb88320, 0xf00f9344, 0xd6d6a3e8, 0xcb61b38c,
    0x9b64c2b0, 0x86d3d2d4, 0xa00ae278, 0xbdbdf21c,
};


static void b8(struct pngout *s, uint8_t n);
static void le16(struct pngout *s, uint32_t n);
static void be32(struct pngout *s, uint32_t n);

static void b8s(struct pngout *s, const uint8_t *b, size_t n);



static uint32_t crc_update(uint32_t crc, uint8_t data)
{
    uint8_t tbl_idx;
    tbl_idx = crc ^ (data >> (0 * 4));
    crc = crc_table[tbl_idx & 0x0f] ^ (crc >> 4);
    tbl_idx = crc ^ (data >> (1 * 4));
    crc = crc_table[tbl_idx & 0x0f] ^ (crc >> 4);
    return crc;
}

/* adler8: Output byte n and update the CRC32 and Adler CRC */
static void adler8(struct pngout *s, uint8_t n)
{
    b8(s, n);
    s->s1 = (s->s1 + n) % 65521;
    s->s2 = (s->s2 + s->s1) % 65521;
}


/* b8: Output byte n and update the CRC32 */
static void b8(struct pngout *s, uint8_t n)
{
    s->output[s->nout++] = n;
    s->crc32 = crc_update(s->crc32, n);

    if (s->nout == kBufferSize)
    {
        fwrite(s->output, s->nout, 1, s->outputFile);
        s->nout = 0;
    }
}

/* le16: Output 16-bit n little-endian */
static void le16(struct pngout *s, uint32_t n)
{
    b8(s, n);
    b8(s, n >> 8);
}

/* be32: Output 32-bit n big-endian */
static void be32(struct pngout *s, uint32_t n)
{
    b8(s, n >> 24);
    b8(s, n >> 16);
    b8(s, n >> 8);
    b8(s, n);
}

/* b8s: Output an array of bytes */
static void b8s(struct pngout *s, const uint8_t *b, size_t n)
{
    while (n--) b8(s, *b++);
}


static void start_chunk(struct pngout *s, const char *typecode, uint32_t size)
{
    be32(s, size);
    s->crc32 = 0xFFFFFFFF;
    b8s(s, (const uint8_t *)typecode, 4);
}

static void end_chunk(struct pngout *s)
{
    be32(s, s->crc32 ^ 0xFFFFFFFF);
}


void SaveImagePNG(char *filename, unsigned char *image, unsigned int width, unsigned int height)
{
    struct pngout s;

    s.outputFile = fopen(filename, "w");
    if (s.outputFile == NULL)
        goto error;

    unsigned int bpl = 1 + 3 * width;
    s.nout = 0;

    static unsigned char head[] = "\x89PNG\x0d\x0a\x1a\x0a";
    b8s(&s, head, sizeof(head) - 1);

    // IHDR
    start_chunk(&s, "IHDR", 13);
    be32(&s, width);
    be32(&s, height);
    static uint8_t rgb[5] = {8, 2, 0, 0, 0};
    b8s(&s, rgb, sizeof(rgb));
    end_chunk(&s);

    // IDAT
    start_chunk(&s, "IDAT", 2 + height * (5 + bpl) + 4);
    b8(&s, 0x78);
    b8(&s, 0x01);

    s.s1 = 1;
    s.s2 = 0;

    fwrite(s.output, s.nout, 1, s.outputFile);
    s.nout = 0;

    for (unsigned int y = 0; y < height; ++y)
    {
        b8(&s, (y + 1) == height);
        le16(&s, bpl);
        le16(&s, ~bpl);
        adler8(&s, 0);

        for (unsigned int x = 0; x < width; ++x)
        {
            unsigned int k = (y * width + x) * 3;
            adler8(&s, image[k + 0]);
            adler8(&s, image[k + 1]);
            adler8(&s, image[k + 2]);
        }
    }

    be32(&s, (s.s2 << 16) + s.s1);
    end_chunk(&s);

    // IEND
    start_chunk(&s, "IEND", 0);
    end_chunk(&s);
    
    fwrite(s.output, s.nout, 1, s.outputFile);
    s.nout = 0;

    fclose(s.outputFile);
    return;

error:
    fprintf(stderr, "Failed to write image file '%s'\n", filename);
    exit(-1);
}

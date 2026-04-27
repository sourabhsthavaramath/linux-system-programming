/************************************************
 * PROGRAM       : bmp_display.c
 *
 * FUNCTIONALITY :
 * Displays a 32-bit BMP image on Linux framebuffer
 *
 * CATEGORY      : Advanced Linux System Programming
 * MODULE        : Framebuffer / Device Programming
 *
 * DESCRIPTION   :
 * Reads a 32-bit BMP image file and displays it
 * directly on the screen using framebuffer (/dev/fb0).
 *
 * SYSTEM CALLS  :
 * open(), read(), lseek(), mmap(), ioctl(), close()
 *
 * USAGE         :
 * ./bmp_display image.bmp
 *
 * AUTHOR        : Sourabh B Sthavaramath
 ************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <stdint.h>

/************************************************
 * FUNCTION_NAME : main
 * @begin        : Entry point of the program
 * @note         :
 *   1. Open framebuffer device (/dev/fb0)
 *   2. Retrieve screen information using ioctl
 *   3. Open BMP file and read header data
 *   4. Validate resolution and bits per pixel
 *   5. Map framebuffer memory using mmap
 *   6. Copy BMP pixel data to framebuffer
 *
 * @parameters   :
 *   int argc     : Argument count
 *   char *argv[] : BMP file name
 *
 * @return       :
 *   0 on success
 *  -1 on failure
 ************************************************/

int main(int argc, char *argv[])
{
    int fbFD, bmpFD;
    struct fb_fix_screeninfo fix_info;
    struct fb_var_screeninfo var_info;
    uint32_t *fb_ptr;
    uint32_t pixel;

    /* Step 1: Validate arguments */
    if (argc != 2)
    {
        printf("Usage: %s <32-bit BMP file>\n", argv[0]);
        return -1;
    }

    /* Step 2: Open framebuffer device */
    fbFD = open("/dev/fb0", O_RDWR);
    if (fbFD < 0)
    {
        perror("Framebuffer open failed");
        return -1;
    }

    /* Step 3: Get fixed screen info */
    if (ioctl(fbFD, FBIOGET_FSCREENINFO, &fix_info) < 0)
    {
        perror("FBIOGET_FSCREENINFO failed");
        close(fbFD);
        return -1;
    }

    /* Step 4: Get variable screen info */
    if (ioctl(fbFD, FBIOGET_VSCREENINFO, &var_info) < 0)
    {
        perror("FBIOGET_VSCREENINFO failed");
        close(fbFD);
        return -1;
    }

    printf("Screen Resolution: %dx%d\n", var_info.xres, var_info.yres);
    printf("Bits Per Pixel  : %d\n", var_info.bits_per_pixel);

    /* Step 5: Open BMP file */
    bmpFD = open(argv[1], O_RDONLY);
    if (bmpFD < 0)
    {
        perror("BMP open failed");
        close(fbFD);
        return -1;
    }

    uint32_t offset, width, height;
    uint16_t bpp;

    /* Step 6: Read BMP header */
    lseek(bmpFD, 10, SEEK_SET);
    read(bmpFD, &offset, 4);

    lseek(bmpFD, 18, SEEK_SET);
    read(bmpFD, &width, 4);
    read(bmpFD, &height, 4);

    lseek(bmpFD, 28, SEEK_SET);
    read(bmpFD, &bpp, 2);

    printf("BMP Resolution: %dx%d\n", width, height);
    printf("BMP BPP       : %d\n", bpp);

    /* Step 7: Validate BMP format */
    if (bpp != 32)
    {
        printf("Error: Only 32-bit BMP supported\n");
        close(fbFD);
        close(bmpFD);
        return -1;
    }

    if (width > var_info.xres || height > var_info.yres)
    {
        printf("Error: BMP size exceeds screen resolution\n");
        close(fbFD);
        close(bmpFD);
        return -1;
    }

    /* Step 8: Map framebuffer memory */
    long screensize = fix_info.line_length * var_info.yres;

    fb_ptr = (uint32_t *)mmap(NULL,
                              screensize,
                              PROT_READ | PROT_WRITE,
                              MAP_SHARED,
                              fbFD,
                              0);

    if (fb_ptr == MAP_FAILED)
    {
        perror("mmap failed");
        close(fbFD);
        close(bmpFD);
        return -1;
    }

    /* Step 9: Move to pixel data */
    lseek(bmpFD, offset, SEEK_SET);

    int line_length = fix_info.line_length / 4;

    /* Step 10: Write pixels (BMP stored bottom-up) */
    for (int i = height - 1; i >= 0; i--)
    {
        for (int j = 0; j < width; j++)
        {
            read(bmpFD, &pixel, 4);
            fb_ptr[i * line_length + j] = pixel;
        }
    }

    printf("Image displayed successfully!\n");

    /* Step 11: Cleanup */
    munmap(fb_ptr, screensize);
    close(bmpFD);
    close(fbFD);

    return 0;
}
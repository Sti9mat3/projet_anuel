/* gcc qrcode.c qrcodegen.c `sdl-config --cflags --libs` -o sdl
 *
 * QR Code generator demo (C)
 * 
 * Run this command-line program with no arguments. The program
 * computes a demonstration QR Codes and print it to the console.
 * 
 * Copyright (c) Project Nayuki. (MIT License)
 * https://www.nayuki.io/page/qr-code-generator-library
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 * - The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 * - The Software is provided "as is", without warranty of any kind, express or
 *   implied, including but not limited to the warranties of merchantability,
 *   fitness for a particular purpose and noninfringement. In no event shall the
 *   authors or copyright holders be liable for any claim, damages or other
 *   liability, whether in an action of contract, tort or otherwise, arising from,
 *   out of or in connection with the Software or the use or other dealings in the
 *   Software.
 */



#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL/SDL.h>
#include "qrcode.h"


// Function prototypes
static void black_square(SDL_Surface *screen, SDL_Rect position, int size_pix);
static void doBasicDemo(void);
static void printQr(const uint8_t qrcode[], int size_screen, int size_pix);
static void pause();


// The main application program.
int main(void) {
	doBasicDemo();
	return EXIT_SUCCESS;
}



/*---- Demo suite ----*/

// Creates a single QR Code, then prints it to the console.
static void doBasicDemo(void) {
	const char *text = "name : Roger\n job : Plombier\n society : Gedimat\n City :Lyon";                // User-supplied text
	enum qrcodegen_Ecc errCorLvl = qrcodegen_Ecc_LOW;  // Error correction level
	
	// Make and print the QR Code symbol
	uint8_t qrcode[qrcodegen_BUFFER_LEN_MAX];
	uint8_t tempBuffer[qrcodegen_BUFFER_LEN_MAX];
	bool ok = qrcodegen_encodeText(text, tempBuffer, qrcode, errCorLvl,
		qrcodegen_VERSION_MIN, qrcodegen_VERSION_MAX, qrcodegen_Mask_AUTO, true);
	if (ok)
		printQr(qrcode, 340, 10);
}

// /*---- Utilities ----*/

// // Prints the given QR Code to the console.
static void printQr(const uint8_t qrcode[], int size_screen, int size_pix) {

	SDL_Surface *screen = NULL;
	SDL_Surface *rect = NULL;
	SDL_Rect position;

	if(SDL_Init(SDL_INIT_VIDEO) == -1)
	{
		fprintf(stderr, "Error in SDL init : %s\n", SDL_GetError()); 
		exit(EXIT_FAILURE);
	}


	screen = SDL_SetVideoMode(size_screen, size_screen, 8, SDL_HWSURFACE);
	SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 17, 206, 112));
	if (screen == NULL)
	{
		fprintf(stderr, "imposible to load video mode  : %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}

	SDL_WM_SetCaption("Qr code", NULL);

	int size = qrcodegen_getSize(qrcode);
	int border = 4;

	position.x = 0;
	position.y = 0;	
	for (int y = 0; y < size ; y++) {
		for (int x = 0; x < size; x++) {
			if (qrcodegen_getModule(qrcode, x, y))
			{
				position.x += size_pix;
			}else{
				black_square(screen, position, size_pix);
				position.x += size_pix;
			}
		}
		position.x = 0;
		position.y += size_pix;
	}
	SDL_SaveBMP(screen, "Qr-code.bmp");
	SDL_Flip(screen);
	pause();
	SDL_Quit();
}


void black_square(SDL_Surface *screen, SDL_Rect position, int size_pix)
{
	SDL_Surface *rect = NULL;
	rect = SDL_CreateRGBSurface(SDL_HWSURFACE, size_pix, size_pix, 8, 0, 0, 0, 0);
	if (rect == NULL)
	 {
	 	fprintf(stderr, "impossible de tracer rect : %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	 }
	 SDL_FillRect(rect, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
	 SDL_BlitSurface(rect, NULL, screen, &position);
}

void pause()
{
    int continuer = 1;
    SDL_Event event;
 
    while (continuer)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case SDL_QUIT:
                continuer = 0;
        }
    }
}

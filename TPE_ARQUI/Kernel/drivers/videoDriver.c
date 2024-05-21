#include <videoDriver.h>

static unsigned int xPos = 0;
static unsigned int yPos = 0;

struct vbe_mode_info_structure {
	uint16_t attributes;		// deprecated, only bit 7 should be of interest to you, and it indicates the mode supports a linear frame buffer.
	uint8_t window_a;			// deprecated
	uint8_t window_b;			// deprecated
	uint16_t granularity;		// deprecated; used while calculating bank numbers
	uint16_t window_size;
	uint16_t segment_a;
	uint16_t segment_b;
	uint32_t win_func_ptr;		// deprecated; used to switch banks from protected mode without returning to real mode
	uint16_t pitch;			// number of bytes per horizontal line
	uint16_t width;			// width in pixels
	uint16_t height;			// height in pixels
	uint8_t w_char;			// unused...
	uint8_t y_char;			// ...
	uint8_t planes;
	uint8_t bpp;			// bits per pixel in this mode
	uint8_t banks;			// deprecated; total number of banks in this mode
	uint8_t memory_model;
	uint8_t bank_size;		// deprecated; size of a bank, almost always 64 KB but may be 16 KB...
	uint8_t image_pages;
	uint8_t reserved0;
 
	uint8_t red_mask;
	uint8_t red_position;
	uint8_t green_mask;
	uint8_t green_position;
	uint8_t blue_mask;
	uint8_t blue_position;
	uint8_t reserved_mask;
	uint8_t reserved_position;
	uint8_t direct_color_attributes;
 
	uint32_t framebuffer;		// physical address of the linear frame buffer; write here to draw to the screen
	uint32_t off_screen_mem_off;
	uint16_t off_screen_mem_size;	// size of memory in the framebuffer but not being displayed on the screen
	uint8_t reserved1[206];
} __attribute__ ((packed));

typedef struct vbe_mode_info_structure * VBEInfoPtr;

VBEInfoPtr VBE_mode_info = (VBEInfoPtr) 0x0000000000005C00;

void putPixel(uint32_t x, uint32_t y, uint32_t color){
	uint8_t * videoPtr = (uint8_t *)((uint64_t)VBE_mode_info->framebuffer) + (VBE_mode_info->bpp / 8) * (x * VBE_mode_info->width + y);

    int blue = color & 0xFF;
    int green = (color >> 8) & 0xFF;
    int red = (color >> 16) & 0xFF;

    *videoPtr = blue;
    videoPtr++;
    *videoPtr = green;
    videoPtr++;
    *videoPtr = red;
}

int putCharIn(int character, int row, int col, int color){
	if (row >= VBE_mode_info->height || col >= VBE_mode_info->width) {
        return 0;
    }
    switch (character) {
        case '\n':
            enter();
            return 0;
        case '\b':
            backspace();
            return 0;
    }
    while (row % CHAR_HEIGHT != 0) {
        row++;
    }
    while (col % CHAR_WIDTH != 0) {
        col++;
    }
    unsigned char *bitMap = charBitmap(character);
    for (int i = 0; i < CHAR_HEIGHT; i++) {
        for (int j = 0; j < CHAR_WIDTH; j++) {
            unsigned int point = ((bitMap[i] >> j) & 0x01);
            if (point == 0) {
                putPixel(row + i, col + CHAR_WIDTH - j, BLACK);
            } else {
                putPixel(row + i, col + CHAR_WIDTH - j, color);
            }
        }
    }
    return 1;
}


void putChar(int character, int color) {
    int ret = putCharIn(character, yPos, xPos, color);
    if (ret) {
        xPos += CHAR_WIDTH;
    }
    updateCoordinates(); //Actualiza la posicon de la pantalla
}

void enter() {
    xPos = 0;
    yPos += CHAR_HEIGHT;
    updateCoordinates();
    return;
}

void backspace() {
    if (xPos == 0 && yPos == 0) {
        return;
    }
    if (xPos == 0) {
        yPos -= CHAR_HEIGHT;
        xPos = VBE_mode_info->width;
    }
    xPos -= CHAR_WIDTH;
    for (int i = 0; i < CHAR_HEIGHT; i++) {
        for (int j = 0; j < CHAR_WIDTH; j++) {
            putPixel(yPos + i, xPos + j, BLACK); //black
        }
    }
    return;
}

void clearScreen() {
    for (int i = 0; i < VBE_mode_info->height; i++) {
        for (int j = 0; j < VBE_mode_info->width; j++) {
            putPixel(i, j, BLACK);
        }
    }
    xPos = 0;
    yPos = 0;
}

void updateCoordinates() {
    if (xPos >= VBE_mode_info->width) { //si x alcanza el ancho de la pantalla paso a la siguiente linea
        xPos = 0;
        yPos += CHAR_HEIGHT;
    }
    if (yPos >= VBE_mode_info->height) { //si y alcanza el largo de la pantalla borra una linea y mueve todo hacia arriba
        yPos = VBE_mode_info->height - CHAR_HEIGHT;
		int size = (VBE_mode_info->bpp / 8);
        int length = (VBE_mode_info->width * VBE_mode_info->height * size) - ((size * VBE_mode_info->width) * 16);
    	memcpy((void *) (uint64_t)(VBE_mode_info->framebuffer), (void *) (uint64_t)(VBE_mode_info->framebuffer + (size * VBE_mode_info->width) * CHAR_HEIGHT), length);
    	for (int i = yPos; i < yPos + CHAR_HEIGHT; i++) {
        	for (int j = 0; j < VBE_mode_info->width; j++) {
            	putPixel(i, j, BLACK);
       	 	}
		}
    }
}

//Imprime a partir de una posicion determinada, indicada por las coordenadas (row, col)
void printIn(char *string, int row, int col, int color) {
    int length = strlen(string);
        for (int i = 0; i < length; i++) {
            if (putCharIn(string[i], row, col, color)) {
                col += CHAR_WIDTH;
                if (col >= WIDTH) {
                    row += CHAR_HEIGHT;
                    col = 0;
                }
                if (row >= HEIGHT) {
                    row = 0;
                }
            }
        }
}
//Imprime a partir de la posicion actual del cursor (xPos, yPos)
void print(char *string, int color){
    int length = strlen(string);
    for (int i = 0; i < length; i++) {
            putChar(string[i], color);
        }
}

void println(char *string, int color) {
    print(string, color);
    print("\n", BLACK);
}

int strlen(char *string) {
    int length = 0;
    while (*string != 0) {
        length++;
        string++;
    }
    return length;
}

// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <naiveConsole.h>


static char buffer[64] = { '0' };
static uint8_t * const video = (uint8_t*)0xB8000;
static uint8_t * currentVideo = (uint8_t*)0xB8000;
static const uint32_t width = 80;
static const uint32_t height = 25 ;

void ncPrint(const char * string)
{
	int i;

	for (i = 0; string[i] != 0; i++)
		ncPrintChar(string[i]);
}

void ncPrintChar(char character)
{
	*currentVideo = character;
	currentVideo += 2;
}

void ncNewline()
{
	do
	{
		ncPrintChar(' ');
	}
	while((uint64_t)(currentVideo - video) % (width * 2) != 0);
}

void ncPrintDec(uint64_t value)
{
	ncPrintBase(value, 10);
}

void ncPrintHex(uint64_t value)
{
	ncPrintBase(value, 16);
}

void ncPrintBin(uint64_t value)
{
	ncPrintBase(value, 2);
}

void ncPrintBase(uint64_t value, uint32_t base)
{
    uintToBase(value, buffer, base);
    ncPrint(buffer);
}

void ncClear()
{
	int i;

	for (i = 0; i < height * width; i++)
		video[i * 2] = ' ';
	currentVideo = video;
}

uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base)
{
	char *p = buffer;
	char *p1, *p2;
	uint32_t digits = 0;

	//Calculate characters for each digit
	do
	{
		uint32_t remainder = value % base;
		*p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
		digits++;
	}
	while (value /= base);

	// Terminate string in buffer.
	*p = 0;

	//Reverse string in buffer.
	p1 = buffer;
	p2 = p - 1;
	while (p1 < p2)
	{
		char tmp = *p1;
		*p1 = *p2;
		*p2 = tmp;
		p1++;
		p2--;
	}

	return digits;
}

void uintToBaseN(uint64_t num, char* buffer, int base) {
	if (base < 2 || base > 36){
		return;
	}

    uint64_t digits = 0;
    int pos = MAX_DIGITS - 2;
    //Calculate characters for each digit
    do {
        uint64_t remainder = num % base;
        if ((pos + 1) % 5 == 0) {
            buffer[pos--] = ' ';
        }
        buffer[pos--] = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
        digits++;
    } while (num /= base);

    while (pos >= 0) {
        if ((pos + 1) % 5 == 0) {
            buffer[pos--] = ' ';
        }
        buffer[pos--] = '0';
    }
    buffer[MAX_DIGITS - 1] = 0;
}

uint64_t turnToBaseN(uint64_t value, int base, char *buffer, int bufferLength) {
    if (base < 2 || base > 26) return -1;

    uint64_t digits = 0;
    int pos = bufferLength - 2;
    //Calculate characters for each digit
    do {
        uint64_t remainder = value % base;
        if ((pos + 1) % 5 == 0) {
            buffer[pos--] = ' ';
        }
        buffer[pos--] = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
        digits++;
    } while (value /= base);

    while (pos >= 0) {
        if ((pos + 1) % 5 == 0) {
            buffer[pos--] = ' ';
        }
        buffer[pos--] = '0';
    }
    buffer[bufferLength - 1] = 0;
    return digits;
}

void printCharColor(uint8_t character, uint8_t attribute){ //aca avanza las 2 dir previamente mencionadas
    *currentVideo++ = character; //1ero carga en la dir de video el caracter y despues avanza 
    *currentVideo++ = attribute ; 
}

void printStringColor(const char * string, uint16_t attribute){
    for(int i=0; string[i]!=0; i++){
        printCharColor(string[i], attribute);
    }
}
#include "blit.h"
#include "string.h"

typedef enum{
	KEY_INTCODE_SHIFT = 1,
	KEY_INTCODE_ALPHA = 2,
	KEY_INTCODE_UP = 3,
	KEY_INTCODE_RIGHT = 4,
	KEY_INTCODE_MODE = 5,
	KEY_INTCODE_ABS = 6,
	KEY_INTCODE_X3 = 7,
	KEY_INTCODE_LEFT = 8,
	KEY_INTCODE_DOWN = 9,
	KEY_INTCODE_X1 = 10,
	KEY_INTCODE_LOG = 11,
	KEY_INTCODE_FRAC = 12,
	KEY_INTCODE_SQRT = 13,
	KEY_INTCODE_X2 = 14,
	KEY_INTCODE_POWER = 15,
	KEY_INTCODE_LOG10 = 16,
	KEY_INTCODE_LN = 17,
	KEY_INTCODE_NEG = 18,
	KEY_INTCODE_DEG = 19,
	KEY_INTCODE_HYP = 20,
	KEY_INTCODE_SIN = 21,
	KEY_INTCODE_COS = 22,
	KEY_INTCODE_TAN = 23,
	KEY_INTCODE_RCL = 24,
	KEY_INTCODE_ENG = 25,
   KEY_INTCODE_BRACL = 26,
	KEY_INTCODE_BRACR = 27,
	KEY_INTCODE_SD = 28,
	KEY_INTCODE_MPLUS = 29,
	KEY_INTCODE_7 = 30,
	KEY_INTCODE_8 = 31,
	KEY_INTCODE_9 = 32,
	KEY_INTCODE_DEL = 33,
	KEY_INTCODE_AC = 34,
	KEY_INTCODE_4 = 35,
	KEY_INTCODE_5 = 36,
	KEY_INTCODE_6 = 37,
	KEY_INTCODE_MUL = 38,
	KEY_INTCODE_DIV = 39,
	KEY_INTCODE_1 = 40,
	KEY_INTCODE_2 = 41,
	KEY_INTCODE_3 = 42,
	KEY_INTCODE_PLUS = 43,
	KEY_INTCODE_MINUS = 44,
	KEY_INTCODE_0 = 45,
	KEY_INTCODE_DOT = 46,
	KEY_INTCODE_10E = 47,
	KEY_INTCODE_ANS = 48,
	KEY_INTCODE_EQUAL = 49
}KEY_INTCODE;

void insert(char c);
void insert(char* s);
void pos_shift(int pos);

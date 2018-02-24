#include <stdint.h>

#define VCCSTATE SSD1331_SWITCHCAPVCC
#define OLED_WIDTH     96
#define OLED_HEIGHT    64
#define WIDTH 96
#define HEIGHT 64

#define RGB(R,G,B)  (((R >> 3) << 11) | ((G >> 2) << 5) | (B >> 3))
enum Color {
	BLACK     = RGB(  0,  0,  0), // black
	GRAY      = RGB(192,192,192), // gray
	WHITE     = RGB(255,255,255), // white
	RED       = RGB(255,  0,  0), // red
	PINK      = RGB(255,192,203), // pink
	YELLOW    = RGB(255,255,  0), // yellow
	GOLDEN    = RGB(255,215,  0), // golden
	BROWN     = RGB(128, 42, 42), // brown
	BLUE      = RGB(  0,  0,255), // blue
	CYAN      = RGB(  0,255,255), // cyan
	GREEN     = RGB(  0,255,  0), // green
	PURPLE    = RGB(160, 32,240), // purple
};
#define DRAW_LINE                       0x21
#define DRAW_RECTANGLE                  0x22
#define COPY_WINDOW                     0x23
#define DIM_WINDOW                      0x24
#define CLEAR_WINDOW                    0x25
#define FILL_WINDOW                     0x26
#define DISABLE_FILL                    0x00
#define ENABLE_FILL                     0x01
#define CONTINUOUS_SCROLLING_SETUP      0x27
#define DEACTIVE_SCROLLING              0x2E
#define ACTIVE_SCROLLING                0x2F

#define SET_COLUMN_ADDRESS              0x15
#define SET_ROW_ADDRESS                 0x75
#define SET_CONTRAST_A                  0x81
#define SET_CONTRAST_B                  0x82
#define SET_CONTRAST_C                  0x83
#define MASTER_CURRENT_CONTROL          0x87
#define SET_PRECHARGE_SPEED_A           0x8A
#define SET_PRECHARGE_SPEED_B           0x8B
#define SET_PRECHARGE_SPEED_C           0x8C
#define SET_REMAP                       0xA0
#define SET_DISPLAY_START_LINE          0xA1
#define SET_DISPLAY_OFFSET              0xA2
#define NORMAL_DISPLAY                  0xA4
#define ENTIRE_DISPLAY_ON               0xA5
#define ENTIRE_DISPLAY_OFF              0xA6
#define INVERSE_DISPLAY                 0xA7
#define SET_MULTIPLEX_RATIO             0xA8
#define DIM_MODE_SETTING                0xAB
#define SET_MASTER_CONFIGURE            0xAD
#define DIM_MODE_DISPLAY_ON             0xAC
#define DISPLAY_OFF                     0xAE
#define NORMAL_BRIGHTNESS_DISPLAY_ON    0xAF
#define POWER_SAVE_MODE                 0xB0
#define PHASE_PERIOD_ADJUSTMENT         0xB1
#define DISPLAY_CLOCK_DIV               0xB3
#define SET_GRAy_SCALE_TABLE            0xB8
#define ENABLE_LINEAR_GRAY_SCALE_TABLE  0xB9
#define SET_PRECHARGE_VOLTAGE           0xBB

#define SET_V_VOLTAGE                   0xBE


extern void OledInit();
extern void SSD1331_draw_point(int x, int y, unsigned short hwColor) ;
extern void SetWindow(int x, int y, int w, int h);
extern void SSD1331_clear();
extern int sent;
extern void ShiftByte(uint8_t dat);
extern void command(uint8_t cmd);

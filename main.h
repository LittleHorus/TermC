
#define DISPLAY_POWER_DOWN GPIO_ResetBits(GPIOB, GPIO_Pin_12)
#define DISPLAY_POWER_UP GPIO_SetBits(GPIOB, GPIO_Pin_12)

#define SH_CLK_HIGH (GPIO_SetBits(GPIOB, GPIO_Pin_6))
#define SH_CLK_LOW (GPIO_ResetBits(GPIOB, GPIO_Pin_6))
#define SH_STB_HIGH (GPIO_SetBits(GPIOB, GPIO_Pin_7))
#define SH_STB_LOW (GPIO_ResetBits(GPIOB, GPIO_Pin_7))
#define SH_DATA_HIGH (GPIO_SetBits(GPIOB, GPIO_Pin_9))
#define SH_DATA_LOW (GPIO_ResetBits(GPIOB, GPIO_Pin_9))
#define SH_OE_ON (GPIO_ResetBits(GPIOB, GPIO_Pin_8))//8
#define SH_OE_OFF (GPIO_SetBits(GPIOB, GPIO_Pin_8))
#define SH_CLEAR_ON (GPIO_ResetBits(GPIOB, GPIO_Pin_5))//5
#define SH_CLEAR_OFF (GPIO_SetBits(GPIOB, GPIO_Pin_5))

#define DIG_1_ON (GPIO_SetBits(GPIOB, GPIO_Pin_4))
#define DIG_1_OFF (GPIO_ResetBits(GPIOB, GPIO_Pin_4))
#define DIG_2_ON (GPIO_SetBits(GPIOB, GPIO_Pin_3))
#define DIG_2_OFF (GPIO_ResetBits(GPIOB, GPIO_Pin_3))
#define DIG_3_ON (GPIO_SetBits(GPIOC, GPIO_Pin_12))
#define DIG_3_OFF (GPIO_ResetBits(GPIOC, GPIO_Pin_12))
#define DIG_4_ON (GPIO_SetBits(GPIOC, GPIO_Pin_11))
#define DIG_4_OFF (GPIO_ResetBits(GPIOC, GPIO_Pin_11))
#define COLON_ON (GPIO_SetBits(GPIOC, GPIO_Pin_10))
#define COLON_OFF (GPIO_ResetBits(GPIOC, GPIO_Pin_10))

#define ERROR_COLOR 0xFF0000
#define SKY_BLUE_COLOR 0x21C6FA
#define SPEECH_GREEN_COLOR 0x21FF09
#define GREY_COLOR 0xEDEEEC
#define BLACK_COLOR 0x000000
#define WHITE_COLOR 0xFFFFFF
#define YELLOW_COLOR 0xF4F714
#define BLUE_COLOR 0x143df7

enum DIGITS{
  dig_0 = 0xfc,
  dig_1 = 0x60,
  dig_2 = 0xda,
  dig_3 = 0xf2,
  dig_4 = 0x66,
  dig_5 = 0xb6,
  dig_6 = 0xbe,
  dig_7 = 0xe0,
  dig_8 = 0xfe,
  dig_9 = 0xf6,
  dig_null = 0x00
};



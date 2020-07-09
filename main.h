#define MENU_LEVEL_MAIN 0
#define MENU_LEVEL_SETTINGS 1
#define MENU_LEVEL_PARAMETERS 2
#define MENU_LEVEL_ACCEPT 3

#define SCREWRUN_WORKTIME_MAX 180
#define SCREWRUN_SLEEPTIME_MAX 180
#define SCREWSUPPORT_WORKTIME_MAX 180
#define SCREWSUPPORT_SLEEPTIME_MAX 600
#define FANSUPPORT_WORKTIME_MAX 180
#define FANSUPPORT_SLEEPTIME_MAX 600
#define FAN_POWER_MAX 100
#define FAN_POWER_STEP 5
#define SCREW_INC_STEP 5
#define FAN_INC_STEP 5

#define TEMP_CO_MAX 85
#define TEMP_CO_STEP 5
#define TEMP_GVS_MAX 85
#define TEMP_GVS_STEP 5

#define GET_TEMP_EXTERNAL adc_ch_array[7]
#define GET_TEMP_CO adc_ch_array[6]
#define GET_TEMP_HW adc_ch_array[5]
#define GET_TEMP_DG adc_ch_array[4]
#define GET_TEMP_SCREW adc_ch_array[3]
#define GET_TEMP_BUNKER adc_ch_array[2]
#define GET_TERMOSTATE adc_ch_array[1]
#define GET_CURRENT_SCREW adc_ch_array[0]



#define SCREW_STATE_STANDBY 0
#define SCREW_STATE_BACKWARD 2
#define SCREW_STATE_FORWARD 1
#define SCREW_BACKWARD_STOP GPIO_ResetBits(GPIOC, GPIO_Pin_3)
#define SCREW_BACKWARD_RUN GPIO_SetBits(GPIOC, GPIO_Pin_3)
#define SCREW_FORWARD_STOP GPIO_ResetBits(GPIOC, GPIO_Pin_2)
#define SCREW_FORWARD_RUN GPIO_SetBits(GPIOC, GPIO_Pin_2)

#define MANUAL 0
#define AUTO 1
#define FAN_OFF 0
#define FAN_ON 1

#define BUTTON_RESPONSE_PERIOD  1000 //100ms
#define BUTTON_UNBOUNCE         700 
#define BTN_PLUS_RESPONSE       GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_4)
#define BTN_MINUS_RESPONSE      GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_6)
#define BTN_ACCEPT_RESPONSE       GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5)
#define BTN_CANCEL_RESPONSE      GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_7)

#define PIEZO_LOW GPIO_ResetBits(GPIOA, GPIO_Pin_15)
#define PIEZO_HIGH GPIO_SetBits(GPIOA, GPIO_Pin_15)

#define DISPLAY_POWER_DOWN      GPIO_ResetBits(GPIOB, GPIO_Pin_12)
#define DISPLAY_POWER_UP        GPIO_SetBits(GPIOB, GPIO_Pin_12)

#define SH_CLK_HIGH             (GPIO_SetBits(GPIOB, GPIO_Pin_6))
#define SH_CLK_LOW              (GPIO_ResetBits(GPIOB, GPIO_Pin_6))
#define SH_STB_HIGH             (GPIO_SetBits(GPIOB, GPIO_Pin_7))
#define SH_STB_LOW              (GPIO_ResetBits(GPIOB, GPIO_Pin_7))
#define SH_DATA_HIGH            (GPIO_SetBits(GPIOB, GPIO_Pin_9))
#define SH_DATA_LOW             (GPIO_ResetBits(GPIOB, GPIO_Pin_9))
#define SH_OE_ON                (GPIO_ResetBits(GPIOB, GPIO_Pin_8))//8
#define SH_OE_OFF               (GPIO_SetBits(GPIOB, GPIO_Pin_8))
#define SH_CLEAR_ON             (GPIO_ResetBits(GPIOB, GPIO_Pin_5))//5
#define SH_CLEAR_OFF            (GPIO_SetBits(GPIOB, GPIO_Pin_5))

#define DIG_1_ON                (GPIO_SetBits(GPIOB, GPIO_Pin_4))
#define DIG_1_OFF               (GPIO_ResetBits(GPIOB, GPIO_Pin_4))
#define DIG_2_ON                (GPIO_SetBits(GPIOB, GPIO_Pin_3))
#define DIG_2_OFF               (GPIO_ResetBits(GPIOB, GPIO_Pin_3))
#define DIG_3_ON                (GPIO_SetBits(GPIOC, GPIO_Pin_12))
#define DIG_3_OFF               (GPIO_ResetBits(GPIOC, GPIO_Pin_12))
#define DIG_4_ON                (GPIO_SetBits(GPIOC, GPIO_Pin_11))
#define DIG_4_OFF               (GPIO_ResetBits(GPIOC, GPIO_Pin_11))
#define COLON_ON                (GPIO_SetBits(GPIOC, GPIO_Pin_10))
#define COLON_OFF               (GPIO_ResetBits(GPIOC, GPIO_Pin_10))

#define ERROR_COLOR             0xFF0000
#define SKY_BLUE_COLOR          0x21C6FA
#define SPEECH_GREEN_COLOR      0x21FF09
#define GREY_COLOR              0xEDEEEC
#define BLACK_COLOR             0x000000
#define WHITE_COLOR             0xFFFFFF
#define YELLOW_COLOR            0xF4F714
#define BLUE_COLOR              0x143df7

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



#include "stm32f0xx_conf.h"
#include "stm32f0xx.h"
#include "main.h"
#include "ft812.h"
#include <string.h>
#include <stdint.h>
#include "stdlib.h"
#include "stdio.h"

typedef struct sRTC_struct{
		 unsigned year : 6; 
		 unsigned month : 4;
		 unsigned week : 3; 
		 unsigned date : 5;
		 unsigned hour : 5;
		 unsigned minute :6;
		 unsigned sec : 6;
}RTC_struct;

void button_port_config(void);
void tim2_init(void);
void adc_init(void);
void gpio_init(void);
void ft812_mainwindow(void);
void ft812_init(char *str1, char *str2);
void spi_init(void);
void delay_ms(uint32_t msecDelay);
void ft812_custom_font_init(void);
void ft812_mainWindowCustom(void);
void ft812_paramWindowCustom(void);
void ft812_settingsWindowCustom(void);
void ft812_acceptWindowCustom(void);
void RTC_Update(RTC_struct volatile *value);
void rtc_init(void);
void RTC_Config(void);


void ft812_userMenu(uint8_t paramMenu);
void ft812_userL1(uint8_t paramMenu);
void ft812_userL2(uint8_t paramMenu);
void ft812_userL3(uint8_t paramMenu);
void ft812_userL4(uint8_t paramMenu);
void ft812_userL5(uint8_t paramMenu);
void ft812_userL6(uint8_t paramMenu);
void ft812_userL7(uint8_t paramMenu);


GPIO_InitTypeDef GPIO_InitStructure;
ADC_InitTypeDef ADC__InitStructure;
DMA_InitTypeDef DMA__InitStructure;
RTC_InitTypeDef   RTC_InitStructure;
RTC_TimeTypeDef RTC_Time;
RTC_DateTypeDef RTC_Date;
#define MEM_PIC1 (17493+148+1000)

uint8_t regIdValue = 0;

uint16_t adc_ch_array[8];
uint32_t main_counter = 0;
uint32_t delay_ms_counter = 0;
uint16_t rtc_1sec_cnt = 0;
uint8_t temp_actualGVSLevel_value[2], temp_needGVSLevel_value[2], temp_actualCOLevel_value[2], temp_needCOLevel_value[2];

uint16_t tim16Input = 0;

float temperature[8];//4*8 = 32 
uint16_t temperature_u16[8];
extern const uint8_t fontData[17493];
extern const uint8_t MetricBlock[148];
extern const uint8_t PictT[1152];
char string[10];
char outstring[50];

//Button variables
uint16_t btnCommonCnt = 0, btnPlusCnt = 0, btnMinusCnt = 0, btnAcceptCnt = 0, btnCancelCnt = 0;
uint8_t btnPlusState = 0, btnMinusState = 0, btnAcceptState = 0, btnCancelState = 0;
uint8_t btnPlusPreState = 0, btnMinusPreState = 0, btnAcceptPreState = 0, btnCancelPreState = 0;
uint8_t btnPlusShortPressEventCnt = 0, btnMinusShortPressEventCnt = 0, btnAcceptShortPressEventCnt = 0, btnCancelShortPressEventCnt = 0;
uint16_t btnPlusLongPressTimeCnt = 0, btnMinusLongPressTimeCnt = 0, btnAcceptLongPressTimeCnt = 0, btnCancelLongPressTimeCnt = 0;
uint8_t btnPlusLongPressEvent = 0, btnMinusLongPressEvent = 0, btnAcceptLongPressEvent = 0, btnCancelLongPressEvent = 0;
uint16_t piezoShortTimerCnt = 0, piezoShortCnt = 0; 

//menu variables
uint8_t menuLevel = 0;//0 - MainMenu, 1 - SettingsMenu, 2 - ParametersMenu, 3 - AcceptMenu
uint16_t displayRefreshTime = 0;
uint8_t userMenuLevelPosition = 0, paramMenuLevelPosition = 0;
uint8_t tempParamfromInnerMenu = 0;
uint8_t tempScrewState = 0, tempPumpCOState = 0, tempPumpGVSState = 0, tempFanState = 0, tempScrewReversState = 0;
uint8_t userMenuListLen[7] = {1, 5, 4, 3, 4, 8, 2};
uint8_t tempScrewRunWorkTime = 1, tempScrewRunSleepTime = 10;
uint16_t tempScrewSupportWorkTime = 5, tempScrewSupportSleepTime = 10;
uint16_t tempFanSupportWorkTime = 5, tempFanSupportSleepTime = 10;
uint8_t tempFanPower = 0;
uint8_t tempBunkerState = 0;//0 - empty; 1 - 100%
uint8_t tempScrewStateManual;
uint8_t tempPumpCOStateManual;
uint8_t tempPumpGVSStateManual;
uint8_t tempFanStateManual;
uint8_t tempScrewReversStateManual;   
uint8_t tempTemperatureCO = 0, tempTemperatureGVS = 0, tempHystCO = 0, tempHystGVS = 0; 
//создаем экземпляр этого типа
volatile RTC_struct rtc;
struct termC_Struct{
    uint8_t termStartStop;
    uint8_t termRegime;//режим работы котла - ручной/авто
    uint8_t screwRegime;//работа шнека ручной/авто
    uint8_t screwState;
    uint8_t pumpCOState;
    uint8_t pumpGVSState;
    uint8_t screwReversState;
    uint8_t screwSpeed;//скорость работы шнека not used
    //uint8_t screwState;
    uint16_t screwCurrentLimit;//ток заклинивания шнека mA
    uint8_t fanState;//работа вентилятора
    uint8_t fanPower;
    uint8_t fanSpeed;//скорость работы вентилятора
    uint8_t tempCO_settled;//ЦО установленная температура
    uint8_t tempCO_current;//ЦО текущая температура
    uint8_t tempHW_settled;//ГВС установленная температура
    uint8_t tempHW_current;//ГВС текущая температура
    uint8_t tempControlMode;//определяет, какой из датчиков используется для контроля работы котла
    uint8_t tempControlState;//определяет стадию работы - розжиг или поддержание или тушение
    int8_t tempOutdoor;
    uint8_t tempBunker;
    uint8_t tempScrew;
    uint8_t termTermostate;// two states 0 and 1
    uint16_t screwRunWorkTime;
    uint16_t screwRunSleepTime;
    uint16_t screwSupportWorkTime;
    uint16_t screwSupportSleepTime;
    uint16_t fanSupportWorkTime;
    uint16_t fanSupportSleepTime;
    uint8_t bunkerState;
    uint8_t screwStateManual;
    uint8_t pumpCOStateManual;
    uint8_t pumpGVSStateManual;
    uint8_t fanStateManual;
    uint8_t screwReversStateManual;
    uint8_t temperatureCO;
    uint8_t temperatureGVS;
    uint8_t hystCO;
    uint8_t hystGVS;

}termStruct;



void fan_control(uint8_t fanSpeed){
  
  //
  
}
void TIM16_IRQHandler(void) {
  if(TIM_GetITStatus(TIM16, TIM_IT_CC1) == SET) 
  {
    /* Clear TIM16 Capture compare interrupt pending bit */
    TIM_ClearITPendingBit(TIM16, TIM_IT_CC1);
    
    tim16Input = TIM_GetCapture1(TIM16);
    

  }
}

void tim_input(void){
  GPIO_InitTypeDef GPIO_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  TIM_ICInitTypeDef  TIM_ICInitStructure;
  /* TIM16 clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM16, ENABLE);

  /* GPIOB clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
  
  /* TIM16 channel 1 pin (PB.8) configuration */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /* Connect TIM pin to AF2 */
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_2);
  
  /* Enable the TIM1 global Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM16_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);  
  
  TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  TIM_ICInitStructure.TIM_ICFilter = 0x0;

  TIM_ICInit(TIM16, &TIM_ICInitStructure);
  
  /* TIM enable counter */
  TIM_Cmd(TIM16, ENABLE);

  /* Enable the CC1 Interrupt Request */
  TIM_ITConfig(TIM16, TIM_IT_CC1, ENABLE);  
}

/******************************************************************************/
int main(){
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
  //rtc_init();
  //RTC_Update(&rtc);
  RTC_Config();
  adc_init(); 
  gpio_init();
  spi_init();
  tim2_init();
  tim_input();
  
  DISPLAY_POWER_UP;
  ft812_init("Initialization", "please, waiting...");
  delay_ms(1000);
  //ft812_mainwindow();
  ft812_custom_font_init();
  ft812_mainWindowCustom();
  //ft812_paramWindowCustom();
  termStruct.termStartStop = 0;
  termStruct.termRegime = MANUAL;
  termStruct.screwRegime = MANUAL;
  termStruct.screwCurrentLimit = 3000;//3A
  termStruct.fanState = FAN_OFF;
  termStruct.fanSpeed = 10;//10%
  termStruct.tempCO_settled = 60;//in degree
  termStruct.tempCO_current = 0;
  termStruct.tempHW_settled = 60;
  termStruct.tempHW_current = 0;
  termStruct.tempControlMode = 0;
  termStruct.tempControlState = 0;
  termStruct.tempScrew = 0;
  termStruct.screwState = 0;
  termStruct.pumpCOState = 0;
  termStruct.pumpGVSState = 0;
  termStruct.fanState = 0;
  termStruct.screwReversState = 0; 
  termStruct.screwRunSleepTime = 10;
  termStruct.screwRunWorkTime = 5;
  termStruct.screwSupportSleepTime = 60;
  termStruct.screwSupportWorkTime = 5;
  termStruct.fanSupportSleepTime = 60;
  termStruct.fanSupportWorkTime = 10;
  termStruct.fanPower = 0;
  termStruct.bunkerState = 0;//empty as default
  termStruct.screwStateManual = 0;
  termStruct.pumpCOStateManual = 0;
  termStruct.pumpGVSStateManual = 0;
  termStruct.fanStateManual = 0;
  termStruct.screwReversStateManual = 0;
  termStruct.hystCO = 1;
  termStruct.hystGVS = 1;
  termStruct.temperatureCO = 45;
  termStruct.temperatureGVS = 45;
  
  while(1){
    temp_actualGVSLevel_value[0] = adc_ch_array[5];
    temp_actualGVSLevel_value[1] = adc_ch_array[5];

    temp_needGVSLevel_value[0] = 0x30 + 6;
    temp_needGVSLevel_value[1] = 0x30 + 0;    

    temp_actualCOLevel_value[0] = adc_ch_array[6];
    temp_actualCOLevel_value[1] = adc_ch_array[6];

    temp_needCOLevel_value[0] = 0x30 + 6;
    temp_needCOLevel_value[1] = 0x30 + 0;    

    if(menuLevel == MENU_LEVEL_MAIN){
      if(displayRefreshTime == 0){
        displayRefreshTime = 200;
        ft812_mainWindowCustom();
        

      }  
      if(btnPlusShortPressEventCnt != 0){
        btnPlusShortPressEventCnt = 0;
      }      
      if(btnMinusShortPressEventCnt != 0){
        btnMinusShortPressEventCnt = 0;
      }      
      if(btnAcceptShortPressEventCnt != 0){
        menuLevel = MENU_LEVEL_SETTINGS;
        btnAcceptShortPressEventCnt = 0;
      }
      if(btnCancelShortPressEventCnt != 0){
        btnCancelShortPressEventCnt = 0;
      }
    }
    
    if(menuLevel == MENU_LEVEL_SETTINGS){
      if(displayRefreshTime == 0){
        displayRefreshTime = 200;
        ft812_userMenu(userMenuLevelPosition);
      }      
      if(btnPlusShortPressEventCnt != 0){
        
        userMenuLevelPosition += 1;
        if(userMenuLevelPosition >= 7) userMenuLevelPosition = 0;
        btnPlusShortPressEventCnt = 0;
      }      
      if(btnMinusShortPressEventCnt != 0){
        if(userMenuLevelPosition != 0)userMenuLevelPosition -= 1;
        else userMenuLevelPosition = 6;        
        btnMinusShortPressEventCnt = 0;
      }      
      if(btnAcceptShortPressEventCnt != 0){
        menuLevel = MENU_LEVEL_PARAMETERS;
        paramMenuLevelPosition = 0;
        if(userMenuLevelPosition == 0)tempParamfromInnerMenu = termStruct.termStartStop;
        if(userMenuLevelPosition == 1){//manual mode, stop auto
          termStruct.termStartStop = 0;
          
        }
        if(userMenuLevelPosition == 2){
          tempScrewRunSleepTime = termStruct.screwRunSleepTime;
          tempScrewRunWorkTime = termStruct.screwRunWorkTime;
          tempScrewSupportSleepTime = termStruct.screwSupportSleepTime;
          tempScrewSupportWorkTime = termStruct.screwSupportWorkTime;

        }
        if(userMenuLevelPosition == 3){
          tempFanSupportSleepTime = termStruct.fanSupportSleepTime;
          tempFanSupportWorkTime = termStruct.fanSupportWorkTime;
          tempFanPower = termStruct.fanPower;
          
        }
        btnAcceptShortPressEventCnt = 0;
      }      
      if(btnCancelShortPressEventCnt != 0){
        menuLevel = MENU_LEVEL_MAIN;
        btnCancelShortPressEventCnt = 0;
      }
    }
    
    if(menuLevel == MENU_LEVEL_PARAMETERS){
      if(displayRefreshTime == 0){
        displayRefreshTime = 200;
        if(userMenuLevelPosition == 0) ft812_userL1(tempParamfromInnerMenu);
        if(userMenuLevelPosition == 1) ft812_userL2(paramMenuLevelPosition);
        if(userMenuLevelPosition == 2) ft812_userL3(paramMenuLevelPosition);
        if(userMenuLevelPosition == 3) ft812_userL4(paramMenuLevelPosition);
        if(userMenuLevelPosition == 4) ft812_userL5(paramMenuLevelPosition);
        if(userMenuLevelPosition == 5) ft812_userL6(paramMenuLevelPosition);
        if(userMenuLevelPosition == 6) ft812_userL7(paramMenuLevelPosition);
      }         
      if(btnPlusShortPressEventCnt != 0){
        if(userMenuLevelPosition == 0){
          if(tempParamfromInnerMenu == 0)tempParamfromInnerMenu = 1;
          else tempParamfromInnerMenu = 0;
        }
        paramMenuLevelPosition += 1;
        if(paramMenuLevelPosition >= userMenuListLen[userMenuLevelPosition])paramMenuLevelPosition = 0;
        
        
        btnPlusShortPressEventCnt = 0;
      }      
      if(btnMinusShortPressEventCnt != 0){
        if(userMenuLevelPosition == 0){
          if(tempParamfromInnerMenu == 0)tempParamfromInnerMenu = 1;
          else tempParamfromInnerMenu = 0;
        }
        
        if(paramMenuLevelPosition != 0)paramMenuLevelPosition -= 1;
        else paramMenuLevelPosition = userMenuListLen[userMenuLevelPosition]-1;
        btnMinusShortPressEventCnt = 0;
      }
      if(btnAcceptShortPressEventCnt != 0){
        if(userMenuLevelPosition == 0){
          termStruct.termStartStop = tempParamfromInnerMenu;
          menuLevel = MENU_LEVEL_SETTINGS;
        }
        if(userMenuLevelPosition != 0){
            menuLevel = MENU_LEVEL_ACCEPT;
            tempScrewStateManual = termStruct.screwStateManual;
            tempScrewReversStateManual = termStruct.screwReversStateManual;
            tempFanStateManual = termStruct.fanStateManual;
            tempPumpCOStateManual = termStruct.pumpCOStateManual;
            tempPumpGVSStateManual = termStruct.pumpGVSStateManual;
            
            tempScrewRunWorkTime = termStruct.screwRunWorkTime;
            tempScrewRunSleepTime = termStruct.screwRunSleepTime;
            tempScrewSupportWorkTime = termStruct.screwSupportWorkTime;
            tempScrewSupportSleepTime = termStruct.screwSupportSleepTime;
            
            tempFanSupportWorkTime = termStruct.fanSupportWorkTime;
            tempFanSupportSleepTime = termStruct.fanSupportSleepTime;
            tempFanPower = termStruct.fanPower;
            
            tempTemperatureCO = termStruct.temperatureCO;
            tempTemperatureGVS = termStruct.temperatureGVS;
            tempHystCO = termStruct.hystCO;
            tempHystGVS = termStruct.hystGVS;
            
            
        }
        
        btnAcceptShortPressEventCnt = 0;
      }      
      if(btnCancelShortPressEventCnt != 0){
        menuLevel = MENU_LEVEL_SETTINGS;
        btnCancelShortPressEventCnt = 0;
      }       
    }
    
    if(menuLevel == MENU_LEVEL_ACCEPT){
      if(displayRefreshTime == 0){
          displayRefreshTime = 200;
          //ft812_acceptWindowCustom();
          if(userMenuLevelPosition == 1) ft812_userL2(paramMenuLevelPosition);  
          if(userMenuLevelPosition == 2) ft812_userL3(paramMenuLevelPosition); 
          if(userMenuLevelPosition == 3) ft812_userL4(paramMenuLevelPosition); 
          if(userMenuLevelPosition == 4) ft812_userL5(paramMenuLevelPosition); 
      }         
      if(btnPlusShortPressEventCnt != 0){
        btnPlusShortPressEventCnt = 0;
        if(userMenuLevelPosition == 1){
            if(paramMenuLevelPosition == 0){
              if(tempScrewStateManual == 0) tempScrewStateManual = 1;
              else tempScrewStateManual = 0;
            }
            if(paramMenuLevelPosition == 1){
              if(tempPumpCOStateManual == 0) tempPumpCOStateManual = 1;
              else tempPumpCOStateManual = 0;
            }
            if(paramMenuLevelPosition == 2){
              if(tempPumpGVSStateManual == 0) tempPumpGVSStateManual = 1;
              else tempPumpGVSStateManual = 0;
            }
            if(paramMenuLevelPosition == 3){
              if(tempFanStateManual == 0) tempFanStateManual = 1;
              else tempFanStateManual = 0;
            }
            if(paramMenuLevelPosition == 4){
              if(tempScrewReversStateManual == 0) tempScrewReversStateManual = 1;
              else tempScrewReversStateManual = 0;
            }            
        }
        if(userMenuLevelPosition == 2){
          if(paramMenuLevelPosition == 0){
            if(tempScrewRunWorkTime <= SCREWRUN_WORKTIME_MAX) tempScrewRunWorkTime += SCREW_INC_STEP; 
          }
          if(paramMenuLevelPosition == 1){
            if(tempScrewRunSleepTime <= SCREWRUN_SLEEPTIME_MAX)tempScrewRunSleepTime += SCREW_INC_STEP;          
          }
          if(paramMenuLevelPosition == 2){
            if(tempScrewSupportWorkTime <= SCREWSUPPORT_WORKTIME_MAX) tempScrewSupportWorkTime += SCREW_INC_STEP;
          }
          if(paramMenuLevelPosition == 3){
            if(tempScrewSupportSleepTime <= SCREWSUPPORT_SLEEPTIME_MAX) tempScrewSupportSleepTime += SCREW_INC_STEP;
          }
        }
        if(userMenuLevelPosition == 3){
          if(paramMenuLevelPosition == 0){
            if(tempFanSupportWorkTime <= FANSUPPORT_SLEEPTIME_MAX) tempFanSupportWorkTime += FAN_INC_STEP;
          }
          if(paramMenuLevelPosition == 1){
            if(tempFanSupportSleepTime <= FANSUPPORT_SLEEPTIME_MAX) tempFanSupportSleepTime += FAN_INC_STEP;
          }
          if(paramMenuLevelPosition == 2){
            if(tempFanPower <= (FAN_POWER_MAX-FAN_POWER_STEP)) tempFanPower += FAN_POWER_STEP;
          }          
        }
        if(userMenuLevelPosition == 4){
          if(paramMenuLevelPosition == 0){
            if(tempTemperatureCO <= TEMP_CO_MAX) tempTemperatureCO += TEMP_CO_STEP;
          }
          if(paramMenuLevelPosition == 1){
            if(tempHystCO <= 5) tempHystCO += 1;
          }
          if(paramMenuLevelPosition == 2){
            if(tempTemperatureGVS <= TEMP_GVS_MAX) tempTemperatureGVS += TEMP_GVS_STEP;
          }
          if(paramMenuLevelPosition == 3){
            if(tempHystGVS <= 5) tempHystGVS += 1;
          }
           
        }

      }      
      if(btnMinusShortPressEventCnt != 0){
        btnMinusShortPressEventCnt = 0;
        if(userMenuLevelPosition == 1){
            if(paramMenuLevelPosition == 0){
              if(tempScrewStateManual == 0) tempScrewStateManual = 1;
              else tempScrewStateManual = 0;
            }
            if(paramMenuLevelPosition == 1){
              if(tempPumpCOStateManual == 0) tempPumpCOStateManual = 1;
              else tempPumpCOStateManual = 0;
            }
            if(paramMenuLevelPosition == 2){
              if(tempPumpGVSStateManual == 0) tempPumpGVSStateManual = 1;
              else tempPumpGVSStateManual = 0;
            }
            if(paramMenuLevelPosition == 3){
              if(tempFanStateManual == 0) tempFanStateManual = 1;
              else tempFanStateManual = 0;
            }
            if(paramMenuLevelPosition == 4){
              if(tempScrewReversStateManual == 0) tempScrewReversStateManual = 1;
              else tempScrewReversStateManual = 0;
            }            
        }
        if(userMenuLevelPosition == 2){
          if(paramMenuLevelPosition == 0){
              if(tempScrewRunWorkTime >= SCREW_INC_STEP) tempScrewRunWorkTime -= SCREW_INC_STEP; //= 1, tempScrewRunSleepTime
              else tempScrewRunWorkTime = 0;
          }
          if(paramMenuLevelPosition == 1){
              if(tempScrewRunSleepTime >= SCREW_INC_STEP) tempScrewRunSleepTime -= SCREW_INC_STEP;
              else tempScrewRunSleepTime = 0;
          }
          if(paramMenuLevelPosition == 2){
            if(tempScrewSupportWorkTime >= SCREW_INC_STEP) tempScrewSupportWorkTime -= SCREW_INC_STEP;
            else tempScrewSupportWorkTime = 0;
          }
          if(paramMenuLevelPosition == 3){
            if(tempScrewSupportSleepTime >= SCREW_INC_STEP) tempScrewSupportSleepTime -= SCREW_INC_STEP;
            else tempScrewSupportSleepTime = 0;
          }          
        }  
        if(userMenuLevelPosition == 3){
          if(paramMenuLevelPosition == 0){
            if(tempFanSupportWorkTime >= FAN_INC_STEP) tempFanSupportWorkTime -= FAN_INC_STEP;
            else tempFanSupportWorkTime = 0;
          }
          if(paramMenuLevelPosition == 1){
            if(tempFanSupportSleepTime >= FAN_INC_STEP) tempFanSupportSleepTime -= FAN_INC_STEP;
            else tempFanSupportSleepTime = 0;
          }
          if(paramMenuLevelPosition == 2){
            if(tempFanPower >= FAN_POWER_STEP) tempFanPower -= FAN_POWER_STEP;
            else tempFanPower = 0;
          }          
        }
        if(userMenuLevelPosition == 4){
          if(paramMenuLevelPosition == 0){
            if(tempTemperatureCO >= TEMP_CO_STEP) tempTemperatureCO -= TEMP_CO_STEP;
          }
          if(paramMenuLevelPosition == 1){
            if(tempHystCO != 0) tempHystCO -= 1;
          }
          if(paramMenuLevelPosition == 2){
            if(tempTemperatureGVS >= TEMP_GVS_STEP) tempTemperatureGVS -= TEMP_GVS_STEP;
          }
          if(paramMenuLevelPosition == 3){
            if(tempHystGVS != 0) tempHystGVS -= 1;
          }
           
        }        
        
      }
      if(btnAcceptShortPressEventCnt != 0){
        menuLevel = MENU_LEVEL_PARAMETERS;
        if(userMenuLevelPosition == 1){
            if(paramMenuLevelPosition == 0)termStruct.screwStateManual = tempScrewStateManual;
            if(paramMenuLevelPosition == 1)termStruct.pumpCOStateManual = tempPumpCOStateManual;
            if(paramMenuLevelPosition == 2)termStruct.pumpGVSStateManual = tempPumpGVSStateManual;
            if(paramMenuLevelPosition == 3)termStruct.fanStateManual = tempFanStateManual;
            if(paramMenuLevelPosition == 4)termStruct.screwReversStateManual = tempScrewReversStateManual;
        }
        if(userMenuLevelPosition == 2){
            if(paramMenuLevelPosition == 0)termStruct.screwRunWorkTime = tempScrewRunWorkTime;
            if(paramMenuLevelPosition == 1)termStruct.screwRunSleepTime = tempScrewRunSleepTime;
            if(paramMenuLevelPosition == 2)termStruct.screwSupportWorkTime = tempScrewSupportWorkTime;
            if(paramMenuLevelPosition == 3)termStruct.screwSupportSleepTime = tempScrewSupportSleepTime;         
        }
        if(userMenuLevelPosition == 3){
            if(paramMenuLevelPosition == 0)termStruct.fanSupportWorkTime = tempFanSupportWorkTime;
            if(paramMenuLevelPosition == 1)termStruct.fanSupportSleepTime = tempFanSupportSleepTime;
            if(paramMenuLevelPosition == 2)termStruct.fanPower = tempFanPower;
        }
        if(userMenuLevelPosition == 4){
            if(paramMenuLevelPosition == 0)termStruct.temperatureCO = tempTemperatureCO;
            if(paramMenuLevelPosition == 1)termStruct.hystCO = tempHystCO;
            if(paramMenuLevelPosition == 2)termStruct.temperatureGVS = tempTemperatureGVS;
            if(paramMenuLevelPosition == 3)termStruct.hystGVS = tempHystGVS;           
        }
        btnAcceptShortPressEventCnt = 0;
      }      
      if(btnCancelShortPressEventCnt != 0){
        menuLevel = MENU_LEVEL_PARAMETERS;
        btnCancelShortPressEventCnt = 0;
      }      
    }
    
    
 
  }//while(1)
  
}//main()

/******************************************************************************/
void gpio_init(void){
    GPIO_InitTypeDef BASE_GPIO_InitStructure;
    //piezo
    BASE_GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
    BASE_GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    BASE_GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_1;
    BASE_GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    BASE_GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_Init(GPIOA, &BASE_GPIO_InitStructure);
    
    //termostate
    BASE_GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    BASE_GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    BASE_GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_1;
    BASE_GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    BASE_GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_Init(GPIOA, &BASE_GPIO_InitStructure);
    
    //user buttons PC4 - B1 PC5 - B3 PC6 - B2 PC7 - B4
    BASE_GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
    BASE_GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    BASE_GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_1;
    BASE_GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    BASE_GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_Init(GPIOC, &BASE_GPIO_InitStructure);    
}
/******************************************************************************/
void adc_init(void){ 
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);  

    DMA__InitStructure.DMA_BufferSize = 8;
    DMA__InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA__InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA__InitStructure.DMA_MemoryBaseAddr = (uint32_t) &adc_ch_array;
    DMA__InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA__InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA__InitStructure.DMA_Mode = DMA_Mode_Circular;//DMA_Mode_Normal
    DMA__InitStructure.DMA_PeripheralBaseAddr = (uint32_t) &ADC1->DR;
    DMA__InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA__InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA__InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_Init(DMA1_Channel1, &DMA__InitStructure);
    DMA_Cmd(DMA1_Channel1, ENABLE);
    //RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    //SYSCFG_DMAChannelRemapConfig(SYSCFG_DMARemap_ADC1, ENABLE);
      
    RCC_ADCCLKConfig(RCC_ADCCLK_PCLK_Div4);  
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

    ADC__InitStructure.ADC_ContinuousConvMode = ENABLE;
    ADC__InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC__InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
    ADC__InitStructure.ADC_Resolution = ADC_Resolution_12b;
    ADC__InitStructure.ADC_ScanDirection = ADC_ScanDirection_Upward;
    ADC_Init(ADC1, &ADC__InitStructure);
     
    ADC_ChannelConfig(ADC1, ADC_Channel_0 , ADC_SampleTime_55_5Cycles);//screws current
    ADC_ChannelConfig(ADC1, ADC_Channel_1 , ADC_SampleTime_55_5Cycles);//termostat
    ADC_ChannelConfig(ADC1, ADC_Channel_2 , ADC_SampleTime_55_5Cycles);//bunker
    ADC_ChannelConfig(ADC1, ADC_Channel_3 , ADC_SampleTime_55_5Cycles);//screw
    ADC_ChannelConfig(ADC1, ADC_Channel_4 , ADC_SampleTime_55_5Cycles);//dym gas
    ADC_ChannelConfig(ADC1, ADC_Channel_5 , ADC_SampleTime_55_5Cycles);//gvs
    ADC_ChannelConfig(ADC1, ADC_Channel_6 , ADC_SampleTime_55_5Cycles);//co
    ADC_ChannelConfig(ADC1, ADC_Channel_7 , ADC_SampleTime_55_5Cycles);//external temp
    
    ADC_GetCalibrationFactor(ADC1);
    ADC_Cmd(ADC1, ENABLE);
    
    while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_ADRDY));
    ADC_StartOfConversion(ADC1);
    ADC_DMACmd(ADC1, ENABLE);
    ADC_DMARequestModeConfig(ADC1, ADC_DMAMode_Circular);
}
/******************************************************************************/
void spi_init(void){
    GPIO_InitTypeDef SPI_GPIO_InitStructure;
    
    SPI_GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    SPI_GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    SPI_GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_1;
    SPI_GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    SPI_GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOC, &SPI_GPIO_InitStructure);

    SPI_GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    SPI_GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    SPI_GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_1;
    SPI_GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    SPI_GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOB, &SPI_GPIO_InitStructure);    
    
    SPI_GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    SPI_GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    SPI_GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;
    SPI_GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    SPI_GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_Init(GPIOB, &SPI_GPIO_InitStructure);
  
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_0);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_0);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_0);   
    
    SPI_InitTypeDef SPI_InitStructure;
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(SPI2, &SPI_InitStructure);
    
    SPI_DataSizeConfig(SPI2, SPI_DataSize_8b);
    
    SPI_Cmd(SPI2, ENABLE);
    SPI_NSSInternalSoftwareConfig(SPI2, SPI_NSSInternalSoft_Set);    

}//spi_init
/******************************************************************************/
void tim2_init(void){
  //СЂР°Р·СЂРµС€Р°РµРј С‚Р°РєС‚РёСЂРѕРІР°РЅРёРµ РјРѕРґСѓР»СЏ С‚Р°Р№РјРµСЂР° 2
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); 
  //РѕР±СЉСЏРІР»СЏРµРј РїРµСЂРµРјРµРЅРЅСѓСЋ РґР»СЏ РЅР°СЃС‚СЂРѕР№РєРё С‚Р°Р№РјРµСЂР° 2
  TIM_TimeBaseInitTypeDef TIM2_InitStructure;
  //РѕР±СЉСЏРІР»СЏРµРј РїРµСЂРµРјРµРЅРЅСѓСЋ РґР»СЏ РЅР°СЃС‚СЂРѕР№РєРё РїСЂРµСЂС‹РІР°РЅРёР№ РѕС‚ С‚Р°Р№РјРµСЂР° 2
  NVIC_InitTypeDef NVIC_TIM2_Init_Structure;
  //СѓРєР°Р·С‹РІР°РµРј РёСЃС‚РѕС‡РЅРёРє РїСЂРµСЂС‹РІР°РЅРёР№ - TIM2_IRQn (СЃРїРёСЃРѕРє РІСЃРµС… РІРѕР·РјРѕР¶РЅС‹С… РёСЃС‚РѕС‡РЅРёРєРѕРІ РЅР°С…РѕРґРёС‚СЃСЏ РІ С„Р°Р№Р»Рµ stm32f0xx.h)
  NVIC_TIM2_Init_Structure.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_TIM2_Init_Structure.NVIC_IRQChannelPriority = 0x01;//РїСЂРёРѕСЂРёС‚РµС‚ С‚РµРєСѓС‰РµРіРѕ РїСЂРµСЂС‹РІР°РЅРёСЏ, РѕС‚ 0 РґРѕ 3
  NVIC_TIM2_Init_Structure.NVIC_IRQChannelCmd = ENABLE;  
  NVIC_Init(&NVIC_TIM2_Init_Structure); 
  
  TIM2_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM2_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM2_InitStructure.TIM_Period = 99;//1MHz / (99 + 1) = 10kHz
  TIM2_InitStructure.TIM_Prescaler = 47;//48MHz / (47+1) = 1MHz OUT  
  TIM2_InitStructure.TIM_RepetitionCounter = 0;//used only with TIM1
  TIM_TimeBaseInit(TIM2, &TIM2_InitStructure);

  TIM_Cmd(TIM2, ENABLE);//Р·Р°РїСѓСЃРєР°РµРј С‚Р°Р№РјРµСЂ
  TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);//СЂР°Р·СЂРµС€Р°РµРј РїСЂРµСЂС‹РІР°РЅРёСЏ РѕС‚ С‚Р°Р№РјРµСЂР° 2 РїРѕ СЃРѕР±С‹С‚РёСЋ РїРµСЂРµРїРѕР»РЅРµРЅРёСЏ  
}
/******************************************************************************/
void TIM2_IRQHandler(void){ 
  if(TIM_GetITStatus(TIM2, TIM_IT_Update) == SET){
    main_counter++;
    if(main_counter >= 10000)main_counter = 0;//1sec
    if(delay_ms_counter != 0)delay_ms_counter--;
    if(displayRefreshTime != 0)displayRefreshTime--;
    rtc_1sec_cnt++;
    if(rtc_1sec_cnt >= 10000){
        RTC_GetTime(RTC_Format_BCD, &RTC_Time);
        RTC_GetDate(RTC_Format_BCD, &RTC_Date);
        rtc_1sec_cnt = 0;
    }
    
    btnCommonCnt++;
    if(BTN_PLUS_RESPONSE == 1)btnPlusCnt++;
    if(BTN_MINUS_RESPONSE == 1)btnMinusCnt++;
    if(BTN_ACCEPT_RESPONSE == 1)btnAcceptCnt++;
    if(BTN_CANCEL_RESPONSE == 1)btnCancelCnt++;
    
    if(btnCommonCnt >= BUTTON_RESPONSE_PERIOD){
      if(btnPlusCnt  > BUTTON_UNBOUNCE){
        btnPlusState = 1;
        if(btnPlusPreState == 0){
          btnPlusLongPressTimeCnt = 0;
        }
        else{
          if(btnPlusLongPressTimeCnt <= 15)btnPlusLongPressTimeCnt++;
          if(btnPlusLongPressTimeCnt == 15){
            btnPlusLongPressEvent = 1;
            piezoShortCnt = 1000;
          }
        }
      }
      else{
        btnPlusState = 0;
        if(btnPlusPreState == 1){
          if(btnPlusLongPressEvent == 1)btnPlusLongPressEvent = 0;
          else{
            btnPlusShortPressEventCnt = 1;
            piezoShortCnt = 1000;
          }
          btnPlusLongPressTimeCnt = 0;
        }
      }
      if(btnMinusCnt > BUTTON_UNBOUNCE){
        btnMinusState = 1;
        if(btnMinusPreState == 0){
          btnMinusLongPressTimeCnt = 0;
        }
        else{
          if(btnMinusLongPressTimeCnt <= 15)btnMinusLongPressTimeCnt++;
          if(btnMinusLongPressTimeCnt == 15){
            btnMinusLongPressEvent = 1;
            piezoShortCnt = 1000;
          }
        }
      }
      else{
        btnMinusState = 0;
        if(btnMinusPreState == 1){
          if(btnMinusLongPressEvent == 1)btnMinusLongPressEvent = 0;
          else{
            btnMinusShortPressEventCnt = 1;
            piezoShortCnt = 1000;
          }
          btnMinusLongPressTimeCnt = 0;
        }
      }
      if(btnAcceptCnt > BUTTON_UNBOUNCE){
        btnAcceptState = 1;
        if(btnAcceptPreState == 0){
          btnAcceptLongPressTimeCnt = 0;
        }
        else{
          if(btnAcceptLongPressTimeCnt <= 15)btnAcceptLongPressTimeCnt++;
          if(btnAcceptLongPressTimeCnt == 15){
            btnAcceptLongPressEvent = 1;
            piezoShortCnt = 1000;
          }
        }
      }
      else{
        btnAcceptState = 0;
        if(btnAcceptPreState == 1){
          if(btnAcceptLongPressEvent == 1)btnAcceptLongPressEvent = 0;
          else{
            btnAcceptShortPressEventCnt = 1;
            piezoShortCnt = 1000;
          }
          btnAcceptLongPressTimeCnt = 0;
        }
      }        
      if(btnCancelCnt > BUTTON_UNBOUNCE){
        btnCancelState = 1;
        if(btnCancelPreState == 0){
          btnCancelLongPressTimeCnt = 0;
        }
        else{
          if(btnCancelLongPressTimeCnt <= 15)btnCancelLongPressTimeCnt++;
          if(btnCancelLongPressTimeCnt == 15){
            btnCancelLongPressEvent = 1;
            piezoShortCnt = 1000;
          }
        }
      }
      else{
        btnCancelState = 0;
        if(btnCancelPreState == 1){
          if(btnCancelLongPressEvent == 1)btnCancelLongPressEvent = 0;
          else{
            btnCancelShortPressEventCnt = 1;
            piezoShortCnt = 1000;
          }
          btnCancelLongPressTimeCnt = 0;
        }
      } 
      
      btnCommonCnt = 0; 
      btnPlusCnt = 0; btnPlusPreState = btnPlusState;
      btnMinusCnt = 0; btnMinusPreState = btnMinusState;
      btnAcceptCnt = 0; btnAcceptPreState = btnAcceptState;
      btnCancelCnt = 0; btnCancelPreState = btnCancelState;
      
    }
    
    if(piezoShortCnt != 0){
      piezoShortCnt--;
      piezoShortTimerCnt++;
      if(piezoShortTimerCnt == 1)
        PIEZO_HIGH;
      if(piezoShortTimerCnt >= 2){
        PIEZO_LOW;
        piezoShortTimerCnt = 0;
      }
      
    }
    if(piezoShortCnt == 0){
      piezoShortCnt = 0;
      PIEZO_LOW;
    }
    
      
    
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

  }//TIM_IT_UPDATE 
}//TIM2_IRQHandler

void delay_ms(uint32_t msecDelay){
  delay_ms_counter = msecDelay;
  while(delay_ms_counter != 0);
  
}

/*******************DISPLAY****SECTION*****************************************/
/******************************************************************************/
void ft_custom_font_inter(char* string, uint8_t len){
  
  for(uint8_t ilen = 0; ilen < len; ilen++){
    if(string[ilen] >= 0xc0)outstring[ilen] = string[ilen] - 0xc0 + 34;
    if(string[ilen] <= 0x41)outstring[ilen] = string[ilen]-31;
  }
  outstring[len] = '\0';
}
void ft_custom_font_edit(char* string){
  uint8_t ilen = 0;
  while(string[ilen] != '\0'){
    if(string[ilen] >= 0xc0)outstring[ilen] = string[ilen] - 0xc0 + 34;
    if(string[ilen] <= 0x41)outstring[ilen] = string[ilen]-31; 
    if(string[ilen] == '^')outstring[ilen] = 115;
    if(string[ilen] == 'Ў')outstring[ilen] = 117;
    if(string[ilen] == '°')outstring[ilen] = 99;
    
    ilen++;
  }
  outstring[ilen] = '\0';
}

//while(text[textindex] != 0)

void ft812_init(char *str1, char *str2){
  wHostCMD(CMD_RST_PULSE);
  wHostCMD(CMD_CLKEXT);
  wHostCMD(CMD_ACTIVE);
  while(regIdValue != 0x7C){
      regIdValue = rReg8(REG_ID);
  }
  wReg16(REG_HCYCLE, 408);
  wReg16(REG_HOFFSET, 70);
  wReg16(REG_HSYNC0, 0);
  wReg16(REG_HSYNC1, 10);
  wReg16(REG_VCYCLE, 263);
  wReg16(REG_VOFFSET, 13);
  wReg16(REG_VSYNC0, 0);
  wReg16(REG_VSYNC1, 2);
  wReg8(REG_SWIZZLE, 2);
  wReg8(REG_PCLK_POL, 1);
  wReg8(REG_CSPREAD, 0);
  wReg16(REG_HSIZE, 320);
  wReg16(REG_VSIZE, 240);
    
  wReg8(REG_TOUCH_MODE, 3);	/* enable touch */
  wReg16(REG_TOUCH_RZTHRESH, 1200); /* eliminate any false touches */
       
  wReg8(REG_GPIO, 0x80);
  wReg8(REG_PCLK, 6); 
    
  wReg8(REG_ROTATE, 0); 
  //Data32 = rReg32(REG_FREQUENCY);
  wReg8(REG_PWM_DUTY, 40);
  
  wReg32(REG_TOUCH_TRANSFORM_A, 0x0000f78b);
  wReg32(REG_TOUCH_TRANSFORM_B, 0x00000427);
  wReg32(REG_TOUCH_TRANSFORM_C, 0xfffcedf8);
  wReg32(REG_TOUCH_TRANSFORM_D, 0xfffffba4);
  wReg32(REG_TOUCH_TRANSFORM_E, 0x0000f756);
  wReg32(REG_TOUCH_TRANSFORM_F, 0x0009279e);      
   
  FT8_start_cmd_burst();
  FT8_cmd_dl(CMD_DLSTART);
  FT8_cmd_dl(DL_CLEAR_RGB | 0x000000);
  FT8_cmd_dl(DL_CLEAR | CLR_COL | CLR_STN | CLR_TAG);
  
  FT8_cmd_dl(DL_COLOR_RGB | 0xffffff);
  FT8_cmd_text(160, 20, 24, 512, str1);
  FT8_cmd_text(160, 50, 21, 512, str2);
  FT8_cmd_spinner(160, 150, 0, 0);
   
  FT8_cmd_dl(DL_DISPLAY);
  FT8_cmd_dl(CMD_SWAP);
  FT8_end_cmd_burst(); /* stop writing to the cmd-fifo */
  FtCmdStart(); 
  
  
  
     
}

void ft812_custom_font_init(void){
  
  FT8_memWrite_flash_buffer(1000, MetricBlock, 148);
  FT8_memWrite_flash_buffer(1000+148, fontData, 17493);
  FT8_memWrite_flash_buffer(1000+148+17493, PictT, 1152);
  //FT8_cmd_loadimage(MEM_PIC1,FT8_OPT_NODL, PictT,sizeof(PictT)); 
  
  FT8_start_cmd_burst();
  FT8_cmd_dl(CMD_DLSTART);
  FT8_cmd_dl(DL_CLEAR_RGB | 0x000000);
  FT8_cmd_dl(DL_CLEAR | CLR_COL | CLR_STN | CLR_TAG);
  
  FT8_cmd_dl(BEGIN(BITMAPS));
  FT8_cmd_dl(DL_COLOR_RGB | 0xfffc10);
  FT8_cmd_dl(BITMAP_HANDLE(11));
  FT8_cmd_dl(BITMAP_SOURCE(1001));
  FT8_cmd_dl(BITMAP_LAYOUT(FT8_L4, 7, 21));
  FT8_cmd_dl(BITMAP_SIZE(FT8_NEAREST, FT8_REPEAT, FT8_REPEAT, 14, 21));
  
  //FT8_cmd_dl(CMD_SETBITMAP);
  FT8_cmd_setfont2(11,1000, 0);
   
  FT8_cmd_dl(BEGIN(BITMAPS));
  //FT8_cmd_dl(DL_COLOR_RGB | 0xfffc10);
  FT8_cmd_dl(BITMAP_HANDLE(1));
  FT8_cmd_dl(BITMAP_SOURCE(1000+148+17493));
  FT8_cmd_dl(BITMAP_LAYOUT(FT8_L4, 48, 48));
  FT8_cmd_dl(BITMAP_SIZE(FT8_NEAREST, FT8_BORDER, FT8_BORDER, 48, 48)); 
  //FT8_cmd_dl(VERTEX2II(10,150,1,20));

  FT8_cmd_dl(DL_DISPLAY);
  FT8_cmd_dl(CMD_SWAP);
  FT8_end_cmd_burst(); /* stop writing to the cmd-fifo */
  FtCmdStart();   
  
}
void ft812_mainWindowCustom(void){
  FT8_start_cmd_burst();
  FT8_cmd_dl(CMD_DLSTART);
  FT8_cmd_dl(DL_CLEAR_RGB | 0x000000);
  FT8_cmd_dl(DL_CLEAR | CLR_COL | CLR_STN | CLR_TAG);


  FT8_cmd_dl(DL_COLOR_RGB | GREY_COLOR);
    
  uint8_t offset = 30;
    
  FT8_cmd_line(0,0+offset,129,0+offset,2);
  FT8_cmd_line(0,138+offset,129,138+offset,2);
  FT8_cmd_line(0,1+offset,0,137+offset,2);
  FT8_cmd_line(129,1+offset,129,137+offset,2);

  FT8_cmd_line(129,0+offset,259,0+offset,2);
  FT8_cmd_line(129,138+offset,259,138+offset,2);
  FT8_cmd_line(129,1+offset,129,137+offset,2);
  FT8_cmd_line(259,1+offset,259,137+offset,2);

  FT8_cmd_line(0,139+offset,319,139+offset,2);
  FT8_cmd_line(0,139+offset,0,239,2);
  FT8_cmd_line(0,239,319,239,2);
  FT8_cmd_line(319,139+offset,319,239,2); 
    
  FT8_cmd_line(64,139+offset,64,239+offset,2);
  FT8_cmd_line(128,139+offset,128,239+offset,2);
  FT8_cmd_line(194,139+offset,194,239+offset,2);
  FT8_cmd_line(259,139+offset,259,239+offset,2);

  
  FT8_cmd_dl(DL_COLOR_RGB | 0xffff00);
  ft_custom_font_edit("Терморегулятор");
  FT8_cmd_text(10,5, 11, 0, outstring);
  FT8_cmd_text(315,5, 23, 2048, "16.03.20 18:56");
  
  ft_custom_font_edit("Уст. Т.ЦО"); //60°
  FT8_cmd_text(10,10+offset, 11, 0, outstring);
  FT8_cmd_text(80,40+offset, 25, 2048, "76");
  FT8_cmd_text(85,40+offset, 19, 2048, "\xf8");
  //ft_custom_font_edit("Т.ЦО"); //60°
  //FT8_cmd_text(10,60+offset, 11, 0, outstring);  
  FT8_cmd_text(60,90+offset, 25, 2048, "72");
  FT8_cmd_text(65,90+offset, 19, 2048, "\xf8");
  
  ft_custom_font_edit("Уст. Т.ГВС");// 47°
  FT8_cmd_text(140,10+offset, 11, 0, outstring);
  FT8_cmd_text(210,40+offset, 25, 2048, "65");
  FT8_cmd_text(215,40+offset, 19, 2048, "\xf8");
  //ft_custom_font_edit("Т.ГВС");// 47°
  //FT8_cmd_text(140,60+offset, 11, 0, outstring);  
  FT8_cmd_text(190,90+offset, 25, 2048, "53");
  FT8_cmd_text(195,90+offset, 19, 2048, "\xf8");
  
  /*
  ft_custom_font_edit("Ошибка <:02>");
  FT8_cmd_text(10,70, 11, 0, outstring);
  ft_custom_font_edit("Настройка ^Ў");
  FT8_cmd_text(10,90, 11, 0, outstring);  
  ft_custom_font_edit("Какой то текст");
  FT8_cmd_text(10,110, 11, 0, outstring);
  ft_custom_font_edit("Еще текст");
  FT8_cmd_text(10,130, 11, 0, outstring);
  */
  
  /* display the logo */
  FT8_cmd_dl(DL_COLOR_RGB | 0xff0000);
  FT8_cmd_dl(BEGIN(BITMAPS));
  FT8_cmd_dl(VERTEX_FORMAT(0));
  FT8_cmd_setbitmap(MEM_PIC1, FT8_L4, 48, 48);
  FT8_cmd_dl(VERTEX2F(80, 80));
  
  FT8_cmd_dl(DL_COLOR_RGB | 0xffff00);
  FT8_cmd_dl(VERTEX2F(210, 80));
  
  FT8_cmd_dl(DL_END);    

  FT8_cmd_dl(DL_DISPLAY);
  FT8_cmd_dl(CMD_SWAP);
  FT8_end_cmd_burst(); /* stop writing to the cmd-fifo */
  FtCmdStart();    
}

void ft812_settingsWindowCustom(void){
  FT8_start_cmd_burst();
  FT8_cmd_dl(CMD_DLSTART);
  FT8_cmd_dl(DL_CLEAR_RGB | 0x000000);
  FT8_cmd_dl(DL_CLEAR | CLR_COL | CLR_STN | CLR_TAG);


  FT8_cmd_dl(DL_COLOR_RGB | GREY_COLOR);
    
  uint8_t offset = 20;

  
  FT8_cmd_dl(DL_COLOR_RGB | 0xeb9123);
  ft_custom_font_edit("Основное меню"); FT8_cmd_text(10,5, 11, 0, outstring);
  FT8_cmd_line(0,25,319,25,3);
  FT8_cmd_dl(DL_COLOR_RGB | 0x246e37);
  FT8_cmd_rect(0, 30, 319, 50, 1);
  FT8_cmd_rect(0, 70, 319, 90, 1);
  FT8_cmd_rect(0, 110, 319, 130, 1);
  FT8_cmd_rect(0, 150, 319, 170, 1);
  FT8_cmd_rect(0, 190, 319, 210, 1);
  
  FT8_cmd_dl(DL_COLOR_RGB | 0xffff00);
  ft_custom_font_edit("Запуск"); FT8_cmd_text(10,10+offset, 11, 0, outstring);
  
  ft_custom_font_edit("Ручной режим"); FT8_cmd_text(10,30+offset, 11, 0, outstring);

  ft_custom_font_edit("Авто"); FT8_cmd_text(10,50+offset, 11, 0, outstring);

  ft_custom_font_edit("Шнек"); FT8_cmd_text(10,70+offset, 11, 0, outstring);  

  ft_custom_font_edit("Вентилятор"); FT8_cmd_text(10,90+offset, 11, 0, outstring); 
  
  ft_custom_font_edit("Температура"); FT8_cmd_text(10,110+offset, 11, 0, outstring); 
  
  
  //ft_custom_font_edit("Т.ЦО"); //60°
  //FT8_cmd_text(10,60+offset, 11, 0, outstring);  
  //FT8_cmd_text(60,90+offset, 25, 2048, "72");
  //FT8_cmd_text(65,90+offset, 19, 2048, "\xf8");
     

  FT8_cmd_dl(DL_DISPLAY);
  FT8_cmd_dl(CMD_SWAP);
  FT8_end_cmd_burst(); /* stop writing to the cmd-fifo */
  FtCmdStart();    
}
void ft812_paramWindowCustom(void){
  FT8_start_cmd_burst();
  FT8_cmd_dl(CMD_DLSTART);
  FT8_cmd_dl(DL_CLEAR_RGB | 0x000000);
  FT8_cmd_dl(DL_CLEAR | CLR_COL | CLR_STN | CLR_TAG);


  FT8_cmd_dl(DL_COLOR_RGB | GREY_COLOR);
    
  uint8_t offset = 20;

  
  FT8_cmd_dl(DL_COLOR_RGB | 0xeb9123);
  ft_custom_font_edit("Меню параметра"); FT8_cmd_text(10,5, 11, 0, outstring);
  FT8_cmd_line(0,25,319,25,3);
  FT8_cmd_dl(DL_COLOR_RGB | 0x246e37);
  FT8_cmd_rect(0, 30, 319, 50, 1);
  FT8_cmd_rect(0, 70, 319, 90, 1);
  FT8_cmd_rect(0, 110, 319, 130, 1);
  FT8_cmd_rect(0, 150, 319, 170, 1);
  FT8_cmd_rect(0, 190, 319, 210, 1);
  
  FT8_cmd_dl(DL_COLOR_RGB | 0xffff00);
  ft_custom_font_edit("Параметр 1"); FT8_cmd_text(10,10+offset, 11, 0, outstring);
  
  ft_custom_font_edit("Параметр 2"); FT8_cmd_text(10,30+offset, 11, 0, outstring);

  ft_custom_font_edit("Параметр 3"); FT8_cmd_text(10,50+offset, 11, 0, outstring);

  ft_custom_font_edit("Параметр 4"); FT8_cmd_text(10,70+offset, 11, 0, outstring);  

  ft_custom_font_edit("Параметр 5"); FT8_cmd_text(10,90+offset, 11, 0, outstring); 
  
  ft_custom_font_edit("Параметр 6"); FT8_cmd_text(10,110+offset, 11, 0, outstring); 
  
  
  //ft_custom_font_edit("Т.ЦО"); //60°
  //FT8_cmd_text(10,60+offset, 11, 0, outstring);  
  //FT8_cmd_text(60,90+offset, 25, 2048, "72");
  //FT8_cmd_text(65,90+offset, 19, 2048, "\xf8");
     

  FT8_cmd_dl(DL_DISPLAY);
  FT8_cmd_dl(CMD_SWAP);
  FT8_end_cmd_burst(); /* stop writing to the cmd-fifo */
  FtCmdStart();  
}
void ft812_acceptWindowCustom(void){
  FT8_start_cmd_burst();
  FT8_cmd_dl(CMD_DLSTART);
  FT8_cmd_dl(DL_CLEAR_RGB | 0x000000);
  FT8_cmd_dl(DL_CLEAR | CLR_COL | CLR_STN | CLR_TAG);


  FT8_cmd_dl(DL_COLOR_RGB | GREY_COLOR);
    
  FT8_cmd_dl(DL_COLOR_RGB | 0xeb9123);
  ft_custom_font_edit("Подтвердить изменения:"); FT8_cmd_text(10,5, 11, 0, outstring);
  FT8_cmd_line(0,25,319,25,3);
  //FT8_cmd_dl(DL_COLOR_RGB | 0x246e37);
  
  FT8_cmd_dl(DL_COLOR_RGB | 0xffff00);
  FT8_cmd_fgcolor(0x00a200);
  ft_custom_font_edit("Да"); FT8_cmd_button(80, 100, 60, 60, 11, 0,outstring);
  FT8_cmd_fgcolor(GREY_COLOR);
  ft_custom_font_edit("Нет"); FT8_cmd_button(180, 100, 60, 60, 11, 0,outstring);
  

  FT8_cmd_dl(DL_DISPLAY);
  FT8_cmd_dl(CMD_SWAP);
  FT8_end_cmd_burst(); /* stop writing to the cmd-fifo */
  FtCmdStart();    
}
void ft812_mainwindow(void){
    FT8_start_cmd_burst();
    FT8_cmd_dl(CMD_DLSTART); /* start the display list */
    FT8_cmd_dl(DL_CLEAR_RGB | 0x000000); /* set the default clear color to white */
    FT8_cmd_dl(DL_CLEAR | CLR_COL | CLR_STN | CLR_TAG); 
    FT8_cmd_dl(TAG(0));  
    //FT8_cmd_dl(DL_COLOR_RGB | SPEECH_GREEN_COLOR);
    //FT8_cmd_text(5,2, 23, 0, "TermC");
    
    FT8_cmd_dl(DL_COLOR_RGB | GREY_COLOR);
    
    uint8_t offset = 30;
    
    FT8_cmd_line(0,0+offset,99,0+offset,2);
    FT8_cmd_line(0,138+offset,99,138+offset,2);
    FT8_cmd_line(0,1+offset,0,137+offset,2);
    FT8_cmd_line(99,1+offset,99,137+offset,2);

    FT8_cmd_line(100,0+offset,199,0+offset,2);
    FT8_cmd_line(100,138+offset,199,138+offset,2);
    FT8_cmd_line(100,1+offset,100,137+offset,2);
    FT8_cmd_line(199,1+offset,199,137+offset,2);

    FT8_cmd_line(0,139+offset,319,139+offset,2);
    FT8_cmd_line(0,139+offset,0,239,2);
    FT8_cmd_line(0,239,319,239,2);
    FT8_cmd_line(319,139+offset,319,239,2); 
    
    FT8_cmd_line(64,139+offset,64,239+offset,2);
    FT8_cmd_line(128,139+offset,128,239+offset,2);
    FT8_cmd_line(192,139+offset,192,239+offset,2);
    FT8_cmd_line(256,139+offset,256,239+offset,2);
    
    FT8_cmd_dl(DL_COLOR_RGB | (0xE5E500));
    FT8_cmd_fgcolor(0x45a29e);    
    FT8_cmd_button(210, 5+offset, 100, 40, 23, 0,"Settings");
    FT8_cmd_button(210, 50+offset, 100, 40, 23, 0,"Status");
    FT8_cmd_button(210, 95+offset, 100, 40, 23, 0,"Log");
    
    FT8_cmd_dl(DL_COLOR_RGB | SPEECH_GREEN_COLOR);
    FT8_cmd_text(5,2, 24, 0, "TermC");
    FT8_cmd_text(315,0, 23, 2048, "14.02.20 21:00");
    
    FT8_cmd_text(80,25+offset, 25, 2048, "60");
    FT8_cmd_text(50,85+offset, 25, 2048, "25");
    FT8_cmd_text(180,25+offset, 25, 2048, "60");
    FT8_cmd_text(150,85+offset, 25, 2048, "25");
    
    FT8_cmd_text(80,25+offset, 19, 2048, "\xf8");
    FT8_cmd_text(50,85+offset, 19, 2048, "\xf8");
    FT8_cmd_text(180,25+offset, 19, 2048, "\xf8");
    FT8_cmd_text(150,85+offset, 19, 2048, "\xf8");
    
    FT8_cmd_dl(DL_DISPLAY); /* instruct the graphics processor to show the list */
    FT8_cmd_dl(CMD_SWAP); /* make this list active */
    FT8_end_cmd_burst(); /* stop writing to the cmd-fifo */
    FtCmdStart();     
}
void ft812_settingswindow(void){
    FT8_start_cmd_burst();
    FT8_cmd_dl(CMD_DLSTART); /* start the display list */
    FT8_cmd_dl(DL_CLEAR_RGB | 0x000000); /* set the default clear color to white */
    FT8_cmd_dl(DL_CLEAR | CLR_COL | CLR_STN | CLR_TAG); 
    FT8_cmd_dl(TAG(0));  
    //FT8_cmd_dl(DL_COLOR_RGB | SPEECH_GREEN_COLOR);
    //FT8_cmd_text(5,2, 23, 0, "TermC");
    
    FT8_cmd_dl(DL_COLOR_RGB | GREY_COLOR);
    
    FT8_cmd_line(0,0,99,0,2);
    FT8_cmd_line(0,238,99,238,2);
    FT8_cmd_line(0,1,0,237,2);
    FT8_cmd_line(99,1,99,237,2);

    FT8_cmd_line(100,0,199,0,2);
    FT8_cmd_line(100,238,199,238,2);
    FT8_cmd_line(100,1,100,237,2);
    FT8_cmd_line(199,1,199,237,2);

    FT8_cmd_dl(DL_COLOR_RGB | (0xE5E500));
    FT8_cmd_fgcolor(0x45a29e);    
    FT8_cmd_button(210, 5, 100, 40, 23, 0,"Settings");
    FT8_cmd_button(210, 50, 100, 40, 23, 0,"Status");
    FT8_cmd_dl(DL_COLOR_RGB | SPEECH_GREEN_COLOR);
    
    FT8_cmd_text(80,25, 25, 2048, "60");//0x30 + temp_needGVSLevel_value[0] | 0x30 + temp_needGVSLevel_value[1]
    FT8_cmd_text(50,85, 25, 2048, "25");//0x30 + temp_actualGVSLevel_value[0] | 0x30 + temp_actualGVSLevel_value[1]
    FT8_cmd_text(180,25, 25, 2048, "60");//0x30 + temp_needCVSLevel_value[0] | 0x30 + temp_needCVSLevel_value[1]
    FT8_cmd_text(150,85, 25, 2048, "25");//0x30 + temp_actualCVSLevel_value[0] | 0x30 + temp_actualCVSLevel_value[1]
    
    FT8_cmd_text(80,25, 19, 2048, "\xf8");
    FT8_cmd_text(50,85, 19, 2048, "\xf8");
    FT8_cmd_text(180,25, 19, 2048, "\xf8");
    FT8_cmd_text(150,85, 19, 2048, "\xf8");
    

    FT8_cmd_dl(DL_DISPLAY); /* instruct the graphics processor to show the list */
    FT8_cmd_dl(CMD_SWAP); /* make this list active */
    FT8_end_cmd_burst(); /* stop writing to the cmd-fifo */
    FtCmdStart();     
  
  
}

	
void RTC_Config(void)
{
  NVIC_InitTypeDef NVIC_InitStructure; 
  EXTI_InitTypeDef EXTI_InitStructure;
  RTC_AlarmTypeDef RTC_AlarmStructure;
  /* Enable the PWR clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
  
  /* Allow access to RTC */
  PWR_BackupAccessCmd(ENABLE);

/* LSI used as RTC source clock */
/* The RTC Clock may varies due to LSI frequency dispersion. */   
  /* Enable the LSI OSC */ 
  //RCC_LSICmd(ENABLE);
  RCC_LSEConfig(RCC_LSE_ON);
  /* Wait till LSI is ready */  
  while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)//RCC_FLAG_LSERDY //RCC_FLAG_LSIRDY
  {
  }

  /* Select the RTC Clock Source */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);//RCC_RTCCLKSource_LSE //RCC_RTCCLKSource_LSI
   
  /* Enable the RTC Clock */
  RCC_RTCCLKCmd(ENABLE);

  /* Wait for RTC APB registers synchronisation */
  RTC_WaitForSynchro();

  /* Calendar Configuration */
  RTC_InitStructure.RTC_AsynchPrediv = 0x7f;
  RTC_InitStructure.RTC_SynchPrediv	=  0xff; /* (40KHz / 100) - 1 = 399*/
  RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;
  RTC_Init(&RTC_InitStructure);  

  /* EXTI configuration *******************************************************/
  EXTI_ClearITPendingBit(EXTI_Line17);
  EXTI_InitStructure.EXTI_Line = EXTI_Line17;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
  
  /* Enable the RTC Wakeup Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
    /* Set the alarm X+5s */
  RTC_AlarmStructure.RTC_AlarmTime.RTC_H12     = RTC_H12_AM;
  RTC_AlarmStructure.RTC_AlarmTime.RTC_Hours   = 0x00;
  RTC_AlarmStructure.RTC_AlarmTime.RTC_Minutes = 0x00;
  RTC_AlarmStructure.RTC_AlarmTime.RTC_Seconds = 0x01;
  RTC_AlarmStructure.RTC_AlarmDateWeekDay = 0x31;
  RTC_AlarmStructure.RTC_AlarmDateWeekDaySel = RTC_AlarmDateWeekDaySel_Date;
  RTC_AlarmStructure.RTC_AlarmMask = RTC_AlarmMask_DateWeekDay;
  RTC_SetAlarm(RTC_Format_BCD, RTC_Alarm_A, &RTC_AlarmStructure);

  RTC_ITConfig(RTC_IT_ALRA, ENABLE);
    
  /* Enable the alarm */
  RTC_AlarmCmd(RTC_Alarm_A, ENABLE);
  
  /* Set the time to 00h 00mn 00s AM */
  //RTC_TimeStructure.RTC_H12     = RTC_H12_AM;
  //RTC_TimeStructure.RTC_Hours   = 0x00;
  //RTC_TimeStructure.RTC_Minutes = 0x00;
  //RTC_TimeStructure.RTC_Seconds = 0x00;  
  
  //RTC_SetTime(RTC_Format_BCD, &RTC_TimeStructure);
  PWR_BackupAccessCmd(DISABLE);
}
void RTC_IRQHandler(void)
{
  /*
  if(RTC_GetITStatus(RTC_IT_ALRA) != RESET)
  {
    RTC_TimeTypeDef RTC_TimeStructure;
    
    RTC_TimeStructure.RTC_H12     = RTC_H12_AM;
    RTC_TimeStructure.RTC_Hours   = 0x00;
    RTC_TimeStructure.RTC_Minutes = 0x00;
    RTC_TimeStructure.RTC_Seconds = 0x00;  
  
    RTC_SetTime(RTC_Format_BCD, &RTC_TimeStructure);
    
    RTC_ClearITPendingBit(RTC_IT_ALRA);
    EXTI_ClearITPendingBit(EXTI_Line17);
  } */
}

void ft812_userMenu(uint8_t paramMenu){
  FT8_start_cmd_burst();
  FT8_cmd_dl(CMD_DLSTART);
  FT8_cmd_dl(DL_CLEAR_RGB | 0x000000);
  FT8_cmd_dl(DL_CLEAR | CLR_COL | CLR_STN | CLR_TAG);

  FT8_cmd_dl(DL_COLOR_RGB | GREY_COLOR);
  uint8_t offset = 20;

  FT8_cmd_dl(DL_COLOR_RGB | 0xeb9123);
  ft_custom_font_edit("Основное меню"); FT8_cmd_text(10,5, 11, 0, outstring);
  FT8_cmd_line(0,25,319,25,3);
  
  FT8_cmd_dl(DL_COLOR_RGB | 0x246e37);
  if(paramMenu == 0)FT8_cmd_rect(0, 30, 319, 50, 1);
  if(paramMenu == 1)FT8_cmd_rect(0, 50, 319, 70, 1);
  if(paramMenu == 2)FT8_cmd_rect(0, 70, 319, 90, 1);
  if(paramMenu == 3)FT8_cmd_rect(0, 90, 319, 110, 1);
  if(paramMenu == 4)FT8_cmd_rect(0, 110, 319, 130, 1);
  if(paramMenu == 5)FT8_cmd_rect(0, 130, 319, 150, 1);
  if(paramMenu == 6)FT8_cmd_rect(0, 150, 319, 170, 1);
  
  FT8_cmd_dl(DL_COLOR_RGB | 0xffff00);
  
  if(termStruct.termStartStop == 0){
    ft_custom_font_edit("Стоп"); FT8_cmd_text(230,10+offset, 11, 0, outstring);
  }
  if(termStruct.termStartStop == 1){
    ft_custom_font_edit("Запущен"); FT8_cmd_text(230,10+offset, 11, 0, outstring);
  }  
  if(termStruct.bunkerState == 0){
    FT8_cmd_text(230,130+offset, 23, 0, "0%");
  }
  if(termStruct.bunkerState == 1){
    FT8_cmd_text(230,130+offset, 23, 0, "100%");
  }
  
  ft_custom_font_edit("1.Запуск/Стоп"); FT8_cmd_text(10,10+offset, 11, 0, outstring);
  ft_custom_font_edit("2.Ручной режим"); FT8_cmd_text(10,30+offset, 11, 0, outstring);
  ft_custom_font_edit("3.Шнек подачи"); FT8_cmd_text(10,50+offset, 11, 0, outstring);
  ft_custom_font_edit("4.Вентилятор"); FT8_cmd_text(10,70+offset, 11, 0, outstring);  
  ft_custom_font_edit("5.Температура"); FT8_cmd_text(10,90+offset, 11, 0, outstring); 
  ft_custom_font_edit("6.Настройки"); FT8_cmd_text(10,110+offset, 11, 0, outstring); 
  ft_custom_font_edit("7.Заполнить бункер"); FT8_cmd_text(10,130+offset, 11, 0, outstring); 

  FT8_cmd_dl(DL_DISPLAY);
  FT8_cmd_dl(CMD_SWAP);
  FT8_end_cmd_burst(); /* stop writing to the cmd-fifo */
  FtCmdStart();    
}
void ft812_userL1(uint8_t paramMenu){
  FT8_start_cmd_burst();
  FT8_cmd_dl(CMD_DLSTART);
  FT8_cmd_dl(DL_CLEAR_RGB | 0x000000);
  FT8_cmd_dl(DL_CLEAR | CLR_COL | CLR_STN | CLR_TAG);

  FT8_cmd_dl(DL_COLOR_RGB | GREY_COLOR);
  uint8_t offset = 20;

  FT8_cmd_dl(DL_COLOR_RGB | 0xeb9123);
  ft_custom_font_edit("Запуск котла"); FT8_cmd_text(10,5, 11, 0, outstring);
  FT8_cmd_line(0,25,319,25,3);
  
  FT8_cmd_dl(DL_COLOR_RGB | 0x246e37);
  FT8_cmd_rect(220, 30, 319, 50, 1);
  
  FT8_cmd_dl(DL_COLOR_RGB | 0xffff00);
  if(paramMenu == 0){
    ft_custom_font_edit("Стоп"); FT8_cmd_text(230,10+offset, 11, 0, outstring);
  }
  if(paramMenu == 1){
    ft_custom_font_edit("Старт"); FT8_cmd_text(230,10+offset, 11, 0, outstring);
  }
  if(termStruct.bunkerState == 0){
    FT8_cmd_text(230,130+offset, 23, 0, "0%");
  }
  if(termStruct.bunkerState == 1){
    FT8_cmd_text(230,130+offset, 23, 0, "100%");
  }
  
  ft_custom_font_edit("1.Запуск/Стоп"); FT8_cmd_text(10,10+offset, 11, 0, outstring);
  ft_custom_font_edit("2.Ручной режим"); FT8_cmd_text(10,30+offset, 11, 0, outstring);
  ft_custom_font_edit("3.Шнек подачи"); FT8_cmd_text(10,50+offset, 11, 0, outstring);
  ft_custom_font_edit("4.Вентилятор"); FT8_cmd_text(10,70+offset, 11, 0, outstring);  
  ft_custom_font_edit("5.Температура"); FT8_cmd_text(10,90+offset, 11, 0, outstring); 
  ft_custom_font_edit("6.Настройки"); FT8_cmd_text(10,110+offset, 11, 0, outstring); 
  ft_custom_font_edit("7.Заполнить бункер"); FT8_cmd_text(10,130+offset, 11, 0, outstring); 
  
  FT8_cmd_dl(DL_DISPLAY);
  FT8_cmd_dl(CMD_SWAP);
  FT8_end_cmd_burst(); /* stop writing to the cmd-fifo */
  FtCmdStart();    
}
void ft812_userL2(uint8_t paramMenu){
  FT8_start_cmd_burst();
  FT8_cmd_dl(CMD_DLSTART);
  FT8_cmd_dl(DL_CLEAR_RGB | 0x000000);
  FT8_cmd_dl(DL_CLEAR | CLR_COL | CLR_STN | CLR_TAG);

  FT8_cmd_dl(DL_COLOR_RGB | GREY_COLOR);
  uint8_t offset = 20;

  FT8_cmd_dl(DL_COLOR_RGB | 0xeb9123);
  ft_custom_font_edit("Ручной режим"); FT8_cmd_text(10,5, 11, 0, outstring);
  FT8_cmd_line(0,25,319,25,3);

  if(menuLevel == MENU_LEVEL_PARAMETERS){
    FT8_cmd_dl(DL_COLOR_RGB | 0x246e37);
    if(paramMenu == 0)FT8_cmd_rect(0, 30, 319, 50, 1);
    if(paramMenu == 1)FT8_cmd_rect(0, 50, 319, 70, 1);
    if(paramMenu == 2)FT8_cmd_rect(0, 70, 319, 90, 1);
    if(paramMenu == 3)FT8_cmd_rect(0, 90, 319, 110, 1);
    if(paramMenu == 4)FT8_cmd_rect(0, 110, 319, 130, 1);
    if(paramMenu == 5)FT8_cmd_rect(0, 130, 319, 150, 1);   
    FT8_cmd_dl(DL_COLOR_RGB | 0xffff00);
    if(termStruct.screwStateManual == 0){ft_custom_font_edit("Выкл."); FT8_cmd_text(230,10+offset, 11, 0, outstring);}
    if(termStruct.screwStateManual == 1){ft_custom_font_edit("Вкл."); FT8_cmd_text(230,10+offset, 11, 0, outstring);}
    if(termStruct.pumpCOStateManual == 0){ft_custom_font_edit("Выкл."); FT8_cmd_text(230,30+offset, 11, 0, outstring);}
    if(termStruct.pumpCOStateManual == 1){ft_custom_font_edit("Вкл."); FT8_cmd_text(230,30+offset, 11, 0, outstring); } 
    if(termStruct.pumpGVSStateManual == 0){ft_custom_font_edit("Выкл."); FT8_cmd_text(230,50+offset, 11, 0, outstring);}
    if(termStruct.pumpGVSStateManual == 1){ft_custom_font_edit("Вкл."); FT8_cmd_text(230,50+offset, 11, 0, outstring);}    
    if(termStruct.fanStateManual == 0){ft_custom_font_edit("Выкл."); FT8_cmd_text(230,70+offset, 11, 0, outstring);}
    if(termStruct.fanStateManual == 1){ft_custom_font_edit("Вкл."); FT8_cmd_text(230,70+offset, 11, 0, outstring);}      
    if(termStruct.screwReversStateManual == 0){ft_custom_font_edit("Выкл."); FT8_cmd_text(230,90+offset, 11, 0, outstring);}
    if(termStruct.screwReversStateManual == 1){ft_custom_font_edit("Вкл."); FT8_cmd_text(230,90+offset, 11, 0, outstring);}
  }  
  
  FT8_cmd_dl(DL_COLOR_RGB | 0xffff00);
  ft_custom_font_edit("1.Шнек"); FT8_cmd_text(10,10+offset, 11, 0, outstring);
  ft_custom_font_edit("2.Насос ЦО"); FT8_cmd_text(10,30+offset, 11, 0, outstring);
  ft_custom_font_edit("3.Насос ГВС"); FT8_cmd_text(10,50+offset, 11, 0, outstring);
  ft_custom_font_edit("4.Вентилятор"); FT8_cmd_text(10,70+offset, 11, 0, outstring);  
  ft_custom_font_edit("5.Реверс шнека"); FT8_cmd_text(10,90+offset, 11, 0, outstring); 

 
  if(menuLevel == MENU_LEVEL_ACCEPT){
    FT8_cmd_dl(DL_COLOR_RGB | 0x246e37);
    if(paramMenu == 0)FT8_cmd_rect(220, 30, 319, 50, 1);
    if(paramMenu == 1)FT8_cmd_rect(220, 50, 319, 70, 1);
    if(paramMenu == 2)FT8_cmd_rect(220, 70, 319, 90, 1);
    if(paramMenu == 3)FT8_cmd_rect(220, 90, 319, 110, 1);
    if(paramMenu == 4)FT8_cmd_rect(220, 110, 319, 130, 1);
    if(paramMenu == 5)FT8_cmd_rect(220, 130, 319, 150, 1); 
    FT8_cmd_dl(DL_COLOR_RGB | 0xffff00);
    if(paramMenu == 0){
      if(tempScrewStateManual == 0){ft_custom_font_edit("Выкл."); FT8_cmd_text(230,10+offset, 11, 0, outstring);}
      if(tempScrewStateManual == 1){ft_custom_font_edit("Вкл. "); FT8_cmd_text(230,10+offset, 11, 0, outstring);}
      if(termStruct.pumpCOStateManual == 0){ft_custom_font_edit("Выкл."); FT8_cmd_text(230,30+offset, 11, 0, outstring);}
      if(termStruct.pumpCOStateManual == 1){ft_custom_font_edit("Вкл. "); FT8_cmd_text(230,30+offset, 11, 0, outstring);} 
      if(termStruct.pumpGVSStateManual == 0){ft_custom_font_edit("Выкл."); FT8_cmd_text(230,50+offset, 11, 0, outstring);}
      if(termStruct.pumpGVSStateManual == 1){ft_custom_font_edit("Вкл. "); FT8_cmd_text(230,50+offset, 11, 0, outstring);}     
      if(termStruct.fanStateManual == 0){ft_custom_font_edit("Выкл."); FT8_cmd_text(230,70+offset, 11, 0, outstring);}
      if(termStruct.fanStateManual == 1){ft_custom_font_edit("Вкл. "); FT8_cmd_text(230,70+offset, 11, 0, outstring);}
      if(termStruct.screwReversStateManual == 0){ft_custom_font_edit("Выкл."); FT8_cmd_text(230,90+offset, 11, 0, outstring);}
      if(termStruct.screwReversStateManual == 1){ft_custom_font_edit("Вкл. "); FT8_cmd_text(230,90+offset, 11, 0, outstring);}      
    }
    if(paramMenu == 1){
      if(termStruct.screwStateManual == 0){ft_custom_font_edit("Выкл."); FT8_cmd_text(230,10+offset, 11, 0, outstring);}
      if(termStruct.screwStateManual == 1){ft_custom_font_edit("Вкл. "); FT8_cmd_text(230,10+offset, 11, 0, outstring);}      
      if(tempPumpCOStateManual == 0){ft_custom_font_edit("Выкл."); FT8_cmd_text(230,30+offset, 11, 0, outstring);}
      if(tempPumpCOStateManual == 1){ft_custom_font_edit("Вкл. "); FT8_cmd_text(230,30+offset, 11, 0, outstring);}
      if(termStruct.pumpGVSStateManual == 0){ft_custom_font_edit("Выкл."); FT8_cmd_text(230,50+offset, 11, 0, outstring);}
      if(termStruct.pumpGVSStateManual == 1){ft_custom_font_edit("Вкл. "); FT8_cmd_text(230,50+offset, 11, 0, outstring);}     
      if(termStruct.fanStateManual == 0){ft_custom_font_edit("Выкл."); FT8_cmd_text(230,70+offset, 11, 0, outstring);}
      if(termStruct.fanStateManual == 1){ft_custom_font_edit("Вкл. "); FT8_cmd_text(230,70+offset, 11, 0, outstring);}  
      if(termStruct.screwReversStateManual == 0){ft_custom_font_edit("Выкл."); FT8_cmd_text(230,90+offset, 11, 0, outstring);}
      if(termStruct.screwReversStateManual == 1){ft_custom_font_edit("Вкл. "); FT8_cmd_text(230,90+offset, 11, 0, outstring);}        
    }
    if(paramMenu == 2){
      if(termStruct.screwStateManual == 0){ft_custom_font_edit("Выкл."); FT8_cmd_text(230,10+offset, 11, 0, outstring);}
      if(termStruct.screwStateManual == 1){ft_custom_font_edit("Вкл. "); FT8_cmd_text(230,10+offset, 11, 0, outstring);}      
      if(termStruct.pumpCOStateManual == 0){ft_custom_font_edit("Выкл."); FT8_cmd_text(230,30+offset, 11, 0, outstring);}
      if(termStruct.pumpCOStateManual == 1){ft_custom_font_edit("Вкл. "); FT8_cmd_text(230,30+offset, 11, 0, outstring);}      
      if(tempPumpGVSStateManual == 0){ft_custom_font_edit("Выкл."); FT8_cmd_text(230,50+offset, 11, 0, outstring);}
      if(tempPumpGVSStateManual == 1){ft_custom_font_edit("Вкл. "); FT8_cmd_text(230,50+offset, 11, 0, outstring);}
      if(termStruct.fanStateManual == 0){ft_custom_font_edit("Выкл."); FT8_cmd_text(230,70+offset, 11, 0, outstring);}
      if(termStruct.fanStateManual == 1){ft_custom_font_edit("Вкл. "); FT8_cmd_text(230,70+offset, 11, 0, outstring);}  
      if(termStruct.screwReversStateManual == 0){ft_custom_font_edit("Выкл."); FT8_cmd_text(230,90+offset, 11, 0, outstring);}
      if(termStruct.screwReversStateManual == 1){ft_custom_font_edit("Вкл. "); FT8_cmd_text(230,90+offset, 11, 0, outstring);}          
    }
    if(paramMenu == 3){
      if(termStruct.screwStateManual == 0){ft_custom_font_edit("Выкл."); FT8_cmd_text(230,10+offset, 11, 0, outstring);}
      if(termStruct.screwStateManual == 1){ft_custom_font_edit("Вкл. "); FT8_cmd_text(230,10+offset, 11, 0, outstring);}
      if(termStruct.pumpCOStateManual == 0){ft_custom_font_edit("Выкл."); FT8_cmd_text(230,30+offset, 11, 0, outstring);}
      if(termStruct.pumpCOStateManual == 1){ft_custom_font_edit("Вкл. "); FT8_cmd_text(230,30+offset, 11, 0, outstring);}  
      if(termStruct.pumpGVSStateManual == 0){ft_custom_font_edit("Выкл."); FT8_cmd_text(230,50+offset, 11, 0, outstring);}
      if(termStruct.pumpGVSStateManual == 1){ft_custom_font_edit("Вкл. "); FT8_cmd_text(230,50+offset, 11, 0, outstring);}        
      if(tempFanStateManual == 0){ft_custom_font_edit("Выкл."); FT8_cmd_text(230,70+offset, 11, 0, outstring);}
      if(tempFanStateManual == 1){ft_custom_font_edit("Вкл. "); FT8_cmd_text(230,70+offset, 11, 0, outstring);}
      if(termStruct.screwReversStateManual == 0){ft_custom_font_edit("Выкл."); FT8_cmd_text(230,90+offset, 11, 0, outstring);}
      if(termStruct.screwReversStateManual == 1){ft_custom_font_edit("Вкл. "); FT8_cmd_text(230,90+offset, 11, 0, outstring);}        
    }
    if(paramMenu == 4){
      if(termStruct.screwStateManual == 0){ft_custom_font_edit("Выкл."); FT8_cmd_text(230,10+offset, 11, 0, outstring);}
      if(termStruct.screwStateManual == 1){ft_custom_font_edit("Вкл. "); FT8_cmd_text(230,10+offset, 11, 0, outstring);}
      if(termStruct.pumpCOStateManual == 0){ft_custom_font_edit("Выкл."); FT8_cmd_text(230,30+offset, 11, 0, outstring);}
      if(termStruct.pumpCOStateManual == 1){ft_custom_font_edit("Вкл. "); FT8_cmd_text(230,30+offset, 11, 0, outstring);}  
      if(termStruct.pumpGVSStateManual == 0){ft_custom_font_edit("Выкл."); FT8_cmd_text(230,50+offset, 11, 0, outstring);}
      if(termStruct.pumpGVSStateManual == 1){ft_custom_font_edit("Вкл. "); FT8_cmd_text(230,50+offset, 11, 0, outstring);}     
      if(termStruct.fanStateManual == 0){ft_custom_font_edit("Выкл."); FT8_cmd_text(230,70+offset, 11, 0, outstring);}
      if(termStruct.fanStateManual == 1){ft_custom_font_edit("Вкл. "); FT8_cmd_text(230,70+offset, 11, 0, outstring);}        
      if(tempScrewReversStateManual == 0){ft_custom_font_edit("Выкл."); FT8_cmd_text(230,90+offset, 11, 0, outstring);}
      if(tempScrewReversStateManual == 1){ft_custom_font_edit("Вкл. "); FT8_cmd_text(230,90+offset, 11, 0, outstring);}
    }    
    
  }
 
  
  FT8_cmd_dl(DL_DISPLAY);
  FT8_cmd_dl(CMD_SWAP);
  FT8_end_cmd_burst(); /* stop writing to the cmd-fifo */
  FtCmdStart();    
}
void ft812_userL3(uint8_t paramMenu){
  FT8_start_cmd_burst();
  FT8_cmd_dl(CMD_DLSTART);
  FT8_cmd_dl(DL_CLEAR_RGB | 0x000000);
  FT8_cmd_dl(DL_CLEAR | CLR_COL | CLR_STN | CLR_TAG);

  FT8_cmd_dl(DL_COLOR_RGB | GREY_COLOR);
  uint8_t offset = 20;

  FT8_cmd_dl(DL_COLOR_RGB | 0xeb9123);
  ft_custom_font_edit("Шнек подачи"); FT8_cmd_text(10,5, 11, 0, outstring);
  FT8_cmd_line(0,25,319,25,3);
  
  if(menuLevel == MENU_LEVEL_PARAMETERS){
    FT8_cmd_dl(DL_COLOR_RGB | 0x246e37);
    if(paramMenu == 0)FT8_cmd_rect(0, 50, 319, 70, 1);
    if(paramMenu == 1)FT8_cmd_rect(0, 70, 319, 90, 1);
    if(paramMenu == 2)FT8_cmd_rect(0, 110, 319, 130, 1);
    if(paramMenu == 3)FT8_cmd_rect(0, 130, 319, 150, 1);
    FT8_cmd_dl(DL_COLOR_RGB | 0xffff00);
  
    char temp_string[5];
    sprintf(temp_string, "%d", termStruct.screwRunWorkTime);
    FT8_cmd_text(230, 30+offset, 23, 0, temp_string);   
    sprintf(temp_string, "%d", termStruct.screwRunSleepTime);
    FT8_cmd_text(230, 50+offset, 23, 0, temp_string);   
    sprintf(temp_string, "%d", termStruct.screwSupportWorkTime);
    FT8_cmd_text(230, 90+offset, 23, 0, temp_string);   
    sprintf(temp_string, "%d", termStruct.screwSupportSleepTime);
    FT8_cmd_text(230, 110+offset, 23, 0, temp_string);     
  }    
  
  
  FT8_cmd_dl(DL_COLOR_RGB | 0xffff00);
  ft_custom_font_edit("1.Нагрев:"); FT8_cmd_text(10,10+offset, 11, 0, outstring);
  ft_custom_font_edit(" 1.1.Работа(сек)"); FT8_cmd_text(10,30+offset, 11, 0, outstring);
  ft_custom_font_edit(" 1.2.Перерыв(сек)"); FT8_cmd_text(10,50+offset, 11, 0, outstring);
  ft_custom_font_edit("2.Поддержка(доп):"); FT8_cmd_text(10,70+offset, 11, 0, outstring);  
  ft_custom_font_edit(" 2.1.Работа(сек)"); FT8_cmd_text(10,90+offset, 11, 0, outstring); 
  ft_custom_font_edit(" 2.2.Перерыв(мин)"); FT8_cmd_text(10,110+offset, 11, 0, outstring); 

  if(menuLevel == MENU_LEVEL_ACCEPT){
    FT8_cmd_dl(DL_COLOR_RGB | 0x246e37);
    if(paramMenu == 0)FT8_cmd_rect(220, 50, 319, 70, 1);
    if(paramMenu == 1)FT8_cmd_rect(220, 70, 319, 90, 1);
    if(paramMenu == 2)FT8_cmd_rect(220, 110, 319, 130, 1);
    if(paramMenu == 3)FT8_cmd_rect(220, 130, 319, 150, 1); 
    FT8_cmd_dl(DL_COLOR_RGB | 0xffff00);
    
    if(paramMenu == 0){
        char temp_string[5];
        sprintf(temp_string, "%d", tempScrewRunWorkTime);
        FT8_cmd_text(230, 30+offset, 23, 0, temp_string);   
        sprintf(temp_string, "%d", termStruct.screwRunSleepTime);
        FT8_cmd_text(230, 50+offset, 23, 0, temp_string);   
        sprintf(temp_string, "%d", termStruct.screwSupportWorkTime);
        FT8_cmd_text(230, 90+offset, 23, 0, temp_string);   
        sprintf(temp_string, "%d", termStruct.screwSupportSleepTime);
        FT8_cmd_text(230, 110+offset, 23, 0, temp_string);        
    }
    if(paramMenu == 1){
        char temp_string[5];
        sprintf(temp_string, "%d", termStruct.screwRunWorkTime);
        FT8_cmd_text(230, 30+offset, 23, 0, temp_string);   
        sprintf(temp_string, "%d", tempScrewRunSleepTime);
        FT8_cmd_text(230, 50+offset, 23, 0, temp_string);   
        sprintf(temp_string, "%d", termStruct.screwSupportWorkTime);
        FT8_cmd_text(230, 90+offset, 23, 0, temp_string);   
        sprintf(temp_string, "%d", termStruct.screwSupportSleepTime);
        FT8_cmd_text(230, 110+offset, 23, 0, temp_string);        
    }
    if(paramMenu == 2){
        char temp_string[5];
        sprintf(temp_string, "%d", termStruct.screwRunWorkTime);
        FT8_cmd_text(230, 30+offset, 23, 0, temp_string);   
        sprintf(temp_string, "%d", termStruct.screwRunSleepTime);
        FT8_cmd_text(230, 50+offset, 23, 0, temp_string);   
        sprintf(temp_string, "%d", tempScrewSupportWorkTime);
        FT8_cmd_text(230, 90+offset, 23, 0, temp_string);   
        sprintf(temp_string, "%d", termStruct.screwSupportSleepTime);
        FT8_cmd_text(230, 110+offset, 23, 0, temp_string);        
    }
    if(paramMenu == 3){
        char temp_string[5];
        sprintf(temp_string, "%d", termStruct.screwRunWorkTime);
        FT8_cmd_text(230, 30+offset, 23, 0, temp_string);   
        sprintf(temp_string, "%d", termStruct.screwRunSleepTime);
        FT8_cmd_text(230, 50+offset, 23, 0, temp_string);   
        sprintf(temp_string, "%d", termStruct.screwSupportWorkTime);
        FT8_cmd_text(230, 90+offset, 23, 0, temp_string);   
        sprintf(temp_string, "%d", tempScrewSupportSleepTime);
        FT8_cmd_text(230, 110+offset, 23, 0, temp_string);        
    }    
  }
  
  FT8_cmd_dl(DL_DISPLAY);
  FT8_cmd_dl(CMD_SWAP);
  FT8_end_cmd_burst(); /* stop writing to the cmd-fifo */
  FtCmdStart();    
}
void ft812_userL4(uint8_t paramMenu){
  FT8_start_cmd_burst();
  FT8_cmd_dl(CMD_DLSTART);
  FT8_cmd_dl(DL_CLEAR_RGB | 0x000000);
  FT8_cmd_dl(DL_CLEAR | CLR_COL | CLR_STN | CLR_TAG);

  FT8_cmd_dl(DL_COLOR_RGB | GREY_COLOR);
  uint8_t offset = 20;
  
  if(menuLevel == MENU_LEVEL_PARAMETERS){
    FT8_cmd_dl(DL_COLOR_RGB | 0x246e37);
    if(paramMenu == 0)FT8_cmd_rect(0, 50, 319, 70, 1);
    if(paramMenu == 1)FT8_cmd_rect(0, 70, 319, 90, 1);
    if(paramMenu == 2)FT8_cmd_rect(0, 90, 319, 110, 1);
    FT8_cmd_dl(DL_COLOR_RGB | 0xffff00);
  
    char temp_string[5];
        
    sprintf(temp_string, "%d", termStruct.fanSupportWorkTime);
    FT8_cmd_text(230, 30+offset, 23, 0, temp_string);   
    sprintf(temp_string, "%d", termStruct.fanSupportSleepTime);
    FT8_cmd_text(230, 50+offset, 23, 0, temp_string);   
    sprintf(temp_string, "%d", termStruct.fanPower);
    FT8_cmd_text(230, 70+offset, 23, 0, temp_string);      
  } 
  
  FT8_cmd_dl(DL_COLOR_RGB | 0xeb9123);
  ft_custom_font_edit("Вентилятор"); FT8_cmd_text(10,5, 11, 0, outstring);
  FT8_cmd_line(0,25,319,25,3);

  
  FT8_cmd_dl(DL_COLOR_RGB | 0xffff00);
  ft_custom_font_edit("1.Поддержка"); FT8_cmd_text(10,10+offset, 11, 0, outstring);
  ft_custom_font_edit(" 1.1.Работа(сек)"); FT8_cmd_text(10,30+offset, 11, 0, outstring);
  ft_custom_font_edit(" 1.2.Перерыв(сек)"); FT8_cmd_text(10,50+offset, 11, 0, outstring);
  ft_custom_font_edit("2.Макс. мощность"); FT8_cmd_text(10,70+offset, 11, 0, outstring);  



  if(menuLevel == MENU_LEVEL_ACCEPT){
    FT8_cmd_dl(DL_COLOR_RGB | 0x246e37);
    if(paramMenu == 0)FT8_cmd_rect(220, 50, 319, 70, 1);
    if(paramMenu == 1)FT8_cmd_rect(220, 70, 319, 90, 1);
    if(paramMenu == 2)FT8_cmd_rect(220, 90, 319, 110, 1);
    FT8_cmd_dl(DL_COLOR_RGB | 0xffff00);
    
    if(paramMenu == 0){
      char temp_string[5]; 
      sprintf(temp_string, "%d", tempFanSupportWorkTime);
      FT8_cmd_text(230, 30+offset, 23, 0, temp_string);   
      sprintf(temp_string, "%d", termStruct.fanSupportSleepTime);
      FT8_cmd_text(230, 50+offset, 23, 0, temp_string);   
      sprintf(temp_string, "%d", termStruct.fanPower);
      FT8_cmd_text(230, 70+offset, 23, 0, temp_string);       
    }
    if(paramMenu == 1){
      char temp_string[5]; 
      sprintf(temp_string, "%d", termStruct.fanSupportWorkTime);
      FT8_cmd_text(230, 30+offset, 23, 0, temp_string);   
      sprintf(temp_string, "%d", tempFanSupportSleepTime);
      FT8_cmd_text(230, 50+offset, 23, 0, temp_string);   
      sprintf(temp_string, "%d", termStruct.fanPower);
      FT8_cmd_text(230, 70+offset, 23, 0, temp_string);         
    }
    if(paramMenu == 2){
      char temp_string[5]; 
      sprintf(temp_string, "%d", termStruct.fanSupportWorkTime);
      FT8_cmd_text(230, 30+offset, 23, 0, temp_string);   
      sprintf(temp_string, "%d", termStruct.fanSupportSleepTime);
      FT8_cmd_text(230, 50+offset, 23, 0, temp_string);   
      sprintf(temp_string, "%d", tempFanPower);
      FT8_cmd_text(230, 70+offset, 23, 0, temp_string);       
    }
 
  }

  
  FT8_cmd_dl(DL_DISPLAY);
  FT8_cmd_dl(CMD_SWAP);
  FT8_end_cmd_burst(); /* stop writing to the cmd-fifo */
  FtCmdStart();    
}
void ft812_userL5(uint8_t paramMenu){
  FT8_start_cmd_burst();
  FT8_cmd_dl(CMD_DLSTART);
  FT8_cmd_dl(DL_CLEAR_RGB | 0x000000);
  FT8_cmd_dl(DL_CLEAR | CLR_COL | CLR_STN | CLR_TAG);

  FT8_cmd_dl(DL_COLOR_RGB | GREY_COLOR);
  uint8_t offset = 20;

  if(menuLevel == MENU_LEVEL_PARAMETERS){
    FT8_cmd_dl(DL_COLOR_RGB | 0x246e37);
    if(paramMenu == 0)FT8_cmd_rect(0, 30, 319, 50, 1);
    if(paramMenu == 1)FT8_cmd_rect(0, 50, 319, 70, 1);
    if(paramMenu == 2)FT8_cmd_rect(0, 70, 319, 90, 1);
    if(paramMenu == 3)FT8_cmd_rect(0, 90, 319, 110, 1);
    FT8_cmd_dl(DL_COLOR_RGB | 0xffff00);
  
    char temp_string[5];
        
    sprintf(temp_string, "+%d", termStruct.temperatureCO);
    FT8_cmd_text(230, 10+offset, 23, 0, temp_string);  
    ft_custom_font_edit("°С"); FT8_cmd_text(260, 10+offset, 11, 0, outstring);
    sprintf(temp_string, "%d", termStruct.hystCO);
    FT8_cmd_text(230, 30+offset, 23, 0, temp_string);
    ft_custom_font_edit("°С"); FT8_cmd_text(260, 30+offset, 11, 0, outstring);
    sprintf(temp_string, "+%d", termStruct.temperatureGVS);
    FT8_cmd_text(230, 50+offset, 23, 0, temp_string);
    ft_custom_font_edit("°С"); FT8_cmd_text(260, 50+offset, 11, 0, outstring);
    sprintf(temp_string, "%d", termStruct.hystGVS);
    FT8_cmd_text(230, 70+offset, 23, 0, temp_string);    
    ft_custom_font_edit("°С"); FT8_cmd_text(260, 70+offset, 11, 0, outstring);
  }   
  
  FT8_cmd_dl(DL_COLOR_RGB | 0xeb9123);
  ft_custom_font_edit("Температура"); FT8_cmd_text(10,5, 11, 0, outstring);
  FT8_cmd_line(0,25,319,25,3);

  FT8_cmd_dl(DL_COLOR_RGB | 0xffff00);
  ft_custom_font_edit("1.Температура ЦО"); FT8_cmd_text(10,10+offset, 11, 0, outstring);
  ft_custom_font_edit("2.Гистерезис ЦО"); FT8_cmd_text(10,30+offset, 11, 0, outstring);
  ft_custom_font_edit("3.Температура ГВС"); FT8_cmd_text(10,50+offset, 11, 0, outstring);
  ft_custom_font_edit("4.Гистерезис ГВС"); FT8_cmd_text(10,70+offset, 11, 0, outstring);  

  if(menuLevel == MENU_LEVEL_ACCEPT){
    FT8_cmd_dl(DL_COLOR_RGB | 0x246e37);
    if(paramMenu == 0)FT8_cmd_rect(220, 30, 319, 50, 1);
    if(paramMenu == 1)FT8_cmd_rect(220, 50, 319, 70, 1);
    if(paramMenu == 2)FT8_cmd_rect(220, 70, 319, 90, 1);
    if(paramMenu == 3)FT8_cmd_rect(220, 90, 319, 110, 1);
    FT8_cmd_dl(DL_COLOR_RGB | 0xffff00);
    
    if(paramMenu == 0){
      char temp_string[5]; 
        sprintf(temp_string, "+%d", tempTemperatureCO);
        FT8_cmd_text(230, 10+offset, 23, 0, temp_string);
        ft_custom_font_edit("°С"); FT8_cmd_text(260, 10+offset, 11, 0, outstring);
        sprintf(temp_string, "%d", termStruct.hystCO);
        FT8_cmd_text(230, 30+offset, 23, 0, temp_string);   
        ft_custom_font_edit("°С"); FT8_cmd_text(260, 30+offset, 11, 0, outstring);
        sprintf(temp_string, "+%d", termStruct.temperatureGVS);
        FT8_cmd_text(230, 50+offset, 23, 0, temp_string);  
        ft_custom_font_edit("°С"); FT8_cmd_text(260, 50+offset, 11, 0, outstring);
        sprintf(temp_string, "%d", termStruct.hystGVS);
        FT8_cmd_text(230, 70+offset, 23, 0, temp_string);       
        ft_custom_font_edit("°С"); FT8_cmd_text(260, 70+offset, 11, 0, outstring);
    }
    if(paramMenu == 1){
      char temp_string[5]; 
        sprintf(temp_string, "+%d", termStruct.temperatureCO);
        FT8_cmd_text(230, 10+offset, 23, 0, temp_string);  
        ft_custom_font_edit("°С"); FT8_cmd_text(260, 10+offset, 11, 0, outstring);
        sprintf(temp_string, "%d", tempHystCO);
        FT8_cmd_text(230, 30+offset, 23, 0, temp_string);   
        ft_custom_font_edit("°С"); FT8_cmd_text(260, 30+offset, 11, 0, outstring);
        sprintf(temp_string, "+%d", termStruct.temperatureGVS);
        FT8_cmd_text(230, 50+offset, 23, 0, temp_string);  
        ft_custom_font_edit("°С"); FT8_cmd_text(260, 50+offset, 11, 0, outstring);
        sprintf(temp_string, "%d", termStruct.hystGVS);
        FT8_cmd_text(230, 70+offset, 23, 0, temp_string);         
        ft_custom_font_edit("°С"); FT8_cmd_text(260, 70+offset, 11, 0, outstring);
    }
    if(paramMenu == 2){
      char temp_string[5]; 
        sprintf(temp_string, "+%d", termStruct.temperatureCO);
        FT8_cmd_text(230, 10+offset, 23, 0, temp_string); 
        ft_custom_font_edit("°С"); FT8_cmd_text(260, 10+offset, 11, 0, outstring);
        sprintf(temp_string, "%d", termStruct.hystCO);
        FT8_cmd_text(230, 30+offset, 23, 0, temp_string);   
        ft_custom_font_edit("°С"); FT8_cmd_text(260, 30+offset, 11, 0, outstring);
        sprintf(temp_string, "+%d", tempTemperatureGVS);
        FT8_cmd_text(230, 50+offset, 23, 0, temp_string);  
        ft_custom_font_edit("°С"); FT8_cmd_text(260, 50+offset, 11, 0, outstring);
        sprintf(temp_string, "%d", termStruct.hystGVS);
        FT8_cmd_text(230, 70+offset, 23, 0, temp_string);      
        ft_custom_font_edit("°С"); FT8_cmd_text(260, 70+offset, 11, 0, outstring);
    }
    if(paramMenu == 3){
      char temp_string[5]; 
        sprintf(temp_string, "+%d", termStruct.temperatureCO);
        FT8_cmd_text(230, 10+offset, 23, 0, temp_string);   
        ft_custom_font_edit("°С"); FT8_cmd_text(260, 10+offset, 11, 0, outstring);
        sprintf(temp_string, "%d", termStruct.hystCO);
        FT8_cmd_text(230, 30+offset, 23, 0, temp_string);   
        ft_custom_font_edit("°С"); FT8_cmd_text(260, 30+offset, 11, 0, outstring);
        sprintf(temp_string, "+%d", termStruct.temperatureGVS);
        FT8_cmd_text(230, 50+offset, 23, 0, temp_string);  
        ft_custom_font_edit("°С"); FT8_cmd_text(260, 50+offset, 11, 0, outstring);
        sprintf(temp_string, "%d", tempHystGVS);
        FT8_cmd_text(230, 70+offset, 23, 0, temp_string);      
        ft_custom_font_edit("°С"); FT8_cmd_text(260, 70+offset, 11, 0, outstring);
    } 
  }  
  
  FT8_cmd_dl(DL_DISPLAY);
  FT8_cmd_dl(CMD_SWAP);
  FT8_end_cmd_burst(); /* stop writing to the cmd-fifo */
  FtCmdStart();    
}
void ft812_userL6(uint8_t paramMenu){
  FT8_start_cmd_burst();
  FT8_cmd_dl(CMD_DLSTART);
  FT8_cmd_dl(DL_CLEAR_RGB | 0x000000);
  FT8_cmd_dl(DL_CLEAR | CLR_COL | CLR_STN | CLR_TAG);

  FT8_cmd_dl(DL_COLOR_RGB | GREY_COLOR);
  uint8_t offset = 20;

  FT8_cmd_dl(DL_COLOR_RGB | 0xeb9123);
  ft_custom_font_edit("Настройки"); FT8_cmd_text(10,5, 11, 0, outstring);
  FT8_cmd_line(0,25,319,25,3);
  FT8_cmd_dl(DL_COLOR_RGB | 0x246e37);
  if(paramMenu == 0)FT8_cmd_rect(0, 30, 319, 50, 1);
  if(paramMenu == 1)FT8_cmd_rect(0, 50, 319, 70, 1);
  if(paramMenu == 2)FT8_cmd_rect(0, 70, 319, 90, 1);
  if(paramMenu == 3)FT8_cmd_rect(0, 90, 319, 110, 1);
  if(paramMenu == 4)FT8_cmd_rect(0, 110, 319, 130, 1);
  if(paramMenu == 5)FT8_cmd_rect(0, 130, 319, 150, 1);
  if(paramMenu == 6)FT8_cmd_rect(0, 150, 319, 170, 1);
  if(paramMenu == 7)FT8_cmd_rect(0, 170, 319, 190, 1);
  //FT8_cmd_rect(0, 70, 319, 90, 1);
  //FT8_cmd_rect(0, 110, 319, 130, 1);
  //FT8_cmd_rect(0, 150, 319, 170, 1);
  //FT8_cmd_rect(0, 190, 319, 210, 1);
  
  FT8_cmd_dl(DL_COLOR_RGB | 0xffff00);
  FT8_cmd_text(10,10+offset, 23, 0, "1.GSM");
  ft_custom_font_edit("2.Автоконтроль"); FT8_cmd_text(10,30+offset, 11, 0, outstring);
  ft_custom_font_edit("2.1.Коэф. подачи"); FT8_cmd_text(10,50+offset, 11, 0, outstring);
  ft_custom_font_edit("2.2.Коэф. вентилятора грубый"); FT8_cmd_text(10,70+offset, 11, 0, outstring); 
  ft_custom_font_edit("2.3.Коэф. вентилятора точный"); FT8_cmd_text(10,90+offset, 11, 0, outstring);  
  ft_custom_font_edit("3.Расписание"); FT8_cmd_text(10,110+offset, 11, 0, outstring);  
  ft_custom_font_edit("3.1.Расписание ЦО"); FT8_cmd_text(10,130+offset, 11, 0, outstring);  
  ft_custom_font_edit("3.2.Расписание ГВС"); FT8_cmd_text(10,150+offset, 11, 0, outstring);  
  ft_custom_font_edit("4.Журнал ошибок"); FT8_cmd_text(10,170+offset, 11, 0, outstring);  
  ft_custom_font_edit("5.Заводские настройки"); FT8_cmd_text(10,190+offset, 11, 0, outstring);  
  ft_custom_font_edit("6.Главный экран"); FT8_cmd_text(10,210+offset, 11, 0, outstring); 
  ft_custom_font_edit("7.Ручная топка"); FT8_cmd_text(10,230+offset, 11, 0, outstring); 
  ft_custom_font_edit("8.Сброс ошибок"); FT8_cmd_text(10,250+offset, 11, 0, outstring); 
  
  FT8_cmd_dl(DL_DISPLAY);
  FT8_cmd_dl(CMD_SWAP);
  FT8_end_cmd_burst(); /* stop writing to the cmd-fifo */
  FtCmdStart();    
}
void ft812_userL7(uint8_t paramMenu){
  FT8_start_cmd_burst();
  FT8_cmd_dl(CMD_DLSTART);
  FT8_cmd_dl(DL_CLEAR_RGB | 0x000000);
  FT8_cmd_dl(DL_CLEAR | CLR_COL | CLR_STN | CLR_TAG);

  FT8_cmd_dl(DL_COLOR_RGB | GREY_COLOR);
  uint8_t offset = 20;

  FT8_cmd_dl(DL_COLOR_RGB | 0xeb9123);
  ft_custom_font_edit("Заполнить бункер"); FT8_cmd_text(10,5, 11, 0, outstring);
  FT8_cmd_line(0,25,319,25,3);
  FT8_cmd_dl(DL_COLOR_RGB | 0x246e37);
  if(paramMenu == 0)FT8_cmd_rect(0, 30, 319, 50, 1);
  if(paramMenu == 1)FT8_cmd_rect(0, 50, 319, 70, 1);
  
  FT8_cmd_dl(DL_COLOR_RGB | 0xffff00);
  ft_custom_font_edit("1.Заполнить 100%"); FT8_cmd_text(10,10+offset, 11, 0, outstring);
  ft_custom_font_edit("2.Пустой 0%"); FT8_cmd_text(10,30+offset, 11, 0, outstring);
  
  FT8_cmd_dl(DL_DISPLAY);
  FT8_cmd_dl(CMD_SWAP);
  FT8_end_cmd_burst(); /* stop writing to the cmd-fifo */
  FtCmdStart();    
}

uint8_t setCurrentLimit(uint16_t current){
  uint16_t tCurrent = current;
  if(tCurrent > 5000)tCurrent = 5000;
  termStruct.screwCurrentLimit = tCurrent;
  
  return 0;
}
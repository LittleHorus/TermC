#include "stm32f0xx_conf.h"
#include "stm32f0xx.h"
#include "main.h"
#include "ft812.h"


void button_port_config(void);
void tim2_init(void);
void adc_init(void);
void ft812_mainwindow(void);
void ft812_init(char *str1, char *str2);
void spi_init(void);
void delay_ms(uint32_t msecDelay);
void ft812_custom_font_init(void);
void ft812_mainWindowCustom(void);

GPIO_InitTypeDef GPIO_InitStructure;
ADC_InitTypeDef ADC__InitStructure;
DMA_InitTypeDef DMA__InitStructure;

#define MEM_PIC1 (17493+148+1000)

uint8_t regIdValue = 0;

uint16_t adc_ch_array[8];
uint32_t main_counter = 0;
uint32_t delay_ms_counter = 0;
uint8_t temp_actualGVSLevel_value[2], temp_needGVSLevel_value[2], temp_actualCOLevel_value[2], temp_needCOLevel_value[2];

float temperature[8];//4*8 = 32 
uint16_t temperature_u16[8];
extern const uint8_t fontData[17493];
extern const uint8_t MetricBlock[148];
extern const uint8_t PictT[1152];
char string[10];
char outstring[50];

/******************************************************************************/
int main(){
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
  
  adc_init(); 
  spi_init();
  tim2_init();
  
  DISPLAY_POWER_UP;
  ft812_init("Initialization", "please, waiting...");
  delay_ms(1000);
  //ft812_mainwindow();
  ft812_custom_font_init();
  ft812_mainWindowCustom();
  while(1){

    temp_actualGVSLevel_value[0] = adc_ch_array[5];
    temp_actualGVSLevel_value[1] = adc_ch_array[5];

    temp_needGVSLevel_value[0] = 0x30 + 6;
    temp_needGVSLevel_value[1] = 0x30 + 0;    

    temp_actualCOLevel_value[0] = adc_ch_array[6];
    temp_actualCOLevel_value[1] = adc_ch_array[6];

    temp_needCOLevel_value[0] = 0x30 + 6;
    temp_needCOLevel_value[1] = 0x30 + 0;    

    string[0] = '�';
    string[1] = ' ';
    
  }//while(1)
  
}//main()

/******************************************************************************/
void adc_init(){ 
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
  //разрешаем тактирование модуля таймера 2
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); 
  //объявляем переменную для настройки таймера 2
  TIM_TimeBaseInitTypeDef TIM2_InitStructure;
  //объявляем переменную для настройки прерываний от таймера 2
  NVIC_InitTypeDef NVIC_TIM2_Init_Structure;
  //указываем источник прерываний - TIM2_IRQn (список всех возможных источников находится в файле stm32f0xx.h)
  NVIC_TIM2_Init_Structure.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_TIM2_Init_Structure.NVIC_IRQChannelPriority = 0x00;//приоритет текущего прерывания, от 0 до 3
  NVIC_TIM2_Init_Structure.NVIC_IRQChannelCmd = ENABLE;  
  NVIC_Init(&NVIC_TIM2_Init_Structure); 
  
  TIM2_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM2_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM2_InitStructure.TIM_Period = 99;//1MHz / (99 + 1) = 10kHz
  TIM2_InitStructure.TIM_Prescaler = 47;//48MHz / (47+1) = 1MHz OUT  
  TIM2_InitStructure.TIM_RepetitionCounter = 0;//used only with TIM1
  TIM_TimeBaseInit(TIM2, &TIM2_InitStructure);

  TIM_Cmd(TIM2, ENABLE);//запускаем таймер
  TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);//разрешаем прерывания от таймера 2 по событию переполнения  
}
/******************************************************************************/
void TIM2_IRQHandler(void){ 
  if(TIM_GetITStatus(TIM2, TIM_IT_Update) == SET){
    main_counter++;
    if(main_counter >= 10000)main_counter = 0;//1sec
    if(delay_ms_counter != 0)delay_ms_counter--;
  
/*
  if(display_refresh_cnt != 0){
    display_tag = FtGetTouchTag();
    tag_x = (uint16_t)(display_tag & 0x0000ffff);
    tag_y = (uint16_t)(display_tag>>16);   
    //button_delay_cnt = 100;    
  }  
  button_response_cnt++;
  if((tag_x != 8000)&&(tag_y != 8000)&&(menu_change_button_timeout == 0)){
    if(display_menu_level == SET_IP_ADDRESS_MENU){
      if(((tag_x<=col_0)&&(tag_x>=col_1))&&((tag_y<=row_0)&&(tag_y>=row_1)))button_1_press_cnt++;
      if(((tag_x<=col_0)&&(tag_x>=col_1))&&((tag_y<=row_2)&&(tag_y>=row_3)))button_2_press_cnt++;
      if(((tag_x<=col_0)&&(tag_x>=col_1))&&((tag_y<=row_4)&&(tag_y>=row_5)))button_3_press_cnt++;
      
      if(((tag_x<=col_2)&&(tag_x>=col_3))&&((tag_y<=row_0)&&(tag_y>=row_1)))button_4_press_cnt++;
      if(((tag_x<=col_2)&&(tag_x>=col_3))&&((tag_y<=row_2)&&(tag_y>=row_3)))button_5_press_cnt++;
      if(((tag_x<=col_2)&&(tag_x>=col_3))&&((tag_y<=row_4)&&(tag_y>=row_5)))button_6_press_cnt++;

      if(((tag_x<=col_4)&&(tag_x>=col_5))&&((tag_y<=row_0)&&(tag_y>=row_1)))button_7_press_cnt++;
      if(((tag_x<=col_4)&&(tag_x>=col_5))&&((tag_y<=row_2)&&(tag_y>=row_3)))button_8_press_cnt++;
      if(((tag_x<=col_4)&&(tag_x>=col_5))&&((tag_y<=row_4)&&(tag_y>=row_5)))button_9_press_cnt++;

      if(((tag_x<=col_6)&&(tag_x>=col_7))&&((tag_y<=row_0)&&(tag_y>=row_1)))button_ok_press_cnt++;
      if(((tag_x<=col_6)&&(tag_x>=col_7))&&((tag_y<=row_2)&&(tag_y>=row_3)))button_0_press_cnt++;
      if(((tag_x<=col_6)&&(tag_x>=col_7))&&((tag_y<=row_4)&&(tag_y>=row_5)))button_cancel_press_cnt++;            
    }
    if(display_menu_level == MAIN_MENU){
      if(((tag_x>=800)&&(tag_x<=900))&&((tag_y>=590)&&(tag_y<=755)))main_menu_fan_button_press_cnt++;
      if(((tag_x>=800)&&(tag_x<=900))&&((tag_y>=780)&&(tag_y<=950)))main_menu_lan_button_press_cnt++;
    }
    if(display_menu_level == COMMUNICATION_MENU){
      if(((tag_x>=800)&&(tag_x<=910))&&((tag_y>=60)&&(tag_y<=220))&&(_menu_param_changed == 1))lan_menu_apply_button_press_cnt++;
      if(((tag_x>=800)&&(tag_x<=910))&&((tag_y>=780)&&(tag_y<=950)))lan_menu_exit_button_press_cnt++;
      
      if(((tag_x>=620)&&(tag_x<=750))&&((tag_y>=385)&&(tag_y<=900)))lan_menu_ip_button_cnt++;
      if(((tag_x>=430)&&(tag_x<=560))&&((tag_y>=385)&&(tag_y<=900)))lan_menu_subnet_button_cnt++;
      if(((tag_x>=250)&&(tag_x<=380))&&((tag_y>=385)&&(tag_y<=900)))lan_menu_ipg_button_cnt++;
      
    }
    
    if(display_menu_level == FAN_MENU){
      if(((tag_x>=500)&&(tag_x<650))&&((tag_y>=490)&&(tag_y<610)))button_minus_press_cnt++;
      if(((tag_x>=500)&&(tag_x<650))&&((tag_y>=820)&&(tag_y<930)))button_plus_press_cnt++;
      
      //if((((tag_x>=800))&&(tag_y<200))&&(_menu_param_changed == 1))button_apply_press_cnt++; 
      if(((tag_x>=800)&&(tag_x<=910))&&((tag_y>=60)&&(tag_y<=220))&&(_menu_param_changed == 1))fan_menu_apply_button_press_cnt++;
      if(((tag_x>=800)&&(tag_x<=910))&&((tag_y>=780)&&(tag_y<=950)))fan_menu_exit_button_press_cnt++;
    }
  }
*/    
    
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
    if(string[ilen] == '�')outstring[ilen] = 117;
    if(string[ilen] == '�')outstring[ilen] = 99;
    
    
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
  wReg8(REG_PWM_DUTY, 80);
  
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
  ft_custom_font_edit("��������������");
  FT8_cmd_text(10,5, 11, 0, outstring);
  FT8_cmd_text(315,5, 23, 2048, "16.03.20 18:56");
  
  ft_custom_font_edit("���. �.��"); //60�
  FT8_cmd_text(10,10+offset, 11, 0, outstring);
  FT8_cmd_text(80,40+offset, 25, 2048, "76");
  FT8_cmd_text(85,40+offset, 19, 2048, "\xf8");
  //ft_custom_font_edit("�.��"); //60�
  //FT8_cmd_text(10,60+offset, 11, 0, outstring);  
  FT8_cmd_text(60,90+offset, 25, 2048, "72");
  FT8_cmd_text(65,90+offset, 19, 2048, "\xf8");
  
  ft_custom_font_edit("���. �.���");// 47�
  FT8_cmd_text(140,10+offset, 11, 0, outstring);
  FT8_cmd_text(210,40+offset, 25, 2048, "65");
  FT8_cmd_text(215,40+offset, 19, 2048, "\xf8");
  //ft_custom_font_edit("�.���");// 47�
  //FT8_cmd_text(140,60+offset, 11, 0, outstring);  
  FT8_cmd_text(190,90+offset, 25, 2048, "53");
  FT8_cmd_text(195,90+offset, 19, 2048, "\xf8");
  
  /*
  ft_custom_font_edit("������ <:02>");
  FT8_cmd_text(10,70, 11, 0, outstring);
  ft_custom_font_edit("��������� ^�");
  FT8_cmd_text(10,90, 11, 0, outstring);  
  ft_custom_font_edit("����� �� �����");
  FT8_cmd_text(10,110, 11, 0, outstring);
  ft_custom_font_edit("��� �����");
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
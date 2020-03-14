
#define FT_SPI          SPI2
#define CS_LOW          GPIO_ResetBits(GPIOC, GPIO_Pin_8)
#define CS_HIGH         GPIO_SetBits(GPIOC, GPIO_Pin_8)

#define FT_SPI_CS_LOW GPIO_ResetBits(GPIOC, GPIO_Pin_8)
#define FT_SPI_CS_HIGH GPIO_SetBits(GPIOC, GPIO_Pin_8)


//host command group
#define CMD_ACTIVE 00
#define CMD_STANDBY 0x41
#define CMD_SLEEP 0x42
#define CMD_PWRDOWN 0x43

#define CMD_CLKEXT 0x44 //Select PLL input from external crystal
#define CMD_CLKINT 0x48
#define CMD_CLKSEL 0x61//62 // not release at current version
#define CMD_RST_PULSE 0x68

#define WRITE_MEM 0x80//b`1000 0000

//end


#define RAM_DL                                  0x300000//DISPLAY List RAM 8kb end adress = 0x301FFF
#define RAM_REG                                 0x302000//Registers                 
#define RAM_CMD                                 0x308000//Command buffer

#define REG_ID                                  0x302000 //read only (7C) |8  bits |
#define REG_FRAMES                              0x302004 //read only |32 bits |
#define REG_CLOCK                               0x302008 //read only |32 bits |
#define REG_FREQUENCY                           0x30200C//rw reset - 60000000 (Hz) |28 bits | 
#define REG_RENDERMODE                          0x302010//rw Rendering mode 0 = normal, 1 = single-line |1 bit|
#define REG_SNAPY                               0x302014//rw Scanline select for RENDERMODE 1 | 11 bits |
#define REG_SNAPSHOT                            0x302018//rw
#define REG_SNAPFORMAT                          0x30201C//rw
#define REG_CPURESET                            0x302020//rw
#define REG_TAP_CRC                             0x302024//ro
#define REG_TAP_MASK                            0x302028//rw
#define REG_HCYCLE                              0x30202C//rw | 12 bits | (224h)
#define REG_HOFFSET                             0x302030//rw
#define REG_HSIZE                               0x302034//rw
#define REG_HSYNC0                              0x302038//rw
#define REG_HSYNC1                              0x30203C//rw
#define REG_VCYCLE                              0x302040//rw
#define REG_VOFFSET                             0x302044//rw
#define REG_VSIZE                               0x302048//rw
#define REG_VSYNC0                              0x30204C//rw
#define REG_VSYNC1                              0x302050//rw
#define REG_DLSWAP                              0x302054//rw
#define REG_ROTATE                              0x302058//rw
#define REG_OUTBITS                             0x30205C//rw
#define REG_DITHER                              0x302060//rw
#define REG_SWIZZLE                             0x302064//rw
#define REG_CSPREAD                             0x302068//rw
#define REG_PCLK_POL                            0x30206C//rw
#define REG_PCLK                                0x302070//rw
#define REG_TAG_X                               0x302074//rw
#define REG_TAG_Y                               0x302078//rw
#define REG_TAG                                 0x30207C//ro
#define REG_VOL_PB                              0x302080//rw
#define REG_VOL_SOUND                           0x302084//rw
#define REG_SOUND                               0x302088//rw
#define REG_PLAY                                0x30208C//rw
#define REG_GPIO_DIR                            0x302090//rw
#define REG_GPIO                                0x302094//rw
#define REG_GPIOX_DIR                           0x302098//rw
#define REG_GPIOX                               0x30209C//rw
#define REG_INT_FLAGS                           0x3020A8//ro
#define REG_INT_EN                              0x3020AC//rw
#define REG_INT_MASK                            0x3020B0//rw
#define REG_PLAYBACK_START                      0x3020B4//rw
#define REG_PLAYBACK_LENGTH                     0x3020B8//rw
#define REG_PLAYBACK_READPTR                    0x3020BC//rw
#define REG_PLAYBACK_FREQ                       0x3020C0//rw
#define REG_PLAYBACK_FORMAT                     0x3020C4//rw
#define REG_PLAYBACK_LOOP                       0x3020C8//rw
#define REG_PLAYBACK_PLAY                       0x3020CC//rw
#define REG_PWM_HZ                              0x3020D0//rw
#define REG_PWM_DUTY                            0x3020D4//rw
#define REG_MACRO_0                             0x3020D8//rw
#define REG_MACRO_1                             0x3020DC//rw
#define REG_CMD_READ                            0x3020F8//rw
#define REG_CMD_WRITE                           0x3020FC//ro
#define REG_CMD_DL                              0x302100//rw
#define REG_TOUCH_MODE                          0x302104//rw
#define REG_TOUCH_ADC_MODE                      0x302108//rw
#define REG_TOUCH_CHARGE                        0x30210C//rw
#define REG_TOUCH_SETTLE                        0x302110//rw
#define REG_TOUCH_OVERSAMPLE                    0x302114//rw
#define REG_TOUCH_RZTHRESH                      0x302118//rw
#define REG_TOUCH_RAW_XY                        0x30211C//ro
#define REG_TOUCH_RZ                            0x302120//ro
#define REG_TOUCH_SCREEN_XY                     0x302124//ro
#define REG_TOUCH_TAG_XY                        0x302128//ro
#define REG_TOUCH_TAG                           0x30212C//ro
#define REG_TOUCH_TAG1_XY                       0x302130//ro
#define REG_TOUCH_TAG1                          0x302134//ro
#define REG_TOUCH_TAG2_XY                       0x302138//ro
#define REG_TOUCH_TAG2                          0x30213C//ro
#define REG_TOUCH_TAG3_XY                       0x302140//ro
#define REG_TOUCH_TAG3                          0x302144//ro
#define REG_TOUCH_TAG4_XY                       0x302148//ro
#define REG_TOUCH_TAG4                           0x30214C//ro
#define REG_TOUCH_TRANSFORM_A                   0x302150//rw
#define REG_TOUCH_TRANSFORM_B                   0x302154//rw
#define REG_TOUCH_TRANSFORM_C                   0x302158//rw
#define REG_TOUCH_TRANSFORM_D                   0x30215C//rw
#define REG_TOUCH_TRANSFORM_E                   0x302160//rw
#define REG_TOUCH_TRANSFORM_F                   0x302164//rw
#define REG_TOUCH_CONFIG                        0x302168//rw
#define REG_CTOUCH_TOUCH4_X                     0x30216C//rw

#define REG_TRIM                                0x302180//rw
#define REG_ANA_COMP                            0x302184//rw
#define REG_SPI_WIDTH                           0x302188//rw
#define REG_TOUCH_DIRECT_XY                     0x30218C//REG_CTOUCH_TOUCH3_X
#define REG_DATESTAMP                           0x302564//r0
#define REG_CMDB_SPACE                          0x302574//rw
#define REG_CMDB_WRITE                          0x302578//wo

//end

/* Coprocessor related commands */
#define CMD_APPEND				0xFFFFFF1EUL
#define CMD_BGCOLOR				0xFFFFFF09UL
#define CMD_BUTTON				0xFFFFFF0DUL
#define CMD_CALIBRATE			        0xFFFFFF15UL
#define CMD_CLOCK				0xFFFFFF14UL
#define CMD_COLDSTART			        0xFFFFFF32UL
#define CMD_DIAL				0xFFFFFF2DUL
#define CMD_DLSTART				0xFFFFFF00UL
#define CMD_FGCOLOR				0xFFFFFF0AUL
#define CMD_GAUGE				0xFFFFFF13UL
#define CMD_GETMATRIX			        0xFFFFFF33UL
#define CMD_GETPROPS			        0xFFFFFF25UL
#define CMD_GETPTR				0xFFFFFF23UL
#define CMD_GRADCOLOR			        0xFFFFFF34UL
#define CMD_GRADIENT			        0xFFFFFF0BUL
#define CMD_INFLATE				0xFFFFFF22UL
#define CMD_INTERRUPT			        0xFFFFFF02UL
#define CMD_KEYS				0xFFFFFF0EUL
#define CMD_LOADIDENTITY		        0xFFFFFF26UL
#define CMD_LOADIMAGE			        0xFFFFFF24UL
#define CMD_LOGO				0xFFFFFF31UL
#define CMD_MEMCPY				0xFFFFFF1DUL
#define CMD_MEMCRC				0xFFFFFF18UL
#define CMD_MEMSET				0xFFFFFF1BUL
#define CMD_MEMWRITE			        0xFFFFFF1AUL
#define CMD_MEMZERO				0xFFFFFF1CUL
#define CMD_NUMBER				0xFFFFFF2EUL
#define CMD_PROGRESS			        0xFFFFFF0FUL
#define CMD_REGREAD				0xFFFFFF19UL
#define CMD_ROTATE				0xFFFFFF29UL
#define CMD_SCALE				0xFFFFFF28UL
#define CMD_SCREENSAVER			        0xFFFFFF2FUL
#define CMD_SCROLLBAR			        0xFFFFFF11UL
#define CMD_SETFONT				0xFFFFFF2BUL
#define CMD_SETMATRIX			        0xFFFFFF2AUL
#define CMD_SKETCH				0xFFFFFF30UL
#define CMD_SLIDER				0xFFFFFF10UL
#define CMD_SNAPSHOT			        0xFFFFFF1FUL
#define CMD_SPINNER				0xFFFFFF16UL
#define CMD_STOP				0xFFFFFF17UL
#define CMD_SWAP				0xFFFFFF01UL
#define CMD_TEXT				0xFFFFFF0CUL
#define CMD_TOGGLE				0xFFFFFF12UL
#define CMD_TRACK				0xFFFFFF2CUL
#define CMD_TRANSLATE	                        0xFFFFFF27UL

#define CMD_MEDIAFIFO			        0xFFFFFF39UL
#define CMD_PLAYVIDEO			        0xFFFFFF3AUL
#define CMD_ROMFONT				0xFFFFFF3FU
#define CMD_SETBASE				0xFFFFFF38UL
#define CMD_SETBITMAP			        0xFFFFFF43UL
#define CMD_SETFONT2			        0xFFFFFF3BUL
#define CMD_SETROTATE			        0xFFFFFF36UL
#define CMD_SETSCRATCH			        0xFFFFFF3CUL
#define CMD_SNAPSHOT2			        0xFFFFFF37UL
#define CMD_VIDEOFRAME			        0xFFFFFF41UL
#define CMD_VIDEOSTART	                        0xFFFFFF40UL

#define BEGIN(primitive)                        (0x1F000000 | (primitive & 15))//
#define BITMAPS                                 0x01//
#define POINTS                                  0x02//
#define LINES                                   0x03//
#define LINE_STRIP                              0x04//
#define EDGE_STRIP_R                            0x05//right side
#define EDGE_STRIP_L                            0x06//left side
#define EDGE_STRIP_A                            0x07//above
#define EDGE_STRIP_B                            0x08//below
#define RECTS                                   0x09//rectangle

#define VERTEX2II(x,y,handle,cell)              ((2UL<<30)|(((x)&511UL)<<21)|(((y)&511UL)<<12)|(((handle)&31UL)<<7)|(((cell)&127UL)<<0))
#define ALPHA_FUNC(func,ref)                    ((9UL<<24)|(((func)&7UL)<<8)|(((ref)&255UL)<<0))
#define CLEAR(c,s,t)                            ((38UL<<24)|(((c)&1UL)<<2)|(((s)&1UL)<<1)|(((t)&1UL)<<0))
#define CLEAR_COLOR_A(alpha)                    ((15UL<<24)|(((alpha)&255UL)<<0))
#define CLEAR_COLOR_RGB(red,green,blue)         ((2UL<<24)|(((red)&255UL)<<16)|(((green)&255UL)<<8)|(((blue)&255UL)<<0))
#define CLEAR_STENCIL(s)                        ((17UL<<24)|(((s)&255UL)<<0))
#define CLEAR_TAG(s)                            ((18UL<<24)|(((s)&255UL)<<0))
#define COLOR_A(alpha)                          ((16UL<<24)|(((alpha)&255UL)<<0))
#define COLOR_MASK(r,g,b,a)                     ((32UL<<24)|(((r)&1UL)<<3)|(((g)&1UL)<<2)|(((b)&1UL)<<1)|(((a)&1UL)<<0))
#define COLOR_RGB(red,green,blue)               ((4UL<<24)|(((red)&255UL)<<16)|(((green)&255UL)<<8)|(((blue)&255UL)<<0))
#define END()                                   ((33UL<<24))
#define JUMP(dest)                              ((30UL<<24)|(((dest)&65535UL)<<0))
#define LINE_WIDTH(width)                       ((14UL<<24)|(((width)&4095UL)<<0))
#define MACRO(m)                                ((37UL<<24)|(((m)&1UL)<<0))
#define POINT_SIZE(size)                        ((13UL<<24)|(((size)&8191UL)<<0))
#define RESTORE_CONTEXT()                       ((35UL<<24))
#define RETURN()                                ((36UL<<24))
#define SAVE_CONTEXT()                          ((34UL<<24))
#define STENCIL_FUNC(func,ref,mask)             ((10UL<<24)|(((func)&7UL)<<16)|(((ref)&255UL)<<8)|(((mask)&255UL)<<0))
#define STENCIL_MASK(mask)                      ((19UL<<24)|(((mask)&255UL)<<0))
#define STENCIL_OP(sfail,spass)                 ((12UL<<24)|(((sfail)&7UL)<<3)|(((spass)&7UL)<<0))
#define TAG(s)                                  ((3UL<<24)|(((s)&255UL)<<0))
#define TAG_MASK(mask)                          ((20UL<<24)|(((mask)&1UL)<<0))
#define VERTEX2F(x,y)                           ((1UL<<30)|(((x)&32767UL)<<15)|(((y)&32767UL)<<0))
#define DISPLAY()                               0x00000000
#define DL_END                                  0x21000000
#define DL_CLEAR		0x26000000UL /* requires OR'd arguments */
#define DL_CLEAR_RGB	0x02000000UL /* requires OR'd arguments */
#define DL_COLOR_RGB	0x04000000UL /* requires OR'd arguments */
#define DL_POINT_SIZE	0x0D000000UL /* requires OR'd arguments */
#define CLR_COL              0x4
#define CLR_STN              0x2
#define CLR_TAG 0x1
#define DL_DISPLAY	0x00000000UL

/* Bitmap formats */
#define FT8_ARGB1555             0UL
#define FT8_L1                   1UL
#define FT8_L4                   2UL
#define FT8_L8                   3UL
#define FT8_RGB332               4UL
#define FT8_ARGB2                5UL
#define FT8_ARGB4                6UL
#define FT8_RGB565               7UL
#define FT8_PALETTED             8UL
#define FT8_TEXT8X8              9UL
#define FT8_TEXTVGA              10UL
#define FT8_BARGRAPH             11UL

/* Bitmap filter types */
#define FT8_NEAREST              0UL
#define FT8_BILINEAR             1UL


/* Bitmap wrap types */
#define FT8_BORDER               0UL
#define FT8_REPEAT               1UL

/* Widget command */
#define FT8_OPT_MONO             1
#define FT8_OPT_NODL             2
#define FT8_OPT_FLAT             256
#define FT8_OPT_CENTERX          512
#define FT8_OPT_CENTERY          1024
#define FT8_OPT_CENTER           (FT8_OPT_CENTERX | FT8_OPT_CENTERY)
#define FT8_OPT_NOBACK           4096
#define FT8_OPT_NOTICKS          8192
#define FT8_OPT_NOHM             16384
#define FT8_OPT_NOPOINTER        16384
#define FT8_OPT_NOSECS           32768
#define FT8_OPT_NOHANDS          49152
#define FT8_OPT_RIGHTX           2048
#define FT8_OPT_SIGNED           256

#define BITMAP_LAYOUT_H(linestride,height) ((40UL<<24)|((((linestride&0xC00)>>10)&3UL)<<2)|((((height&0x600)>>9)&3UL)<<0))
#define BITMAP_SIZE_H(width,height) ((41UL<<24)|((((width&0x600)>>9)&3UL)<<2)|((((height&0x600)>>9)&3UL)<<0))

#define BITMAP_SOURCE(addr) ((1UL<<24)|(((addr)&4194303UL)<<0))

#define BITMAP_HANDLE(handle) ((5UL<<24)|(((handle)&31UL)<<0))
#define BITMAP_LAYOUT(format,linestride,height) ((7UL<<24)|(((format)&31UL)<<19)|(((linestride)&1023UL)<<9)|(((height)&511UL)<<0))
#define BITMAP_SIZE(filter,wrapx,wrapy,width,height) ((8UL<<24)|(((filter)&1UL)<<20)|(((wrapx)&1UL)<<19)|(((wrapy)&1UL)<<18)|(((width)&511UL)<<9)|(((height)&511UL)<<0))


#define BITMAP_SOURCE(addr) ((1UL<<24)|(((addr)&4194303UL)<<0))
#define NOP() ((45UL<<24))
#define PALETTE_SOURCE(addr) ((42UL<<24)|(((addr)&4194303UL)<<0))
#define SCISSOR_SIZE(width,height) ((28UL<<24)|(((width)&4095UL)<<12)|(((height)&4095UL)<<0))
#define SCISSOR_XY(x,y) ((27UL<<24)|(((x)&2047UL)<<11)|(((y)&2047UL)<<0))
#define VERTEX_FORMAT(frac) ((39UL<<24)|(((frac)&7UL)<<0))
#define VERTEX_TRANSLATE_X(x) ((43UL<<24)|(((x)&131071UL)<<0))
#define VERTEX_TRANSLATE_Y(y) ((44UL<<24)|(((y)&131071UL)<<0))

void spiTransfer(uint8_t data);
uint8_t spiTransferReceive(uint8_t data);
void ft_spi_cs_set();
void ft_spi_cs_reset();


void wHostCMD(uint8_t CMD);
void wReg8(uint32_t ft_adress, uint8_t ft_data);
void wReg16(uint32_t ft_adress, uint16_t ft_data);
void wReg32(uint32_t ft_adress, uint32_t ft_data);
uint8_t rReg8(uint32_t ft_adress);
uint16_t rReg16(uint32_t ft_adress);
uint32_t rReg32(uint32_t ft_adress);
uint8_t FtBusy(void);
uint32_t FtGetTouchTag(void);
void FtCmdStart(void);
void FtCmdExecute(void);
uint16_t FT8_report_cmdoffset(void);
void FT8_inc_cmdoffset(uint16_t increment);
void FT8_start_cmd_burst(void);
void FT8_end_cmd_burst(void);
void FT8_start_cmd(uint32_t command);
void FT8_cmd_dl(uint32_t command);
void FT8_write_string(const char *text);
void FT8_cmd_text(int16_t x0, int16_t y0, int16_t font, uint16_t options, const char* text);
void FT8_cmd_button(int16_t x0, int16_t y0, int16_t w0, int16_t h0, int16_t font, uint16_t options, const char* text);
void FT8_cmd_bgcolor(uint32_t color);
void FT8_cmd_fgcolor(uint32_t color);
void FT8_cmd_gradcolor(uint32_t color);
void FT8_cmd_gradient(int16_t x0, int16_t y0, uint32_t rgb0, int16_t x1, int16_t y1, uint32_t rgb1);
void FT8_cmd_keys(int16_t x0, int16_t y0, int16_t w0, int16_t h0, int16_t font, uint16_t options, const char* text);
void FT8_cmd_progress(int16_t x0, int16_t y0, int16_t w0, int16_t h0, uint16_t options, uint16_t val, uint16_t range);
void FT8_cmd_toggle(int16_t x0, int16_t y0, int16_t w0, int16_t font, uint16_t options, uint16_t state, const char* text);
void FT8_cmd_setbase(uint32_t base);
void FT8_cmd_setbitmap(uint32_t addr, uint16_t fmt, uint16_t width, uint16_t height);
void FT8_cmd_number(int16_t x0, int16_t y0, int16_t font, uint16_t options, int32_t number);
void FT8_cmd_append(uint32_t ptr, uint32_t num);
void FT8_cmd_interrupt(uint32_t ms);
void FT8_cmd_point(int16_t x0, int16_t y0, uint16_t size);
void FT8_cmd_line(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t width);
void FT8_cmd_rect(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t corner);
void FT8_cmd_spinner(int16_t x0, int16_t y0, uint16_t style, uint16_t scale);
uint16_t FT8_cmd_getprops(uint32_t ptr);
uint16_t FT8_cmd_regread(uint32_t ptr);
uint16_t FT8_cmd_getptr(void);
uint16_t FT8_cmd_memcrc(uint32_t ptr, uint32_t num);
void FT8_cmd_track(int16_t x0, int16_t y0, int16_t w0, int16_t h0, int16_t tag);
void FT8_cmd_snapshot2(uint32_t fmt, uint32_t ptr, int16_t x0, int16_t y0, int16_t w0, int16_t h0);
void FT8_cmd_snapshot(uint32_t ptr);
void FT8_cmd_sketch(int16_t x0, int16_t y0, uint16_t w0, uint16_t h0, uint32_t ptr, uint16_t format);
void FT8_cmd_setscratch(uint32_t handle);
void FT8_cmd_setrotate(uint32_t r);
void FT8_cmd_setfont2(uint32_t font, uint32_t ptr, uint32_t firstchar);
void FT8_cmd_setfont(uint32_t font, uint32_t ptr);
void FT8_cmd_romfont(uint32_t font, uint32_t romslot);
void FT8_cmd_calibrate(void);
void FT8_cmd_getmatrix(int32_t a, int32_t b, int32_t c, int32_t d, int32_t e, int32_t f);
void FT8_cmd_rotate(int32_t ang);
void FT8_cmd_scale(int32_t sx, int32_t sy);
void FT8_cmd_translate(int32_t tx, int32_t ty);
void FT8_cmd_mediafifo(uint32_t ptr, uint32_t size);
void FT8_cmd_loadimage(uint32_t ptr, uint32_t options, const uint8_t *data, uint16_t len);
void FT8_cmd_inflate(uint32_t ptr, const uint8_t *data, uint16_t len);
void FT8_memWrite_flash_buffer(uint32_t ftAddress, const uint8_t *data, uint16_t len);











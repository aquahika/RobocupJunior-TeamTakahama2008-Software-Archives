/*********************************************************************************************

   �t�@�C����	:aki_lcd.h

   ���e		:Akizuki 16x2 LCD Setting and Output Header File.

   �쐬		:Hikaru from TeamTakahama

   Version		:0.90(2008-1-2) Intial

*********************************************************************************************/

void main(void);						/* ���C���֐�		 */
void lcd_init(void);						/* LCD�����ݒ�		 */
void lcdwriteinit( unsigned char);				/* LCD�����ݒ�R�}���h�o�� */
void lcdwrite1command( unsigned char);			/* LCD1�R�}���h�o��	 */
void lcdwrite1data( unsigned char );				/* LCD1�f�[�^�o��	 */
void wait1( void );						/* 0.1ms�E�G�C�g	 */
void wait2( void );						/* 4.1ms�E�G�C�g	 */
void wait3( void );						/* 15ms�E�G�C�g	 */
unsigned char lcdbusycheck( void );				/* LCD�r�W�[�`�F�b�N */
void lcddebug1(unsigned char,unsigned char[],unsigned long);
void lcdvar(char,unsigned long);
void lcdtext(unsigned char para,unsigned char text[]);
void lcdclear( void );

#define	PORTIN		0x0				/* �|�[�g�������W�X�^����͂ɐݒ肷��ׂ̃f�[�^ */
#define	PORTOUT	0xff				/* �|�[�g�������W�X�^���o�͂ɐݒ肷��ׂ̃f�[�^ */
#define	PORTOUTIN	0xf0				/* �|�[�g�������W�X�^�����4�r�b�g���o�� */
							/* ����4�r�b�g����͂ɐݒ肷��ׂ̃f�[�^ */
#define	LCDRS		p4_6				/* RS�[�q		 */
#define	LCDRW		p4_5				/* RW�[�q		 */
#define	LCDE		p4_4				/* E�[�q		 */
#define	HIGH		1				/* �[�q�o�́gH�h 	 */
#define	LOW		0				/* �[�q�o�́gL�h	 */
#define	LCD_COMMAND	0				/* RS-command�w	 */
#define	LCD_DATA	1				/* RS-data�w�� 	 */
#define	BUSY		1				/* LCD��������buzy 	 */
#define	NOBUSY		0				/* LCD��������OK 	 */
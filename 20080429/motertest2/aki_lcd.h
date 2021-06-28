/*********************************************************************************************

   ファイル名	:aki_lcd.h

   内容		:Akizuki 16x2 LCD Setting and Output Header File.

   作成		:Hikaru from TeamTakahama

   Version		:0.90(2008-1-2) Intial

*********************************************************************************************/

void main(void);						/* メイン関数		 */
void lcd_init(void);						/* LCD初期設定		 */
void lcdwriteinit( unsigned char);				/* LCD初期設定コマンド出力 */
void lcdwrite1command( unsigned char);			/* LCD1コマンド出力	 */
void lcdwrite1data( unsigned char );				/* LCD1データ出力	 */
void wait1( void );						/* 0.1msウエイト	 */
void wait2( void );						/* 4.1msウエイト	 */
void wait3( void );						/* 15msウエイト	 */
unsigned char lcdbusycheck( void );				/* LCDビジーチェック */
void lcddebug1(unsigned char,unsigned char[],unsigned long);
void lcdvar(char,unsigned long);
void lcdtext(unsigned char para,unsigned char text[]);
void lcdclear( void );

#define	PORTIN		0x0				/* ポート方向レジスタを入力に設定する為のデータ */
#define	PORTOUT	0xff				/* ポート方向レジスタを出力に設定する為のデータ */
#define	PORTOUTIN	0xf0				/* ポート方向レジスタを上位4ビットを出力 */
							/* 下位4ビットを入力に設定する為のデータ */
#define	LCDRS		p4_6				/* RS端子		 */
#define	LCDRW		p4_5				/* RW端子		 */
#define	LCDE		p4_4				/* E端子		 */
#define	HIGH		1				/* 端子出力“H” 	 */
#define	LOW		0				/* 端子出力“L”	 */
#define	LCD_COMMAND	0				/* RS-command指	 */
#define	LCD_DATA	1				/* RS-data指定 	 */
#define	BUSY		1				/* LCD書き込みbuzy 	 */
#define	NOBUSY		0				/* LCD書き込みOK 	 */
	
/***********************************************************************************************
LCD表示関係ソースファイル


***********************************************************************************************/
#include <stdio.h>
#include <string.h>
#include "aki_lcd.h"
#include "sfr62p.h"

/*--------------------------------------------------------------*/
/* 関数名	:lcd_init()					*/
/* 機能		:LCD初期設定					*/
/*--------------------------------------------------------------*/

void lcd_init(void)
{
	wait3();						/*15msウエイト*/
	lcdwriteinit( 0x03 );			/*LCDファンクションセット*/
	wait2();						/*4.1msウエイト*/
	lcdwriteinit( 0x03 );			/*LCDファンクションセット*/
	wait1();						/*0.1msウエイト*/
	lcdwriteinit( 0x03 );			/*LCDファンクションセット*/
	wait1();
	lcdwriteinit( 0x02 );			/*LCDデータを4ビット長に設定*/
	wait1();
	lcdwrite1command(0x28);			/*4bit､2行文､5×7ドットに設定*/
	wait1();

	/* 	ここまでで4ビット×2回のデータ転送ができるように設定されたので
	この後はビジーチェックをしてからデータ転送を行なう。 */
	
	while(BUSY == lcdbusycheck())	/*LCDビジーチェック*/
	;
	lcdwrite1command(0x08);			/*表示オフ*/

	while(BUSY == lcdbusycheck())	/*LCDビジーチェック*/
	;
	lcdwrite1command(0x01);			/*表示クリア*/

	while(BUSY == lcdbusycheck())	/*LCDビジーチェック*/
	;
	lcdwrite1command(0x06);			/*エントリーモード､インクリメント*/

	while(BUSY == lcdbusycheck())	/*LCDビジーチェック*/
	;
	lcdwrite1command(0x0c);			/*表示オン､カーソルオフ*/

}
/*----------------------------------------------------------*/
/* 関数名	:lcdwriteinit()									*/
/* 機能		:LCD初期設定コマンドセット					    */
/*----------------------------------------------------------*/

void lcdwriteinit( unsigned char command )
{

	p4 = 0x00;				/*P4初期値セット　RW－0(W指定) RS－0(コマンド指定）E－0 */
	pd4 = PORTOUT;			/*P4出力に設定 */

	command &= 0x0f;		/*P4コントロールデータセット(引数から）RW－0(W指定) RS－0(コマンド指定）E－0 */
	p4 = command;			/*P4コントロールデータ出力*/
	LCDE = HIGH;			/*E－１*/
#pragma ASM					/*アセンブラ表記*/
	NOP						/* 時間調整の為のNOP */
	NOP
	NOP
	NOP
#pragma ENDASM				/*アセンブラ表記終了*/
	LCDE = LOW;				/*E－0*/
}
/*----------------------------------------------------------*/
/* 関数名	:lcdwrite1command(）							*/
/* 機能		:LCDコマンド出力							    */
/*----------------------------------------------------------*/

void lcdwrite1command( unsigned char command )
{
	unsigned char outcommand;

	p4 = 0x00;				/*P4初期値セット　RW－0(W指定) RS－0(コマンド指定）E－0 */
	pd4 = PORTOUT;			/*P4出力に設定 */

	outcommand = command>>4;/*上位4ビットを下位4ビットへ*/
	outcommand &= 0x0f;		/*マスク処理*/
	p4 = outcommand;		/*コマンドデータ上位4ビットを出力　RW－0(W指定) RS－0(コマンド指定）E－0 */

	LCDE = HIGH;			/*E－１*/
#pragma ASM					/*アセンブラ表記*/
	NOP						/*時間調整の為のNOP*/
	NOP
	NOP
	NOP
#pragma ENDASM				/*アセンブラ表記終了*/
	LCDE = LOW;				/*E－0*/
	
	outcommand = command&0x0f;	/*コマンドデータの下位4ビット抽出(上位4ビットマスク)*/
	p4 = outcommand;			/*コマンドデータ下位4ビットを出力*/

	LCDE = HIGH;			/*E－１*/
#pragma ASM					/*アセンブラ表記*/
	NOP						/*時間調整の為のNOP*/
	NOP
	NOP
	NOP
#pragma ENDASM					/*アセンブラ表記終了*/
	LCDE = LOW;					/*E－0*/

}
/*----------------------------------------------------------*/
/* 関数名	:lcdwrite1data(）								*/
/* 機能		:LCDデータ出力								    */
/*----------------------------------------------------------*/

void	lcdwrite1data( unsigned char data )
{
	unsigned char lcddata;
	p4 = 0x00;				/*P4初期値セット　RW－0(W指定) RS－0(コマンド指定）E－0 */
	pd4 = PORTOUT;			/*P4出力に設定 */

	lcddata = data>>4;		/*LCDデータ上位4ビットを下位4ビットへ*/
	lcddata &= 0x0f;		/*マスク処理*/
	p4 = lcddata;			/*LCDデータ上位4ビットを出力*/
	LCDRS = LCD_DATA;		/*RS－1(データ指定）*/
	LCDE = HIGH;			/*E－１*/
#pragma ASM					/*アセンブラ表記*/
	NOP						/* 時間調整の為のNOP */
	NOP
	NOP
	NOP
#pragma ENDASM				/*アセンブラ表記終了*/
	LCDE = LOW;				/*E－0*/
	
	lcddata =data & 0x0f;	/*LCDデータの下位4ビット抽出(上位4ビットマスク)*/
	p4 = lcddata;			/*LCDデータ下位4ビットを出力*/

	LCDRS = LCD_DATA;		/*RS－1(データ指定）*/
	LCDE = HIGH;			/*E－1*/
#pragma ASM					/*アセンブラ表記*/
	NOP						/* 時間調整の為のNOP */
	NOP
	NOP
	NOP
#pragma ENDASM				/*アセンブラ表記終了*/
	LCDE = LOW;				/*E－0*/
}

/*----------------------------------------------------------*/
/* 関数名	:wait1()										*/
/* 機能		:0.1msウエイト								    */
/*----------------------------------------------------------*/
void wait1(void){			/* 約0.1msのウエイト */
#pragma ASM					/*アセンブラ表記*/
	MOV.W 	#0C8H,A0		/*カウンタ初期値セット*/
LOOP1:
	NOP
	NOP
	NOP
	DEC.W	A0
	JNZ	LOOP1				/*ループ終了？*/
#pragma ENDASM				/*アセンブラ表記終了*/
}
/*----------------------------------------------------------*/
/* 関数名	:wait2()										*/
/* 機能		:4.1msウエイト								    */
/*----------------------------------------------------------*/
void wait2(void){			/* 約4.1msのウエイト */
#pragma ASM					/*アセンブラ表記*/
	MOV.W 	#2007H,A0		/*カウンタ初期値セット*/
LOOP2:
	NOP
	NOP
	NOP
	DEC.W	A0
	JNZ	LOOP2				/*ループ終了？*/
#pragma ENDASM				/*アセンブラ表記終了*/
}
/*----------------------------------------------------------*/
/* 関数名	:wait3()										*/
/* 機能		:15msウエイト									*/
/*----------------------------------------------------------*/
void wait3(void){			/* 約15msのウエイト */
#pragma ASM					/*アセンブラ表記*/
	MOV.W 	#7530H,A0		/*カウンタ初期値セット*/
LOOP3:
	NOP
	NOP
	NOP
	DEC.W	A0
	JNZ	LOOP3				/*ループ終了？*/
#pragma ENDASM				/*アセンブラ表記終了*/
}
/*----------------------------------------------------------*/
/* 関数名	:lcdbusycheck()									*/
/* 機能		:LCDビジーチェック							    */
/*----------------------------------------------------------*/

unsigned char lcdbusycheck( void )
{
	unsigned char command_high,command_low,b_data;

	p4 = 0x00;				/*P4初期値セット　RW－0(W指定) RS－0(コマンド指定）E－0 */
	pd4 = PORTOUTIN;		/*ポート方向レジスタを上位4ビットを出力下位4ビットを入力に設定*/

	LCDRW = HIGH;			/*RW－1(R指定)*/

	LCDE = HIGH;			/*E－1*/
#pragma ASM					/*アセンブラ表記*/
	NOP						/* 時間調整の為のNOP */
	NOP
	NOP
	NOP
#pragma ENDASM				/*アセンブラ表記終了*/
	command_high = p4;		/*コマンド上位4ビット読み込み*/
	LCDE = LOW;				/*E－0*/

	command_high <<=4;		/*下位4ビットを上位へ*/
	command_high &= 0xf0;	/*上位4ビット抽出(下位4ビットマスク処理)*/

	LCDE = HIGH;			/*E－1*/
#pragma ASM					/*アセンブラ表記*/
	NOP						/* 時間調整の為のNOP */
	NOP
	NOP
	NOP
#pragma ENDASM				/*アセンブラ表記終了*/
	command_low = p4;		/*コマンド下位4ビット読み込み*/
	LCDE = LOW;				/*E－0*/

	command_low &= 0x0f;	/*下位4ビット抽出(上位4ビットマスク処理)*/
	b_data = command_high|command_low;	/* 2つの4ビットコマンドデータを8ビットにまとめる*/
	b_data &= 0x80;			/*ビット7のマスク処理*/
	if(b_data==0)
		b_data = NOBUSY;	/*ビット7が0ならLCD書き込みokを返す*/
	else
		b_data = BUSY;		/*ビット7が1ならLCD書き込みbuzyを返す*/
		
	return(b_data);
}

void lcddebug1(unsigned char para,unsigned char text[],unsigned long var)
{
char i;
unsigned long bf_ascii[10];
unsigned long af_ascii[10];
char chr_size;
char textsize;

bf_ascii[0]=var/1000000000;
bf_ascii[1]=(var-(bf_ascii[0]*1000000000))/100000000;
bf_ascii[2]=(var-(bf_ascii[0]*1000000000+bf_ascii[1]*100000000))/10000000;
bf_ascii[3]=(var-(bf_ascii[0]*1000000000+bf_ascii[1]*100000000+bf_ascii[2]*10000000))/1000000;
bf_ascii[4]=(var-(bf_ascii[0]*1000000000+bf_ascii[1]*100000000+bf_ascii[2]*10000000+bf_ascii[3]*1000000))/100000;
bf_ascii[5]=(var-(bf_ascii[0]*1000000000+bf_ascii[1]*100000000+bf_ascii[2]*10000000+bf_ascii[3]*1000000+bf_ascii[4]*100000))/10000;
bf_ascii[6]=(var-(bf_ascii[0]*1000000000+bf_ascii[1]*100000000+bf_ascii[2]*10000000+bf_ascii[3]*1000000+bf_ascii[4]*100000+bf_ascii[5]*10000))/1000;
bf_ascii[7]=(var-(bf_ascii[0]*1000000000+bf_ascii[1]*100000000+bf_ascii[2]*10000000+bf_ascii[3]*1000000+bf_ascii[4]*100000+bf_ascii[5]*10000+bf_ascii[6]*1000))/100;
bf_ascii[8]=(var-(bf_ascii[0]*1000000000+bf_ascii[1]*100000000+bf_ascii[2]*10000000+bf_ascii[3]*1000000+bf_ascii[4]*100000+bf_ascii[5]*10000+bf_ascii[6]*1000+bf_ascii[7]*100))/10;
bf_ascii[9]=(var-(bf_ascii[0]*1000000000+bf_ascii[1]*100000000+bf_ascii[2]*10000000+bf_ascii[3]*1000000+bf_ascii[4]*100000+bf_ascii[5]*10000+bf_ascii[6]*1000+bf_ascii[7]*100+bf_ascii[8]*10));

af_ascii[0]=bf_ascii[0]+48;
af_ascii[1]=bf_ascii[1]+48;
af_ascii[2]=bf_ascii[2]+48;
af_ascii[3]=bf_ascii[3]+48;
af_ascii[4]=bf_ascii[4]+48;
af_ascii[5]=bf_ascii[5]+48;
af_ascii[6]=bf_ascii[6]+48;
af_ascii[7]=bf_ascii[7]+48;
af_ascii[8]=bf_ascii[8]+48;
af_ascii[9]=bf_ascii[9]+48;

			chr_size=9;	//soa size is 9 or less.
if (var>=10)		chr_size=8;	//soa size is 19 or less.
if (var>=100)		chr_size=7;	//soa size is 199 or less.
if (var>=1000)		chr_size=6;	//soa size is 1999 or less.
if (var>=10000)		chr_size=5;	//soa size is 19999 or less.
if (var>=100000)	chr_size=4;	//soa size is 199999 or less.
if (var>=1000000)	chr_size=3;	//soa size is 1999999 or less.
if (var>=10000000)	chr_size=2;	//soa size is 19999999 or less.
if (var>=100000000)	chr_size=1;	//soa size is 199999999 or less.
if (var>=1000000000)	chr_size=0;	//soa size is 4294967295 or less.

	while(BUSY == lcdbusycheck());
	if(para>=0x80)lcdwrite1command ( para );	
	textsize=strlen(text);

	for(i=0; i<=(textsize-1); i++){
		while(BUSY == lcdbusycheck());
		lcdwrite1data(text[i]);
	}

	for(i=chr_size; i<=9; i++){
		while(BUSY == lcdbusycheck());
		lcdwrite1data(af_ascii[i]);
	}

return;
}

void lcdvar(char para,unsigned long var)
{
char i;
unsigned long bf_ascii[10];
unsigned long af_ascii[10];
char chr_size;

bf_ascii[0]=var/1000000000;
bf_ascii[1]=(var-(bf_ascii[0]*1000000000))/100000000;
bf_ascii[2]=(var-(bf_ascii[0]*1000000000+bf_ascii[1]*100000000))/10000000;
bf_ascii[3]=(var-(bf_ascii[0]*1000000000+bf_ascii[1]*100000000+bf_ascii[2]*10000000))/1000000;
bf_ascii[4]=(var-(bf_ascii[0]*1000000000+bf_ascii[1]*100000000+bf_ascii[2]*10000000+bf_ascii[3]*1000000))/100000;
bf_ascii[5]=(var-(bf_ascii[0]*1000000000+bf_ascii[1]*100000000+bf_ascii[2]*10000000+bf_ascii[3]*1000000+bf_ascii[4]*100000))/10000;
bf_ascii[6]=(var-(bf_ascii[0]*1000000000+bf_ascii[1]*100000000+bf_ascii[2]*10000000+bf_ascii[3]*1000000+bf_ascii[4]*100000+bf_ascii[5]*10000))/1000;
bf_ascii[7]=(var-(bf_ascii[0]*1000000000+bf_ascii[1]*100000000+bf_ascii[2]*10000000+bf_ascii[3]*1000000+bf_ascii[4]*100000+bf_ascii[5]*10000+bf_ascii[6]*1000))/100;
bf_ascii[8]=(var-(bf_ascii[0]*1000000000+bf_ascii[1]*100000000+bf_ascii[2]*10000000+bf_ascii[3]*1000000+bf_ascii[4]*100000+bf_ascii[5]*10000+bf_ascii[6]*1000+bf_ascii[7]*100))/10;
bf_ascii[9]=(var-(bf_ascii[0]*1000000000+bf_ascii[1]*100000000+bf_ascii[2]*10000000+bf_ascii[3]*1000000+bf_ascii[4]*100000+bf_ascii[5]*10000+bf_ascii[6]*1000+bf_ascii[7]*100+bf_ascii[8]*10));

af_ascii[0]=bf_ascii[0]+48;
af_ascii[1]=bf_ascii[1]+48;
af_ascii[2]=bf_ascii[2]+48;
af_ascii[3]=bf_ascii[3]+48;
af_ascii[4]=bf_ascii[4]+48;
af_ascii[5]=bf_ascii[5]+48;
af_ascii[6]=bf_ascii[6]+48;
af_ascii[7]=bf_ascii[7]+48;
af_ascii[8]=bf_ascii[8]+48;
af_ascii[9]=bf_ascii[9]+48;

			chr_size=9;	//soa size is 9 or less.
if (var>=10)		chr_size=8;	//soa size is 19 or less.
if (var>=100)		chr_size=7;	//soa size is 199 or less.
if (var>=1000)		chr_size=6;	//soa size is 1999 or less.
if (var>=10000)		chr_size=5;	//soa size is 19999 or less.
if (var>=100000)	chr_size=4;	//soa size is 199999 or less.
if (var>=1000000)	chr_size=3;	//soa size is 1999999 or less.
if (var>=10000000)	chr_size=2;	//soa size is 19999999 or less.
if (var>=100000000)	chr_size=1;	//soa size is 199999999 or less.
if (var>=1000000000)	chr_size=0;	//soa size is 4294967295 or less.

	while(BUSY == lcdbusycheck());
	if(para>=0x80)lcdwrite1command ( para );	

	for(i=chr_size; i<=9; i++){
		while(BUSY == lcdbusycheck());
		lcdwrite1data(af_ascii[i]);
	}

return;
}


void lcdtext(unsigned char para,unsigned char text[])
{
char i;
char textsize;

	while(BUSY == lcdbusycheck());
	if(para>=0x80)lcdwrite1command ( para );	
	textsize=strlen(text);

	for(i=0; i<=(textsize-1); i++){
		while(BUSY == lcdbusycheck());
		lcdwrite1data(text[i]);
	}

return;
}

void lcdclear(void)
{
unsigned long i;

	while(BUSY == lcdbusycheck());
	lcdwrite1command ( 0x80 );	
	for(i=0; i<=20; i++){
		while(BUSY == lcdbusycheck());
		lcdwrite1data(0x20);
	}

	while(BUSY == lcdbusycheck());
	lcdwrite1command ( 0xc0 );	
	for(i=0; i<=20; i++){
		while(BUSY == lcdbusycheck());
		lcdwrite1data(0x20);
	}

	while(BUSY == lcdbusycheck());
	lcdwrite1command ( 0x94 );	
	for(i=0; i<=20; i++){
		while(BUSY == lcdbusycheck());
		lcdwrite1data(0x20);
	}

	while(BUSY == lcdbusycheck());
	lcdwrite1command ( 0xd4 );	
	for(i=0; i<=20; i++){
		while(BUSY == lcdbusycheck());
		lcdwrite1data(0x20);
	}

}



#include "sfr62p.h"
#include "aki_lcd.h"
#include "teamtakahama.h"
#include	<string.h>	

#pragma ADDRESS romver1 0b0000H

void main(void){
	unsigned char debug_txt[]="0";
	unsigned char lcd_text[]="0";
	unsigned int deb_mode=0;
	unsigned int debs_mode=0;
	unsigned int romver1;
	unsigned int speed;

	unsigned short mode=0;
	unsigned short px=0;
	unsigned short mag;
	unsigned short irr;
	unsigned short irc;
	unsigned short irl; 
	unsigned short usonicr;
	unsigned short usonicl;
	unsigned short gr;

	unsigned short grr;

	unsigned long timer;
	unsigned long deb_time=0;
	unsigned long i=0;
	unsigned long deb_tim;
	unsigned long holdcou1=0;
	unsigned long holdcou2=0;

	pd0 = 0b00000000;
	pd1 = 0xff;
	pd2 = 0xff;
	pd5 = PORTOUT;
	p5_1=1;
	p5_2=1;
	p5_3=1;
	pd3 = 0x00;
	p4 =  0x00;						/*ポート4(LCD)初期値設定*/
	pd4 = PORTOUT;						/*ポート4方向を出力に設定*/
	pd9 = PORTIN;

	grr=520;
	speed=30;

	pur0=0b11111111;
	pur1=0b00001011;

	init_ir();					//赤外線センサー
	init_moter();
	lcd_init();


//||=||=||=||=||=||=||=||=||=||=||=||=||=||=||=||=||=||=||=||=||=||=||=||=||=||=||=||={メインループ開始}
strcpy(lcd_text,"TeamTakahama ");lcdtext(0x80,lcd_text);
strcpy(lcd_text,"    Debugging System");lcdtext(0xc0,lcd_text);
strcpy(lcd_text,"Copyright ");lcdtext(0x94,lcd_text);
strcpy(lcd_text,"   2008 TeamTakahama");lcdtext(0xd4,lcd_text);


for(timer=0;timer<=1000000;timer++){
if((p0_5==0)||(p0_6==0)||(p0_7==0))break;
}
lcdclear();

for(;;){

	p2=0xff;
	p1=0xff;

	for(;p0_7==1;){
		strcpy(lcd_text,"Speed=");lcddebug1(0x80,lcd_text,speed);
		if((p0_5==0)&&(speed<=255))speed=speed+1;
		if((p0_6==0)&&(speed>0))speed=speed-1;
		for(i=0;i<10000;i++){}
		lcdclear();
	}

	pwm(50,100,30,30,64);
	p2=0x00;
	p1=0x99;
	for(i=0; i<900000; i++){}
	



}
}

/**********************************************************************************************


	TeamTakahama 2008 OAKS16-62P Programs
			For SPIDER & OCTOPUS
	Copyright Hikaru from 2008 TeamTakahama

	Foward & Keeper Hybrid Program  Template Rev1.00
			Written by Hikaru


***********************************************************************************************/


#include "sfr62p.h"
#include "aki_lcd.h"
#include "teamtakahama.h"
#include <string.h>	

#define ball_catch p9_7

void main(void){

	unsigned char debug_txt[]="0";		//LCD用の文字配列
	unsigned char lcd_text[]="0";		//LCD用の文字配列
	unsigned short _far *romwr;		//romwrのポインタ宣言
	unsigned short _far *romwrb;		//romwrbのポインタ宣言
	unsigned short _far *romfk;		//romfkのポインタ宣言

	unsigned int deb_mode=0;		//デバッグモード変数

	unsigned int px=0;			//X座標変数
	unsigned int py=0;			//Y座標変数
	unsigned int ramfk;			//RAM上の仮想romfk

	unsigned short wh_grset;		//黒白反転用のパラメータ

	unsigned short movetime=0;		//デバッグモードの画面切り替えタイマ設定変数

	unsigned short mag=0;			//方向センサー用の変数
	unsigned short mode=0;		//キーパーやフォワードプログラム内のモード変数
	unsigned short fkset=1;		//キーパー・フォワード設定変数(電源投入後)
	unsigned short irr;			//右IR変数
	unsigned short irc;			//中IR変数
	unsigned short irl; 			//左IR変数
	unsigned short usonicr;		//右超音波変数
	unsigned short usonicl;		//左超音波変数
	unsigned short gr;			//グレースケール変数
	unsigned short grset[3]=0;		//デバッグ用のGR設定変数
	unsigned short grset_mode=0;		//デバッグ用のGR設定モード変数

	unsigned short gra=0;			//前GR領域設定変数
	unsigned short grb=0;			//待機位置領域設定変数
	unsigned short grc=0;			//後ろGR領域設定変数

	unsigned short margin_wait=0;		//待機位置用帯域設定
	unsigned short back_spd_bdline=0;	//待機位置に戻る際、スピードを変える線を設定

	unsigned long holdcou;
	unsigned short holdtime;

	unsigned short grwb=0;		//白攻め・黒攻め設定変数

	unsigned long timer[16];		//汎用ソフトタイマー
	unsigned long deb_time=0;		//デバッグモードの画面切り替えタイマ変数
	unsigned int deb_clutch=0;		//デバッグモードの画面切り替え時、クラッチ？

	unsigned long grset_wr_timer=0;		//デバッグモード時にボタン長押し監視
	unsigned long i=0;			//汎用変数(ワンループするものには使わない)

	unsigned int kp_check=0;		//キーパー車検モード
	unsigned short ball_catch_pt=0;		//ボールに接触した時点でのグレースケールの値

	unsigned short margin_bdline;		//(KP)中立点停止エリアの帯域

	unsigned short ball_push_timer_r;	//(KP)右のボールを押し続ける時間
	unsigned short ball_push_timer_l;	//(KP)左のボールを押し続ける時間

	pd0 = 0b00000011;
	pd1 = 0xff;
	pd2 = 0xff;
	pd5 = PORTOUT;
	p0_1=1;
	p5_1=1;
	p5_2=1;
	p5_3=1;
	pd3 = 0x00;
	p4 =  0x00;				//ポート4(LCD)初期値設定
	pd4 = PORTOUT;				//ポート4方向を出力に設定
	pd9 = PORTIN;

	movetime=15000;				//デバッグ画面移り変わりタイマー
//	wh_grset=600;				//白側設定オフセット値
	wh_grset=800;

	mode=0;					//キーパー&フォワード内モード
	


	pur0=0b11111111;				//ソフトフェア　プルアップ設定0
	pur1=0b00001011;				//ソフトウェア　プルアップ設定1

	init_ir();					//赤外線センサー初期設定
	init_moter();				//ポート・PWM初期設定
	lcd_init();				//LCD初期化


//***************************************************************{メインプログラム}***/
if(p0_3==1){					//タイトル表示
	strcpy(lcd_text,"TeamTakahama ");lcdtext(0x80,lcd_text);
	strcpy(lcd_text,"    Debugging System");lcdtext(0xc0,lcd_text);
	strcpy(lcd_text,"Copyright ");lcdtext(0x94,lcd_text);
	strcpy(lcd_text,"   2008 TeamTakahama");lcdtext(0xd4,lcd_text);
	
	for(i=0;i<=1000000;i++){
		if((p0_5==0)||(p0_6==0)||(p0_7==0))break;	//タクトSWが押されたらタイトル中止
		if(p0_3==0)break;			//実行モードがRUNになったら中止
	}
	lcdclear();
}

	romfk= (unsigned short _far *)0x0C0000;		//フォワード・キーパーの切り替えROM領域の設定
	romwr= (unsigned short _far *)0x0D0000;		//黒側グレースケールの格納ROM領域の設定
	romwrb=(unsigned short _far *)0x0E0000;		//白側グレースケールの格納ROM領域の設定		

if(romfk[0]==0xFFFF){		//キーパーかフォワードの設定が無設定(0xFFFF)だったらフォワード(0x0000)にする
	prc0=1;
	p0_0=0;
	cm06=0;
	
	cm16=1;
	cm17=1;
	pm17=1;
	
	fmr01=0;
	fmr01=1;
	fmr11=0;
	fmr11=1;

	romfk[0]=0x40;
	romfk[0]=0x0000;

	for(;fmr00==0;){}

	fmr01=0;
	cm06=0;
	cm16=0;
	cm17=0;
	pm17=0;
}

	if(p0_4==0)grwb=0;	//左側トグルSWが0だったら白攻めに設定する
	if(p0_4==1)grwb=1;	//右側トグルSWが1だったら黒攻めに設定する


/****************************************************************[メインループ開始]***/
for(;;){
	if(romfk[0]==0)fkset=0;
	if(romfk[0]==1)fkset=1;

	

	if(grwb==0){				//白攻め設定ならば、各グレースケール変数を設定する
		gra=romwr[2];
		grb=romwr[1];
		grc=romwr[0];
		
		margin_wait=8;			//白攻め待機位置 帯域設定
		back_spd_bdline=20;
		margin_bdline=10;
		
	}

	if(grwb==1){				//黒攻め設定ならば、各グレースケール変数を設定する
		gra=romwrb[2];
		grb=romwrb[1];
		grc=romwrb[0];

		margin_wait=60;			//黒攻め待機位置　帯域設定
		back_spd_bdline=50;
		margin_bdline=60;

	}


	if(p0_3==0)p0_0=0;				//実行モードがRUNモードなら、LEDを発光
	if(p0_3==1)p0_0=1;				//実行モードがデバッグモードなら、LEDを消灯

if(timer[1]<6000)timer[1]++;

/***************************************************************[A/D値代入]***/

		mag=magget();			//magに現在の方位センサーの値を代入。
		irl		=ad0;		//irlに左ＩＲセンサー(ad0)の値を代入。
		irc		=ad1;		//ircに中ＩＲセンサー(ad1)の値を代入。
		irr		=ad2;		//irrに右ＩＲセンサー(ad2)の値を代入。
		usonicl		=ad3;		//usoniclに左超音波の値を代入。
		usonicr		=ad4;		//usonicrに右超音波の値を代入。
		if(grwb==0)	gr=ad5;		//黒守りの時grにグレースケールの値を代入。
		if(grwb==1)	gr=wh_grset-ad5;	//白守りの時grにグレースケールの値を代入。

/********************************************************[px判定モジュール]***/


		if(fkset==1){
			if(usonicl+35<usonicr)px=1;
			if(usonicr+35<usonicl)px=3;
			if((usonicr+35>usonicl)&&(usonicl+35>usonicr))px=2;
		}

		if(fkset==0){
			if(usonicl+60<usonicr)px=1;
			if(usonicr+60<usonicl)px=3;
			if((usonicr+60>usonicl)&&(usonicl+60>usonicr))px=2;
		}

/********************************************************[py判定モジュール]***/

		if(gr>grc)py=3;
		if(gr>grb)py=2;
		if(gr>gra)py=1;
		if(gr<grc)py=4;


/*************************************************[通常動作モード(Forward)]***/
if((p0_3==0)&&(fkset==0)){	

	timer[2]=0;
	p0_1=1;


	if((mode==0)&&(mag==0)){
		p5_1=1;
		p5_2=1;
		p5_3=1;

		if(py==1){
			if(px==1){
				pwm(255,255,128,128,64);
				p2=0x06;
				p1=0x06;
			}

			if(px==2){
				pwm(255,255,255,255,64);
				p2=0x06;
				p1=0x96;
			}

			if(px==3){
				pwm(255,255,128,128,64);
				p2=0x06;
				p1=0x90;	
			}
		}

		if(py==2){
			if(px==1){
				pwm(255,255,255,255,64);
				p2=0x00;
				p1=0x66;	
			}

			if(px==2){
				pwm(255,255,255,255,64);
				p2=0xff;
				p1=0xff;
			}

			if(px==3){
				pwm(255,255,128,128,64);
				p2=0x00;
				p1=0x99;	
			}
		}

		if(py==3){
			if(px==1){
				pwm(255,255,255,255,64);
				p2=0x00;
				p1=0x66;	
			}

			if(px==2){
				pwm(255,255,255,255,64);
				p2=0xff;
				p1=0xff;
			}

			if(px==3){
				pwm(255,255,128,128,64);
				p2=0x00;
				p1=0x99;	
			}
		}
		if(py==4){
			if(px==1){
				pwm(255,255,128,128,64);
				p2=0x09;
				p1=0x60;
			}

			if(px==2){
				pwm(100,100,100,100,64);
				p2=0x09;
				p1=0x69;
			}

			if(px==3){
				pwm(255,255,128,128,64);
				p2=0x09;
				p1=0x09;	
			}
		}

	}
	if((mode==0)&&(mag!=0))magmatch(mag,64,32);




	if((mode==0)&&(irr+irc+irl>400))mode=20;
	if((mode==20)&&(irr+irc+irl<=400))mode=0;

	if((mode==20)&&(mag==0)){

			if(irr+irc+irl>700){
				hpwm(128,64);
				p5_1=0;
				p5_2=1;
			}else{
				p5_1=1;
				p5_2=1;
			}
				

			if(irl*2 > irc+irr-30){				//左が一番大きい場合は
				pwm(256,256,128,128,64);
				p2=0x09;
				p1=0x09;
			}
	
			if(irc*2-30 > irr+irl){				//中央が一番大きい場合は
				pwm(128,128,128,128,64);
				p2=0x09;
				p1=0x69;
			}
	
			if(irr*2 > irc+irl-30){				//右が一番大きい場合は
				pwm(256,256,128,128,64);
				p2=0x09;
				p1=0x60;
			}	
	}

	if((mode==20)&&(mag!=0))magmatch(mag,60,30);

	if((mode==0)&&(p9!=0xFF))mode=60;
	if((mode==60)&&(p9==0xFF))mode=0;

	if((mode==60)&&(mag==0)){
		
		if(p9_0==0){
			pwm(256,256,128,128,64);
			p2=0xff;
			p1=0x66;
		}

		if(p9_1==0){
			pwm(256,256,128,128,64);
			p2=0x06;
			p1=0x06;
		}

		if(p9_2==0){
			pwm(256,256,256,256,64);
			p2=0x06;
			p1=0x96;
		}

		if(p9_3==0){
			if(px==1){
				pwm(256,256,128,128,64);
				p2=0x06;
				p1=0x06;
			}
	
			if(px==2){
				pwm(256,256,128,128,64);
				p2=0x06;
				p1=0x06;
			}
	
			if(px==3){
				pwm(256,256,128,128,64);
				p2=0x06;
				p1=0x90;
			}
		}

		if(p9_4==0){
			pwm(256,256,256,256,64);
			p2=0x06;
			p1=0x96;
		}

		if(p9_5==0){
			pwm(256,256,128,128,64);
			p2=0x06;
			p1=0x90;
		}

	
		if(p9_6==0){
			pwm(256,256,128,128,64);
			p2=0xff;
			p1=0x99;
		}

	}

	if((mode==60)&&(mag!=0))magmatch(mag,60,30);
	
	if(mode==60){
		p5_1=1;
		p5_2=1;
	}
		


	if((mode!=50)&&(ball_catch==0)){
		mode=50;
		holdcou=0;
		ball_catch_pt=py;

		p5_1=0;
		p5_2=1;
		hpwm(256,64);
	}

	if((mode==50)&&(ball_catch==0)){
	
		if(ball_catch_pt!=1){

			if(irl*2 > irc+irr){		//左が一番大きい場合は
				pwm(16,16,16,16,64);	
				p2=0xf9;
				p1=0x09;
			}
	
			if(irc*2 > irr+irl){		//中央が一番大きい場合は
				pwm(8,8,8,8,64);
				p2=0xf9;	
				p1=0x69;
			}
	
			if(irr*2 > irc+irl){		//右が一番大きい場合は
				pwm(16,16,16,16,64);
				p2=0x09;
				p1=0x60;
			}
		}

		if(ball_catch_pt==1){
			p2=0xff;
			p1=0xff;
		}
	
			holdtime=30;
			holdcou++;
		}

	if((mode==50)&&(ball_catch==1)) holdtime--;
	if((mode==50)&&(holdtime==0))   mode=0;


	if((mode==50)&&(holdcou>700)&&(mag==0)){

	pwm(30,30,25,25,64);
	p2=0x06;
	p1=0x96;
	for(i=0; i<90000; i++){}
	p2=0xff;
	p1=0xff;
	for(i=0; i<10000; i++){}
	
		if(gr>grc)py=3;
		if(gr>grb)py=2;
		if(gr>gra)py=1;
		if(gr<grc)py=4;

	ball_catch_pt=py;


	if(px==1){
		if(ball_catch_pt==1){
			for(i=0; i<200000; i++){
				mag=magget();
				if(mag==0){
					pwm(30,15,0,0,64);
					p2=0x06;
					p1=0x96;
				}
					i++;

				if(ball_catch==0)holdtime=0;
				if(ball_catch==1)holdtime++;
				if(holdtime>300)break;

				if(mag!=0)magmatch(mag,30,30);
			}

			pwm(64,64,64,64,64);
			p2=0x00;
			p1=0b10111011;
			for(i=0; i<80000; i++){}
		}

		if(ball_catch_pt==2){
			pwm(64,64,64,64,64);
			p2=0x00;
			p1=0b10111011;
			for(i=0; i<200000; i++){}
		}

		if(ball_catch_pt==3){
			pwm(64,64,64,64,64);
			p2=0x00;
			p1=0b10111011;
			for(i=0; i<150000; i++){}
		}

		if(ball_catch_pt==4){
			pwm(64,64,64,64,64);
			p2=0x00;
			p1=0b10111011;
			for(i=0; i<120000; i++){}
		}

	}

	if(px==3){
		if(ball_catch_pt==1){

			for(i=0; i<200000; i++){
				mag=magget();
				if(mag==0){
					pwm(30,15,0,0,64);
					p2=0x06;
					p1=0x96;
				}
					i++;

				if(ball_catch==0)holdtime=0;
				if(ball_catch==1)holdtime++;
				if(holdtime>300)break;

				if(mag!=0)magmatch(mag,30,30);
			}

			pwm(64,64,64,64,64);
			p2=0x00;
			p1=0b01110111;
			for(i=0; i<80000; i++){
				mag=magget();
				//if(mag==0)break;
			}
		}

		if(ball_catch_pt==2){
			pwm(64,64,64,64,64);
			p2=0x00;
			p1=0b01110111;
			for(i=0; i<200000; i++){}
		}

		if(ball_catch_pt==3){
			pwm(64,64,64,64,64);
			p2=0x00;
			p1=0b01110111;
			for(i=0; i<150000; i++){}
		}

		if(ball_catch_pt==4){
			pwm(64,64,64,64,64);
			p2=0x00;
			p1=0b01110111;
			for(i=0; i<120000; i++){}
		}
	}


	p1=0x00;
	p2=0x00;
	for(i=0; i<200000; i++){}
		
		pwm(128,128,128,128,64);
		p2=0x09;
		p1=0x69;

		p5_1=0;
		p5_2=0;
		for(i=0; i<100000; i++){}
		p5_3=0;
		for(i=0; i<300000; i++){}
		p5_3=1;
		holdcou=0;
		mode=0;
	}
		
	if((mode==50)&&(mag!=0)) magmatch(mag,32,32);
			
		

	







/*--------------------------------------------------------------情報---*/


	strcpy(debug_txt,"mode=");lcddebug1(0x98,debug_txt,mode);
	strcpy(debug_txt,"px=");lcddebug1(0xc0,debug_txt,px);
	strcpy(debug_txt,"py=");lcddebug1(0xc5,debug_txt,py);
 




}



/*****************************************************************************[通常動作モード(Keeper)]***/
/***********  *******************************************************************************************/
/***********  *********       ***************************************************************************/
/***********  ***  *** ******** *************************************************************************/
/***********  *  *****          *************************************************************************/
/***********  **  ****  *********************************************************************************/
/***********  ***  *****        *************************************************************************/





if((p0_3==0)&&(fkset==1)){
	timer[2]=0;
	p0_1=1;

	//0は待機モード

	if(mode==0){
		p5_1=1;
		p5_2=1;
		p5_3=1;
	
	}


	if((mode==0)&&(mag==0)){						//待機モードで方向センサーが0ならば

		if((gr > grb + margin_wait)&&(gr < grb + margin_wait + back_spd_bdline)){	//待機位置よりも前に居る場合(近い)
			
			if(px==1){				//左側に居る場合
				pwm(128,128,64,64,64);
				p2=0x06;
				p1=0x06;
			}

			if(px==2){				//中央に居る場合
				pwm(100,100,100,100,64);
				p2=0x06;
				p1=0x96;
			}

			if(px==3){				//右側に居る場合
				pwm(128,128,64,64,64);
				p2=0x06;
				p1=0x90;
			}
		}

		if((gr > grb + margin_wait)&&(gr >= grb + margin_wait + back_spd_bdline)){	//待機位置よりも前に居る場合(遠い)
			
			if(px==1){				//左側に居る場合
				pwm(255,255,128,128,64);
				p2=0x06;
				p1=0x06;
			}

			if(px==2){				//中央に居る場合
				pwm(255,255,255,255,64);
				p2=0x06;
				p1=0x96;
			}

			if(px==3){				//右側に居る場合
				pwm(255,255,128,128,64);
				p2=0x06;
				p1=0x90;
			}
		}

		if((gr < grb + margin_wait) && (gr > grb)){	//待機位置内に居る場合

			if(px==1){				//左側に居る場合
				pwm(200,200,200,200,64);
				p2=0xff;
				p1=0x66;
			}

			if(px==2){				//中央に居る場合
				p2=0xff;
				p1=0xff;
			}

			if(px==3){
				pwm(200,200,200,200,64);	//右側に居る場合
				p2=0xff;
				p1=0x99;
			}
				

		}

		if(gr < grb){				//待機位置よりも後ろに居る場合

			if(px==1){				//左に居る場合
				pwm(128,128,64,64,64);
				p2=0x09;
				p1=0x69;
			}

			if(px==2){				//中央に居る場合
				pwm(30,30,30,30,64);
				p2=0x09;
				p1=0x69;
			}

			if(px==3){				//右に居る場合
				pwm(128,128,64,64,64);
				p2=0x09;
				p1=0x09;
			}
		}

	}

	if((mode==0)&&(mag!=0))magmatch(mag,32,32);		//待機モードで方向センサーが0では無い場合	

/********************************************************************************************************************************/

	if((mode!=25)&&(mode!=29)&&(mode!=50)&&(mode!=51)&&(irr+irc+irl>=200)) mode=20;	//中立点モード時以外にボールが発見→通常追いかけるモード
	if((mode==20)&&(irr+irc+irl<300)) mode=0;			//通常追いかけるモードでボールが無かったら


	if(mode==20){
		hpwm(128,64);
		p5_1=0;
		p5_2=1;
	}


	if((mode==20)&&(mag==0)){				//通常追いかけるモードで方向センサーが0ならば

		if(irl*2 > irc+irr-30){				//左が一番大きい場合は
			pwm(256,256,128,128,64);
			p2=0x09;
			p1=0x09;
		}

		if(irc*2-30 > irr+irl){				//中央が一番大きい場合は
			pwm(128,128,128,128,64);
			p2=0x09;
			p1=0x69;
		}

		if(irr*2 > irc+irl-30){				//右が一番大きい場合は
			pwm(256,256,128,128,64);
			p2=0x09;
			p1=0x60;
		}		

	}

	if((mode==20)&&(mag!=0))magmatch(mag,32,32);		//通常追いかけるモードで方向センサーが0でなかったら


/********************************************************************************************************************************/
	if((mode==25)&&(irr+irc+irl<400)) mode=0;			//振り返って追いかけるモード


	if(mode==25){
		hpwm(128,64);
		p5_1=0;
		p5_2=1;
	}

	
	if(mode==25){					//振り返って追いかけるモード
		if(irl*2 > irc+irr){				//左が一番大きい場合は
			pwm(20,20,10,10,64);
			p2=0x09;
			p1=0x09;
		}

		if(irc*2 > irr+irl){				//中央が一番大きい場合は
			pwm(20,20,20,20,64);
			p2=0x09;
			p1=0x69;
		}

		if(irr*2 > irc+irl){				//右が一番大きい場合は
			pwm(20,20,10,10,64);
			p2=0x09;
			p1=0x60;
		}		

	}

/************************************************************ 中立点***/ 

	if((mode==20)&&(gr > gra)) mode=29;
	if((mode==29)&&(irr+irc+irl < 500)) mode=0;

	if((mode==29)&&(mag==0)){

		hpwm(128,64);
		p5_1=0;
		p5_2=1;
		p5_3=1;

		if(gr < gra-margin_bdline)mode=0;

		if(gr > gra){
			pwm(64,64,64,64,64);
			p2=0x06;
			p1=0x96;
		}

		if((gr <= gra)&&(irl*2 > irc + irr)){
			pwm(128,128,128,128,64);
			p2=0xff;
			p1=0x99;
		}

		if((gr <= gra)&&(irc*2 > irr + irl)){
			p2=0x00;
			p1=0x00;
		}

		if((gr <= gra)&&(irr*2 > irc + irl)){
			pwm(128,128,128,128,64);
			p2=0xff;
			p1=0x66;
		}

	}

	if((mode==29)&&(mag!=0))magmatch(mag,32,32);



/*************************************************************デジタル***/


	if((mode!=25)&&(mode!=50)&&(gr > gra )&&(p9!=0xff)){			//立ち入り禁止エリアでデジタル赤外線センサーが反応していたら
		if(p9_0==0)mode=60;
		if(p9_1==0)mode=61;
		if(p9_2==0)mode=62;
		if(p9_3==0)mode=63;
		if(p9_4==0)mode=64;
		if(p9_5==0)mode=65;
		if(p9_6==0)mode=66;
	}


	if((mode!=25)&&(mode!=50)&&(gr < gra )&&(gr > gra - margin_bdline)&&(p9!=0xff)){	//中立点エリアでデジタル赤外線センサーが本能していたら
		if(p9_0==0)mode=70;
		if(p9_1==0)mode=71;
		if(p9_2==0)mode=72;
		if(p9_3==0)mode=73;
		if(p9_4==0)mode=74;
		if(p9_5==0)mode=75;
		if(p9_6==0)mode=76;
	}

	if((mode!=25)&&(mode!=50)&&(gr < gra - margin_bdline)&&(gr >= grc )&&(p9!=0xff)){	//通常動作エリアでデジタル赤外線センサーが反応していたら
		if(p9_0==0)mode=80;
		if(p9_1==0)mode=81;
		if(p9_2==0)mode=82; 
		if(p9_3==0)mode=83;
		if(p9_4==0)mode=84;
		if(p9_5==0)mode=85;
		if(p9_6==0)mode=86;
	}

	if((mode!=25)&&(mode!=50)&&(gr < grc )&&(p9!=0xff)){			//待機位置以下でデジタル赤外線センサーが反応していたら
		if(p9_0==0)mode=90;
		if(p9_1==0)mode=91;
		if(p9_2==0)mode=92;
		if(p9_3==0)mode=93;
		if(p9_4==0)mode=94;
		if(p9_5==0)mode=95;
		if(p9_6==0)mode=96;
	}


	if((mode>=60)&&(p9==0xff))mode=0;		//デジタル赤外線が反応しているモードで赤外線センサーが反応していなかったら




	if((mode==60)&&(mag==0)){
			pwm(256,256,256,256,64);
			p2=0x06;
			p1=0x96;
	}

	if((mode==61)&&(mag==0)){
			pwm(256,256,128,128,64);
			p2=0x06;
			p1=0x96;

	}

	if((mode==62)&&(mag==0)){
			pwm(256,256,128,128,64);
			p2=0x06;
			p1=0x96;
	}

	if((mode==63)&&(mag==0)){
		if(px==1){
			pwm(256,256,128,128,64);
			p2=0x06;
			p1=0x06;
		}

		if(px==2){
			pwm(256,256,128,128,64);
			p2=0x06;
			p1=0x06;
		}

		if(px==3){
			pwm(256,256,128,128,64);
			p2=0x06;
			p1=0x90;
		}
	}

	if((mode==64)&&(mag==0)){
			pwm(256,256,128,128,64);
			p2=0x06;
			p1=0x96;
	}

	if((mode==65)&&(mag==0)){
			pwm(256,256,128,128,64);
			p2=0x06;
			p1=0x96;
	}

	
	if((mode==66)&&(mag==0)){
			pwm(256,256,128,128,64);
			p2=0x06;
			p1=0x96;
	}









	if((mode==63)&&(mag==0)){			//立ち入り禁止エリアで後ろIRが反応していたら
		if(px==1){
			pwm(256,256,128,128,64);
			p2=0x06;
			p1=0x06;
		}

		if(px==2){
			pwm(256,256,128,128,64);
			p2=0x06;
			p1=0x06;
		}

		if(px==3){
			pwm(256,256,128,128,64);
			p2=0x06;
			p1=0x90;
		}
	}
	


	if((mode==70)&&(mag==0)){
		if(kp_check==1){
			pwm(255,255,128,128,64);
			p2=0x09;
			p1=0x60;
		}
		if(kp_check==0){
			pwm(128,128,128,128,64);
			p2=0x00;
			p1=0x66;
		}
	}

	if((mode==71)&&(mag==0)){
		pwm(256,256,128,128,64);
		p2=0x06;
		p1=0x06;
	}

	if((mode==72)&&(mag==0)){
		pwm(256,256,256,256,64);
		p2=0x06;
		p1=0x96;
	}

	if((mode==73)&&(mag==0)){
		if(px==1){
			pwm(256,256,128,128,64);
			p2=0x06;
			p1=0x06;
		}

		if(px==2){
			pwm(256,256,128,128,64);
			p2=0x06;
			p1=0x06;
		}

		if(px==3){
			pwm(256,256,128,128,64);
			p2=0x06;
			p1=0x90;
		}
	}

	if((mode==74)&&(mag==0)){
		pwm(256,256,256,256,64);
		p2=0x06;
		p1=0x96;
	}

	if((mode==75)&&(mag==0)){
		pwm(256,256,128,128,64);
		p2=0x06;
		p1=0x90;
	}

	
	if((mode==76)&&(mag==0)){
		if(kp_check==1){
			pwm(255,30,128,128,64);
			p2=0x09;
			p1=0x99;
		}
		if(kp_check==0){
			pwm(128,128,128,128,64);
			p2=0x00;
			p1=0x99;
		}
	}



	if((mode==80)&&(mag==0)){
		if(kp_check==1){
			pwm(255,255,128,128,64);
			p2=0x09;
			p1=0x60;
		}

		if(kp_check==0){
			pwm(256,256,128,128,64);
			p2=0xff;
			p1=0x66;
		}
	}

	if((mode==81)&&(mag==0)){

		if(kp_check==1){
			pwm(255,255,128,128,64);
			p2=0x09;
			p1=0x60;
		}


		if(kp_check==0){
			pwm(256,256,128,128,64);
			p2=0x06;
			p1=0x06;
		}

	}

	if((mode==82)&&(mag==0)){
		pwm(256,256,256,256,64);
		p2=0x06;
		p1=0x96;
	}

	if((mode==83)&&(mag==0)){
		if(px==1){
			pwm(256,256,128,128,64);
			p2=0x06;
			p1=0x06;
		}

		if(px==2){
			pwm(256,256,128,128,64);
			p2=0x06;
			p1=0x06;
		}

		if(px==3){
			pwm(256,256,128,128,64);
			p2=0x06;
			p1=0x90;
		}
	}

	if((mode==84)&&(mag==0)){
		pwm(256,256,256,256,64);
		p2=0x06;
		p1=0x96;
	}

	if((mode==85)&&(mag==0)){
		if(kp_check==1){
			pwm(255,30,128,128,64);
			p2=0x09;
			p1=0x99;
		}

		if(kp_check==0){
			pwm(256,256,128,128,64);
			p2=0x06;
			p1=0x90;
		}
	}

	
	if((mode==86)&&(mag==0)){
		if(kp_check==1){
			pwm(255,30,128,128,64);
			p2=0x09;
			p1=0x99;
		}

		if(kp_check==0){
			pwm(256,256,128,128,64);
			p2=0xff;
			p1=0x99;
		}
	}









	if((mode==90)&&(mag==0)){
		if(kp_check==1){
			pwm(255,255,128,128,64);
			p2=0x09;
			p1=0x60;
		}

		if(kp_check==0){
			pwm(256,256,128,128,64);
			p2=0xff;
			p1=0x66;
		}
	}

	if((mode==91)&&(mag==0)){
		pwm(256,256,128,128,64);
		p2=0xff;
		p1=0x66;
	}

	if((mode==92)&&(mag==0)){

		if(ball_push_timer_r <= 1000){
			pwm(256,256,256,256,64);
			p2=0x06;
			p1=0x96;
			ball_push_timer_r++;
		}		

		if(ball_push_timer_r >  1000){
			pwm(32,32,32,32,64);
			p2=0x0A;
			p1=0xAA;
			for(i=0; i<500000; i++){
				if(ad0+ad1+ad2>400)break;
			}
			mode=25;
	
			p2=0xff;
			p1=0xff;
			for(i=0; i<50000; i++){}
			ball_push_timer_r=0;
		}
		
	}else{
		ball_push_timer_r=0;
	}

	if((mode==93)&&(mag==0)){
		pwm(64,64,64,64,64);
		p2=0x09;
		p1=0x69;
		for(i=0; i<200000; i++){}

		pwm(32,32,32,32,64);
		p2=0x0A;
		p1=0xAA;
		for(i=0; i<700000; i++){}

		mode=25;

		p2=0xff;
		p1=0xff;
		for(i=0; i<50000; i++){}	
	}

	if((mode==94)&&(mag==0)){
		if(ball_push_timer_l <= 1000){
			pwm(256,256,256,256,64);
			p2=0x06;
			p1=0x96;
			ball_push_timer_l++;
		}	

		if(ball_push_timer_l >  1000){
			pwm(32,32,32,32,64);
			p2=0x05;
			p1=0x55;
			for(i=0; i<500000; i++){
				if(ad0+ad1+ad2>400)break;
			}
			mode=25;
	
			p2=0xff;
			p1=0xff;
			for(i=0; i<50000; i++){}
		}
	}else{
		ball_push_timer_l=0;
	}

	if((mode==95)&&(mag==0)){
		pwm(256,256,128,128,64);
		p2=0xff;
		p1=0x99;
	}

	
	if((mode==96)&&(mag==0)){
		if(kp_check==1){
			pwm(255,255,128,128,64);
			p2=0x09;
			p1=0x09;
		}

		if(kp_check==0){
			pwm(256,256,128,128,64);
			p2=0xff;
			p1=0x99;
		}
	}

	

	if((mode>=60)&&(mode<=96)&&(mag!=0))magmatch(mag,32,32);

	


//蹴る

	if(mode!=25){
		if((mode!=50)&&(mode!=51)&&(ball_catch==0)){
			mode=50;
			holdcou=0;
			ball_catch_pt=gr;
			p2=0x00;
			p1=0x00;
			p5_1=0;
			p5_2=1;
			hpwm(256,64);
		}
	}

	if((mode==50)&&(ball_catch==0)){
		if((ball_catch_pt<gra+5)&&(irl*2 > irc+irr)){		//左が一番大きい場合は
			pwm(64,64,32,32,64);
			p2=0x09;
			p1=0x09;
		}

		if((ball_catch_pt<gra+5)&&(irc*2 > irr+irl)){		//中央が一番大きい場合は
			pwm(16,16,16,16,64);
			p2=0x09;
			p1=0x69;
		}

		if((ball_catch_pt<gra+5)&&(irr*2 > irc+irl)){		//右が一番大きい場合は
			pwm(64,64,32,32,64);
			p2=0x09;
			p1=0x60;
		}

		if(ball_catch_pt>=gra+5){
			p2=0xff;
			p1=0xff;
		}

		holdtime=30;
		holdcou++;
	}

	if((mode==50)&&(ball_catch==1)) holdtime--;
	if((mode==50)&&(holdtime==0))   mode=0;


	if((mode==50)&&(holdcou>300)&&(mag==0)){
		if(ball_catch_pt<gra+5){
			pwm(128,128,128,128,64);
			p2=0x09;
			p1=0x69;
		}

		if(ball_catch_pt>=gra+5){
			p2=0xff;
			p1=0xff;
		}

		p5_1=0;
		p5_2=0;
		for(i=0; i<100000; i++){}
		p5_3=0;
		for(i=0; i<300000; i++){}
		p5_3=1;
		holdcou=0;
		mode=0;
	}
		
	if((mode==50)&&(mag!=0)) magmatch(mag,32,32);



/*--------------------------蹴る(振り返り後)---*/
	if(mode==25){
		if((mode!=50)&&(mode!=51)&&(ball_catch==0)){
			mode=51;
			holdcou=0;
			p2=0x00;
			p1=0x00;
			p5_1=0;
			p5_2=1;
			hpwm(256,64);
		}
	}

	if((mode==51)&&(ball_catch==0)){
			p2=0xff;
			p1=0xff;

		holdtime=30;
		holdcou++;
	}

	if((mode==51)&&(ball_catch==1)) holdtime--;
	if((mode==51)&&(holdtime==0))   mode=25;


	if((mode==51)&&(holdcou>300)&&(mag==0)){		//

		p2=0xff;
		p1=0x00;

		for(i=0; i<30000; i++){}

		pwm(128,128,128,128,64);
		p2=0x09;
		p1=0x69;
		p5_1=0;
		p5_2=0;
		for(i=0; i<100000; i++){}
		p5_3=0;
		for(i=0; i<300000; i++){}
		p5_3=1;
		holdcou=0;
		mode=0;
	}
		
	if((mode==51)&&(holdcou>300)&&(mag!=0)) magmatch(mag,9,9);




/*************************************************モード情報表示***/

	strcpy(debug_txt,"mode");lcddebug1(0x98,debug_txt,mode);
	strcpy(debug_txt,"gra");lcddebug1(0xc0,debug_txt,gra);
	strcpy(debug_txt,"grb");lcddebug1(0x00,debug_txt,grb);
	strcpy(debug_txt,"grc");lcddebug1(0x00,debug_txt,grc); 
	if(kp_check==1){
		strcpy(debug_txt,"*KP CHK MODE");lcdtext(0x80,debug_txt);
	}
	}


/**********************************************************[デバッグモード]***/
if(p0_3==1){

		if(timer[2]==0){		//デバッグモード1ループ目の場合
	
			if((p0_5==0)&&(p0_6==0)&&(p0_7==0)){
				kp_check=1;
			}else{
				kp_check=0;
			}

			lcdclear();
			p5_1=1;
			p5_2=1;
			p5_3=1;
			p2=0x00;
			p1=0x00;
			timer[2]=1;
			mode=0;
		}
			

	/******************************************** デバッグモード切替 ***/

	if ((p0_5==0)&&(deb_mode<5)&&(deb_clutch==0)){
		deb_clutch=1;
		deb_mode++;
		deb_time=0;
		lcdclear();
	}
	
	if ((p0_5==0)&&(deb_mode==5)&&(deb_clutch==0)){
		deb_clutch=1;
		deb_mode=0;
		deb_time=0;
		lcdclear();
	}

	if((deb_clutch==1)&&(p0_5==1)&&(deb_time>3000))deb_clutch=0;
	if(deb_time<1500)p0_1=0;
	if(deb_time>=1500)p0_1=1;	

	
	if ((deb_mode==0)&&(deb_time==1)){
			strcpy(debug_txt,"IR data mode");lcdtext(0x80,debug_txt);
			deb_time++;
	}
	
	if ((deb_mode==1)&&(deb_time==1)){
			strcpy(debug_txt,"Field data mode");lcdtext(0x80,debug_txt);
			deb_time++;
	}
	
	if ((deb_mode==2)&&(deb_time==1)){
			strcpy(debug_txt,"Black GR Set");lcdtext(0x80,debug_txt);
			deb_time++;
			grset_mode=0;
			grset_wr_timer=0;
			grset[0]=0;
			grset[1]=0;
			grset[2]=0;
		}
	
	if ((deb_mode==3)&&(deb_time==1)){
			strcpy(debug_txt,"White GR Set");lcdtext(0x80,debug_txt);
			deb_time++;
			grset_mode=0;
			grset_wr_timer=0;
			grset[0]=0;
			grset[1]=0;
			grset[2]=0;
	}	


	if ((deb_mode==4)&&(deb_time==1)){
			strcpy(debug_txt,"Holder & Kicker Test");lcdtext(0x80,debug_txt);
			deb_time++;
	}

	if ((deb_mode==5)&&(deb_time==1)){
			strcpy(debug_txt,"FW/KP Set");lcdtext(0x80,debug_txt);
			ramfk=romfk[0];
			timer[3]=0;
			deb_time++;
	}

	if((deb_time<=movetime))deb_time++;
	
	/*-------------------------------------------------------------------------赤外線モード*/
	
	
	if ((deb_mode==0)&&(deb_time>movetime)){
	
			strcpy(debug_txt,"irl=");lcddebug1(0x80,debug_txt,irl);
			strcpy(debug_txt,"irr=");lcddebug1(0x8d,debug_txt,irr);
			strcpy(debug_txt,"irc=");lcddebug1(0xc6,debug_txt,irc);
	
			strcpy(debug_txt,"ﾃﾞｼﾞﾀﾙIR=0b");lcddebug1(0xd4,debug_txt,p9_7);
			lcdvar(0x00,p9_6);lcdvar(0x00,p9_5);lcdvar(0x00,p9_4);lcdvar(0x00,p9_3);
			lcdvar(0x00,p9_2);lcdvar(0x00,p9_1);lcdvar(0x00,p9_0);
	
			for(i=0;i<10000;i++){}
			lcdclear();
	}
	
	/*-----------------------------------------------------------------------各種コート情報*/
	
	if ((deb_mode==1)&&(deb_time>movetime)){
			strcpy(debug_txt,"gr=");lcddebug1(0x80,debug_txt,gr);
			strcpy(debug_txt,"mag=");lcddebug1(0x88,debug_txt,mag);
	
			strcpy(debug_txt,"px=");lcddebug1(0xc0,debug_txt,px);
			strcpy(debug_txt,"py=");lcddebug1(0xc9,debug_txt,px);
	
			strcpy(debug_txt,"usr=");lcddebug1(0xe1,debug_txt,usonicr);
			strcpy(debug_txt,"usl=");lcddebug1(0xd4,debug_txt,usonicl);
	
			strcpy(debug_txt,"ﾃﾞｼﾞﾀﾙMAG=0b");lcddebug1(0x94,debug_txt,p3_7);
			lcdvar(0x00,p3_6);lcdvar(0x00,p3_5);lcdvar(0x00,p3_4);lcdvar(0x00,p3_3);
			lcdvar(0x00,p3_2);lcdvar(0x00,p3_1);lcdvar(0x00,p3_0);
	
			
		
			for(i=0;i<10000;i++){}
			lcdclear();
	}
	
	/*----------------------------------------------------------黒側グレースケールセット*/
	
	if ((deb_mode==2)&&(deb_time>movetime)){
			
			if(grset_mode<=3){
				strcpy(debug_txt,"Black GR Set");lcdtext(0x80,debug_txt);
				strcpy(debug_txt,"ROM=");lcdtext(0xc0,debug_txt);
				lcdvar(0xc5,romwr[0]);
				lcdvar(0xc9,romwr[1]);
				lcdvar(0xcd,romwr[2]);
			
			}	
	
			if(grset_mode<=3){
				strcpy(debug_txt,"RAM=");lcdtext(0x94,debug_txt);
			}
			if(grset_mode==0){
			lcdvar(0x99,ad5);
			lcdvar(0x9d,0);
			lcdvar(0xa1,0);
	
			}
	
			if(grset_mode==1){
				lcdvar(0x99,grset[0]);
				lcdvar(0x9d,ad5);
				lcdvar(0xa1,0);
			}
	
			if(grset_mode==2){
				lcdvar(0x99,grset[0]);
				lcdvar(0x9d,grset[1]);
				lcdvar(0xa1,ad5);
			}
	
			if(grset_mode==3){
				lcdvar(0x99,grset[0]);
				lcdvar(0x9d,grset[1]);
				lcdvar(0xa1,grset[2]);
				strcpy(debug_txt,"Write? - PUSH P07");lcdtext(0xd4,debug_txt);
			}

			
			if((grset_mode==0)&&(p0_6==0)){
				grset[0]=ad5;
				grset_mode=1;
				for(;p0_6==0;){}
				for(i=0; i<100000; i++){}
			}

			if((grset_mode==1)&&(p0_6==0)){
				grset[1]=ad5;
				grset_mode=2;
				for(;p0_6==0;){}
				for(i=0; i<3000; i++){}
				for(i=0; i<100000; i++){}
			}

			if((grset_mode==2)&&(p0_6==0)){
				grset[2]=ad5;
				grset_mode=3;
				for(;p0_6==0;){}
				for(i=0; i<300000; i++){}
			}

			if((grset_mode==3)&&(p0_6==0)){
				grset_mode=0;
				for(;p0_6==0;){}
				for(i=0; i<100000; i++){}
			}
	
			if((grset_mode==3)&&(p0_7==0)&&(grset_wr_timer<70))grset_wr_timer++;
			if((grset_mode==3)&&(p0_7==1)&&(grset_wr_timer<70))grset_wr_timer=0;
	
			if((grset_mode==3)&&(grset_wr_timer>=70)){
				grset_wr_timer=0;
				grset_mode=4;
				lcdclear();
				strcpy(lcd_text,"Black GR Set");lcdtext(0x80,lcd_text);
				strcpy(lcd_text,"ｶｷｺﾐ?");lcdtext(0xc6,lcd_text);
				strcpy(debug_txt,"No    Yes   Cancel");lcdtext(0x95,debug_txt);
				for(;p0_7==0;){}
			}

			if(grset_mode==4){
				strcpy(lcd_text,"Black GR Set");lcdtext(0x80,lcd_text);
				strcpy(lcd_text,"ｶｷｺﾐ?");lcdtext(0xc6,lcd_text);
				strcpy(debug_txt,"No    Yes   Cancel");lcdtext(0x95,debug_txt);
			}
	
			if((grset_mode==4)&&(p0_7==0)){
				grset_mode=3;
				for(;p0_7==0;){}
			}
			if((grset_mode==4)&&(p0_6==0)){
				grset_mode=5;
				for(;p0_6==0;){}
			}
	
			if(grset_mode==5){
				lcdclear();			
				strcpy(lcd_text,"ｶｷｺﾐﾁｭｳ.....");lcdtext(0xc3,lcd_text);
			
				//------------CPU書き換えモード	EW1でフラッシュROMへ書き込みする。
	
				prc0=1;
				p0_0=0;
				cm06=0;
	
				cm16=1;
				cm17=1;
				pm17=1;
	
				fmr01=0;
				fmr01=1;
				fmr11=0;
				fmr11=1;
				
				romwr[0]=0x20;
				romwr[0]=0xd0;
	
				for(;fmr00=0;){}

				romwr[0]=0x40;
				romwr[0]=grset[0];
	
				for(;fmr00=0;){}

				romwr[1]=0x40;
				romwr[1]=grset[1];

				for(;fmr00=0;){}
	
				romwr[2]=0x40;
				romwr[2]=grset[2];

				for(;fmr00=0;){}

				fmr01=0;
				cm06=0;
				cm16=0;
				cm17=0;
				pm17=0;
			
				grset_mode=3;
			}


			for(i=0;i<10000;i++){}
			lcdclear();
}


/*--------------------------------------------------------------白側グレースケールセット*/

	if ((deb_mode==3)&&(deb_time>movetime)){
	
			if(grset_mode<=3){
				strcpy(debug_txt,"White GR Set");lcdtext(0x80,debug_txt);
				strcpy(debug_txt,"ROM=");lcdtext(0xc0,debug_txt);
				lcdvar(0xc5,romwrb[0]);
				lcdvar(0xc9,romwrb[1]);
				lcdvar(0xcd,romwrb[2]);
			
			}

			if(grset_mode<=3){
				strcpy(debug_txt,"RAM=");lcdtext(0x94,debug_txt);
			}
			if(grset_mode==0){
				lcdvar(0x99,wh_grset-ad5);
				lcdvar(0x9d,0);
				lcdvar(0xa1,0);

			}

			if(grset_mode==1){
				lcdvar(0x99,grset[0]);
				lcdvar(0x9d,wh_grset-ad5);
				lcdvar(0xa1,0);
			}
		
			if(grset_mode==2){
				lcdvar(0x99,grset[0]);
				lcdvar(0x9d,grset[1]);
				lcdvar(0xa1,wh_grset-ad5);
			}

			if(grset_mode==3){
				lcdvar(0x99,grset[0]);
				lcdvar(0x9d,grset[1]);
				lcdvar(0xa1,grset[2]);
				strcpy(debug_txt,"Write? - PUSH P07");lcdtext(0xd4,debug_txt);
			}

			
			if((grset_mode==0)&&(p0_6==0)){
				grset[0]=wh_grset-ad5;
				grset_mode=1;
				for(;p0_6==0;){}
				for(i=0; i<100000; i++){}
			}

			if((grset_mode==1)&&(p0_6==0)){
				grset[1]=wh_grset-ad5;
				grset_mode=2;
				for(;p0_6==0;){}
				for(i=0; i<100000; i++){}
			}

			if((grset_mode==2)&&(p0_6==0)){
				grset[2]=wh_grset-ad5;
				grset_mode=3;
				for(;p0_6==0;){}
				for(i=0; i<300000; i++){}
			}
	
			if((grset_mode==3)&&(p0_6==0)){
				grset_mode=0;
				for(;p0_6==0;){}
				for(i=0; i<100000; i++){}
			}
	
			if((grset_mode==3)&&(p0_7==0)&&(grset_wr_timer<70))grset_wr_timer++;
			if((grset_mode==3)&&(p0_7==1)&&(grset_wr_timer<70))grset_wr_timer=0;
	
			if((grset_mode==3)&&(grset_wr_timer>=70)){
				grset_wr_timer=0;
				grset_mode=4;
				lcdclear();
				strcpy(lcd_text,"White GR Set");lcdtext(0x80,lcd_text);
				strcpy(lcd_text,"ｶｷｺﾐ?");lcdtext(0xc6,lcd_text);
				strcpy(debug_txt,"No    Yes   Cancel");lcdtext(0x95,debug_txt);
				for(;p0_7==0;){}
			}
	
			if(grset_mode==4){
				strcpy(lcd_text,"White GR Set");lcdtext(0x80,lcd_text);
				strcpy(lcd_text,"ｶｷｺﾐ?");lcdtext(0xc6,lcd_text);
				strcpy(debug_txt,"No    Yes   Cancel");lcdtext(0x95,debug_txt);
			}
	
			if((grset_mode==4)&&(p0_7==0)){
				grset_mode=3;
				for(;p0_7==0;){}
			}
			if((grset_mode==4)&&(p0_6==0)){
				grset_mode=5;
				for(;p0_6==0;){}
			}
	
			if(grset_mode==5){
				lcdclear();			
				strcpy(lcd_text,"ｶｷｺﾐﾁｭｳ.....");lcdtext(0xc3,lcd_text);
			
				//------------CPU書き換えモード	EW1でフラッシュROMへ書き込みする。
	
				prc0=1;
				p0_0=0;
				cm06=0;
	
				cm16=1;
				cm17=1;
				pm17=1;
	
				fmr01=0;
				fmr01=1;
				fmr11=0;
				fmr11=1;
				
				romwrb[0]=0x20;
				romwrb[0]=0xd0;
	
				for(;fmr00=0;){}
	
				romwrb[0]=0x40;
				romwrb[0]=grset[0];
	
				for(;fmr00=0;){}
	
				romwrb[1]=0x40;
				romwrb[1]=grset[1];
	
				for(;fmr00=0;){}
	
				romwrb[2]=0x40;
				romwrb[2]=grset[2];
	
				for(;fmr00=0;){}
	
				fmr01=0;
				cm06=0;
				cm16=0;
				cm17=0;
				pm17=0;
				
				grset_mode=3;
			}
				
	
			for(i=0;i<10000;i++){}
			lcdclear();
	}
	
	/*----------------------------------------------------------------------キッカーテスト*/
	
	if ((deb_mode==4)&&(deb_time>movetime)){

			strcpy(debug_txt,"P07-ﾎｰﾙﾄﾞ    P06-ｷｯｸ");lcdtext(0x80,debug_txt);
			if((p5_1==0)&&(p5_2==1)){
			strcpy(debug_txt,"      ﾎ-ﾙﾄﾞﾁｭｳ      ");lcdtext(0x94,debug_txt);
			}

			if((p5_1==1)&&(p5_2==1)){
			strcpy(debug_txt,"      ﾎｰﾙﾄﾞﾃｲｼ      ");lcdtext(0x94,debug_txt);
			}

			if(p5_3==0){
			strcpy(debug_txt,"       ｷｯｶｰON       ");lcdtext(0xd4,debug_txt);
			}

			if(p5_3==1){
			strcpy(debug_txt,"       ｷｯｶｰOFF      ");lcdtext(0xd4,debug_txt);
			}

			p5_3=p0_6;

			if((p5_1==1)&&(p5_2==1)&&(p0_7==0)){
				p5_1=0;
				p5_2=1;
				for(;p0_7==0;){}
				for(i=0;i<300;i++){}
			}

			if((p5_1==0)&&(p5_2==1)&&(p0_7==0)){
				p5_1=1;
				p5_2=1;
				for(;p0_7==0;){}
				for(i=0;i<300;i++){}
			}
				

			for(i=0;i<10000;i++){}
			lcdclear();
	}
	

	/*----------------------------------------------------------------------Forward / Keeper Change*/
	
	if ((deb_mode==5)&&(deb_time>movetime)){
	

			if((timer[3]<69)&&(grwb==0)){
				strcpy(debug_txt,"    White Attack    ");lcdtext(0xc0,debug_txt);
			}

			if((timer[3]<69)&&(grwb==1)){
				strcpy(debug_txt,"    Black Attack    ");lcdtext(0xc0,debug_txt);
			}

	
			if(timer[3]<69){
				strcpy(debug_txt,"    ｹﾞﾝｻﾞｲﾉｾｯﾃｲﾊ    ");lcdtext(0x80,debug_txt);
			}

			if((timer[3]<69)&&(romfk[0]==0)&&(romfk[0]==ramfk)){
				strcpy(debug_txt,"       Forward      ");lcdtext(0x94,debug_txt);
			}

			if((timer[3]<69)&&(romfk[0]==1)&&(romfk[0]==ramfk)){
				strcpy(debug_txt,"        Keeper      ");lcdtext(0x94,debug_txt);

			}

			if((timer[3]<69)&&(romfk[0]==0)&&(ramfk==1)){
				strcpy(debug_txt,"        Keeper      ");lcdtext(0x94,debug_txt);
				strcpy(debug_txt,"        ﾍﾝｺｳ?       ");lcdtext(0xd4,debug_txt);

			}

			if((timer[3]<69)&&(romfk[0]==1)&&(ramfk==0)){
				strcpy(debug_txt,"       Forward      ");lcdtext(0x94,debug_txt);
				strcpy(debug_txt,"        ﾍﾝｺｳ?       ");lcdtext(0xd4,debug_txt);
			}



					

			if((romfk[0]!=0)&&(romfk[0]!=1)&&(timer[3]<70)){
				strcpy(debug_txt,"ｴﾗｰ(");lcddebug1(0x97,debug_txt,romfk[0]);
				strcpy(debug_txt,")");lcdtext(0x00,debug_txt);
			}


			if((ramfk==0)&&(p0_6==0)&&(timer[3]<=68)){
				ramfk=1;
				for(;p0_6==0;){}
			}

			if((ramfk==1)&&(p0_6==0)&&(timer[3]<=68)){
				ramfk=0;
				for(;p0_6==0;){}
			}				

			if((romfk[0]!=ramfk)&&(p0_7==0)&&(timer[3]<=68))timer[3]++;
			if((p0_7==1)&&(timer[3]==69))timer[3]=70;
			if((p0_7==0)&&(timer[3]==69)){
				lcdclear();
				strcpy(lcd_text,"FW/KP Set");lcdtext(0x80,lcd_text);
				strcpy(lcd_text,"ｶｷｺﾐ?");lcdtext(0xc6,lcd_text);
				strcpy(debug_txt,"No    Yes   Cancel");lcdtext(0x95,debug_txt);
			}

			if((p0_7==1)&&(timer[3]<=68))timer[3]=0;

			if(timer[3]>=70){

				strcpy(lcd_text,"FW/KP Set");lcdtext(0x80,lcd_text);
				strcpy(lcd_text,"ｶｷｺﾐ?");lcdtext(0xc6,lcd_text);
				strcpy(debug_txt,"No    Yes   Cancel");lcdtext(0x95,debug_txt);

				if(p0_6==0){
					lcdclear();			
					strcpy(lcd_text,"ｶｷｺﾐﾁｭｳ.....");lcdtext(0xc3,lcd_text);
					prc0=1;
					p0_0=0;
					cm06=0;
		
					cm16=1;
					cm17=1;
					pm17=1;
		
					fmr01=0;
					fmr01=1;
					fmr11=0;
					fmr11=1;
					
					romfk[0]=0x20;
					romfk[0]=0xd0;
		
					for(;fmr00=0;){}
		
					romfk[0]=0x40;
					romfk[0]=ramfk;
	
					fmr01=0;
					cm06=0;
					cm16=0;
					cm17=0;
					pm17=0;

					timer[3]=0;
				}
	
	
				if(p0_7==0){
					timer[3]=0;
				}

					for(;p0_6==0;){}
					for(;p0_7==0;){}

			}	

			for(i=0;i<10000;i++){}
			lcdclear();
	}
	
	}
}
}

/**********************************************************************************************


	TeamTakahama 2008 OAKS16-62P Programs
			For SPIDER & OCTOPUS
	Copyright Hikaru from 2008 TeamTakahama

	22 June 2008 : Developing for Robocup2008 Suzhou

	Foward & Keeper Hybrid Program Template Rev1.00
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

	unsigned short px=0;			//X座標変数
	unsigned short py=0;			//Y座標変数
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
	unsigned short usonicb;		//後超音波変数
	unsigned short gr;			//グレースケール変数
	unsigned short grset[3]=0;		//デバッグ用のGR設定変数
	unsigned short grset_mode=0;		//デバッグ用のGR設定モード変数

	unsigned short grwb=0;		//白攻め・黒攻め設定変数

	unsigned long timer[16];		//汎用ソフトタイマー
	unsigned long deb_time=0;		//デバッグモードの画面切り替えタイマ変数
	unsigned int deb_clutch=0;		//デバッグモードの画面切り替え時、クラッチ？

	unsigned long grset_wr_timer=0;		//デバッグモード時にボタン長押し監視
	unsigned long i=0;			//汎用変数(ワンループするものには使わない)

	unsigned short uss_find_back=128;	//待機位置復帰時に超音波で壁を認識する値
	unsigned short uss_mj_point;		//待機位置復帰時にグレースケール側長から超音波側長に切り替える値

/*-----------------------------------------------------------------------------------------------------------------------------*/
	unsigned short wp_clutch=1;		//一旦、待機位置に戻ったら、横方向に動いても戻らない
	unsigned short wait_position_uss=138;	//後超音波測定時待機位置の超音波の値
	unsigned short wait_position_margin=100;	//後超音波測定時待機位置のマージン(余白)
	unsigned long wait_positiong_timer=1000;	//待機位置に場合に
	unsigned short uss_add_var=320;		//超音波の左右合計値(これ以上で正常測定認識)
	unsigned long search_spin_timer=0;	//超音波異常時に回転して正常値を探す
	unsigned short search_reverse_cunt=0;	//超音波以上に回転反転した回数をカウント
	unsigned short search_reverse_limit=5;	//reverse_cuntのリミット
	unsigned long search_next_timer=0;	//search_reverse_cuntで一定以上カウントした後に止まっている時間
	unsigned short latest_wrong_mag=0;	//最後にMag修正した時のmagの値
/*-----------------------------------------------------------------------------------------------------------------------------*/
	unsigned short irfind=400;		//通常時ボール赤外線認識値
	unsigned short irfind_line=600;		//前線停止エリアでボール赤外線認識値
	unsigned short dis_irfind=350;		//通常時ボール赤外線不認識値(irfind > disirfind)
	unsigned short dis_irfind_line=580;	//前線停止エリアでボール赤外線不認識値(irfind_line > disirfind_line)
	unsigned short gr_stop_line;		//前線停止エリアのGR値
	unsigned short gr_stop_line_margin;	//前線停止エリアのGR値

	unsigned short gr_wait_line;
	unsigned short gr_wait_line_margin;

	unsigned short heikin_gr_count=5;		//GR積分の回数
	unsigned char heikin_rireki[5];			//GR積分の配列
	unsigned short heikin_gr_start=0;		
	unsigned short before_gr=0;			//積分前のGRの値
	unsigned long add_gr=0;	


	unsigned short gr_under;

	unsigned short hold_cou;
	unsigned short hold_dis_cou;
	unsigned short hold_dis_limit=50;
	unsigned short hold_cou_limit=200;

	unsigned short grsup_on=1;		//グレースケール・サポートモードを使うか 0=Off 1=On 
	unsigned short grsup_back_var;		//グレースケール・サポートモードの場合に前に行く値
	

	//--------------------フォワード---

	unsigned short f_gr_wait = 98;
	unsigned short f_gr_wait_margin = 15;
	unsigned short f_gr_wait_speed = 25;

	unsigned short f_gr_hold = 370;

	unsigned short f_hold_cou=0;
	unsigned short f_hold_discou=0;

	unsigned short f_hold_cou_limit=200;
	unsigned short f_hold_discou_limit=50;


	unsigned short f_ir_find = 100;
	unsigned short f_ir_disfind = 80;

	unsigned int vsc=0;			//タイチュウゴク戦モード 0=off; 1=on;


	//--------------------------------

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
	wh_grset=450;				//JoinMax JE-1111用オフセット

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

	if((grwb==0)&&(fkset==0)){				//フォワードで白攻め設定ならば、各グレースケール変数を設定する
		f_gr_wait = romwr[1];		//待機位置基準
		f_gr_wait_margin = 30;	//待機位置余白
		f_gr_wait_speed = 30;		//待機位置接近減速余白

		f_gr_hold = romwr[2];		//前壁接近線
	}

	if((grwb==1)&&(fkset==0)){				//フォワードで黒攻め設定ならば、各グレースケール変数を設定する

		f_gr_wait = romwrb[1];	//待機位置基準
		f_gr_wait_margin = 15;	//待機位置余白
		f_gr_wait_speed = 25;		//待機位置接近減速余白

		f_gr_hold = romwrb[2];	//前壁接近線

	}


	if((grwb==0)&&(fkset==1)){				//キーパーで白攻め設定ならば、各グレースケール変数を設定する
		gr_stop_line = romwr[2];
		gr_wait_line = romwr[1];
		gr_under = romwr[0];
	
		uss_mj_point = gr_stop_line - 2;
		grsup_back_var=gr_under - 2;

		gr_stop_line_margin=15;
		gr_wait_line_margin=8;

	}

	if((grwb==1)&&(fkset==1)){				//キーパー黒攻め設定ならば、各グレースケール変数を設定する
		gr_stop_line = romwrb[2];
		gr_wait_line = romwrb[1];
		gr_under = romwrb[0];
	
		uss_mj_point = gr_stop_line - 2;
		grsup_back_var=gr_under - 2;

		gr_stop_line_margin=25;
		gr_wait_line_margin=20;
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
		usonicb		=ad6;
		if(grwb==0)	before_gr=ad7;		//黒守りの時grにグレースケールの値を代入。
		if(grwb==1)	before_gr=wh_grset-ad7;	//白守りの時grにグレースケールの値を代入。

		//if(mag==1)mag=0;
/********************************************************[GR積分モジュール]***/

/*--------------------------------------------------------------------------------------

		if(heikin_gr_count>=heikin_gr_start){
			heikin_gr_start++;
		}

		if(heikin_gr_count>=heikin_gr_start){
			heikin_rireki[heikin_gr_start] = before_gr;
			gr = before_gr;
		}

		if(heikin_gr_start>heikin_gr_count){
			for(i=heikin_gr_count;i>=1; i--){
				heikin_rireki[i] = heikin_rireki[i-1];
			}

			heikin_rireki[0]=before_gr;

			add_gr=0;			
			for(i=0;i<=heikin_gr_count;i++){
				add_gr = add_gr + heikin_rireki[i];
			}

			gr = (add_gr / heikin_gr_count);
			
			
		}


------------------------------------------------------------------------------------*/


gr=before_gr;



/********************************************************[px判定モジュール]***/
	if(fkset == 0){			//フォワード設定ならば
		if(usonicl+100<usonicr)px=1;
		if(usonicr+100<usonicl)px=3;
		if((usonicr+100>usonicl)&&(usonicl+100>usonicr))px=2;
	}

	if(fkset == 1){			//キーパー設定ならば
		if(usonicl+75<usonicr)px=1;
		if(usonicr+75<usonicl)px=3;
		if((usonicr+75>usonicl)&&(usonicl+75>usonicr))px=2;
	}

/********************************************************[py判定モジュール]***/
/*----蘇州大会ではPY無使用なので無効化
		if(gr>grc)py=3;
		if(gr>grb)py=2;
		if(gr>gra)py=1;
		if(gr<grc)py=4;
*/
/*************************************************[通常動作モード(Forward)]***/
if((p0_3==0)&&(fkset==0)){	

	timer[2]=0;
	p0_1=1;


	if((mode==0)&&(mag==0)){
		p5_1=1;
		p5_2=1;
		p5_3=1;
		if((gr > f_gr_wait + f_gr_wait_margin + f_gr_wait_speed)&&(px == 1)){	//待機エリアよりも遠く離れていて、座標が左だったら
			pwm(128,128,256,256,64);
			p2 = 0x06;
			p1 = 0xf6;
		}

		if((gr > f_gr_wait + f_gr_wait_margin + f_gr_wait_speed)&&(px == 2)){	//待機エリアよりも遠く離れていて、座標が中心だったら
			pwm(256,256,256,256,64);
			p2 = 0x06;
			p1 = 0x96;
		}

		if((gr > f_gr_wait + f_gr_wait_margin + f_gr_wait_speed)&&(px == 3)){	//待機エリアよりも遠く離れていて、座標が右だったら
			pwm(128,128,256,256,64);
			p2 = 0x06;
			p1 = 0x9f;
		}



		if((gr > f_gr_wait + f_gr_wait_margin )&&(gr <= f_gr_wait + f_gr_wait_margin + f_gr_wait_speed)&&(px == 1)){	//待機エリアよりも近く離れていて、座標が右だったら
			pwm(64,64,128,128,64);
			p2 = 0x06;
			p1 = 0x9f;
		}

		if((gr > f_gr_wait + f_gr_wait_margin )&&(gr <= f_gr_wait + f_gr_wait_margin + f_gr_wait_speed)&&(px == 2)){	//待機エリアよりも近く離れていて、座標が中だったら
			pwm(128,128,128,128,64);
			p2 = 0x06;
			p1 = 0x96;
		}

		if((gr > f_gr_wait + f_gr_wait_margin )&&(gr <= f_gr_wait + f_gr_wait_margin + f_gr_wait_speed)&&(px == 3)){	//待機エリアよりも近く離れていて、座標が左だったら
			pwm(64,64,128,128,64);
			p2 = 0x06;
			p1 = 0xf6;
		}




		if((gr > f_gr_wait)&&(gr <= f_gr_wait + f_gr_wait_margin )&&(px == 1)){	//待機エリア帯で、座標が左だったら
			pwm(128,128,128,128,64);
			p2 = 0x0f;
			p1 = 0x66;
		}

		if((gr > f_gr_wait)&&(gr <= f_gr_wait + f_gr_wait_margin )&&(px == 2)){	//待機エリア帯で、座標が中心だったら(待機位置)
			p2 = 0x0f;
			p1 = 0xff;
		}

		if((gr > f_gr_wait)&&(gr <= f_gr_wait + f_gr_wait_margin )&&(px == 3)){	//待機エリア帯で、座標が右だったら
			pwm(128,128,128,128,64);
			p2 = 0x0f;
			p1 = 0x99;
		}




		if((gr <= f_gr_wait)&&(gr > f_gr_wait - f_gr_wait_speed )&&(px == 1)){	//待機エリア帯より近く後ろで、座標が左だったら
			pwm(64,64,128,128,64);
			p2 = 0x09;
			p1 = 0x6f;
		}

		if((gr <= f_gr_wait)&&(gr > f_gr_wait - f_gr_wait_speed )&&(px == 2)){	//待機エリア帯より近く後ろで、座標が中心だったら
			pwm(128,128,128,128,64);
			p2 = 0x09;
			p1 = 0x69;
		}

		if((gr <= f_gr_wait)&&(gr > f_gr_wait - f_gr_wait_speed )&&(px == 3)){	//待機エリア帯より近く後ろで、座標が右だったら
			pwm(64,64,128,128,64);
			p2 = 0x09;
			p1 = 0xf9;
		}




		if((gr <= f_gr_wait - f_gr_wait_speed )&&(px == 1)){	//待機エリア帯より遠く後ろで、座標が左だったら
			pwm(128,128,256,256,64);
			p2 = 0x09;
			p1 = 0x6f;
		}

		if((gr <= f_gr_wait - f_gr_wait_speed )&&(px == 2)){	//待機エリア帯より近く後ろで、座標が中心だったら
			pwm(256,256,256,256,64);
			p2 = 0x09;
			p1 = 0x69;
		}

		if((gr <= f_gr_wait - f_gr_wait_speed )&&(px == 3)){	//待機エリア帯より近く後ろで、座標が右だったら
			pwm(128,128,256,256,64);
			p2 = 0x09;
			p1 = 0xf9;
		}

	}
	if((mode==0)&&(mag!=0))magmatch(mag,64,32);




	if(((mode==0)||(mode==60))&&(irr+irc+irl>300))mode=20;
	if((mode==20)&&(irr+irc+irl<=300))mode=0;

	if((mode==20)&&(mag==0)){

			if((vsc == 0)&&(irr+irc+irl>700)){
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
			if(usonicr > 60){
				pwm(256,256,256,256,64);
				p2=0x06;
				p1=0x96;
			}

			if(usonicr <= 60){
				pwm(128,128,256,256,64);
				p2=0x06;
				p1=0x90;
			}
		}

		if(p9_3==0){
			if((usonicr > 60)&&(usonicl > 60)){
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

			if(usonicr <= 60){
				pwm(128,128,256,256,64);
				p2=0x06;
				p1=0x90;
			}

			if(usonicl <= 60){
				pwm(128,128,256,256,64);
				p2=0x06;
				p1=0x06;
			}
				
				
		}

		if(p9_4==0){
			if(usonicr > 60){
				pwm(256,256,256,256,64);
				p2=0x06;
				p1=0x96;
			}

			if(usonicl <= 60){
				pwm(128,128,256,256,64);
				p2=0x06;
				p1=0x06;
			}
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
		

/*------------------------------------------------------------------キック---*/

	if((mode!=50)&&(ball_catch==0)&&(vsc == 0)){	//VSCがオフだったら
		mode=50;
		f_hold_cou=0;

		p5_1=0;
		p5_2=1;
		hpwm(256,64);
	}

	if((mode!=50)&&(ball_catch==0)&&(vsc == 1)&&(gr > f_gr_hold)){	//VSCがオンだったら
		mode=50;
		f_hold_cou=0;

		p5_1=0;
		p5_2=1;
		hpwm(256,64);
	}

	if((mode==50)&&(ball_catch==0)){
	
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
	
		f_hold_discou=0;
		f_hold_cou++;
	}

	if((mode==50)&&(ball_catch == 1)) f_hold_discou++;
	if((mode==50)&&(f_hold_discou >= f_hold_discou_limit))   mode=0;


	if((mode==50)&&(f_hold_cou > f_hold_cou_limit)&&(mag==0)){

		// lcdvar(0xd9,px);lcdvar(0xdc,f_gr_hold);lcdvar(0xc0,gr);
		f_hold_discou=0;

		if((px!=2)&&(gr > f_gr_hold)){
			lcdvar(0xc5,1);
			f_hold_discou=0;
			for(i=0; i<70000; i++){
				mag=magget();
				if(mag==0){
					pwm(30,15,0,0,64);
					p2=0x06;
					p1=0x96;
				}

				if(ball_catch==0)f_hold_discou=0;
				if(ball_catch==1)f_hold_discou++;
				if(f_hold_discou >= f_hold_discou_limit)break;

				if(mag!=0)magmatch(mag,16,16);
			}
					
			if(ad3+100<ad4)px=1;
			if(ad4+100<ad3)px=3;
			if((ad4+100>ad3)&&(ad3+100>ad4))px=2;	
	
		}

		if((px == 1)&&(gr <= f_gr_hold)){		//座標が左で前壁に接近していない
			lcdvar(0xc5,3);
			pwm(64,64,64,64,64);
			p2=0x00;
			p1=0b10111011;
			for(i=0; i<120000; i++){}
		}
	
		if((px == 3)&&(gr <= f_gr_hold)){		//座標が右で前壁に接近していない
			lcdvar(0xc5,4);
			pwm(64,64,64,64,64);
			p2=0x00;
			p1=0b01110111;
			for(i=0; i<120000; i++){}
		}

		if((px == 1)&&(gr > f_gr_hold)){		//座標が左で前壁に接近している
			for(i=0; i<130000; i++){
				mag=magget();
				if(mag==0){
					if(i < 50000)pwm(30,30,30,30,64);
					else pwm(40,40,40,40,64);
					p2=0xff;
					p1=0x66;
				}

				if(ball_catch==0)f_hold_discou=0;
				if(ball_catch==1)f_hold_discou++;
				if(f_hold_discou >= f_hold_discou_limit)break;

				if(mag!=0)magmatch(mag,16,16);
			}

		}

		if((px == 3)&&(gr > f_gr_hold)){		//座標が右で前壁に接近している
			for(i=0; i<130000; i++){
				mag=magget();
				if(mag==0){
					if(i < 50000)pwm(30,30,30,30,64);
					else pwm(40,40,40,40,64);
					p2=0xff;
					p1=0x99;
				}

				if(ball_catch==0)f_hold_discou=0;
				if(ball_catch==1)f_hold_discou++;
				if(f_hold_discou >= f_hold_discou_limit)break;

				if(mag!=0)magmatch(mag,16,16);
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
		f_hold_cou=0;
		f_hold_discou=0;
		mode=0;
	}

	if(mode != 50){
		f_hold_cou=0;
		f_hold_discou=0;
	}
		
	if((mode==50)&&(mag!=0)) magmatch(mag,32,32);





/*--------------------------------------------------------------情報---*/


	strcpy(debug_txt,"mode=");lcddebug1(0x98,debug_txt,mode);
	strcpy(debug_txt,"px=");lcddebug1(0xc0,debug_txt,px);
	strcpy(debug_txt,"py=");lcddebug1(0xc5,debug_txt,py);
	strcpy(debug_txt,"WAIT=");lcddebug1(0xc5,debug_txt,f_gr_wait);
 




}
/*************************************************[通常動作モード(Keeper)]***/
if((p0_3==0)&&(fkset == 1)){	
	timer[2]=0;
	p0_1=1;

	/*--------------------------------------------------------------------------元の位置に戻る----------------*/

	if((mode==0)&&(gr>uss_mj_point)&&(mag==0)){		//(待機位置復帰)現在位置が後超音波側長エリアよりも前
	
		if(px==1){					//右側の場合
			pwm(128,128,256,256,64);
			p2=0x06;
			p1=0xf6;
		}		

		if(px==2){					//中央の場合
			pwm(256,256,256,256,64);
			p2=0x06;
			p1=0x96;
		}

		if(px==3){					//左側の場合
			pwm(128,128,256,256,64);
			p2=0x06;
			p1=0x9f;
		}
	}


	if((mode==0)&&(gr<=uss_mj_point)&&(mag==0)){		//(待機位置復帰)現在位置が後超音波側長エリア内
		if(((grsup_on == 0)&&(usonicb<wait_position_uss))||((grsup_on == 1)&&(gr < gr_wait_line ))){				//待機位置よりも後
			if((usonicr+usonicl > uss_add_var)&&(usonicr+usonicl<1000)){
				pwm(32,32,32,32,64);
				p2=0x09;
				p1=0x69;
			}

		}

		if((px==1)&&(((grsup_on == 0)&&(usonicb>wait_position_uss+wait_position_margin))||((grsup_on == 1)&&(gr >= gr_wait_line + gr_wait_line_margin )))){		//待機位置よりも斜め左前
			if((usonicr+usonicl > uss_add_var)&&(usonicr+usonicl<1000)){
				pwm(64,64,128,128,64);
				p1=0x06;
				p2=0x06;
			}
		}

		if((px==2)&&(((grsup_on == 0)&&(usonicb>wait_position_uss+wait_position_margin))||((grsup_on == 1)&&(gr >= gr_wait_line + gr_wait_line_margin )))){		//待機位置よりも前
			if((usonicr+usonicl > uss_add_var)&&(usonicr+usonicl<1000)){
				pwm(64,64,64,64,64);
				p1=0x06;
				p2=0x96;
			}
		}

		if((px==3)&&(((grsup_on == 0)&&(usonicb>wait_position_uss+wait_position_margin))||((grsup_on == 1)&&(gr >= gr_wait_line + gr_wait_line_margin )))){		//待機位置よりも斜め右前
			if((usonicr+usonicl > uss_add_var)&&(usonicr+usonicl<1000)){
				pwm(64,64,128,128,64);
				p2=0x06;
				p1=0x90;
			}
		}

		if((px==1)&&(((grsup_on == 0)&&(usonicb>wait_position_uss)&&(usonicb<wait_position_uss+wait_position_margin))||((grsup_on == 1)&&(gr >= gr_wait_line)&&(gr <gr_wait_line + gr_wait_line_margin )))){	//待機位置よりも左
			if((usonicr+usonicl > uss_add_var)&&(usonicr+usonicl<1000)){
				pwm(128,128,128,128,64);
				p2=0xff;
				p1=0x66;
			}

		}

		if((px==2)&&(((grsup_on == 0)&&(usonicb>wait_position_uss)&&(usonicb<wait_position_uss+wait_position_margin))||((grsup_on == 1)&&(gr >= gr_wait_line)&&(gr <gr_wait_line + gr_wait_line_margin )))){	//待機位置
			if((usonicr+usonicl > uss_add_var)&&(usonicr+usonicl<1000)){
				p2=0xff;
				p1=0xff;
			}
		}

		if((px==3)&&(((grsup_on == 0)&&(usonicb>wait_position_uss)&&(usonicb<wait_position_uss+wait_position_margin))||((grsup_on == 1)&&(gr >= gr_wait_line)&&(gr <gr_wait_line + gr_wait_line_margin )))){	//待機位置よりも右

			if((usonicr+usonicl > uss_add_var)&&(usonicr+usonicl<1000)){
				pwm(128,128,128,128,64);
				p2=0xff;
				p1=0x99;
			}
		
		}			

		if(gr<uss_mj_point){						//待機位置圏内又は後ろ
			if((usonicr+usonicl < uss_add_var)||(usonicr+usonicl > 1000)){	//超音波センサーが異常
				if((search_spin_timer < 200)&&(search_reverse_cunt<search_reverse_limit)){		//壁検索回転方向反転タイマーがX以下
					pwm(16,16,16,16,128);
						if(latest_wrong_mag == 1){	//最後にmagmatchした時のmagが1
							pwm(16,16,16,16,128);
							p2=0x55;
							p1=0x55;
						}
						if(latest_wrong_mag == 15){	//最後にmagmatchした時のmagが15
							pwm(16,16,16,16,128);
							p2=0xAA;
							p1=0xAA;
						}
						if(latest_wrong_mag == 0){	//一度もmagmatchした事が無い時
							pwm(16,16,16,16,128);
							p2=0xAA;
							p1=0xAA;
						}
					search_spin_timer++;
				}

				if((search_spin_timer >= 200)&&(search_reverse_cunt<search_reverse_limit)){		//壁検索回転方向反転タイマーがX以上
					if(p1 == 0x55){		
						pwm(16,16,16,16,128);
						search_spin_timer=0;
						search_reverse_cunt++;
						latest_wrong_mag=15;
						if(search_spin_timer<10000)latest_wrong_mag=1;
					}
					if(p1 == 0xAA){
						pwm(16,16,16,16,128);
						search_spin_timer=0;
						search_reverse_cunt++;
						latest_wrong_mag=1;
						if(search_spin_timer<10000)latest_wrong_mag=15;
					}


				}

				if(search_reverse_cunt>=search_reverse_limit){
					p2=0xff;
					p1=0xff;
					search_next_timer++;
					if(search_next_timer>1000){
						search_spin_timer=0;
						search_next_timer=0;
						search_reverse_cunt=0;
					}

				}
				
				if((grsup_on == 1)&&(gr < grsup_back_var)){
					pwm(32,32,32,32,64);
					p2=0x09;
					p1=0x69;
				}
					
			}else{
				search_spin_timer=0;
				search_reverse_cunt=0;
				search_next_timer=0;
			}	
		}
	}

	if(mode == 0){
		p5_1=0;
		p5_2=0;
		p5_3=1;
	}	

	if(mode!=0){
		search_spin_timer=0;
		search_reverse_cunt=0;
		search_next_timer=0;
	}


	if((mode==0)&&(mag!=0)){
		latest_wrong_mag=mag;
		search_spin_timer=10000;
		magmatch(mag,32,32);
	}

/*---------------------------------------------------------------------------------(アナログ)ボールを追いかける----*/


	if((mode == 0)&&(irr+irl+irc > irfind)&&(gr < gr_stop_line))mode=20;	//通常動作エリア内でボール発見→ボール追跡モード
	if((mode == 20)&&(irr+irl+irc < dis_irfind))mode=0;
	
	if((mode == 20)&&(mag == 0)&&(irr*2 > irl+irc)){		//ボール追跡モードで右が一番大きかったら
		pwm(128,128,256,256,64);
		p2=0x09;
		p1=0x6f;
	}

	if((mode == 20)&&(mag == 0)&&(irc*2 > irr+irl)){		//ボール追跡モードで中が一番大きかったら
		pwm(128,128,128,128,64);
		p2=0x09;
		p1=0x69;
	}

	if((mode == 20)&&(mag == 0)&&(irl*2 > irr+irc)){		//ボール追跡モードで左が一番大きかったら
		pwm(128,128,256,256,64);
		p2=0x09;
		p1=0xf9;
	}

	if((mode == 20)&&(irc > 700)){
		hpwm(128,64);
		p5_1=0;
		p5_2=1;
		p5_3=1;
	}

	if((mode == 20)&&(mag != 0))magmatch(mag,32,32);		//ボール追跡モードでmagが0で無かったら




/*-----------------------------------------------------------------------------------------------前線停止エリア----*/

	if((mode == 20)&&(gr > gr_stop_line))mode=29;
	if((mode == 0)&&(gr > gr_stop_line)&&(irr+irc+irl > irfind_line))mode=29;
	if((mode == 29)&&(gr < gr_stop_line-gr_stop_line_margin))mode=20;
	if((mode == 29)&&(irr+irc+irl < dis_irfind_line))mode=0;

	if((mode == 29)&&(mag == 0)&&(gr <= gr_stop_line)&&(irr*2 > irl+irc)){		//前線停止エリアで右が一番大きかったら
		if((irc > 700)&&(irr > irc)){					//もしもボールが右の遠くの方なら
			pwm(64,64,64,64,128);
		}else{
			pwm(200,200,200,200,64);
		}
		p2=0xff;
		p1=0x66;
	}

	if((mode == 29)&&(mag == 0)&&(gr <= gr_stop_line)&&(irc*2 > irr+irl)){		//前線停止エリアで中が一番大きかったら
		p2=0xff;
		p1=0xff;
	}

	if((mode == 29)&&(mag == 0)&&(gr <= gr_stop_line)&&(irl*2 > irr+irc)){		//前線停止エリアで左が一番大きかったら
		if((irc > 700)&&(irl > irc)){					//もしもボールが左の遠くの方なら
			pwm(64,64,64,64,128);
		}else{
			pwm(200,200,200,200,64);
		}
		p2=0xff;
		p1=0x99;
	}

	if((mode == 29)&&(mag == 0)&&(gr > gr_stop_line)&&(irr*2 > irl+irc)){		//前線停止エリアより前で右が一番大きかったら
		if((irc > 700)&&(irr > irc)){					//もしもボールが右の遠くの方なら
			pwm(32,32,64,64,128);
		}else{
			 pwm(100,100,200,200,64);
		}
		p2=0x06;
		p1=0x90;
	}

	if((mode == 29)&&(mag == 0)&&(gr > gr_stop_line)&&(irc*2 > irr+irl)){		//前線停止エリアより前で中が一番大きかったら
		pwm(32,32,32,32,128);
		p2=0x06;
		p1=0x96;
	}

	if((mode == 29)&&(mag == 0)&&(gr > gr_stop_line)&&(irl*2 > irr+irc)){		//前線停止エリアより前で左が一番大きかったら
		if((irc > 700)&&(irl > irc)){					//もしもボールが左の遠くの方なら
			pwm(32,32,64,64,128);
		}else{
			pwm(100,100,200,200,64);
		}
		p2=0x06;
		p1=0x06;
	}


	if((mode == 29)&&(irc > 700)){
		hpwm(128,64);
		p5_1=0;
		p5_2=1;
		p5_3=1;
	}	
	if((mode == 29)&&(mag != 0))magmatch(mag,32,32);


/*-------------------------------------------------------------------------------------回り込みモード---*/
	strcpy(debug_txt,"mode=");lcddebug1(0x98,debug_txt,mode);
	if((mode == 0 )&&((p9 != 0xFF)||(p8_1 == 0)||(p8_2 == 0))) mode = 90;				//もしもデジタルIRが反応していたら回り込み
	if((mode == 90)&&(p9 == 0xFF)&&(p8_1 == 1)&&(p8_2 == 1)) mode = 0;				//もしも回り込みモードでデジタルが反応していなかったら

	if((mode == 90)&&(mag == 0)&&(gr >= gr_stop_line)){
		pwm(256,256,256,256,64);
		p2=0x06;
		p1=0x96;
	}

	if((mode == 90)&&(mag == 0)&&(gr > gr_stop_line - gr_stop_line_margin)&&(gr < gr_stop_line)){	//上記前線停止エリア内でデジタルIR発見したら
		lcdvar(0xda,1);

		if((p9_0 == 0)&&(p9_1 == 1)){					//右斜め前が反応
			pwm(256,256,256,256,64);
			p2=0xff;
			p1=0x66;
		}

		if(p9_1 == 0){					//右真横が反応
			pwm(128,128,256,256,64);
			p2=0x06;
			p1=0xf6;
		}

		if((p9_2 == 0)&&(p9_1 == 1)){					//右斜め後ろが反応
			pwm(256,256,256,256,64);
			p2=0x06;
			p1=0x96;
		}

		if(p9_3 == 0){					//真後ろが反応

			if(px == 1){				//現在X座標が1ならば
				pwm(128,128,256,256,64);
				p2=0x06;
				p1=0x06;
			}
			if(px == 2){				//現在X座標が2ならば
				pwm(128,128,256,256,64);
				p2=0x06;
				p1=0x06;
			}
			if(px == 3){				//現在X座標が3ならば
				pwm(128,128,256,256,64);
				p2=0x06;
				p1=0x90;
			}
		}
		

		if((p9_4 == 0)&&(p9_5 == 1)){					//左斜め後ろが反応
			pwm(256,256,256,256,64);
			p2=0x06;
			p1=0x96;
		}				

		if(p9_5 == 0){					//左真横が反応
			pwm(128,128,256,256,64);
			p2=0x06;
			p1=0x9f;
		}

		if((p9_6 == 0)&&(p9_5 == 1)){					//左斜め前が反応
			pwm(256,256,256,256,64);
			p2=0xff;
			p1=0x99;
		}


	}



	if((mode == 90)&&(mag == 0)&&(gr <= gr_stop_line - gr_stop_line_margin )&&(((grsup_on == 1)&&(gr > gr_under))||((grsup_on == 0)&&(usonicb >= 75)))){		//もしも前線停止エリアより後ろ

		lcdvar(0xda,2);
		if(p9_0 == 0){					//右斜め前が反応
			if(irr > irfind) pwm(128,128,128,128,64);
			else pwm(256,256,256,256,64);
			p2=0xff;
			p1=0x66;
		}

		if(p9_1 == 0){					//右真横が反応
			pwm(128,128,256,256,64);
			p2=0x06;
			p1=0x06;
		}

		if(p9_2 == 0){					//右斜め後ろが反応
			pwm(256,256,256,256,64);
			p2=0x06;
			p1=0x96;
		}

		if(p9_3 == 0){					//真後ろが反応

			if(px == 1){				//現在X座標が1ならば
				pwm(128,128,256,256,64);
				p2=0x06;
				p1=0x06;
			}
			if(px == 2){				//現在X座標が2ならば
				pwm(128,128,256,256,64);
				p2=0x06;
				p1=0x06;
			}
			if(px == 3){				//現在X座標が3ならば
				pwm(128,128,256,256,64);
				p2=0x06;
				p1=0x90;
			}
		}
		

		if(p9_4 == 0){					//左斜め後ろが反応
			pwm(256,256,256,256,64);
			p2=0x06;
			p1=0x96;
		}				

		if(p9_5 == 0){					//左真横が反応
			pwm(128,128,256,256,64);
			p2=0x06;
			p1=0x90;
		}

		if(p9_6 == 0){					//左斜め前が反応
			if(irl > irfind) pwm(128,128,128,128,64);
			else pwm(256,256,256,256,64);
			p2=0xff;
			p1=0x99;
		}

		if((p8_1 == 0)&&(p9_1 == 1)){
			pwm(256,256,256,256,64);
			p2=0x06;
			p1=0x96;
		}

		if((p8_2 == 0)&&(p9_5 == 1)){
			pwm(256,256,256,256,64);
			p2=0x06;
			p1=0x96;
		}
	}

	if((mode == 90)&&(mag == 0)&&(gr <= gr_stop_line - gr_stop_line_margin )&&(((grsup_on == 1)&&(gr <= gr_under))||((grsup_on == 0)&&(usonicb < 75)))){			//もし後ろに壁があるならば
		lcdvar(0xda,3);

		if(p9_0 == 0){					//右斜め前が反応
			pwm(256,256,256,256,64);
			p2=0xff;
			p1=0x66;
		}

		if(p9_1 == 0){					//右真横が反応
			pwm(256,256,256,256,64);
			p2=0xff;
			p1=0x66;
		}

		if(p9_2 == 0){					//右斜め後ろが反応
			pwm(256,256,256,256,64);
			p2=0xff;
			p1=0x66;
		}

		if(p9_3 == 0){					//真後ろが反応
			pwm(32,32,32,32,64);				//回って取りにいく
			p2=0x06;
			p1=0x96;
		}
		

		if(p9_4 == 0){					//左斜め後ろが反応
			pwm(256,256,256,256,64);
			p2=0xff;
			p1=0x99;
		}				

		if(p9_5 == 0){					//左真横が反応
			pwm(256,256,256,256,64);
			p2=0xff;
			p1=0x99;
		}

		if(p9_6 == 0){					//左斜め前が反応
			pwm(256,256,256,256,64);
			p2=0xff;
			p1=0x99;
		}


		if(p8_1 == 0){
			pwm(256,256,256,256,64);
			p2=0xff;
			p1=0x99;
		}

		if(p8_2 == 0){
			pwm(256,256,256,256,64);
			p2=0xff;
			p1=0x66;
		}



	}
			

	if(mode == 90)lcdvar(0xda,5);
	if(mode != 90)lcdvar(0xda,0);

	if((mode == 90)&&(mag != 0))magmatch(mag,64,32);


/*--------------------------------------------------------------------------------------[ホールド & キック]----*/


	if((p9_7 == 0)&&(gr < gr_stop_line)) mode=50;
	if((mode == 50)&&(p9_7 == 0)){
		hold_cou++;
		hold_dis_cou=0;
	}
	if((mode == 50)&&(p9_7 == 1)) hold_dis_cou++;
	if((mode == 50)&&(hold_dis_cou >= hold_dis_limit)) mode=0;

	if((mode == 50)&&(hold_cou <= hold_cou_limit)&&(mag == 0)){
		if(gr < gr_stop_line - gr_stop_line_margin){
			pwm(64,64,64,64,180);
			p2=0x09;
			p1=0x69;
		}

		if(gr >= gr_stop_line - gr_stop_line_margin){
			p2=0xff;
			p1=0xff;
		}

		hpwm(256,64);
		p5_1=0;
		p5_2=1;
		p5_3=1;

	}

	if((mode == 50)&&(hold_cou > hold_cou_limit)&&(mag == 0)){
		p5_1=0;		//ホールダー停止
		p5_2=0;		//ホールダー停止

		for(i=0;i<10000;i++){}

		pwm(128,128,128,128,200);
		p2=0x09;		//オムニ前進
		p1=0x69;		//オムニ前進

		for(i=0;i<20000;i++){}

		p5_3=0;		//蹴る

		for(i=0;i<300000;i++){}

		p2=0xff;		//オムニ停止
		p1=0xff;		//オムニ停止

		for(i=0;i<50000;i++){}

		p5_3=1;

		mode=0;

		hold_dis_cou=0;
		hold_cou=0;
		
	}
		

	if((mode == 50)&&(mag != 0))magmatch(mag,16,16);
		
	if(mode!=50){
		hold_dis_cou=0;
		hold_cou=0;
	}

/*--------------------------------------------------------------------------------------------------------------*/


	strcpy(debug_txt,"px=");lcddebug1(0xc0,debug_txt,px);
	strcpy(debug_txt,"py=");lcddebug1(0xc5,debug_txt,py);

	lcdvar(0xdc,p8_1);
	lcdvar(0xdd,p8_2);

	lcdvar(0xd4,search_next_timer);

}



/**********************************************************[デバッグモード]***/
if(p0_3==1){
		if(timer[2]==0){		//デバッグモード1ループ目の場合
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

			strcpy(debug_txt,"usl=");lcddebug1(0xd4,debug_txt,usonicl);	
			lcdvar(0xdc,usonicb);
			strcpy(debug_txt,"usr=");lcddebug1(0xe0,debug_txt,usonicr);

	
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
			lcdvar(0x99,ad7);
			lcdvar(0x9d,0);
			lcdvar(0xa1,0);
	
			}
	
			if(grset_mode==1){
				lcdvar(0x99,grset[0]);
				lcdvar(0x9d,ad7);
				lcdvar(0xa1,0);
			}
	
			if(grset_mode==2){
				lcdvar(0x99,grset[0]);
				lcdvar(0x9d,grset[1]);
				lcdvar(0xa1,ad7);
			}
	
			if(grset_mode==3){
				lcdvar(0x99,grset[0]);
				lcdvar(0x9d,grset[1]);
				lcdvar(0xa1,grset[2]);
				strcpy(debug_txt,"Write? - PUSH P07");lcdtext(0xd4,debug_txt);
			}

			
			if((grset_mode==0)&&(p0_6==0)){
				grset[0]=ad7;
				grset_mode=1;
				for(;p0_6==0;){}
				for(i=0; i<100000; i++){}
			}

			if((grset_mode==1)&&(p0_6==0)){
				grset[1]=ad7;
				grset_mode=2;
				for(;p0_6==0;){}
				for(i=0; i<3000; i++){}
				for(i=0; i<100000; i++){}
			}

			if((grset_mode==2)&&(p0_6==0)){
				grset[2]=ad7;
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
				lcdvar(0x99,wh_grset-ad7);
				lcdvar(0x9d,0);
				lcdvar(0xa1,0);

			}

			if(grset_mode==1){
				lcdvar(0x99,grset[0]);
				lcdvar(0x9d,wh_grset-ad7);
				lcdvar(0xa1,0);
			}
		
			if(grset_mode==2){
				lcdvar(0x99,grset[0]);
				lcdvar(0x9d,grset[1]);
				lcdvar(0xa1,wh_grset-ad7);
			}

			if(grset_mode==3){
				lcdvar(0x99,grset[0]);
				lcdvar(0x9d,grset[1]);
				lcdvar(0xa1,grset[2]);
				strcpy(debug_txt,"Write? - PUSH P07");lcdtext(0xd4,debug_txt);
			}

			
			if((grset_mode==0)&&(p0_6==0)){
				grset[0]=wh_grset-ad7;
				grset_mode=1;
				for(;p0_6==0;){}
				for(i=0; i<100000; i++){}
			}

			if((grset_mode==1)&&(p0_6==0)){
				grset[1]=wh_grset-ad7;
				grset_mode=2;
				for(;p0_6==0;){}
				for(i=0; i<100000; i++){}
			}

			if((grset_mode==2)&&(p0_6==0)){
				grset[2]=wh_grset-ad7;
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


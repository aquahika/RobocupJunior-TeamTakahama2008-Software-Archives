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

	unsigned char debug_txt[]="0";		//LCD�p�̕����z��
	unsigned char lcd_text[]="0";		//LCD�p�̕����z��
	unsigned short _far *romwr;		//romwr�̃|�C���^�錾
	unsigned short _far *romwrb;		//romwrb�̃|�C���^�錾
	unsigned short _far *romfk;		//romfk�̃|�C���^�錾

	unsigned int deb_mode=0;		//�f�o�b�O���[�h�ϐ�

	unsigned int px=0;			//X���W�ϐ�
	unsigned int py=0;			//Y���W�ϐ�
	unsigned int ramfk;			//RAM��̉��zromfk

	unsigned short wh_grset;		//�������]�p�̃p�����[�^

	unsigned short movetime=0;		//�f�o�b�O���[�h�̉�ʐ؂�ւ��^�C�}�ݒ�ϐ�

	unsigned short mag=0;			//�����Z���T�[�p�̕ϐ�
	unsigned short mode=0;		//�L�[�p�[��t�H���[�h�v���O�������̃��[�h�ϐ�
	unsigned short fkset=1;		//�L�[�p�[�E�t�H���[�h�ݒ�ϐ�(�d��������)
	unsigned short irr;			//�EIR�ϐ�
	unsigned short irc;			//��IR�ϐ�
	unsigned short irl; 			//��IR�ϐ�
	unsigned short usonicr;		//�E�����g�ϐ�
	unsigned short usonicl;		//�������g�ϐ�
	unsigned short gr;			//�O���[�X�P�[���ϐ�
	unsigned short grset[3]=0;		//�f�o�b�O�p��GR�ݒ�ϐ�
	unsigned short grset_mode=0;		//�f�o�b�O�p��GR�ݒ胂�[�h�ϐ�

	unsigned short gra=0;			//�OGR�̈�ݒ�ϐ�
	unsigned short grb=0;			//�ҋ@�ʒu�̈�ݒ�ϐ�
	unsigned short grc=0;			//���GR�̈�ݒ�ϐ�

	unsigned short margin_wait=0;		//�ҋ@�ʒu�p�ш�ݒ�
	unsigned short back_spd_bdline=0;	//�ҋ@�ʒu�ɖ߂�ہA�X�s�[�h��ς������ݒ�

	unsigned long holdcou;
	unsigned short holdtime;

	unsigned short grwb=0;		//���U�߁E���U�ߐݒ�ϐ�

	unsigned long timer[16];		//�ėp�\�t�g�^�C�}�[
	unsigned long deb_time=0;		//�f�o�b�O���[�h�̉�ʐ؂�ւ��^�C�}�ϐ�
	unsigned int deb_clutch=0;		//�f�o�b�O���[�h�̉�ʐ؂�ւ����A�N���b�`�H

	unsigned long grset_wr_timer=0;		//�f�o�b�O���[�h���Ƀ{�^���������Ď�
	unsigned long i=0;			//�ėp�ϐ�(�������[�v������̂ɂ͎g��Ȃ�)

	unsigned int kp_check=0;		//�L�[�p�[�Ԍ����[�h
	unsigned short ball_catch_pt=0;		//�{�[���ɐڐG�������_�ł̃O���[�X�P�[���̒l

	unsigned short margin_bdline;		//(KP)�����_��~�G���A�̑ш�

	unsigned short ball_push_timer_r;	//(KP)�E�̃{�[�������������鎞��
	unsigned short ball_push_timer_l;	//(KP)���̃{�[�������������鎞��

	pd0 = 0b00000011;
	pd1 = 0xff;
	pd2 = 0xff;
	pd5 = PORTOUT;
	p0_1=1;
	p5_1=1;
	p5_2=1;
	p5_3=1;
	pd3 = 0x00;
	p4 =  0x00;				//�|�[�g4(LCD)�����l�ݒ�
	pd4 = PORTOUT;				//�|�[�g4�������o�͂ɐݒ�
	pd9 = PORTIN;

	movetime=15000;				//�f�o�b�O��ʈڂ�ς��^�C�}�[
//	wh_grset=600;				//�����ݒ�I�t�Z�b�g�l
	wh_grset=800;

	mode=0;					//�L�[�p�[&�t�H���[�h�����[�h
	


	pur0=0b11111111;				//�\�t�g�t�F�A�@�v���A�b�v�ݒ�0
	pur1=0b00001011;				//�\�t�g�E�F�A�@�v���A�b�v�ݒ�1

	init_ir();					//�ԊO���Z���T�[�����ݒ�
	init_moter();				//�|�[�g�EPWM�����ݒ�
	lcd_init();				//LCD������


//***************************************************************{���C���v���O����}***/
if(p0_3==1){					//�^�C�g���\��
	strcpy(lcd_text,"TeamTakahama ");lcdtext(0x80,lcd_text);
	strcpy(lcd_text,"    Debugging System");lcdtext(0xc0,lcd_text);
	strcpy(lcd_text,"Copyright ");lcdtext(0x94,lcd_text);
	strcpy(lcd_text,"   2008 TeamTakahama");lcdtext(0xd4,lcd_text);
	
	for(i=0;i<=1000000;i++){
		if((p0_5==0)||(p0_6==0)||(p0_7==0))break;	//�^�N�gSW�������ꂽ��^�C�g�����~
		if(p0_3==0)break;			//���s���[�h��RUN�ɂȂ����璆�~
	}
	lcdclear();
}

	romfk= (unsigned short _far *)0x0C0000;		//�t�H���[�h�E�L�[�p�[�̐؂�ւ�ROM�̈�̐ݒ�
	romwr= (unsigned short _far *)0x0D0000;		//�����O���[�X�P�[���̊i�[ROM�̈�̐ݒ�
	romwrb=(unsigned short _far *)0x0E0000;		//�����O���[�X�P�[���̊i�[ROM�̈�̐ݒ�		

if(romfk[0]==0xFFFF){		//�L�[�p�[���t�H���[�h�̐ݒ肪���ݒ�(0xFFFF)��������t�H���[�h(0x0000)�ɂ���
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

	if(p0_4==0)grwb=0;	//�����g�O��SW��0�������甒�U�߂ɐݒ肷��
	if(p0_4==1)grwb=1;	//�E���g�O��SW��1�������獕�U�߂ɐݒ肷��


/****************************************************************[���C�����[�v�J�n]***/
for(;;){
	if(romfk[0]==0)fkset=0;
	if(romfk[0]==1)fkset=1;

	

	if(grwb==0){				//���U�ߐݒ�Ȃ�΁A�e�O���[�X�P�[���ϐ���ݒ肷��
		gra=romwr[2];
		grb=romwr[1];
		grc=romwr[0];
		
		margin_wait=8;			//���U�ߑҋ@�ʒu �ш�ݒ�
		back_spd_bdline=20;
		margin_bdline=10;
		
	}

	if(grwb==1){				//���U�ߐݒ�Ȃ�΁A�e�O���[�X�P�[���ϐ���ݒ肷��
		gra=romwrb[2];
		grb=romwrb[1];
		grc=romwrb[0];

		margin_wait=60;			//���U�ߑҋ@�ʒu�@�ш�ݒ�
		back_spd_bdline=50;
		margin_bdline=60;

	}


	if(p0_3==0)p0_0=0;				//���s���[�h��RUN���[�h�Ȃ�ALED�𔭌�
	if(p0_3==1)p0_0=1;				//���s���[�h���f�o�b�O���[�h�Ȃ�ALED������

if(timer[1]<6000)timer[1]++;

/***************************************************************[A/D�l���]***/

		mag=magget();			//mag�Ɍ��݂̕��ʃZ���T�[�̒l�����B
		irl		=ad0;		//irl�ɍ��h�q�Z���T�[(ad0)�̒l�����B
		irc		=ad1;		//irc�ɒ��h�q�Z���T�[(ad1)�̒l�����B
		irr		=ad2;		//irr�ɉE�h�q�Z���T�[(ad2)�̒l�����B
		usonicl		=ad3;		//usonicl�ɍ������g�̒l�����B
		usonicr		=ad4;		//usonicr�ɉE�����g�̒l�����B
		if(grwb==0)	gr=ad5;		//�����̎�gr�ɃO���[�X�P�[���̒l�����B
		if(grwb==1)	gr=wh_grset-ad5;	//�����̎�gr�ɃO���[�X�P�[���̒l�����B

/********************************************************[px���胂�W���[��]***/


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

/********************************************************[py���胂�W���[��]***/

		if(gr>grc)py=3;
		if(gr>grb)py=2;
		if(gr>gra)py=1;
		if(gr<grc)py=4;


/*************************************************[�ʏ퓮�샂�[�h(Forward)]***/
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
				

			if(irl*2 > irc+irr-30){				//������ԑ傫���ꍇ��
				pwm(256,256,128,128,64);
				p2=0x09;
				p1=0x09;
			}
	
			if(irc*2-30 > irr+irl){				//��������ԑ傫���ꍇ��
				pwm(128,128,128,128,64);
				p2=0x09;
				p1=0x69;
			}
	
			if(irr*2 > irc+irl-30){				//�E����ԑ傫���ꍇ��
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

			if(irl*2 > irc+irr){		//������ԑ傫���ꍇ��
				pwm(16,16,16,16,64);	
				p2=0xf9;
				p1=0x09;
			}
	
			if(irc*2 > irr+irl){		//��������ԑ傫���ꍇ��
				pwm(8,8,8,8,64);
				p2=0xf9;	
				p1=0x69;
			}
	
			if(irr*2 > irc+irl){		//�E����ԑ傫���ꍇ��
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
			
		

	







/*--------------------------------------------------------------���---*/


	strcpy(debug_txt,"mode=");lcddebug1(0x98,debug_txt,mode);
	strcpy(debug_txt,"px=");lcddebug1(0xc0,debug_txt,px);
	strcpy(debug_txt,"py=");lcddebug1(0xc5,debug_txt,py);
 




}



/*****************************************************************************[�ʏ퓮�샂�[�h(Keeper)]***/
/***********  *******************************************************************************************/
/***********  *********       ***************************************************************************/
/***********  ***  *** ******** *************************************************************************/
/***********  *  *****          *************************************************************************/
/***********  **  ****  *********************************************************************************/
/***********  ***  *****        *************************************************************************/





if((p0_3==0)&&(fkset==1)){
	timer[2]=0;
	p0_1=1;

	//0�͑ҋ@���[�h

	if(mode==0){
		p5_1=1;
		p5_2=1;
		p5_3=1;
	
	}


	if((mode==0)&&(mag==0)){						//�ҋ@���[�h�ŕ����Z���T�[��0�Ȃ��

		if((gr > grb + margin_wait)&&(gr < grb + margin_wait + back_spd_bdline)){	//�ҋ@�ʒu�����O�ɋ���ꍇ(�߂�)
			
			if(px==1){				//�����ɋ���ꍇ
				pwm(128,128,64,64,64);
				p2=0x06;
				p1=0x06;
			}

			if(px==2){				//�����ɋ���ꍇ
				pwm(100,100,100,100,64);
				p2=0x06;
				p1=0x96;
			}

			if(px==3){				//�E���ɋ���ꍇ
				pwm(128,128,64,64,64);
				p2=0x06;
				p1=0x90;
			}
		}

		if((gr > grb + margin_wait)&&(gr >= grb + margin_wait + back_spd_bdline)){	//�ҋ@�ʒu�����O�ɋ���ꍇ(����)
			
			if(px==1){				//�����ɋ���ꍇ
				pwm(255,255,128,128,64);
				p2=0x06;
				p1=0x06;
			}

			if(px==2){				//�����ɋ���ꍇ
				pwm(255,255,255,255,64);
				p2=0x06;
				p1=0x96;
			}

			if(px==3){				//�E���ɋ���ꍇ
				pwm(255,255,128,128,64);
				p2=0x06;
				p1=0x90;
			}
		}

		if((gr < grb + margin_wait) && (gr > grb)){	//�ҋ@�ʒu���ɋ���ꍇ

			if(px==1){				//�����ɋ���ꍇ
				pwm(200,200,200,200,64);
				p2=0xff;
				p1=0x66;
			}

			if(px==2){				//�����ɋ���ꍇ
				p2=0xff;
				p1=0xff;
			}

			if(px==3){
				pwm(200,200,200,200,64);	//�E���ɋ���ꍇ
				p2=0xff;
				p1=0x99;
			}
				

		}

		if(gr < grb){				//�ҋ@�ʒu�������ɋ���ꍇ

			if(px==1){				//���ɋ���ꍇ
				pwm(128,128,64,64,64);
				p2=0x09;
				p1=0x69;
			}

			if(px==2){				//�����ɋ���ꍇ
				pwm(30,30,30,30,64);
				p2=0x09;
				p1=0x69;
			}

			if(px==3){				//�E�ɋ���ꍇ
				pwm(128,128,64,64,64);
				p2=0x09;
				p1=0x09;
			}
		}

	}

	if((mode==0)&&(mag!=0))magmatch(mag,32,32);		//�ҋ@���[�h�ŕ����Z���T�[��0�ł͖����ꍇ	

/********************************************************************************************************************************/

	if((mode!=25)&&(mode!=29)&&(mode!=50)&&(mode!=51)&&(irr+irc+irl>=200)) mode=20;	//�����_���[�h���ȊO�Ƀ{�[�����������ʏ�ǂ������郂�[�h
	if((mode==20)&&(irr+irc+irl<300)) mode=0;			//�ʏ�ǂ������郂�[�h�Ń{�[��������������


	if(mode==20){
		hpwm(128,64);
		p5_1=0;
		p5_2=1;
	}


	if((mode==20)&&(mag==0)){				//�ʏ�ǂ������郂�[�h�ŕ����Z���T�[��0�Ȃ��

		if(irl*2 > irc+irr-30){				//������ԑ傫���ꍇ��
			pwm(256,256,128,128,64);
			p2=0x09;
			p1=0x09;
		}

		if(irc*2-30 > irr+irl){				//��������ԑ傫���ꍇ��
			pwm(128,128,128,128,64);
			p2=0x09;
			p1=0x69;
		}

		if(irr*2 > irc+irl-30){				//�E����ԑ傫���ꍇ��
			pwm(256,256,128,128,64);
			p2=0x09;
			p1=0x60;
		}		

	}

	if((mode==20)&&(mag!=0))magmatch(mag,32,32);		//�ʏ�ǂ������郂�[�h�ŕ����Z���T�[��0�łȂ�������


/********************************************************************************************************************************/
	if((mode==25)&&(irr+irc+irl<400)) mode=0;			//�U��Ԃ��Ēǂ������郂�[�h


	if(mode==25){
		hpwm(128,64);
		p5_1=0;
		p5_2=1;
	}

	
	if(mode==25){					//�U��Ԃ��Ēǂ������郂�[�h
		if(irl*2 > irc+irr){				//������ԑ傫���ꍇ��
			pwm(20,20,10,10,64);
			p2=0x09;
			p1=0x09;
		}

		if(irc*2 > irr+irl){				//��������ԑ傫���ꍇ��
			pwm(20,20,20,20,64);
			p2=0x09;
			p1=0x69;
		}

		if(irr*2 > irc+irl){				//�E����ԑ傫���ꍇ��
			pwm(20,20,10,10,64);
			p2=0x09;
			p1=0x60;
		}		

	}

/************************************************************ �����_***/ 

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



/*************************************************************�f�W�^��***/


	if((mode!=25)&&(mode!=50)&&(gr > gra )&&(p9!=0xff)){			//��������֎~�G���A�Ńf�W�^���ԊO���Z���T�[���������Ă�����
		if(p9_0==0)mode=60;
		if(p9_1==0)mode=61;
		if(p9_2==0)mode=62;
		if(p9_3==0)mode=63;
		if(p9_4==0)mode=64;
		if(p9_5==0)mode=65;
		if(p9_6==0)mode=66;
	}


	if((mode!=25)&&(mode!=50)&&(gr < gra )&&(gr > gra - margin_bdline)&&(p9!=0xff)){	//�����_�G���A�Ńf�W�^���ԊO���Z���T�[���{�\���Ă�����
		if(p9_0==0)mode=70;
		if(p9_1==0)mode=71;
		if(p9_2==0)mode=72;
		if(p9_3==0)mode=73;
		if(p9_4==0)mode=74;
		if(p9_5==0)mode=75;
		if(p9_6==0)mode=76;
	}

	if((mode!=25)&&(mode!=50)&&(gr < gra - margin_bdline)&&(gr >= grc )&&(p9!=0xff)){	//�ʏ퓮��G���A�Ńf�W�^���ԊO���Z���T�[���������Ă�����
		if(p9_0==0)mode=80;
		if(p9_1==0)mode=81;
		if(p9_2==0)mode=82; 
		if(p9_3==0)mode=83;
		if(p9_4==0)mode=84;
		if(p9_5==0)mode=85;
		if(p9_6==0)mode=86;
	}

	if((mode!=25)&&(mode!=50)&&(gr < grc )&&(p9!=0xff)){			//�ҋ@�ʒu�ȉ��Ńf�W�^���ԊO���Z���T�[���������Ă�����
		if(p9_0==0)mode=90;
		if(p9_1==0)mode=91;
		if(p9_2==0)mode=92;
		if(p9_3==0)mode=93;
		if(p9_4==0)mode=94;
		if(p9_5==0)mode=95;
		if(p9_6==0)mode=96;
	}


	if((mode>=60)&&(p9==0xff))mode=0;		//�f�W�^���ԊO�����������Ă��郂�[�h�ŐԊO���Z���T�[���������Ă��Ȃ�������




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









	if((mode==63)&&(mag==0)){			//��������֎~�G���A�Ō��IR���������Ă�����
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

	


//�R��

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
		if((ball_catch_pt<gra+5)&&(irl*2 > irc+irr)){		//������ԑ傫���ꍇ��
			pwm(64,64,32,32,64);
			p2=0x09;
			p1=0x09;
		}

		if((ball_catch_pt<gra+5)&&(irc*2 > irr+irl)){		//��������ԑ傫���ꍇ��
			pwm(16,16,16,16,64);
			p2=0x09;
			p1=0x69;
		}

		if((ball_catch_pt<gra+5)&&(irr*2 > irc+irl)){		//�E����ԑ傫���ꍇ��
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



/*--------------------------�R��(�U��Ԃ��)---*/
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




/*************************************************���[�h���\��***/

	strcpy(debug_txt,"mode");lcddebug1(0x98,debug_txt,mode);
	strcpy(debug_txt,"gra");lcddebug1(0xc0,debug_txt,gra);
	strcpy(debug_txt,"grb");lcddebug1(0x00,debug_txt,grb);
	strcpy(debug_txt,"grc");lcddebug1(0x00,debug_txt,grc); 
	if(kp_check==1){
		strcpy(debug_txt,"*KP CHK MODE");lcdtext(0x80,debug_txt);
	}
	}


/**********************************************************[�f�o�b�O���[�h]***/
if(p0_3==1){

		if(timer[2]==0){		//�f�o�b�O���[�h1���[�v�ڂ̏ꍇ
	
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
			

	/******************************************** �f�o�b�O���[�h�ؑ� ***/

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
	
	/*-------------------------------------------------------------------------�ԊO�����[�h*/
	
	
	if ((deb_mode==0)&&(deb_time>movetime)){
	
			strcpy(debug_txt,"irl=");lcddebug1(0x80,debug_txt,irl);
			strcpy(debug_txt,"irr=");lcddebug1(0x8d,debug_txt,irr);
			strcpy(debug_txt,"irc=");lcddebug1(0xc6,debug_txt,irc);
	
			strcpy(debug_txt,"�޼���IR=0b");lcddebug1(0xd4,debug_txt,p9_7);
			lcdvar(0x00,p9_6);lcdvar(0x00,p9_5);lcdvar(0x00,p9_4);lcdvar(0x00,p9_3);
			lcdvar(0x00,p9_2);lcdvar(0x00,p9_1);lcdvar(0x00,p9_0);
	
			for(i=0;i<10000;i++){}
			lcdclear();
	}
	
	/*-----------------------------------------------------------------------�e��R�[�g���*/
	
	if ((deb_mode==1)&&(deb_time>movetime)){
			strcpy(debug_txt,"gr=");lcddebug1(0x80,debug_txt,gr);
			strcpy(debug_txt,"mag=");lcddebug1(0x88,debug_txt,mag);
	
			strcpy(debug_txt,"px=");lcddebug1(0xc0,debug_txt,px);
			strcpy(debug_txt,"py=");lcddebug1(0xc9,debug_txt,px);
	
			strcpy(debug_txt,"usr=");lcddebug1(0xe1,debug_txt,usonicr);
			strcpy(debug_txt,"usl=");lcddebug1(0xd4,debug_txt,usonicl);
	
			strcpy(debug_txt,"�޼���MAG=0b");lcddebug1(0x94,debug_txt,p3_7);
			lcdvar(0x00,p3_6);lcdvar(0x00,p3_5);lcdvar(0x00,p3_4);lcdvar(0x00,p3_3);
			lcdvar(0x00,p3_2);lcdvar(0x00,p3_1);lcdvar(0x00,p3_0);
	
			
		
			for(i=0;i<10000;i++){}
			lcdclear();
	}
	
	/*----------------------------------------------------------�����O���[�X�P�[���Z�b�g*/
	
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
				strcpy(lcd_text,"����?");lcdtext(0xc6,lcd_text);
				strcpy(debug_txt,"No    Yes   Cancel");lcdtext(0x95,debug_txt);
				for(;p0_7==0;){}
			}

			if(grset_mode==4){
				strcpy(lcd_text,"Black GR Set");lcdtext(0x80,lcd_text);
				strcpy(lcd_text,"����?");lcdtext(0xc6,lcd_text);
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
				strcpy(lcd_text,"�������.....");lcdtext(0xc3,lcd_text);
			
				//------------CPU�����������[�h	EW1�Ńt���b�V��ROM�֏������݂���B
	
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


/*--------------------------------------------------------------�����O���[�X�P�[���Z�b�g*/

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
				strcpy(lcd_text,"����?");lcdtext(0xc6,lcd_text);
				strcpy(debug_txt,"No    Yes   Cancel");lcdtext(0x95,debug_txt);
				for(;p0_7==0;){}
			}
	
			if(grset_mode==4){
				strcpy(lcd_text,"White GR Set");lcdtext(0x80,lcd_text);
				strcpy(lcd_text,"����?");lcdtext(0xc6,lcd_text);
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
				strcpy(lcd_text,"�������.....");lcdtext(0xc3,lcd_text);
			
				//------------CPU�����������[�h	EW1�Ńt���b�V��ROM�֏������݂���B
	
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
	
	/*----------------------------------------------------------------------�L�b�J�[�e�X�g*/
	
	if ((deb_mode==4)&&(deb_time>movetime)){

			strcpy(debug_txt,"P07-ΰ���    P06-���");lcdtext(0x80,debug_txt);
			if((p5_1==0)&&(p5_2==1)){
			strcpy(debug_txt,"      �-������      ");lcdtext(0x94,debug_txt);
			}

			if((p5_1==1)&&(p5_2==1)){
			strcpy(debug_txt,"      ΰ���ò�      ");lcdtext(0x94,debug_txt);
			}

			if(p5_3==0){
			strcpy(debug_txt,"       ����ON       ");lcdtext(0xd4,debug_txt);
			}

			if(p5_3==1){
			strcpy(debug_txt,"       ����OFF      ");lcdtext(0xd4,debug_txt);
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
				strcpy(debug_txt,"    ��ݻ޲ɾ�ò�    ");lcdtext(0x80,debug_txt);
			}

			if((timer[3]<69)&&(romfk[0]==0)&&(romfk[0]==ramfk)){
				strcpy(debug_txt,"       Forward      ");lcdtext(0x94,debug_txt);
			}

			if((timer[3]<69)&&(romfk[0]==1)&&(romfk[0]==ramfk)){
				strcpy(debug_txt,"        Keeper      ");lcdtext(0x94,debug_txt);

			}

			if((timer[3]<69)&&(romfk[0]==0)&&(ramfk==1)){
				strcpy(debug_txt,"        Keeper      ");lcdtext(0x94,debug_txt);
				strcpy(debug_txt,"        �ݺ�?       ");lcdtext(0xd4,debug_txt);

			}

			if((timer[3]<69)&&(romfk[0]==1)&&(ramfk==0)){
				strcpy(debug_txt,"       Forward      ");lcdtext(0x94,debug_txt);
				strcpy(debug_txt,"        �ݺ�?       ");lcdtext(0xd4,debug_txt);
			}



					

			if((romfk[0]!=0)&&(romfk[0]!=1)&&(timer[3]<70)){
				strcpy(debug_txt,"�װ(");lcddebug1(0x97,debug_txt,romfk[0]);
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
				strcpy(lcd_text,"����?");lcdtext(0xc6,lcd_text);
				strcpy(debug_txt,"No    Yes   Cancel");lcdtext(0x95,debug_txt);
			}

			if((p0_7==1)&&(timer[3]<=68))timer[3]=0;

			if(timer[3]>=70){

				strcpy(lcd_text,"FW/KP Set");lcdtext(0x80,lcd_text);
				strcpy(lcd_text,"����?");lcdtext(0xc6,lcd_text);
				strcpy(debug_txt,"No    Yes   Cancel");lcdtext(0x95,debug_txt);

				if(p0_6==0){
					lcdclear();			
					strcpy(lcd_text,"�������.....");lcdtext(0xc3,lcd_text);
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

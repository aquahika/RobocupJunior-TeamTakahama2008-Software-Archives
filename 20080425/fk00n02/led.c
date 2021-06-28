/****************************************************************

	TeamTakahama 2008 OAKS16-62P Programs
			For SPIDER & OCTOPUS
	Copyright Hikaru from 2008 TeamTakahama

	Foward & Keeper Hybrid Program  Template Rev1.00
			Written by Hikaru

****************************************************************/


#include "sfr62p.h"
#include "aki_lcd.h"
#include "teamtakahama.h"
#include <string.h>	

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
	unsigned short back_spd_bdline=0;		//�ҋ@�ʒu�ɖ߂�ہA�X�s�[�h��ς������ݒ�

	unsigned short grwb=0;		//���U�߁E���U�ߐݒ�ϐ�

	unsigned long timer[16];		//�ėp�\�t�g�^�C�}�[
	unsigned long deb_time=0;		//�f�o�b�O���[�h�̉�ʐ؂�ւ��^�C�}�ϐ�
	unsigned int deb_clutch=0;		//�f�o�b�O���[�h�̉�ʐ؂�ւ����A�N���b�`�H

	unsigned long grset_wr_timer=0;		//�f�o�b�O���[�h���Ƀ{�^���������Ď�
	unsigned long i=0;			//�ėp�ϐ�(�������[�v������̂ɂ͎g��Ȃ�)

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
	wh_grset=600;				//�����ݒ�I�t�Z�b�g�l

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

fkset=1;

	if(grwb==0){				//���U�ߐݒ�Ȃ�΁A�e�O���[�X�P�[���ϐ���ݒ肷��
		gra=romwr[2];
		grb=romwr[1];
		grc=romwr[0];
		
		margin_wait=5;			//���U�ߑҋ@�ʒu �ш�ݒ�
		back_spd_bdline=30;
		
	}

	if(grwb==1){				//���U�ߐݒ�Ȃ�΁A�e�O���[�X�P�[���ϐ���ݒ肷��
		gra=romwrb[2];
		grb=romwrb[1];
		grc=romwrb[0];

		margin_wait=20;			//���U�ߑҋ@�ʒu�@�ш�ݒ�
		back_spd_bdline=100;

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

		if(usonicl+35<usonicr)px=1;
		if(usonicr+35<usonicl)px=3;
		if((usonicr+35>usonicl)&&(usonicl+35>usonicr))px=2;

/*************************************************[�ʏ퓮�샂�[�h(Forward)]***/
if((p0_3==0)&&(fkset==0)){	
	p0_1=p0_7;
	lcdvar(0x80,p0_1);
}



/**************************************************[�ʏ퓮�샂�[�h(Keeper)]***/
if((p0_3==0)&&(fkset==1)){
	timer[2]=0;
	p0_1=1;

	//0�͑ҋ@���[�h

	if((mode==0)&&(mag==0)){						//�ҋ@���[�h�ŕ����Z���T�[��0�Ȃ��

		if((gr > 100 + margin_wait)&&(gr < 100 + margin_wait + back_spd_bdline)){	//�ҋ@�ʒu�����O�ɋ���ꍇ(�߂�)
			
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

		if((gr > 100 + margin_wait)&&(gr >= 100 + margin_wait + back_spd_bdline)){	//�ҋ@�ʒu�����O�ɋ���ꍇ(����)
			
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

		if((gr < 100 + margin_wait) && (gr > 100)){	//�ҋ@�ʒu���ɋ���ꍇ

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

		if(gr < 100){				//�ҋ@�ʒu�������ɋ���ꍇ

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

	if((mode==0)&&(irr+irc+irl>=400)&&(gr < 160 + 7)) mode=20;	//�ҋ@���[�h���Ƀ{�[�����������ʏ�ǂ������郂�[�h
	if((mode==20)&&(irr+irc+irl<400)) mode=0;			//�ʏ�ǂ������郂�[�h�Ń{�[��������������

	if((mode==20)&&(mag==0)){				//�ʏ�ǂ������郂�[�h�ŕ����Z���T�[��0�Ȃ��

		if(irl*2 > irc+irr){				//������ԑ傫���ꍇ��
			pwm(64,64,32,32,64);
			p2=0x09;
			p1=0x09;
		}

		if(irc*2 > irr+irl){				//��������ԑ傫���ꍇ��
			pwm(64,64,64,64,64);
			p2=0x09;
			p1=0x69;
		}

		if(irr*2 > irc+irl){				//�E����ԑ傫���ꍇ��
			pwm(64,64,32,32,64);
			p2=0x09;
			p1=0x60;
		}		

	}

	if((mode==20)&&(mag!=0))magmatch(mag,32,32);		//�ʏ�ǂ������郂�[�h�ŕ����Z���T�[��0�łȂ�������


/***************************************************************/

	if((mode==20)&&(gr > 160)) mode=21;
	if((mode==21)&&(gr < 160-20)) mode=20;
	if((mode==0)&&(irr+irc+irl > 400)&&(gr > 160)) mode=21;
	if((mode==21)&&(irr+irc+irl < 400)) mode=0;

	if((mode==21)&&(mag==0)){

		if(gr > 160){
			pwm(64,64,64,64,64);
			p2=0x06;
			p1=0x96;
		}

		if((gr < 160)&&(irl*2 > irc + irr)){
			pwm(64,64,64,64,64);
			p2=0xff;
			p1=0x99;
		}

		if((gr < 160)&&(irc*2 > irr + irl)){
			p2=0xff;
			p1=0xff;
		}

		if((gr < 160)&&(irr*2 > irc + irl)){
			pwm(64,64,64,64,64);
			p2=0xff;
			p1=0x66;
		}

	}

	if((mode==21)&&(mag!=0))magmatch(mag,32,32);





/*************************************************���[�h���\��***/

	strcpy(debug_txt,"mode");lcddebug1(0x98,debug_txt,mode);


	}


/**********************************************************[�f�o�b�O���[�h]***/
if(p0_3==1){

		if(timer[2]==0){		//�f�o�b�O���[�h1���[�v�ڂ̏ꍇ
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
			}

			if((grset_mode==1)&&(p0_6==0)){
				grset[1]=ad5;
				grset_mode=2;
				for(;p0_6==0;){}
			}

			if((grset_mode==2)&&(p0_6==0)){
				grset[2]=ad5;
				grset_mode=3;
				for(;p0_6==0;){}
			}

			if((grset_mode==3)&&(p0_6==0)){
				grset_mode=0;
				for(;p0_6==0;){}
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
			}

			if((grset_mode==1)&&(p0_6==0)){
				grset[1]=wh_grset-ad5;
				grset_mode=2;
				for(;p0_6==0;){}
			}

			if((grset_mode==2)&&(p0_6==0)){
				grset[2]=wh_grset-ad5;
				grset_mode=3;
				for(;p0_6==0;){}
			}
	
			if((grset_mode==3)&&(p0_6==0)){
				grset_mode=0;
				for(;p0_6==0;){}
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
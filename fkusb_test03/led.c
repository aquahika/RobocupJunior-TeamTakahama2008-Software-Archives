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

	unsigned char debug_txt[]="0";		//LCD�p�̕����z��
	unsigned char lcd_text[]="0";		//LCD�p�̕����z��
	unsigned short _far *romwr;		//romwr�̃|�C���^�錾
	unsigned short _far *romwrb;		//romwrb�̃|�C���^�錾
	unsigned short _far *romfk;		//romfk�̃|�C���^�錾

	unsigned int deb_mode=0;		//�f�o�b�O���[�h�ϐ�

	unsigned short px=0;			//X���W�ϐ�
	unsigned short py=0;			//Y���W�ϐ�
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
	unsigned short usonicb;		//�㒴���g�ϐ�
	unsigned short gr;			//�O���[�X�P�[���ϐ�
	unsigned short grset[3]=0;		//�f�o�b�O�p��GR�ݒ�ϐ�
	unsigned short grset_mode=0;		//�f�o�b�O�p��GR�ݒ胂�[�h�ϐ�

	unsigned short grwb=0;		//���U�߁E���U�ߐݒ�ϐ�

	unsigned long timer[16];		//�ėp�\�t�g�^�C�}�[
	unsigned long deb_time=0;		//�f�o�b�O���[�h�̉�ʐ؂�ւ��^�C�}�ϐ�
	unsigned int deb_clutch=0;		//�f�o�b�O���[�h�̉�ʐ؂�ւ����A�N���b�`�H

	unsigned long grset_wr_timer=0;		//�f�o�b�O���[�h���Ƀ{�^���������Ď�
	unsigned long i=0;			//�ėp�ϐ�(�������[�v������̂ɂ͎g��Ȃ�)

	unsigned short uss_find_back=128;	//�ҋ@�ʒu���A���ɒ����g�ŕǂ�F������l
	unsigned short uss_mj_point;		//�ҋ@�ʒu���A���ɃO���[�X�P�[���������璴���g�����ɐ؂�ւ���l

/*-----------------------------------------------------------------------------------------------------------------------------*/
	unsigned short wp_clutch=1;		//��U�A�ҋ@�ʒu�ɖ߂�����A�������ɓ����Ă��߂�Ȃ�
	unsigned short wait_position_uss=138;	//�㒴���g���莞�ҋ@�ʒu�̒����g�̒l
	unsigned short wait_position_margin=100;	//�㒴���g���莞�ҋ@�ʒu�̃}�[�W��(�]��)
	unsigned long wait_positiong_timer=1000;	//�ҋ@�ʒu�ɏꍇ��
	unsigned short uss_add_var=320;		//�����g�̍��E���v�l(����ȏ�Ő��푪��F��)
	unsigned long search_spin_timer=0;	//�����g�ُ펞�ɉ�]���Đ���l��T��
	unsigned short search_reverse_cunt=0;	//�����g�ȏ�ɉ�]���]�����񐔂��J�E���g
	unsigned short search_reverse_limit=5;	//reverse_cunt�̃��~�b�g
	unsigned long search_next_timer=0;	//search_reverse_cunt�ň��ȏ�J�E���g������Ɏ~�܂��Ă��鎞��
	unsigned short latest_wrong_mag=0;	//�Ō��Mag�C����������mag�̒l
/*-----------------------------------------------------------------------------------------------------------------------------*/
	unsigned short irfind=400;		//�ʏ펞�{�[���ԊO���F���l
	unsigned short irfind_line=600;		//�O����~�G���A�Ń{�[���ԊO���F���l
	unsigned short dis_irfind=350;		//�ʏ펞�{�[���ԊO���s�F���l(irfind > disirfind)
	unsigned short dis_irfind_line=580;	//�O����~�G���A�Ń{�[���ԊO���s�F���l(irfind_line > disirfind_line)
	unsigned short gr_stop_line;		//�O����~�G���A��GR�l
	unsigned short gr_stop_line_margin;	//�O����~�G���A��GR�l

	unsigned short gr_wait_line;
	unsigned short gr_wait_line_margin;

	unsigned short heikin_gr_count=5;		//GR�ϕ��̉�
	unsigned char heikin_rireki[5];			//GR�ϕ��̔z��
	unsigned short heikin_gr_start=0;		
	unsigned short before_gr=0;			//�ϕ��O��GR�̒l
	unsigned long add_gr=0;	


	unsigned short gr_under;

	unsigned short hold_cou;
	unsigned short hold_dis_cou;
	unsigned short hold_dis_limit=50;
	unsigned short hold_cou_limit=200;

	unsigned short grsup_on=1;		//�O���[�X�P�[���E�T�|�[�g���[�h���g���� 0=Off 1=On 
	unsigned short grsup_back_var;		//�O���[�X�P�[���E�T�|�[�g���[�h�̏ꍇ�ɑO�ɍs���l
	

	//--------------------�t�H���[�h---

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

	unsigned int vsc=0;			//�^�C�`���E�S�N�탂�[�h 0=off; 1=on;


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
	p4 =  0x00;				//�|�[�g4(LCD)�����l�ݒ�
	pd4 = PORTOUT;				//�|�[�g4�������o�͂ɐݒ�
	pd9 = PORTIN;

	movetime=15000;				//�f�o�b�O��ʈڂ�ς��^�C�}�[
//	wh_grset=600;				//�����ݒ�I�t�Z�b�g�l
	wh_grset=450;				//JoinMax JE-1111�p�I�t�Z�b�g

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

	if((grwb==0)&&(fkset==0)){				//�t�H���[�h�Ŕ��U�ߐݒ�Ȃ�΁A�e�O���[�X�P�[���ϐ���ݒ肷��
		f_gr_wait = romwr[1];		//�ҋ@�ʒu�
		f_gr_wait_margin = 30;	//�ҋ@�ʒu�]��
		f_gr_wait_speed = 30;		//�ҋ@�ʒu�ڋߌ����]��

		f_gr_hold = romwr[2];		//�O�ǐڋߐ�
	}

	if((grwb==1)&&(fkset==0)){				//�t�H���[�h�ō��U�ߐݒ�Ȃ�΁A�e�O���[�X�P�[���ϐ���ݒ肷��

		f_gr_wait = romwrb[1];	//�ҋ@�ʒu�
		f_gr_wait_margin = 15;	//�ҋ@�ʒu�]��
		f_gr_wait_speed = 25;		//�ҋ@�ʒu�ڋߌ����]��

		f_gr_hold = romwrb[2];	//�O�ǐڋߐ�

	}


	if((grwb==0)&&(fkset==1)){				//�L�[�p�[�Ŕ��U�ߐݒ�Ȃ�΁A�e�O���[�X�P�[���ϐ���ݒ肷��
		gr_stop_line = romwr[2];
		gr_wait_line = romwr[1];
		gr_under = romwr[0];
	
		uss_mj_point = gr_stop_line - 2;
		grsup_back_var=gr_under - 2;

		gr_stop_line_margin=15;
		gr_wait_line_margin=8;

	}

	if((grwb==1)&&(fkset==1)){				//�L�[�p�[���U�ߐݒ�Ȃ�΁A�e�O���[�X�P�[���ϐ���ݒ肷��
		gr_stop_line = romwrb[2];
		gr_wait_line = romwrb[1];
		gr_under = romwrb[0];
	
		uss_mj_point = gr_stop_line - 2;
		grsup_back_var=gr_under - 2;

		gr_stop_line_margin=25;
		gr_wait_line_margin=20;
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
		usonicb		=ad6;
		if(grwb==0)	before_gr=ad7;		//�����̎�gr�ɃO���[�X�P�[���̒l�����B
		if(grwb==1)	before_gr=wh_grset-ad7;	//�����̎�gr�ɃO���[�X�P�[���̒l�����B

		//if(mag==1)mag=0;
/********************************************************[GR�ϕ����W���[��]***/

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



/********************************************************[px���胂�W���[��]***/
	if(fkset == 0){			//�t�H���[�h�ݒ�Ȃ��
		if(usonicl+100<usonicr)px=1;
		if(usonicr+100<usonicl)px=3;
		if((usonicr+100>usonicl)&&(usonicl+100>usonicr))px=2;
	}

	if(fkset == 1){			//�L�[�p�[�ݒ�Ȃ��
		if(usonicl+75<usonicr)px=1;
		if(usonicr+75<usonicl)px=3;
		if((usonicr+75>usonicl)&&(usonicl+75>usonicr))px=2;
	}

/********************************************************[py���胂�W���[��]***/
/*----�h�B���ł�PY���g�p�Ȃ̂Ŗ�����
		if(gr>grc)py=3;
		if(gr>grb)py=2;
		if(gr>gra)py=1;
		if(gr<grc)py=4;
*/
/*************************************************[�ʏ퓮�샂�[�h(Forward)]***/
if((p0_3==0)&&(fkset==0)){	

	timer[2]=0;
	p0_1=1;


	if((mode==0)&&(mag==0)){
		p5_1=1;
		p5_2=1;
		p5_3=1;
		if((gr > f_gr_wait + f_gr_wait_margin + f_gr_wait_speed)&&(px == 1)){	//�ҋ@�G���A������������Ă��āA���W������������
			pwm(128,128,256,256,64);
			p2 = 0x06;
			p1 = 0xf6;
		}

		if((gr > f_gr_wait + f_gr_wait_margin + f_gr_wait_speed)&&(px == 2)){	//�ҋ@�G���A������������Ă��āA���W�����S��������
			pwm(256,256,256,256,64);
			p2 = 0x06;
			p1 = 0x96;
		}

		if((gr > f_gr_wait + f_gr_wait_margin + f_gr_wait_speed)&&(px == 3)){	//�ҋ@�G���A������������Ă��āA���W���E��������
			pwm(128,128,256,256,64);
			p2 = 0x06;
			p1 = 0x9f;
		}



		if((gr > f_gr_wait + f_gr_wait_margin )&&(gr <= f_gr_wait + f_gr_wait_margin + f_gr_wait_speed)&&(px == 1)){	//�ҋ@�G���A�����߂�����Ă��āA���W���E��������
			pwm(64,64,128,128,64);
			p2 = 0x06;
			p1 = 0x9f;
		}

		if((gr > f_gr_wait + f_gr_wait_margin )&&(gr <= f_gr_wait + f_gr_wait_margin + f_gr_wait_speed)&&(px == 2)){	//�ҋ@�G���A�����߂�����Ă��āA���W������������
			pwm(128,128,128,128,64);
			p2 = 0x06;
			p1 = 0x96;
		}

		if((gr > f_gr_wait + f_gr_wait_margin )&&(gr <= f_gr_wait + f_gr_wait_margin + f_gr_wait_speed)&&(px == 3)){	//�ҋ@�G���A�����߂�����Ă��āA���W������������
			pwm(64,64,128,128,64);
			p2 = 0x06;
			p1 = 0xf6;
		}




		if((gr > f_gr_wait)&&(gr <= f_gr_wait + f_gr_wait_margin )&&(px == 1)){	//�ҋ@�G���A�тŁA���W������������
			pwm(128,128,128,128,64);
			p2 = 0x0f;
			p1 = 0x66;
		}

		if((gr > f_gr_wait)&&(gr <= f_gr_wait + f_gr_wait_margin )&&(px == 2)){	//�ҋ@�G���A�тŁA���W�����S��������(�ҋ@�ʒu)
			p2 = 0x0f;
			p1 = 0xff;
		}

		if((gr > f_gr_wait)&&(gr <= f_gr_wait + f_gr_wait_margin )&&(px == 3)){	//�ҋ@�G���A�тŁA���W���E��������
			pwm(128,128,128,128,64);
			p2 = 0x0f;
			p1 = 0x99;
		}




		if((gr <= f_gr_wait)&&(gr > f_gr_wait - f_gr_wait_speed )&&(px == 1)){	//�ҋ@�G���A�т��߂����ŁA���W������������
			pwm(64,64,128,128,64);
			p2 = 0x09;
			p1 = 0x6f;
		}

		if((gr <= f_gr_wait)&&(gr > f_gr_wait - f_gr_wait_speed )&&(px == 2)){	//�ҋ@�G���A�т��߂����ŁA���W�����S��������
			pwm(128,128,128,128,64);
			p2 = 0x09;
			p1 = 0x69;
		}

		if((gr <= f_gr_wait)&&(gr > f_gr_wait - f_gr_wait_speed )&&(px == 3)){	//�ҋ@�G���A�т��߂����ŁA���W���E��������
			pwm(64,64,128,128,64);
			p2 = 0x09;
			p1 = 0xf9;
		}




		if((gr <= f_gr_wait - f_gr_wait_speed )&&(px == 1)){	//�ҋ@�G���A�т�艓�����ŁA���W������������
			pwm(128,128,256,256,64);
			p2 = 0x09;
			p1 = 0x6f;
		}

		if((gr <= f_gr_wait - f_gr_wait_speed )&&(px == 2)){	//�ҋ@�G���A�т��߂����ŁA���W�����S��������
			pwm(256,256,256,256,64);
			p2 = 0x09;
			p1 = 0x69;
		}

		if((gr <= f_gr_wait - f_gr_wait_speed )&&(px == 3)){	//�ҋ@�G���A�т��߂����ŁA���W���E��������
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
		

/*------------------------------------------------------------------�L�b�N---*/

	if((mode!=50)&&(ball_catch==0)&&(vsc == 0)){	//VSC���I�t��������
		mode=50;
		f_hold_cou=0;

		p5_1=0;
		p5_2=1;
		hpwm(256,64);
	}

	if((mode!=50)&&(ball_catch==0)&&(vsc == 1)&&(gr > f_gr_hold)){	//VSC���I����������
		mode=50;
		f_hold_cou=0;

		p5_1=0;
		p5_2=1;
		hpwm(256,64);
	}

	if((mode==50)&&(ball_catch==0)){
	
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

		if((px == 1)&&(gr <= f_gr_hold)){		//���W�����őO�ǂɐڋ߂��Ă��Ȃ�
			lcdvar(0xc5,3);
			pwm(64,64,64,64,64);
			p2=0x00;
			p1=0b10111011;
			for(i=0; i<120000; i++){}
		}
	
		if((px == 3)&&(gr <= f_gr_hold)){		//���W���E�őO�ǂɐڋ߂��Ă��Ȃ�
			lcdvar(0xc5,4);
			pwm(64,64,64,64,64);
			p2=0x00;
			p1=0b01110111;
			for(i=0; i<120000; i++){}
		}

		if((px == 1)&&(gr > f_gr_hold)){		//���W�����őO�ǂɐڋ߂��Ă���
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

		if((px == 3)&&(gr > f_gr_hold)){		//���W���E�őO�ǂɐڋ߂��Ă���
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





/*--------------------------------------------------------------���---*/


	strcpy(debug_txt,"mode=");lcddebug1(0x98,debug_txt,mode);
	strcpy(debug_txt,"px=");lcddebug1(0xc0,debug_txt,px);
	strcpy(debug_txt,"py=");lcddebug1(0xc5,debug_txt,py);
	strcpy(debug_txt,"WAIT=");lcddebug1(0xc5,debug_txt,f_gr_wait);
 




}
/*************************************************[�ʏ퓮�샂�[�h(Keeper)]***/
if((p0_3==0)&&(fkset == 1)){	
	timer[2]=0;
	p0_1=1;

	/*--------------------------------------------------------------------------���̈ʒu�ɖ߂�----------------*/

	if((mode==0)&&(gr>uss_mj_point)&&(mag==0)){		//(�ҋ@�ʒu���A)���݈ʒu���㒴���g�����G���A�����O
	
		if(px==1){					//�E���̏ꍇ
			pwm(128,128,256,256,64);
			p2=0x06;
			p1=0xf6;
		}		

		if(px==2){					//�����̏ꍇ
			pwm(256,256,256,256,64);
			p2=0x06;
			p1=0x96;
		}

		if(px==3){					//�����̏ꍇ
			pwm(128,128,256,256,64);
			p2=0x06;
			p1=0x9f;
		}
	}


	if((mode==0)&&(gr<=uss_mj_point)&&(mag==0)){		//(�ҋ@�ʒu���A)���݈ʒu���㒴���g�����G���A��
		if(((grsup_on == 0)&&(usonicb<wait_position_uss))||((grsup_on == 1)&&(gr < gr_wait_line ))){				//�ҋ@�ʒu������
			if((usonicr+usonicl > uss_add_var)&&(usonicr+usonicl<1000)){
				pwm(32,32,32,32,64);
				p2=0x09;
				p1=0x69;
			}

		}

		if((px==1)&&(((grsup_on == 0)&&(usonicb>wait_position_uss+wait_position_margin))||((grsup_on == 1)&&(gr >= gr_wait_line + gr_wait_line_margin )))){		//�ҋ@�ʒu�����΂ߍ��O
			if((usonicr+usonicl > uss_add_var)&&(usonicr+usonicl<1000)){
				pwm(64,64,128,128,64);
				p1=0x06;
				p2=0x06;
			}
		}

		if((px==2)&&(((grsup_on == 0)&&(usonicb>wait_position_uss+wait_position_margin))||((grsup_on == 1)&&(gr >= gr_wait_line + gr_wait_line_margin )))){		//�ҋ@�ʒu�����O
			if((usonicr+usonicl > uss_add_var)&&(usonicr+usonicl<1000)){
				pwm(64,64,64,64,64);
				p1=0x06;
				p2=0x96;
			}
		}

		if((px==3)&&(((grsup_on == 0)&&(usonicb>wait_position_uss+wait_position_margin))||((grsup_on == 1)&&(gr >= gr_wait_line + gr_wait_line_margin )))){		//�ҋ@�ʒu�����΂߉E�O
			if((usonicr+usonicl > uss_add_var)&&(usonicr+usonicl<1000)){
				pwm(64,64,128,128,64);
				p2=0x06;
				p1=0x90;
			}
		}

		if((px==1)&&(((grsup_on == 0)&&(usonicb>wait_position_uss)&&(usonicb<wait_position_uss+wait_position_margin))||((grsup_on == 1)&&(gr >= gr_wait_line)&&(gr <gr_wait_line + gr_wait_line_margin )))){	//�ҋ@�ʒu������
			if((usonicr+usonicl > uss_add_var)&&(usonicr+usonicl<1000)){
				pwm(128,128,128,128,64);
				p2=0xff;
				p1=0x66;
			}

		}

		if((px==2)&&(((grsup_on == 0)&&(usonicb>wait_position_uss)&&(usonicb<wait_position_uss+wait_position_margin))||((grsup_on == 1)&&(gr >= gr_wait_line)&&(gr <gr_wait_line + gr_wait_line_margin )))){	//�ҋ@�ʒu
			if((usonicr+usonicl > uss_add_var)&&(usonicr+usonicl<1000)){
				p2=0xff;
				p1=0xff;
			}
		}

		if((px==3)&&(((grsup_on == 0)&&(usonicb>wait_position_uss)&&(usonicb<wait_position_uss+wait_position_margin))||((grsup_on == 1)&&(gr >= gr_wait_line)&&(gr <gr_wait_line + gr_wait_line_margin )))){	//�ҋ@�ʒu�����E

			if((usonicr+usonicl > uss_add_var)&&(usonicr+usonicl<1000)){
				pwm(128,128,128,128,64);
				p2=0xff;
				p1=0x99;
			}
		
		}			

		if(gr<uss_mj_point){						//�ҋ@�ʒu�������͌��
			if((usonicr+usonicl < uss_add_var)||(usonicr+usonicl > 1000)){	//�����g�Z���T�[���ُ�
				if((search_spin_timer < 200)&&(search_reverse_cunt<search_reverse_limit)){		//�ǌ�����]�������]�^�C�}�[��X�ȉ�
					pwm(16,16,16,16,128);
						if(latest_wrong_mag == 1){	//�Ō��magmatch��������mag��1
							pwm(16,16,16,16,128);
							p2=0x55;
							p1=0x55;
						}
						if(latest_wrong_mag == 15){	//�Ō��magmatch��������mag��15
							pwm(16,16,16,16,128);
							p2=0xAA;
							p1=0xAA;
						}
						if(latest_wrong_mag == 0){	//��x��magmatch��������������
							pwm(16,16,16,16,128);
							p2=0xAA;
							p1=0xAA;
						}
					search_spin_timer++;
				}

				if((search_spin_timer >= 200)&&(search_reverse_cunt<search_reverse_limit)){		//�ǌ�����]�������]�^�C�}�[��X�ȏ�
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

/*---------------------------------------------------------------------------------(�A�i���O)�{�[����ǂ�������----*/


	if((mode == 0)&&(irr+irl+irc > irfind)&&(gr < gr_stop_line))mode=20;	//�ʏ퓮��G���A���Ń{�[���������{�[���ǐՃ��[�h
	if((mode == 20)&&(irr+irl+irc < dis_irfind))mode=0;
	
	if((mode == 20)&&(mag == 0)&&(irr*2 > irl+irc)){		//�{�[���ǐՃ��[�h�ŉE����ԑ傫��������
		pwm(128,128,256,256,64);
		p2=0x09;
		p1=0x6f;
	}

	if((mode == 20)&&(mag == 0)&&(irc*2 > irr+irl)){		//�{�[���ǐՃ��[�h�Œ�����ԑ傫��������
		pwm(128,128,128,128,64);
		p2=0x09;
		p1=0x69;
	}

	if((mode == 20)&&(mag == 0)&&(irl*2 > irr+irc)){		//�{�[���ǐՃ��[�h�ō�����ԑ傫��������
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

	if((mode == 20)&&(mag != 0))magmatch(mag,32,32);		//�{�[���ǐՃ��[�h��mag��0�Ŗ���������




/*-----------------------------------------------------------------------------------------------�O����~�G���A----*/

	if((mode == 20)&&(gr > gr_stop_line))mode=29;
	if((mode == 0)&&(gr > gr_stop_line)&&(irr+irc+irl > irfind_line))mode=29;
	if((mode == 29)&&(gr < gr_stop_line-gr_stop_line_margin))mode=20;
	if((mode == 29)&&(irr+irc+irl < dis_irfind_line))mode=0;

	if((mode == 29)&&(mag == 0)&&(gr <= gr_stop_line)&&(irr*2 > irl+irc)){		//�O����~�G���A�ŉE����ԑ傫��������
		if((irc > 700)&&(irr > irc)){					//�������{�[�����E�̉����̕��Ȃ�
			pwm(64,64,64,64,128);
		}else{
			pwm(200,200,200,200,64);
		}
		p2=0xff;
		p1=0x66;
	}

	if((mode == 29)&&(mag == 0)&&(gr <= gr_stop_line)&&(irc*2 > irr+irl)){		//�O����~�G���A�Œ�����ԑ傫��������
		p2=0xff;
		p1=0xff;
	}

	if((mode == 29)&&(mag == 0)&&(gr <= gr_stop_line)&&(irl*2 > irr+irc)){		//�O����~�G���A�ō�����ԑ傫��������
		if((irc > 700)&&(irl > irc)){					//�������{�[�������̉����̕��Ȃ�
			pwm(64,64,64,64,128);
		}else{
			pwm(200,200,200,200,64);
		}
		p2=0xff;
		p1=0x99;
	}

	if((mode == 29)&&(mag == 0)&&(gr > gr_stop_line)&&(irr*2 > irl+irc)){		//�O����~�G���A���O�ŉE����ԑ傫��������
		if((irc > 700)&&(irr > irc)){					//�������{�[�����E�̉����̕��Ȃ�
			pwm(32,32,64,64,128);
		}else{
			 pwm(100,100,200,200,64);
		}
		p2=0x06;
		p1=0x90;
	}

	if((mode == 29)&&(mag == 0)&&(gr > gr_stop_line)&&(irc*2 > irr+irl)){		//�O����~�G���A���O�Œ�����ԑ傫��������
		pwm(32,32,32,32,128);
		p2=0x06;
		p1=0x96;
	}

	if((mode == 29)&&(mag == 0)&&(gr > gr_stop_line)&&(irl*2 > irr+irc)){		//�O����~�G���A���O�ō�����ԑ傫��������
		if((irc > 700)&&(irl > irc)){					//�������{�[�������̉����̕��Ȃ�
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


/*-------------------------------------------------------------------------------------��荞�݃��[�h---*/
	strcpy(debug_txt,"mode=");lcddebug1(0x98,debug_txt,mode);
	if((mode == 0 )&&((p9 != 0xFF)||(p8_1 == 0)||(p8_2 == 0))) mode = 90;				//�������f�W�^��IR���������Ă������荞��
	if((mode == 90)&&(p9 == 0xFF)&&(p8_1 == 1)&&(p8_2 == 1)) mode = 0;				//��������荞�݃��[�h�Ńf�W�^�����������Ă��Ȃ�������

	if((mode == 90)&&(mag == 0)&&(gr >= gr_stop_line)){
		pwm(256,256,256,256,64);
		p2=0x06;
		p1=0x96;
	}

	if((mode == 90)&&(mag == 0)&&(gr > gr_stop_line - gr_stop_line_margin)&&(gr < gr_stop_line)){	//��L�O����~�G���A���Ńf�W�^��IR����������
		lcdvar(0xda,1);

		if((p9_0 == 0)&&(p9_1 == 1)){					//�E�΂ߑO������
			pwm(256,256,256,256,64);
			p2=0xff;
			p1=0x66;
		}

		if(p9_1 == 0){					//�E�^��������
			pwm(128,128,256,256,64);
			p2=0x06;
			p1=0xf6;
		}

		if((p9_2 == 0)&&(p9_1 == 1)){					//�E�΂ߌ�낪����
			pwm(256,256,256,256,64);
			p2=0x06;
			p1=0x96;
		}

		if(p9_3 == 0){					//�^��낪����

			if(px == 1){				//����X���W��1�Ȃ��
				pwm(128,128,256,256,64);
				p2=0x06;
				p1=0x06;
			}
			if(px == 2){				//����X���W��2�Ȃ��
				pwm(128,128,256,256,64);
				p2=0x06;
				p1=0x06;
			}
			if(px == 3){				//����X���W��3�Ȃ��
				pwm(128,128,256,256,64);
				p2=0x06;
				p1=0x90;
			}
		}
		

		if((p9_4 == 0)&&(p9_5 == 1)){					//���΂ߌ�낪����
			pwm(256,256,256,256,64);
			p2=0x06;
			p1=0x96;
		}				

		if(p9_5 == 0){					//���^��������
			pwm(128,128,256,256,64);
			p2=0x06;
			p1=0x9f;
		}

		if((p9_6 == 0)&&(p9_5 == 1)){					//���΂ߑO������
			pwm(256,256,256,256,64);
			p2=0xff;
			p1=0x99;
		}


	}



	if((mode == 90)&&(mag == 0)&&(gr <= gr_stop_line - gr_stop_line_margin )&&(((grsup_on == 1)&&(gr > gr_under))||((grsup_on == 0)&&(usonicb >= 75)))){		//�������O����~�G���A�����

		lcdvar(0xda,2);
		if(p9_0 == 0){					//�E�΂ߑO������
			if(irr > irfind) pwm(128,128,128,128,64);
			else pwm(256,256,256,256,64);
			p2=0xff;
			p1=0x66;
		}

		if(p9_1 == 0){					//�E�^��������
			pwm(128,128,256,256,64);
			p2=0x06;
			p1=0x06;
		}

		if(p9_2 == 0){					//�E�΂ߌ�낪����
			pwm(256,256,256,256,64);
			p2=0x06;
			p1=0x96;
		}

		if(p9_3 == 0){					//�^��낪����

			if(px == 1){				//����X���W��1�Ȃ��
				pwm(128,128,256,256,64);
				p2=0x06;
				p1=0x06;
			}
			if(px == 2){				//����X���W��2�Ȃ��
				pwm(128,128,256,256,64);
				p2=0x06;
				p1=0x06;
			}
			if(px == 3){				//����X���W��3�Ȃ��
				pwm(128,128,256,256,64);
				p2=0x06;
				p1=0x90;
			}
		}
		

		if(p9_4 == 0){					//���΂ߌ�낪����
			pwm(256,256,256,256,64);
			p2=0x06;
			p1=0x96;
		}				

		if(p9_5 == 0){					//���^��������
			pwm(128,128,256,256,64);
			p2=0x06;
			p1=0x90;
		}

		if(p9_6 == 0){					//���΂ߑO������
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

	if((mode == 90)&&(mag == 0)&&(gr <= gr_stop_line - gr_stop_line_margin )&&(((grsup_on == 1)&&(gr <= gr_under))||((grsup_on == 0)&&(usonicb < 75)))){			//�������ɕǂ�����Ȃ��
		lcdvar(0xda,3);

		if(p9_0 == 0){					//�E�΂ߑO������
			pwm(256,256,256,256,64);
			p2=0xff;
			p1=0x66;
		}

		if(p9_1 == 0){					//�E�^��������
			pwm(256,256,256,256,64);
			p2=0xff;
			p1=0x66;
		}

		if(p9_2 == 0){					//�E�΂ߌ�낪����
			pwm(256,256,256,256,64);
			p2=0xff;
			p1=0x66;
		}

		if(p9_3 == 0){					//�^��낪����
			pwm(32,32,32,32,64);				//����Ď��ɂ���
			p2=0x06;
			p1=0x96;
		}
		

		if(p9_4 == 0){					//���΂ߌ�낪����
			pwm(256,256,256,256,64);
			p2=0xff;
			p1=0x99;
		}				

		if(p9_5 == 0){					//���^��������
			pwm(256,256,256,256,64);
			p2=0xff;
			p1=0x99;
		}

		if(p9_6 == 0){					//���΂ߑO������
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


/*--------------------------------------------------------------------------------------[�z�[���h & �L�b�N]----*/


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
		p5_1=0;		//�z�[���_�[��~
		p5_2=0;		//�z�[���_�[��~

		for(i=0;i<10000;i++){}

		pwm(128,128,128,128,200);
		p2=0x09;		//�I���j�O�i
		p1=0x69;		//�I���j�O�i

		for(i=0;i<20000;i++){}

		p5_3=0;		//�R��

		for(i=0;i<300000;i++){}

		p2=0xff;		//�I���j��~
		p1=0xff;		//�I���j��~

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

			strcpy(debug_txt,"usl=");lcddebug1(0xd4,debug_txt,usonicl);	
			lcdvar(0xdc,usonicb);
			strcpy(debug_txt,"usr=");lcddebug1(0xe0,debug_txt,usonicr);

	
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


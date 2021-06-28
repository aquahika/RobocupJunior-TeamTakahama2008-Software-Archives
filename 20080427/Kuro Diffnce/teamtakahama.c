/********************************************************************************************************************************************

TeamTakahama OAKS16 Main moving unit module.

File name: teamtakahama.c

���e:
�o�v�l�̏����ݒ�@�o�v�l�̑��x�ρ@�`�c�̏����ݒ�
�l�`�f�̎擾�A�l�`�f�̂O���킹

Date:2008-01-02

Build by Hikaru from TeamTakahama

*********************************************************************************************************************************************/

#include <stdio.h>
#include "teamtakahama.h"
#include "aki_lcd.h"
#include "sfr62p.h"


/*====================================================================================
init_moter�֐��@PWM/8bit MD�̏����ݒ�
�g����	init_moter();
=====================================================================================*/
void init_moter(void){
	ta0mr=0x27;		//�^�C�}�[A1�̐ݒ�
	ta0=62;			//�^�C�}�[A1�̎����E�f���[�e�B�[�̐ݒ�
	ta1mr=0x27;		//�^�C�}�[A1�̐ݒ�
	ta1=62;			//�^�C�}�[A1�̎����E�f���[�e�B�[�̐ݒ�
	ta2mr=0x27;		//�^�C�}�[A2�̐ݒ�
	ta2=62;			//�^�C�}�[A2�̎����E�f���[�e�B�[�̐ݒ�
	ta3mr=0x27;		//�^�C�}�[A3�̐ݒ�
	ta3=62;			//�^�C�}�[A3�̎����E�f���[�e�B�[�̐ݒ�
	ta4mr=0x27;		//�^�C�}�[A4�̐ݒ�
	ta4=62;			//�^�C�}�[A4�̎����E�f���[�e�B�[�̐ݒ�
	tabsr=0x00;		//�S�^�C�}�[�ɒ�~�t���O���o���B
	ta0=0xb040;
	ta1=0xb040;		//�^�C�}�[A1�̎�����ݒ�(2byte)
	ta2=0xb040;
	ta3=0xb040;
	ta4=0xb040;
	tabsr=0b00011111;	//�^�C�}�[A1~A4�̊J�n�t���O�𗧂Ă�
	pd1=0xff;		//���[�^�[�o�̓|�[�g���o�͂ɐݒ肷��B
}


/*====================================================================================
ir_init�֐��@ANALOG IR�̏����ݒ�
�g����	init_ir();
=====================================================================================*/
void init_ir(void){
	pd10=0x00;
	adcon2=0x01;
	adcon0=0b10011000;
	adcon1=0b00101011;
	adcon0=0b11011000;	
}


/*====================================================================================
�o�v�l�֐��@TA1�`TA4��PWM�̒l��ݒ�
�g����	
pwm(TA1�̃f���[�e�B��,TA2�̃f���[�e�B��,TA3�̃f���[�e�B��,TA4�̃f���[�e�B��,CYCLE);
=====================================================================================*/
void pwm(int pwm1,int pwm2,int pwm3, int pwm4,int cycle){
	ta1=cycle+pwm1*225;
	ta2=cycle+pwm2*225;
	ta3=cycle+pwm3*225;
	ta4=cycle+pwm4*225;

	return;
}

/*====================================================================================
HPWM�֐��@TA0��PWM�̒l��ݒ�
�g����	
hpwm(TA0�̃f���[�e�B��,CYCLE);
=====================================================================================*/
void hpwm(int pwm1,int cycle){
	ta0=cycle+pwm1*225;
	return;
}



char magget(){
char mag_a;

switch(p3){ 
	case 0b11111111:
		mag_a=0;
		break;
	case 0b11011101:
		mag_a=0;
		break;
	case 0b11011111:
		mag_a=1;
		break;
	case 0b11111011:
		mag_a=1;
		break;
	case 0b11011011:
		mag_a=2;
		break;
	case 0b11011010:
		mag_a=3;
		break;
	case 0b11010010:
		mag_a=4;
		break;
	case 0b11010110:
		mag_a=5;
		break;
	case 0b11110110:
		mag_a=6;
		break;
	case 0b11010100:
		mag_a=6;
		break;
	case 0b11110100:
		mag_a=7;
		break;
	case 0b11100100:
		mag_a=8;
		break;
	case 0b11110000:
		mag_a=8;
		break;
	case 0b11100000:
		mag_a=9;
		break;
	case 0b11010000:
		mag_a=9;
		break;
	case 0b11010001:
		mag_a=10;
		break;
	case 0b11000000:
		mag_a=10;
		break;
	case 0b11000001:
		mag_a=11;
		break;
	case 0b11001001:
		mag_a=12;
		break;
	case 0b11001101:
		mag_a=13;
		break;
	case 0b11101101:
		mag_a=14;
		break;
	case 0b11011001:
		mag_a=14;
		break;
	case 0b11101111:
		mag_a=15;
		break;
	case 0b11111101:
		mag_a=15;
		break;
	default:
		mag_a=16;
}

return mag_a;
}


void magmatch(unsigned short mag_s,char s1,char s2){
	if((mag_s>7)&&(mag_s||0)&&(mag_s||16)){
	pwm(s1,s1,s1,s1,64);
	if(mag_s==15)pwm(s2,s2,s2,s2,64);
	p2=0b10101010;
	p1=0b10101010;
	}

	if((mag_s<=7)&&(mag_s||0)){
	if(mag_s==1)pwm(s2,s2,s2,s2,64);
	p2=0b1010101;
	p1=0b1010101;
	}
return;
}
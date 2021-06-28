	
/***********************************************************************************************
LCD�\���֌W�\�[�X�t�@�C��


***********************************************************************************************/
#include <stdio.h>
#include <string.h>
#include "aki_lcd.h"
#include "sfr62p.h"

/*--------------------------------------------------------------*/
/* �֐���	:lcd_init()					*/
/* �@�\		:LCD�����ݒ�					*/
/*--------------------------------------------------------------*/

void lcd_init(void)
{
	wait3();						/*15ms�E�G�C�g*/
	lcdwriteinit( 0x03 );			/*LCD�t�@���N�V�����Z�b�g*/
	wait2();						/*4.1ms�E�G�C�g*/
	lcdwriteinit( 0x03 );			/*LCD�t�@���N�V�����Z�b�g*/
	wait1();						/*0.1ms�E�G�C�g*/
	lcdwriteinit( 0x03 );			/*LCD�t�@���N�V�����Z�b�g*/
	wait1();
	lcdwriteinit( 0x02 );			/*LCD�f�[�^��4�r�b�g���ɐݒ�*/
	wait1();
	lcdwrite1command(0x28);			/*4bit�2�s���5�~7�h�b�g�ɐݒ�*/
	wait1();

	/* 	�����܂ł�4�r�b�g�~2��̃f�[�^�]�����ł���悤�ɐݒ肳�ꂽ�̂�
	���̌�̓r�W�[�`�F�b�N�����Ă���f�[�^�]�����s�Ȃ��B */
	
	while(BUSY == lcdbusycheck())	/*LCD�r�W�[�`�F�b�N*/
	;
	lcdwrite1command(0x08);			/*�\���I�t*/

	while(BUSY == lcdbusycheck())	/*LCD�r�W�[�`�F�b�N*/
	;
	lcdwrite1command(0x01);			/*�\���N���A*/

	while(BUSY == lcdbusycheck())	/*LCD�r�W�[�`�F�b�N*/
	;
	lcdwrite1command(0x06);			/*�G���g���[���[�h��C���N�������g*/

	while(BUSY == lcdbusycheck())	/*LCD�r�W�[�`�F�b�N*/
	;
	lcdwrite1command(0x0c);			/*�\���I����J�[�\���I�t*/

}
/*----------------------------------------------------------*/
/* �֐���	:lcdwriteinit()									*/
/* �@�\		:LCD�����ݒ�R�}���h�Z�b�g					    */
/*----------------------------------------------------------*/

void lcdwriteinit( unsigned char command )
{

	p4 = 0x00;				/*P4�����l�Z�b�g�@RW�|0(W�w��) RS�|0(�R�}���h�w��jE�|0 */
	pd4 = PORTOUT;			/*P4�o�͂ɐݒ� */

	command &= 0x0f;		/*P4�R���g���[���f�[�^�Z�b�g(��������jRW�|0(W�w��) RS�|0(�R�}���h�w��jE�|0 */
	p4 = command;			/*P4�R���g���[���f�[�^�o��*/
	LCDE = HIGH;			/*E�|�P*/
#pragma ASM					/*�A�Z���u���\�L*/
	NOP						/* ���Ԓ����ׂ̈�NOP */
	NOP
	NOP
	NOP
#pragma ENDASM				/*�A�Z���u���\�L�I��*/
	LCDE = LOW;				/*E�|0*/
}
/*----------------------------------------------------------*/
/* �֐���	:lcdwrite1command(�j							*/
/* �@�\		:LCD�R�}���h�o��							    */
/*----------------------------------------------------------*/

void lcdwrite1command( unsigned char command )
{
	unsigned char outcommand;

	p4 = 0x00;				/*P4�����l�Z�b�g�@RW�|0(W�w��) RS�|0(�R�}���h�w��jE�|0 */
	pd4 = PORTOUT;			/*P4�o�͂ɐݒ� */

	outcommand = command>>4;/*���4�r�b�g������4�r�b�g��*/
	outcommand &= 0x0f;		/*�}�X�N����*/
	p4 = outcommand;		/*�R�}���h�f�[�^���4�r�b�g���o�́@RW�|0(W�w��) RS�|0(�R�}���h�w��jE�|0 */

	LCDE = HIGH;			/*E�|�P*/
#pragma ASM					/*�A�Z���u���\�L*/
	NOP						/*���Ԓ����ׂ̈�NOP*/
	NOP
	NOP
	NOP
#pragma ENDASM				/*�A�Z���u���\�L�I��*/
	LCDE = LOW;				/*E�|0*/
	
	outcommand = command&0x0f;	/*�R�}���h�f�[�^�̉���4�r�b�g���o(���4�r�b�g�}�X�N)*/
	p4 = outcommand;			/*�R�}���h�f�[�^����4�r�b�g���o��*/

	LCDE = HIGH;			/*E�|�P*/
#pragma ASM					/*�A�Z���u���\�L*/
	NOP						/*���Ԓ����ׂ̈�NOP*/
	NOP
	NOP
	NOP
#pragma ENDASM					/*�A�Z���u���\�L�I��*/
	LCDE = LOW;					/*E�|0*/

}
/*----------------------------------------------------------*/
/* �֐���	:lcdwrite1data(�j								*/
/* �@�\		:LCD�f�[�^�o��								    */
/*----------------------------------------------------------*/

void	lcdwrite1data( unsigned char data )
{
	unsigned char lcddata;
	p4 = 0x00;				/*P4�����l�Z�b�g�@RW�|0(W�w��) RS�|0(�R�}���h�w��jE�|0 */
	pd4 = PORTOUT;			/*P4�o�͂ɐݒ� */

	lcddata = data>>4;		/*LCD�f�[�^���4�r�b�g������4�r�b�g��*/
	lcddata &= 0x0f;		/*�}�X�N����*/
	p4 = lcddata;			/*LCD�f�[�^���4�r�b�g���o��*/
	LCDRS = LCD_DATA;		/*RS�|1(�f�[�^�w��j*/
	LCDE = HIGH;			/*E�|�P*/
#pragma ASM					/*�A�Z���u���\�L*/
	NOP						/* ���Ԓ����ׂ̈�NOP */
	NOP
	NOP
	NOP
#pragma ENDASM				/*�A�Z���u���\�L�I��*/
	LCDE = LOW;				/*E�|0*/
	
	lcddata =data & 0x0f;	/*LCD�f�[�^�̉���4�r�b�g���o(���4�r�b�g�}�X�N)*/
	p4 = lcddata;			/*LCD�f�[�^����4�r�b�g���o��*/

	LCDRS = LCD_DATA;		/*RS�|1(�f�[�^�w��j*/
	LCDE = HIGH;			/*E�|1*/
#pragma ASM					/*�A�Z���u���\�L*/
	NOP						/* ���Ԓ����ׂ̈�NOP */
	NOP
	NOP
	NOP
#pragma ENDASM				/*�A�Z���u���\�L�I��*/
	LCDE = LOW;				/*E�|0*/
}

/*----------------------------------------------------------*/
/* �֐���	:wait1()										*/
/* �@�\		:0.1ms�E�G�C�g								    */
/*----------------------------------------------------------*/
void wait1(void){			/* ��0.1ms�̃E�G�C�g */
#pragma ASM					/*�A�Z���u���\�L*/
	MOV.W 	#0C8H,A0		/*�J�E���^�����l�Z�b�g*/
LOOP1:
	NOP
	NOP
	NOP
	DEC.W	A0
	JNZ	LOOP1				/*���[�v�I���H*/
#pragma ENDASM				/*�A�Z���u���\�L�I��*/
}
/*----------------------------------------------------------*/
/* �֐���	:wait2()										*/
/* �@�\		:4.1ms�E�G�C�g								    */
/*----------------------------------------------------------*/
void wait2(void){			/* ��4.1ms�̃E�G�C�g */
#pragma ASM					/*�A�Z���u���\�L*/
	MOV.W 	#2007H,A0		/*�J�E���^�����l�Z�b�g*/
LOOP2:
	NOP
	NOP
	NOP
	DEC.W	A0
	JNZ	LOOP2				/*���[�v�I���H*/
#pragma ENDASM				/*�A�Z���u���\�L�I��*/
}
/*----------------------------------------------------------*/
/* �֐���	:wait3()										*/
/* �@�\		:15ms�E�G�C�g									*/
/*----------------------------------------------------------*/
void wait3(void){			/* ��15ms�̃E�G�C�g */
#pragma ASM					/*�A�Z���u���\�L*/
	MOV.W 	#7530H,A0		/*�J�E���^�����l�Z�b�g*/
LOOP3:
	NOP
	NOP
	NOP
	DEC.W	A0
	JNZ	LOOP3				/*���[�v�I���H*/
#pragma ENDASM				/*�A�Z���u���\�L�I��*/
}
/*----------------------------------------------------------*/
/* �֐���	:lcdbusycheck()									*/
/* �@�\		:LCD�r�W�[�`�F�b�N							    */
/*----------------------------------------------------------*/

unsigned char lcdbusycheck( void )
{
	unsigned char command_high,command_low,b_data;

	p4 = 0x00;				/*P4�����l�Z�b�g�@RW�|0(W�w��) RS�|0(�R�}���h�w��jE�|0 */
	pd4 = PORTOUTIN;		/*�|�[�g�������W�X�^�����4�r�b�g���o�͉���4�r�b�g����͂ɐݒ�*/

	LCDRW = HIGH;			/*RW�|1(R�w��)*/

	LCDE = HIGH;			/*E�|1*/
#pragma ASM					/*�A�Z���u���\�L*/
	NOP						/* ���Ԓ����ׂ̈�NOP */
	NOP
	NOP
	NOP
#pragma ENDASM				/*�A�Z���u���\�L�I��*/
	command_high = p4;		/*�R�}���h���4�r�b�g�ǂݍ���*/
	LCDE = LOW;				/*E�|0*/

	command_high <<=4;		/*����4�r�b�g����ʂ�*/
	command_high &= 0xf0;	/*���4�r�b�g���o(����4�r�b�g�}�X�N����)*/

	LCDE = HIGH;			/*E�|1*/
#pragma ASM					/*�A�Z���u���\�L*/
	NOP						/* ���Ԓ����ׂ̈�NOP */
	NOP
	NOP
	NOP
#pragma ENDASM				/*�A�Z���u���\�L�I��*/
	command_low = p4;		/*�R�}���h����4�r�b�g�ǂݍ���*/
	LCDE = LOW;				/*E�|0*/

	command_low &= 0x0f;	/*����4�r�b�g���o(���4�r�b�g�}�X�N����)*/
	b_data = command_high|command_low;	/* 2��4�r�b�g�R�}���h�f�[�^��8�r�b�g�ɂ܂Ƃ߂�*/
	b_data &= 0x80;			/*�r�b�g7�̃}�X�N����*/
	if(b_data==0)
		b_data = NOBUSY;	/*�r�b�g7��0�Ȃ�LCD��������ok��Ԃ�*/
	else
		b_data = BUSY;		/*�r�b�g7��1�Ȃ�LCD��������buzy��Ԃ�*/
		
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



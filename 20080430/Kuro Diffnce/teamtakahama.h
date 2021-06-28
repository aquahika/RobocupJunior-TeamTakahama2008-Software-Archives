void init_moter(void);
void init_ir(void);
void pwm(int pwm1,int pwm2,int pwm3, int pwm4,int cycle);
void hpwm(int pwm1,int cycle);
void _main(void);
void magmatch(unsigned short mag_s,char s1,char s2);
char magget(void);



#define LED_on		0xfe
#define LED_off		0xff
#define md		p1
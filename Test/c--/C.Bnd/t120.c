static int x;
static int y;
static int x=1;
static int a=5;
static int b=3;
static int c=2;
static int f(int _cmm_1P,char *_cmm_2P);
static int f(int _cmm_1P,char *_cmm_2P);
static int f(int _cmm_1P,char *_cmm_2P){
return (_cmm_1P=b);
}
static int f(int _cmm_1P,char *_cmm_2P);
#define _cmm_0S "abc\r\n"
int main(){
(a=x);
(b=y);
{
int _cmm_1L;
(_cmm_1L=a);
for(;(_cmm_1L<b);(_cmm_1L=(_cmm_1L+c)))
(c=((a+b)+f((_cmm_1L*_cmm_1L),_cmm_0S)));
}
return 0;
}

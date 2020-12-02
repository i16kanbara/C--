char *strCpy(char *_cmm_1P,char *_cmm_2P);
char *strNcpy(char *_cmm_1P,char *_cmm_2P,int _cmm_3P);
char *strCat(char *_cmm_1P,char *_cmm_2P);
char *strNcat(char *_cmm_1P,char *_cmm_2P,int _cmm_3P);
int strCmp(char *_cmm_1P,char *_cmm_2P);
int strNcmp(char *_cmm_1P,char *_cmm_2P,int _cmm_3P);
int strLen(char *_cmm_1P);
int strChr(char *_cmm_1P,char _cmm_2P);
int strRchr(char *_cmm_1P,char _cmm_2P);
int strStr(char *_cmm_1P,char *_cmm_2P);
char *strCpy(char *_cmm_1P,char *_cmm_2P){
{
int _cmm_1L;
(_cmm_1L=0);
for(;((_cmm_1P[_cmm_1L]=_cmm_2P[_cmm_1L])!=0);(_cmm_1L=(_cmm_1L+1)))
;
}
return _cmm_1P;
}
char *strNcpy(char *_cmm_1P,char *_cmm_2P,int _cmm_3P){
int _cmm_1L;
(_cmm_1L=0);
{
;
for(;((_cmm_1L<(_cmm_3P+-1))&&(_cmm_2P[_cmm_1L]!=0));(_cmm_1L=(_cmm_1L+1)))
(_cmm_1P[_cmm_1L]=_cmm_2P[_cmm_1L]);
}
{
;
for(;(_cmm_1L<_cmm_3P);(_cmm_1L=(_cmm_1L+1)))
(_cmm_1P[_cmm_1L]=0);
}
return _cmm_1P;
}
char *strCat(char *_cmm_1P,char *_cmm_2P){
int _cmm_1L;
(_cmm_1L=0);
for(;(_cmm_1P[_cmm_1L]!=0);)
(_cmm_1L=(_cmm_1L+1));
{
int _cmm_2L;
(_cmm_2L=_cmm_1L);
for(;((_cmm_1P[_cmm_1L]=_cmm_2P[(_cmm_1L-_cmm_2L)])!=0);(_cmm_1L=(_cmm_1L+1)))
;
}
return _cmm_1P;
}
char *strNcat(char *_cmm_1P,char *_cmm_2P,int _cmm_3P){
int _cmm_1L;
(_cmm_1L=0);
for(;(_cmm_1P[_cmm_1L]!=0);)
(_cmm_1L=(_cmm_1L+1));
{
int _cmm_2L;
(_cmm_2L=0);
for(;((_cmm_2L<(_cmm_3P+-1))&&(_cmm_2P[_cmm_2L]!=0));((_cmm_1L=(_cmm_1L+1)),(_cmm_2L=(_cmm_2L+1))))
(_cmm_1P[_cmm_1L]=_cmm_2P[_cmm_2L]);
}
(_cmm_1P[_cmm_1L]=0);
return _cmm_1P;
}
int strCmp(char *_cmm_1P,char *_cmm_2P){
int _cmm_1L;
{
(_cmm_1L=0);
for(;(_cmm_1P[_cmm_1L]==_cmm_2P[_cmm_1L]);(_cmm_1L=(_cmm_1L+1)))
{if((_cmm_1P[_cmm_1L]==0))return 0;
}}
return (_cmm_1P[_cmm_1L]-_cmm_2P[_cmm_1L]);
}
int strNcmp(char *_cmm_1P,char *_cmm_2P,int _cmm_3P){
int _cmm_1L;
{
(_cmm_1L=0);
for(;((_cmm_1L<_cmm_3P)&&(_cmm_1P[_cmm_1L]==_cmm_2P[_cmm_1L]));(_cmm_1L=(_cmm_1L+1)))
{if((_cmm_1P[_cmm_1L]==0))return 0;
}}
{if((_cmm_1L>=_cmm_3P))return 0;
}return (_cmm_1P[_cmm_1L]-_cmm_2P[_cmm_1L]);
}
int strLen(char *_cmm_1P){
int _cmm_1L;
(_cmm_1L=0);
for(;(_cmm_1P[_cmm_1L]!=0);)
(_cmm_1L=(_cmm_1L+1));
return _cmm_1L;
}
int strChr(char *_cmm_1P,char _cmm_2P){
int _cmm_1L;
{
(_cmm_1L=0);
for(;(_cmm_1P[_cmm_1L]!=0);(_cmm_1L=(_cmm_1L+1)))
{if((_cmm_1P[_cmm_1L]==_cmm_2P))return _cmm_1L;
}}
return -1;
}
int strRchr(char *_cmm_1P,char _cmm_2P){
int _cmm_1L;
int _cmm_2L;
(_cmm_2L=-1);
{
(_cmm_1L=0);
for(;(_cmm_1P[_cmm_1L]!=0);(_cmm_1L=(_cmm_1L+1)))
{if((_cmm_1P[_cmm_1L]==_cmm_2P))(_cmm_2L=_cmm_1L);
}}
return _cmm_2L;
}
int strStr(char *_cmm_1P,char *_cmm_2P){
int _cmm_1L;
(_cmm_1L=strLen(_cmm_2P));
int _cmm_2L;
(_cmm_2L=(strLen(_cmm_1P)-_cmm_1L));
{
int _cmm_3L;
(_cmm_3L=0);
for(;(_cmm_3L<_cmm_2L);(_cmm_3L=(_cmm_3L+1)))
{
int _cmm_4L;
{
(_cmm_4L=0);
for(;(_cmm_4L<_cmm_1L);(_cmm_4L=(_cmm_4L+1)))
{if((_cmm_1P[(_cmm_3L+_cmm_4L)]!=_cmm_2P[_cmm_4L]))break;
}}
{if((_cmm_4L==_cmm_1L))return _cmm_3L;
}}
}
return -1;
}

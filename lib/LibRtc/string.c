_CA *strCpy(_CA *_cmm_1P,_CA *_cmm_2P);
_CA *strNcpy(_CA *_cmm_1P,_CA *_cmm_2P,int _cmm_3P);
_CA *strCat(_CA *_cmm_1P,_CA *_cmm_2P);
_CA *strNcat(_CA *_cmm_1P,_CA *_cmm_2P,int _cmm_3P);
int strCmp(_CA *_cmm_1P,_CA *_cmm_2P);
int strNcmp(_CA *_cmm_1P,_CA *_cmm_2P,int _cmm_3P);
int strLen(_CA *_cmm_1P);
int strChr(_CA *_cmm_1P,char _cmm_2P);
int strRchr(_CA *_cmm_1P,char _cmm_2P);
int strStr(_CA *_cmm_1P,_CA *_cmm_2P);
_CA *strCpy(_CA *_cmm_1P,_CA *_cmm_2P){
{
int _cmm_1L;
(_cmm_1L=0);
for(;(((*_CCA(_cmm_1P,_cmm_1L,__FILE__,36))=(*_CCA(_cmm_2P,_cmm_1L,__FILE__,36)))!=0);(_cmm_1L=(_cmm_1L+1)))
;
}
return _cmm_1P;
}
_CA *strNcpy(_CA *_cmm_1P,_CA *_cmm_2P,int _cmm_3P){
int _cmm_1L;
(_cmm_1L=0);
{
;
for(;((_cmm_1L<(_cmm_3P+-1))&&((*_CCA(_cmm_2P,_cmm_1L,__FILE__,44))!=0));(_cmm_1L=(_cmm_1L+1)))
((*_CCA(_cmm_1P,_cmm_1L,__FILE__,45))=(*_CCA(_cmm_2P,_cmm_1L,__FILE__,45)));
}
{
;
for(;(_cmm_1L<_cmm_3P);(_cmm_1L=(_cmm_1L+1)))
((*_CCA(_cmm_1P,_cmm_1L,__FILE__,47))=0);
}
return _cmm_1P;
}
_CA *strCat(_CA *_cmm_1P,_CA *_cmm_2P){
int _cmm_1L;
(_cmm_1L=0);
for(;((*_CCA(_cmm_1P,_cmm_1L,__FILE__,54))!=0);)
(_cmm_1L=(_cmm_1L+1));
{
int _cmm_2L;
(_cmm_2L=_cmm_1L);
for(;(((*_CCA(_cmm_1P,_cmm_1L,__FILE__,56))=(*_CCA(_cmm_2P,(_cmm_1L-_cmm_2L),__FILE__,56)))!=0);(_cmm_1L=(_cmm_1L+1)))
;
}
return _cmm_1P;
}
_CA *strNcat(_CA *_cmm_1P,_CA *_cmm_2P,int _cmm_3P){
int _cmm_1L;
(_cmm_1L=0);
for(;((*_CCA(_cmm_1P,_cmm_1L,__FILE__,64))!=0);)
(_cmm_1L=(_cmm_1L+1));
{
int _cmm_2L;
(_cmm_2L=0);
for(;((_cmm_2L<(_cmm_3P+-1))&&((*_CCA(_cmm_2P,_cmm_2L,__FILE__,66))!=0));((_cmm_1L=(_cmm_1L+1)),(_cmm_2L=(_cmm_2L+1))))
((*_CCA(_cmm_1P,_cmm_1L,__FILE__,67))=(*_CCA(_cmm_2P,_cmm_2L,__FILE__,67)));
}
((*_CCA(_cmm_1P,_cmm_1L,__FILE__,68))=0);
return _cmm_1P;
}
int strCmp(_CA *_cmm_1P,_CA *_cmm_2P){
int _cmm_1L;
{
(_cmm_1L=0);
for(;((*_CCA(_cmm_1P,_cmm_1L,__FILE__,75))==(*_CCA(_cmm_2P,_cmm_1L,__FILE__,75)));(_cmm_1L=(_cmm_1L+1)))
{if(((*_CCA(_cmm_1P,_cmm_1L,__FILE__,76))==0))return 0;
}}
return ((*_CCA(_cmm_1P,_cmm_1L,__FILE__,77))-(*_CCA(_cmm_2P,_cmm_1L,__FILE__,77)));
}
int strNcmp(_CA *_cmm_1P,_CA *_cmm_2P,int _cmm_3P){
int _cmm_1L;
{
(_cmm_1L=0);
for(;((_cmm_1L<_cmm_3P)&&((*_CCA(_cmm_1P,_cmm_1L,__FILE__,83))==(*_CCA(_cmm_2P,_cmm_1L,__FILE__,83))));(_cmm_1L=(_cmm_1L+1)))
{if(((*_CCA(_cmm_1P,_cmm_1L,__FILE__,84))==0))return 0;
}}
{if((_cmm_1L>=_cmm_3P))return 0;
}return ((*_CCA(_cmm_1P,_cmm_1L,__FILE__,86))-(*_CCA(_cmm_2P,_cmm_1L,__FILE__,86)));
}
int strLen(_CA *_cmm_1P){
int _cmm_1L;
(_cmm_1L=0);
for(;((*_CCA(_cmm_1P,_cmm_1L,__FILE__,92))!=0);)
(_cmm_1L=(_cmm_1L+1));
return _cmm_1L;
}
int strChr(_CA *_cmm_1P,char _cmm_2P){
int _cmm_1L;
{
(_cmm_1L=0);
for(;((*_CCA(_cmm_1P,_cmm_1L,__FILE__,101))!=0);(_cmm_1L=(_cmm_1L+1)))
{if(((*_CCA(_cmm_1P,_cmm_1L,__FILE__,102))==_cmm_2P))return _cmm_1L;
}}
return -1;
}
int strRchr(_CA *_cmm_1P,char _cmm_2P){
int _cmm_1L;
int _cmm_2L;
(_cmm_2L=-1);
{
(_cmm_1L=0);
for(;((*_CCA(_cmm_1P,_cmm_1L,__FILE__,109))!=0);(_cmm_1L=(_cmm_1L+1)))
{if(((*_CCA(_cmm_1P,_cmm_1L,__FILE__,110))==_cmm_2P))(_cmm_2L=_cmm_1L);
}}
return _cmm_2L;
}
int strStr(_CA *_cmm_1P,_CA *_cmm_2P){
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
{if(((*_CCA(_cmm_1P,(_cmm_3L+_cmm_4L),__FILE__,121))!=(*_CCA(_cmm_2P,_cmm_4L,__FILE__,121))))break;
}}
{if((_cmm_4L==_cmm_1L))return _cmm_3L;
}}
}
return -1;
}

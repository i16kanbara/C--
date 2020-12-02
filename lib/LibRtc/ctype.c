char isSpace(char _cmm_1P){
return ((((((9==_cmm_1P)||(10==_cmm_1P))||(11==_cmm_1P))||(12==_cmm_1P))||(13==_cmm_1P))||(32==_cmm_1P));
}
char isDigit(char _cmm_1P){
return ((48<=_cmm_1P)&&(_cmm_1P<=57));
}
char isXdigit(char _cmm_1P){
return ((isDigit(_cmm_1P)||((65<=_cmm_1P)&&(_cmm_1P<=70)))||((97<=_cmm_1P)&&(_cmm_1P<=102)));
}
char isPrint(char _cmm_1P){
return ((32<=_cmm_1P)&&(_cmm_1P<=126));
}
char isLower(char _cmm_1P){
return ((97<=_cmm_1P)&&(_cmm_1P<=122));
}
char isUpper(char _cmm_1P){
return ((65<=_cmm_1P)&&(_cmm_1P<=90));
}
char isAlpha(char _cmm_1P){
return (isUpper(_cmm_1P)||isLower(_cmm_1P));
}
char isAlnum(char _cmm_1P){
return (isDigit(_cmm_1P)||isAlpha(_cmm_1P));
}
char toLower(char _cmm_1P){
{if(isUpper(_cmm_1P))(_cmm_1P=(0xff&(_cmm_1P+32)));
}return _cmm_1P;
}
char toUpper(char _cmm_1P){
{if(isLower(_cmm_1P))(_cmm_1P=(0xff&(_cmm_1P+-32)));
}return _cmm_1P;
}

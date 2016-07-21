/*
 * Programing Language C-- "Compiler"
 *    Tokuyama kousen Educational Computer 16bit Ver.
 *
 * Copyright (C) 2016 by
 *                      Dept. of Computer Science and Electronic Engineering,
 *                      Tokuyama College of Technology, JAPAN
 *
 *   上記著作権者は，Free Software Foundation によって公開されている GNU 一般公
 * 衆利用許諾契約書バージョン２に記述されている条件を満たす場合に限り，本ソース
 * コード(本ソースコードを改変したものを含む．以下同様)を使用・複製・改変・再配
 * 布することを無償で許諾する．
 *
 *   本ソースコードは＊全くの無保証＊で提供されるものである。上記著作権者および
 * 関連機関・個人は本ソースコードに関して，その適用可能性も含めて，いかなる保証
 * も行わない．また，本ソースコードの利用により直接的または間接的に生じたいかな
 * る損害に関しても，その責任を負わない．
 *
 *
 */

/*
 * util.c : 段階コンパイル版用の良く使う関数
 *
 * 2016.06.04         : 初期バージョン
 *
 * $Id$
 *
 */

#include <stdio.h>
#include <strings.h>
#include <ctype.h>

#include "../sytree.h"
#include "../util.h"
#include "util.h"
#define StrMAX  128
static char str[StrMAX + 1];            // 名前を返す場合、その綴

// 入力ファイル名をもらって、拡張子を変更して書込みオープンする
static void tooLongFname() {
  error("ファイル名が長すぎる");
}

FILE *openDstWithExt(char *srcName, char *ext) {
  char dstName[StrMAX + 1];

  if (strlen(srcName)>StrMAX) tooLongFname();
  strcpy(dstName, srcName);
  
  char *lastDot = rindex(dstName, '.');
  if (lastDot!=NULL) *lastDot='\0';

  if (strlen(dstName)+strlen(ext)>StrMAX) tooLongFname();
  strcat(dstName, ext);

  return eOpen(dstName, "w");
}

// 10進数を読んで整数を返す, EOFならEOFを返す
int getDec(FILE *fp) {
  int v = 0;                                     // 初期値は 0
  char ch = fgetc(fp);
  boolean minusflg = false;
  if(ch==EOF)
    return EOF;
  else if(ch=='-'){
    minusflg = true;
    ch = fgetc(fp);
  }
  while (isdigit(ch)) {                          // 10進数字の間
    v = v*10 + ch - '0';                         // 値を計算
    ch = fgetc(fp);                              // 次の文字を読む
  }
  if(minusflg) return -v;
  return v;                                      // 10進数の値を返す
}

// 名前か文字列を読み込む
char *getStr(FILE *fp){
  int i;
  char ch = fgetc(fp);
  for (i=0; ch!='\n' && ch!=EOF; i=i+1) {        // 行末まで
    if (i>=StrMAX-1) error("名前が長すぎる");
    str[i] = ch;                                 // strに読み込む
    ch = fgetc(fp);                              // 次の文字を読む
  }
  str[i] = '\0';                                 // 文字列を完成させる
  return str;
}

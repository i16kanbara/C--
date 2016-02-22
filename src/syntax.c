/*
 * Programing Language C-- "Compiler"
 *    Tokuyama kousen Educational Computer 16bit Ver.
 *
 * Copyright (C) 2002-2016 by
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
 * syntax.c : C--コンパイラの構文解析ルーチン
 *
 * 2016.02.05 v3.0.0  : main() 関数を main.c に分離して新規作成
 *                      トランスレータと統合
 * 2016.01.12         : 可変個引数関数の実引数にvoid型が来た時のバグ発見(未解決)
 * 2015.12.15 v2.1.1  : void 型以外の関数が return で終わっているかの判定にバグ
 * 2015.10.04         : -Wall でも警告が出ないように無駄な初期化を3箇所追加
 * 2015.08.31 v2.1.0  : 関数の入口でスタックのオーバーフローをチェックする関数
 *                      __stkChek() を呼出するようにする。カーネルコンパイル用
 *                      に、呼出さないための "-K" オプションを追加。
 *                      interruput 関数は "-K" のときだけ使用可能
 * 2015.03.10         : 関数の２重定義に関するバグ訂正
 * 2012.12.30         : エラーメッセージを一部変更
 * 2012.08.15 v2.0.0  : getCnst で、"-O" オプション無しでも定数式を計算する
 * 2010.12.24         : public struct を禁止
 * 2010.09.12 v1.0.0  : 最適化と外部変数の定数式による初期化ができる H8 統合版
 * 2010.09.08         : 最適化オプション "-O" を追加
 * 2010.08.21         : バージョン表示にコンパイル日時を追加
 * 2010.07.23         : バージョン表示オプション "-v" を追加
 * 2010.07.20         : Subversion による管理を開始
 * 2010.04.06         : データ初期化を木に格納しコード生成が行うように変更
 * 2010.03.31         : ソースコードを整理し、関数名、変数名を一部変更
 * 2010.03.30 v0.9.11 : ローカル変数と仮引数を区別するために SyARG を追加
 * 2010.03.12         : 大域変数の型チェックに関するバグ訂正
 * 2010.03.12 v0.9.10 : public 修飾に対応
 * 2009.11.03 v0.9.9  : addrof 演算子に対応
 * 2009.08.09         : 構造体配列の sizeof のバグ他訂正
 * 2009.07.31         : (a[1])[2] が正しく処理できないコード生成のバグ訂正
 * 2009.07.24 v0.9.0  : 構文木を作成後、構文木を使用してコード生成する
 * 2009.07.23         : 関数とプロトタイプの型が一致するかのチェック不足訂正
 * 2009.07.22         : 名前表を namtbl.c へ分離
 * 2009.07.21 v0.8.0  : util.h, lexical.h の再検討
 * 2009.05.08         : 未使用変数を2箇所削除
 * 2009.05.02 v0.7.1  : do-while 文中の continue ジャンプ先のバグ訂正
 * 2009.05.01 v0.7.0  : sizeof(構造体)で名前表の0番目にある構造体のバグを訂正
 * 2008.09.01 v0.6.2  : 負の整数で初期化できないバグを訂正
 * 2008.08.30 v0.6.1  : if (配列==...) { がエラーになるバグを訂正
 * 2008.08.26 v0.6    : 参照の比較を可能にする
 *                      標準入力からの読み込みを可能にする
 * 2008.06.12 v0.5.1  : プロトタイプ宣言が2重定義になるバグ、
 *                      void型変数が作れるバグを訂正、','演算子追加
 * 2007.09.29 v0.5    : 初期バージョン
 *
 * $Id$
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"                   // その他機能モジュール
#include "lexical.h"                // 字句解析モジュール
#include "optree.h"                 // 構文木最適化モジュール
#include "code.h"                   // コード生成モジュール
#include "namtbl.h"                 // 名前表モジュール
#include "sytree.h"                 // 構文木モジュール
#include "syntax.h"

/*
 * グローバルデータ
 */
static char   *curName;             // 最近登録した名前のつづり
static int     curScope;            // 現在のスコープ
static int     curType;             // 最近読み込んだ型
static int     curDim;              // 最近読み込んだ型の配列次元数
static int     curCnt;              // 最近読み込んだローカル変数や引数、
                                    //   構造体フィールドの番号
static boolean pubFlag;             // public 修飾子を読んだ
static int     funcIdx;             // 現在読込中の関数

static boolean optFlag = true;      // 最適化を行う
static boolean krnFlag = false;     // カーネルコンパイルモード

// (# 行番号 "path")ディレクティブの処理
#define  CMMINC "/cmmInclude/"      // C--用システムヘッダファイルの目印

static void getFile() {
  char * fname = lxGetStr();                 // 現在のファイル

  if (strstr(fname, CMMINC)!=null &&         // システムディレクトリの
      strEndsWith(fname, ".hmm"))  {         // ヘッダファイルなら
    fname[strlen(fname)-2]='\0';             //   ".hmm" を ".h" に改変し
    genOff(strrchr(fname,'/')+1);            // システムヘッダ内容は出力しない
  } else {                                   // (代替のディレクティブを出力)
    genOn();                                 // システムヘッダ以外は出力する
  }
}

// トークンの読み込み
static int tok;                              // 次のトークン

static int getTok() {
  tok = lxGetTok();                          // 次のトークンを入力する
  while (tok==LxFILE) {                      // ディレクティブなら
    getFile();                               //   ディレクティブを処理する
    tok = lxGetTok();                        //   次のトークンを入力する
  }
  return tok;
}

/*
 * 構文解析
 */
// 次のトークンを調べ予想通りなら読み捨てる、予想との比較結果true/falseを返す
static boolean isTok(int expTok) {
  if (tok==expTok) {
    getTok();                                    // 予想通りなら読み捨てた
    return true;                                 //   上で true を返す
  }
  return false;                                  // 予想と異なるとfalseを返す
}

// 次のトークンを調べ予想通りなら読み捨てる、そうでなければエラーにする
static void chkTok(int expTok, char *msg) {
  if (!isTok(expTok)) error(msg);                // 予想と異なるとエラー
}

// 型か調べ、curType を書換える．型の場合はtrue、型ではない場合はfalseを返す
static boolean isType(void) {
  curType=TyNON;                                 // 取り合えず型ではない
  if      (tok==LxINT ) curType=TyINT;           // int 型
  else if (tok==LxCHAR) curType=TyCHAR;          // char 型
  else if (tok==LxVOID) curType=TyVOID;          // void 型
  else if (tok==LxBOOL) curType=TyBOOL;          // boolean 型
  else if (tok==LxINTR) curType=TyINTR;          // interrupt 型
  else if (tok==LxNAME)                          // 名前なら構造体型かも
    curType=ntSrcStruct(lxGetStr());             //   構造体名を検索する

  return curType!=TyNON;                         // TyNON 以外なら型だった
}

// 型名と配列を表す '[]' を読み込む
static void getType(void) {
  if (!isType()) error("型がない");              // 型かどうか調べる
  getTok();                                      // 型名を読み飛ばす
  for (curDim=0; isTok('['); curDim=curDim+1)    // 配列型なら次元を増やす
    chkTok(']',"配列宣言に ']' が不足");
}

// 名前を読み込み、表に登録する(引数は public 名の意味)
static void getName(boolean pub) {
  chkTok(LxNAME, "名前がない");                          // 名前か調べる
  if (curScope!=ScPROT && curType==TyVOID && curDim==0)  // 関数以外でvoid型は
    error("void型変数/引数は使用できない");              //   ポインタだけOK
  if (curScope!=ScPROT && curType==TyINTR)               // 関数以外でintrrupt
    error("interrupt型変数/引数は使用できない");         //   型は使用できない
  if (curType==TyINTR && !krnFlag)                       // カーネルのみintrrupt
    error("interrupt型はカーネルのみで使用可");          //   型は使用できる
  ntDefName(lxGetStr(), curScope, curType,               // 名前表に登録する
	    curDim, curCnt, pub);
  curName = ntGetName(ntGetSize()-1);                    // 最後に登録した名前
}

/*
 * 構造体の宣言
 */
// 構造体フィールド１行を読み込む
static void getFieldLine(void) {
  getType();                                    // 型を読み込む
  getName(false);                               // 名前を読み込んで表に登録
  curCnt = curCnt + 1;                          // フィールドの番号
  while (isTok(',')) {                          // ','が続く間
    getName(false);                             //    名前を読み込んで表に登録
    curCnt = curCnt + 1;                        // フィールドの番号
  }
  chkTok(';', "構造体フィールドの宣言が ';' で終わっていない");
}

// 構造体宣言を読み込む
static void getStruct(void) {
  chkTok('{', "構造体宣言に '{' がない");
  int structIdx=ntGetSize();                   // 構造体宣言は関数の外にあるの
  curScope = ScLVAR;                           //   でフィールド名の衝突チェッ
  curCnt   = 0;                                //   クは局所変数と同様でよい
  getFieldLine();                              // フィールド宣言1行分を読む
  while (!isTok('}'))                          // '}'以外の間
    getFieldLine();                            //   フィールド宣言1行分を読む
  chkTok(';', "構造体宣言が ';' で終わっていない");
  for (int i=structIdx; i<ntGetSize(); i=i+1)  // 名前の衝突チェックが終わった
    ntSetScope(i, ScVOID);                     //   のでフィールドのscopeに変更
  ntSetCnt(structIdx-1,ntGetSize()-structIdx); // フィールド数を表に記録
  genStruc(structIdx-1);                       // 構造体宣言を出力
}                                              // (トランスレータ版だけで必要)

/*
 * 式の処理
 */
// 式を監視(watch)するデータ構造
struct watch {
  int type;                                    // 式の型
  int dim;                                     // 式が配列を表すとき次元数
  boolean lhs;                                 // Left-Hand-Side Expression
  int tree;                                    // 式を表すツリーの root
};

// watch を割り当てる
static struct watch* newWatch(void) {
  return ealloc(sizeof(struct watch));
}

// watch に値を設定する
static void setWatch(struct watch *w, int t, int d, int l, int r) {
  w->type = t;
  w->dim  = d;
  w->lhs  = l;
  w->tree = r;

}

// watch の領域を解放する
static void freeWatch(struct watch* w) {
  free(w);                                       // 領域をシステムに返却する
}

// 式(w)の型が type 型の基本型か調べる
static void chkType(struct watch *w, int type) {
  if (w->dim>0)      error("配列型は使用できない");
  if (w->type==TyVOID) error("void型は使用できない");
  if (type==TyBOOL && w->type!=TyBOOL)  error("論理型が必要");
  if (type==TyINT  && w->type!=TyINT )  error("整数型が必要");
  if (type==TyCHAR && w->type!=TyCHAR)  error("文字型が必要");
  if (type!=TyBOOL && type!=TyINT && type!=TyCHAR) error("バグ...chkType");
}

// 式(w)の値を type, dim 型へ代入可能か調べる
static void chkCmpat(struct watch* w, int type, int dim) {
  if (!(w->type==TyVOID && w->dim>0 && dim>0) &&    // void[] はどの [] にも OK
      !(type==TyVOID && dim>0 && w->dim>0) &&
      !(w->type==TyVOID && w->dim==1 && type<=0)&&  // void[] と構造体は OK
      !(type==TyVOID && dim==1 && w->type<=0) &&
      (w->type!=type || w->dim!=dim))               // それ以外は正確に型が合
    error("代入/比較/初期化/引数/returnの型が合わない");// わないと代入できない
}

static void getAsExpr(struct watch *w);        // 再帰呼出の関係で
static void getExpr(struct watch *w);          //   プロトタイプ宣言が必要

/*
// 後置演算子(添字('[n]')と構造体フィールド('.'))の処理
static void getPostOP(struct watch *w) {
  while (tok=='[' || tok=='.') {               // 後置演算子が続く間繰り返す
    struct watch *w2 = newWatch();
    int stype = SyPOST;
    int rval = 0;
    if (isTok('[')) {                          // 添字の場合
      if (w->dim<=0) error("添字が多すぎる");
      getAsExpr(w2);                           // 添字式(w2)はカンマ式不可
      chkTok(']', "']' がない");
      if (w2->type!=TyINT || w2->dim!=0)       // 添字式(w2)の型チェック
	error("配列の添字が整数以外になっている");
      w->dim = w->dim - 1;                     // 式(w)の次元を下げる
      rval = w2->tree;
      if ((w->type==TyCHAR || w->type==TyBOOL) // char, boolean の配列は
	  && w->dim==0) stype = SyBYTE;        //   バイト単位で詰め込む
    } else if (isTok('.')) {                   // 構造体の場合は
      if (w->type>0 || w->dim>0)               //   式(w)が基本型または配列の
	error("構造体以外に'.' がある");       //   場合は'.'が続いてはならない
      chkTok(LxNAME, "'.' の次に名前がない");  // '.'の次は必ず名前が必要
      int n = ntSrcField(w->type,lxGetStr());  // 構造体をフィールド名でサーチ
      w->type = ntGetType(n);                  // 式(w)をフィールドの型と
      w->dim  = ntGetDim(n);                   //   次元に変更する
      rval = syNewNode(SyCNST,ntGetCnt(n),SyNULL); // フィールドオフセット
    } else error("バグ...getPostOP");
    w->tree = syNewNode(stype, w->tree, rval); // 後置演算を構文木に追加
    w->lhs = true;                             // 演算結果は代入可
    freeWatch(w2); 
  }
}
*/

// 後置演算子(添字('[n]'))の処理
static void getIdxOP(struct watch *w) {
  struct watch *w2 = newWatch();               // w2 は添字式
  if (w->dim<=0) error("添字が多すぎる");
  getAsExpr(w2);                               // 添字式(w2)はカンマ式不可
  chkTok(']', "']' がない");
  if (w2->type!=TyINT || w2->dim!=0)           // 添字式(w2)の型チェック
    error("配列の添字が整数以外になっている");
  w->dim = w->dim - 1;                         // 式(w)の次元を下げる

  int stype = w->type;
  if (w->dim>0 || stype<=0) stype = SyIDXW;    // 参照はワード配列
  else if (stype==TyINT)    stype = SyIDXW;    // int はワード配列
  else if (stype==TyCHAR)   stype = SyIDXB;    // char はバイト配列
  else if (stype==TyBOOL)   stype = SyIDXB;    // boolean はバイト配列
  else error("バグ...getIdxOP");

  w->tree = syNewNode(stype, w->tree, w2->tree);// 
  w->lhs = true;                               // 演算結果は代入可
  freeWatch(w2); 
}

// 後置演算子(構造体フィールド('.'))の処理
static void getDotOP(struct watch *w) {
  if (w->type>0 || w->dim>0)                   //   式(w)が基本型または配列の
    error("構造体以外に'.' がある");           //   場合は'.'が続いてはならない
  chkTok(LxNAME, "'.' の次に名前がない");      // '.'の次は必ず名前が必要
  int n = ntSrcField(w->type,lxGetStr());      // 構造体をフィールド名でサーチ
  w->type = ntGetType(n);                      // 式(w)をフィールドの型と
  w->dim  = ntGetDim(n);                       //   次元に変更する
  int cnst = syNewNode(SyCNST, n, SyNULL);     // フィールドの名前表インデクス
  w->tree = syNewNode(SyDOT, w->tree, cnst);   // '.'演算を構文木に追加
  w->lhs = true;                               // 演算結果は代入可
}

// 後置演算子(添字('[n]')と構造体フィールド('.'))の処理
static void getPostOP(struct watch *w) {
  while (true) {                               // 後置演算子が続く間繰り返す
    if (isTok('[')) getIdxOP(w);               // '[n]' の処理
    else if (isTok('.')) getDotOP(w);          // '.' の処理
    else break;
  }
}

// 実引数を読み込む
static void getArgs(struct watch* w, int func) {   // funcは表の現在の関数を指す
  int idx = func + 1;                              // 仮引数を指すインデクス
  int lastIdx = func + ntGetCnt(func);             // 最後の仮引数のインデクス
  int list = SyNULL;                               // 引数リストを作る
  if (tok!=')') {                                  // 引数が存在するなら
    do {                                           //
      getAsExpr(w);                                // 引数の式(w)(カンマ式不可)
      list = syCatNode(list, w->tree);             // 引数リストに追加
      if (idx>lastIdx) error("引数が多い");
      if (ntGetType(idx)!=TyDOTDOTDOT) {           // 可変個引数ではないなら
	chkCmpat(w,ntGetType(idx),ntGetDim(idx));  //     型チェックしてから
	idx=idx+1;                                 //     インデクスを進める
      }
    } while (isTok(','));                          // ','が続く間繰り返す
  }
  chkTok(')', "関数呼出に ')' がない");
  if (ntGetType(idx)!=TyDOTDOTDOT && idx<=lastIdx) // '...'でなくidxが小さい
    error("引数が少ない");
  w->tree = list;                                  // 引数リストを返す
}

// 関数呼出
static void getCall(struct watch* w, int n) {   // n は名前表で関数の位置
  chkTok('(',"関数名の次に '(' がない");
  if (ntGetType(n)==TyINTR)
    error("interrupt型関数は呼び出せない");
  getArgs(w, n);                                // 実引数を読み込む
  int s = syNewNode(SyFUNC, n, w->tree);        // 関数を表現するノード
  setWatch(w,ntGetType(n),ntGetDim(n),false,s); // 関数の値は変更不可
}

// sizeof 演算子
static void getSizeof(struct watch* w) {
  chkTok('(', "sizeof に '(' が続かない");
  int ty=curType, dm=curDim;                  // getType が壊すので保存し、
  getType();                                  // 型を読む
  int s = NWORD / 8;                          // INT またはポインタのサイズ
  if (curDim==0) {                            // 配列以外で
    if (curType<=0)                           //   構造体の場合は
      s = s * ntGetCnt(-curType);             //     フィールド数×INTのサイズ
    else if (curType==TyCHAR ||
	     curType==TyBOOL)                 //   char, boolean なら
      s = NBYTE / 8;                          //     バイトのサイズ
  }
  curType=ty;                                 // 保存したものをもとに戻す
  curDim = dm;
  chkTok(')', "sizeof が ')' で終わらない");
  int a = syNewNode(SyCNST, s, SyNULL);       // サイズを格納するノード
  setWatch(w, TyINT, 0, false, a);            // sizeof は整数型の定数
}

// addrof 演算子
static void getAddrof(struct watch* w) {
  chkTok('(', "addrof に '(' が続かない");
  chkTok(LxNAME, "addrof( に名前が続かない");
  int n = ntSrcName(lxGetStr());              // 表から名前を探し
  int s = ntGetScope(n);                      // それのスコープを調べ
  if (s==ScPROT || s==ScFUNC ||               // 関数名または
      s==ScCOMM || s==ScGVAR) {               // 大域変数なら OK
    int a = syNewNode(SyLABL, n, SyNULL);     //   ラベル定数を表現するノード
    setWatch(w, TyINT, 0, false, a);          //   addrof は整数型の定数
  } else error("addrof( に大域名が続かない"); // それ以外の名前はあり得ない
  chkTok(')', "addrof が ')' で終わらない");
}

// ord 演算子
static void getOrd(struct watch* w) {
  chkTok('(', "ord に '(' が続かない");
  getExpr(w);                                 // 括弧内の式を読み込む
  chkTok(')', "ord が ')' で終わらない");
  if ((w->type!=TyBOOL && w->type!=TyCHAR) || // char と bool を変換できる
      w->dim!=0) error("ord 括弧内の型が不正");
  w->type = TyINT;                            // int 型に変換
  w->lhs  = false;                            // 代入不可
}

// chr 演算子
static void getChr(struct watch* w) {
  chkTok('(', "chr に '(' が続かない");
  getExpr(w);                                 // 括弧内の式を読み込む
  chkTok(')', "chr が ')' で終わらない");
  if (w->dim!=0 || w->type!=TyINT) error("chr 括弧内が int 以外になっている");
  w->tree = syNewNode(SyCHAR, w->tree, SyNULL);
  w->type = TyCHAR;                           // char 型に変換
  w->lhs  = false;                            // 代入不可
}

// bool 演算子
static void getBool(struct watch* w) {
  chkTok('(', "bool に '(' が続かない");
  getExpr(w);                                 // 括弧内の式を読み込む
  chkTok(')', "bool が ')' で終わらない");
  if (w->dim!=0 || w->type!=TyINT) error("bool 括弧内が int 以外になっている");
  w->tree = syNewNode(SyBOOL, w->tree, SyNULL);
  w->type = TyBOOL;                           // boolean 型に変換
  w->lhs  = false;                            // 代入不可
}

// 関数呼出しか変数（登録済みの名前で始まる）
static void getIdent(struct watch* w) {
  int n = ntSrcName(lxGetStr());              // 表から名前を探し
  int s = ntGetScope(n);                      //   それのスコープを調べ
  int t = ntGetType(n);                       //   それの型を調べ、
  int d = ntGetDim(n);                        //   それの配列次元数を調べ、
  int c = ntGetCnt(n);                        //   それの番号を調べる
  if (s==ScPROT || s==ScFUNC) {               // 関数名の場合は、
    getCall(w,n);                             //   関数呼び出しの処理
  } else if (s==ScCOMM || s==ScGVAR) {        // 大域変数の場合
    int a = syNewNode(SyGLB, n, SyNULL);      //   大域変数を表現するノード
    setWatch(w, t, d, true, a);               //   式(w)が大域変数になる
  } else if (s>=ScLVAR) {                     // ScLVAR の場合は
    if (c>0) {                                //   c>0 なら局所変数
      int a = syNewNode(SyLOC, c, SyNULL);    //     局所変数のノード
      setWatch(w, t, d, true, a);             //     式(w)が局所変数になる
    } else {                                  //   c<0 なら仮引数
      int a = syNewNode(SyARG,-c, SyNULL);    //     仮引数のノード
      setWatch(w, t, d, true, a);             //     式(w)が仮引数になる
    }
  } else error("バグ...getIdent");            // それ以外の名前はあり得ない
}

// 因子
static void getFactor(struct watch* w) {
  if (isTok('(')) {                           // '(' の場合は
    getExpr(w);                               // 括弧内の式を読み込む
    chkTok(')', "')' が不足");
  } else if (isTok(LxNAME)) {                 // 名前の場合
    getIdent(w);                              //   関数呼び出しか変数
  } else if (isTok(LxINTEGER)) {              // 整数定数の場合は
    int a = syNewNode(SyCNST, lxGetVal(), SyNULL);  // 定数を格納するノード
    setWatch(w, TyINT, 0, false, a);          //   式(w)がint型定数になる
  } else if (isTok(LxCHARACTER)) {            // 文字定数の場合は
    int a = syNewNode(SyCNST, lxGetVal(), SyNULL);  // 定数を格納するノード
    setWatch(w, TyCHAR, 0, false, a);         //   式(w)がint型定数になる
  } else if (isTok(LxLOGICAL)) {              // 論理値の場合は
    int a = syNewNode(SyCNST, lxGetVal(), SyNULL);  // 定数を格納するノード
    setWatch(w, TyBOOL, 0, false, a);         //   式(w)がboolean型定数になる
  } else if (isTok(LxSTRING)) {               // 文字列の場合は
    int lab = genStr(lxGetStr());             // 文字列を出力しラベルを付ける
    int a = syNewNode(SySTR, lab, SyNULL);    //    ラベル番号を格納するノード
    setWatch(w, TyCHAR, 1,  false, a);        //    式(w)が文字配列型になる
  } else if (isTok(LxNUL)) {                  // null の場合は
    int a = syNewNode(SyCNST, 0, SyNULL);     //   NULL を格納するノード
    setWatch(w, TyVOID, 1, false, a);         //   式(w)は void[] 型になる
  } else if (isTok(LxSIZEOF)) {               // sizeof 演算子の場合
    getSizeof(w);
  } else if (isTok(LxADDROF)) {               // addrof 演算子の場合
    getAddrof(w);
  } else if (isTok(LxORD)) {                  // ord 演算子の場合
    getOrd(w);
  } else if (isTok(LxCHR)) {                  // chr 演算子の場合
    getChr(w);
  } else if (isTok(LxBOL)) {                  // bool 演算子の場合
    getBool(w);
  } else                                     // それ以外は因子にならない
    error("式がない/式が突然終わっている/C--で使用できない演算子");
  getPostOP(w);                               // 因子の後半 '.'や'[]'の処理
}

// 単項演算
static void getUniExpr(struct watch* w) {
  if (isTok('+')) {                           // '+' の場合は
    getUniExpr(w);                            //   まず、式(w)を読み込み
    chkType(w, TyINT);                        //   式(w)が整数型か調べる
    w->lhs = false;
  } else if (isTok('-')) {                    // '-' の場合は
    getUniExpr(w);                            //   まず、式(w)を読み込み
    chkType(w, TyINT);                        //   式(w)が整数型か調べ
    w->tree = syNewNode(SyNEG, w->tree, SyNULL);
    w->lhs  = false;
  } else if (isTok('!')) {                    // '!'(論理の否定)の場合は
    getUniExpr(w);                            //   まず、式(w)を読み込み
    chkType(w, TyBOOL);                       //   式(w)が論理型か調べ
    w->tree = syNewNode(SyNOT, w->tree, SyNULL);
    w->lhs  = false;
  } else if (isTok('~')) {                    // '~'(1の補数)の場合は
    getUniExpr(w);                            //   まず、式(w)を読み込み
    chkType(w, TyINT);                        //   式(w)が整数型か調べ
    w->tree = syNewNode(SyBNOT, w->tree, SyNULL);
    w->lhs  = false;
  } else getFactor(w);                        // どれでもなければ因子
}

// 型チェックを行った上で2項演算の木を作る
static void cal2(int op, struct watch *w1, struct watch *w2, int type) {
  chkType(w1, type);                          // 左辺も
  chkType(w2, type);                          //   右辺も同じ型かチェック
  w1->tree = syNewNode(op, w1->tree, w2->tree);
  w1->lhs  = false;
  freeWatch(w2);                              // w2はもういらない
}

// 掛算，割算
static void getMulExpr(struct watch* w) {
  getUniExpr(w);                               // 左辺(w)を読み込む
  while (true) {
    int op;
    if      (isTok('*')) op=SyMUL;             // '*' か
    else if (isTok('/')) op=SyDIV;             // '/' か
    else if (isTok('%')) op=SyMOD;             // '%' が続く間
    else break;
    struct watch* w2 = newWatch();
    getUniExpr(w2);                            // 右辺(w2)を読み込む
    cal2(op, w, w2, TyINT);                    // 2項演算の木を作る(w2解放)
  }
}

// 足算，引算
static void getAddExpr(struct watch* w) {
  getMulExpr(w);                               // 左辺(w)を読み込む
  while (true) {
    int op;
    if      (isTok('+')) op = SyADD;           // '+' か
    else if (isTok('-')) op = SySUB;           // '-' が続く間
    else break;
    struct watch* w2 = newWatch();
    getMulExpr(w2);                            // 右辺(w2)を読み込む
    cal2(op, w, w2, TyINT);                    // 2項演算の木を作る(w2解放)
  }
}

// シフト演算
static void getSftExpr(struct watch* w) {
  getAddExpr(w);                               // 左辺(w)を読み込む
  while (true) {
    int op;
    if      (isTok(LxSHL)) op = SySHL;         // '<<' か
    else if (isTok(LxSHR)) op = SySHR;         // '>>' が続く間
    else break;
    struct watch* w2 = newWatch();
    getAddExpr(w2);                            // 右辺(w2)を読み込む
    cal2(op, w, w2, TyINT);                    // 2項演算の木を作る(w2解放)
  }
}

// 大小比較演算
static void getCmpExpr(struct watch* w) {
  getSftExpr(w);                               // 左辺(w)を読み込む
  while (true) {
    int op;
    if      (isTok('<'))   op = SyLT;          // '<'  か
    else if (isTok(LxLTE)) op = SyLE;          // '<=' か
    else if (isTok('>'))   op = SyGT;          // '>'  か
    else if (isTok(LxGTE)) op = SyGE;          // '>=' が続く間
    else break;
    struct watch* w2 = newWatch();
    getSftExpr(w2);                            // 右辺(w2)を読み込む
    cal2(op, w, w2, TyINT);                    // 2項演算の木を作る(w2解放)
    w->type = TyBOOL;                          // 演算結果は boolean
  }
}

// 比較演算
static void getEquExpr(struct watch* w) {
  getCmpExpr(w);                               // 左辺(w)を読み込む
  while (true) {
    int op;
    if      (isTok(LxEQU)) op = SyEQU;         // '==' か
    else if (isTok(LxNEQ)) op = SyNEQ;         // '!=' が続く間
    else break;
    struct watch* w2 = newWatch();
    getCmpExpr(w2);                            // 右辺(w2)を読み込む
    chkCmpat(w2, w->type, w->dim);             // 代入可能な型なら比較可能
    int n = syNewNode(op, w->tree, w2->tree);  // 新しいノードを作る
    setWatch(w, TyBOOL, 0, false, n);          // 式(w)は boolean 型になる
    freeWatch(w2);                             // w2解放
  }
}

// ビット毎の論理積
static void getAndExpr(struct watch* w) {
  getEquExpr(w);                               // 左辺(w)を読み込む
  while (isTok('&')) {                         // '&' が続く間
    struct watch* w2 = newWatch();
    getEquExpr(w2);                            // 右辺(w2)を読み込む
    cal2(SyBAND, w, w2, TyINT);                // 2項演算の木を作る(w2解放)
  }
}

// ビット毎の排他的論理和
static void getXorExpr(struct watch* w) {
  getAndExpr(w);                               // 左辺(w)を読み込む
  while (isTok('^')) {                         // '^' が続く間
    struct watch* w2 = newWatch();
    getAndExpr(w2);                            // 右辺(w2)を読み込む
    cal2(SyBXOR, w, w2, TyINT);                // 2項演算の木を作る(w2解放)
  }
}

// ビット毎の論理和
static void getOrExpr(struct watch* w) {
  getXorExpr(w);                               // 左辺(w)を読み込む
  while (isTok('|')) {                         // '|' が続く間
    struct watch* w2 = newWatch();
    getXorExpr(w2);                            // 右辺を読み込む
    cal2(SyBOR, w, w2, TyINT);                 // 2項演算の木を作る(w2解放)
  }
}

// 論理積
static void getLogAndExpr(struct watch* w) {
  getOrExpr(w);                                // 左辺(w)を読み込む
  while (isTok(LxAND)) {                       // '&&' が続く間
    struct watch* w2 = newWatch();
    getOrExpr(w2);                             // 右辺を読み込む
    cal2(SyAND, w, w2, TyBOOL);                // 2項演算の木を作る(w2解放)
  }
}

// 論理和
static void getLogOrExpr(struct watch* w) {
  getLogAndExpr(w);                            // 左辺(w)を読み込む
  while (isTok(LxOR)) {                        // '||' が続く間
    struct watch* w2 = newWatch();
    getLogAndExpr(w2);                         // 右辺を読み込む
    cal2(SyOR, w, w2, TyBOOL);                 // 2項演算の木を作る(w2解放)
  }
}

// 代入式
static void getAsExpr(struct watch* w) {
  getLogOrExpr(w);                             // 左辺式(w)を読み込む
  if (isTok('=')) {                            // '=' があるなら
    struct watch* w2 = newWatch();
    getAsExpr(w2);                             // 右辺(w2)を読み込む
    chkCmpat(w2, w->type, w->dim);             // 代入の型は合っているか
    if (!w->lhs) error("左辺が代入可能ではない");
    w->tree = syNewNode(SyASS, w->tree, w2->tree);
    w->lhs  = false;
    freeWatch(w2);                             // w2解放
  }
}

// カンマ式
static void getExpr(struct watch* w) {
  getAsExpr(w);                                // 左辺(w)を読み込む
  while (isTok(',')) {                         // ',' があるなら
    int rVal = w->tree;
    getAsExpr(w);                              // 右辺(w)を読み込む
    w->tree = syNewNode(SyCOMM, rVal, w->tree);
    w->lhs  = false;
  }
}

/*
 * ローカル変数の処理
 */
// ローカル変数の宣言を読み込む
static int maxCnt;                             // 関数内の最大スタック使用量
                                               //   (最大ローカル変数の深さ)
/*
static int getLVar(void) {
  int sta = SyNULL;
  curCnt = curCnt + 1;                         // ローカル変数の番号
  getName(false);                              // 変数名を読み込み表に登録
  if (isTok('=')) {                            // '='があれば、初期化がある
    sta = syNewNode(SyLOC, curCnt, SyNULL);    // 初期化の左辺を作る
    struct watch *w = newWatch();
    getAsExpr(w);                              // 初期化式にはカンマ式不可
    chkCmpat(w, curType, curDim);              // 初期化(代入)できるかチェック
    sta = syNewNode(SyASS, sta, w->tree);      // 左辺と右辺を接続
    freeWatch(w);                              // 式(w)は役目を終えた
  }
  return sta;
}
*/

static int getLVar(void) {
  curCnt = curCnt + 1;                         // ローカル変数の番号
  getName(false);                              // 変数名を読み込み表に登録
  int dec = syNewNode(SySEMI,curType,curDim);  // 変数の型
  dec = syNewNode(SyVAR, curCnt, dec);         // 変数宣言
  if (isTok('=')) {                            // '='があれば、初期化がある
    int sta = syNewNode(SyLOC,curCnt,SyNULL);  // 初期化の左辺を作る
    struct watch *w = newWatch();
    getAsExpr(w);                              // 初期化式にはカンマ式不可
    chkCmpat(w, curType, curDim);              // 初期化(代入)できるかチェック
    sta = syNewNode(SyASS, sta, w->tree);      // 左辺と右辺を接続
    freeWatch(w);                              // 式(w)は役目を終えた
    dec = syCatNode(dec, sta);                 // 宣言と初期化を接続
  }
  return dec;
}

// ローカル変数の宣言並びを読み込む
static int getLVars(void) {
  getType();                                   // 型を読み込む
  int sta = getLVar();                         // 名前 [ = 式 ] を読み込む
  while (isTok(','))                           // ',' が続く間
    sta = syCatNode(sta, getLVar());           //   名前 [ = 式 ] を読み込む
  if (curCnt > maxCnt) maxCnt = curCnt;        // もっとも大きな番号を記録
  return sta;
}

/*
 *  実行文の処理
 */
static int depth = 0;
static int getStat(void);                      // 再帰呼出の関係で
static int getBlock(void);                     //   プロトタイプ宣言が必要

// IF文
static int getIf(void) {
  chkTok('(', "if の次に '(' がない");
  struct watch* w = newWatch();
  getExpr(w);                                  // 条件式(w)を読み込む
  chkType(w, TyBOOL);                          // 条件式は論理型か
  chkTok(')', "if 条件式の次に ')' がない");
  int sta = syNewNode(SyIF,w->tree,getStat()); // 本体を読み込み if 文を作る
  if (isTok(LxELSE)) {                         // 'else'が続けばelse節がある
    sta = syNewNode(SyELS, sta, getStat());    //   else 節を読み込み登録する
  }
  freeWatch(w);                                // w は役目を終えた
  return sta;
}    

// WHILE文
static int getWhile(void) {
  chkTok('(', "while の次に '(' がない");
  struct watch* w = newWatch();
  getExpr(w);                                  // 条件式(w)を読み込む
  chkType(w, TyBOOL);                          // 条件式は論理型か
  chkTok(')', "while 条件式の次に ')' がない");
  depth = depth + 1;
  int sta = syNewNode(SySEMI,getStat(),SyNULL);// SyNULLは再初期化無しの意味
  depth = depth - 1;
  sta =  syNewNode(SyWHL, w->tree, sta);       // while を完成
  freeWatch(w);                                // w は役目を終えた
  return sta;
}    

// DO - WHILE文
static int getDoWhile(void) {
  depth = depth + 1;
  int sta = getStat();                         // 本体を読み込む
  depth = depth - 1;
  chkTok(LxWHILE, "do - while 文の while がない");
  chkTok('(',   "while の次に '(' がない");
  struct watch* w = newWatch();
  getExpr(w);                                  // 条件式(w)を読み込む
  chkType(w, TyBOOL);                          // 条件式は論理型か
  chkTok(')', "do - while 条件式の次に ')' がない");
  chkTok(';', "do - while 文の次に ';' がない");
  sta = syNewNode(SyDO, sta, w->tree);        // 登録
  freeWatch(w);                                // w は役割を終えた
  return sta;
}    

// FOR文
static int getFor(void) {
  // curScope = curScope + 1;     // C言語と同じスコープルールにするなら
  int tmpIdx  = ntGetSize();                   // for 文はひとつのスコープに
  int tmpCnt  = curCnt;                        //   なるのでローカル変数をもつ
  chkTok('(', "for の次に '(' がない");

  // 初期化式
  int ini = SyNULL;
  if (tok!=';') {                              // 初期化式ありの場合
    if (isType()) {                            //   型が続けば
      ini = getLVars();                        //       変数定義を読み込む
    } else {
      struct watch* w = newWatch();            // それ以外なら
      getExpr(w);                              //   初期化式(w)を読む
      ini = w->tree;
      freeWatch(w);                            // w は役割を終えた
    }
  }
  chkTok(';', "for 初期化の次に ';' がない");

  // 条件式
  int cnd = SyNULL;
  if (tok!=';') {                              // 条件式ありの場合
    struct watch* w = newWatch();
    getExpr(w);                                // 条件式(w)を読み込む
    chkType(w, TyBOOL);
    cnd = w->tree;
    freeWatch(w);                              // w は役割を終えた
  }
  chkTok(';', "for 条件の次に ';' がない");

  // 再初期化式
  int rini = SyNULL;
  if (tok!=')') {                              // 再初期化式ありの場合
    struct watch* w = newWatch();
    getExpr(w);                                // 再初期化式(w)を読み込む
    rini = w->tree;
    freeWatch(w);                              // w は役割を終えた
  }
  chkTok(')', "for 再初期化の次に ')' がない");

  // 本文
  depth = depth + 1;
  int sta = getStat();                         // 本体の読み込み
  depth = depth - 1;

  // 木を組み立てる
  // for (int i=1; i<10; i=i+1) sum = sum + a[i];  は
  // {int i=1; for (;i<10; i=i+1) sum = sum + a[i]; にする
  sta = syNewNode(SySEMI, sta, rini);          // 再初期化と本文
  sta = syNewNode(SyWHL, cnd, sta);            // while文相当部分
  sta = syNewNode(SyBLK, ini, sta);            // 初期化とwhile文でブロック
  
  ntUndefName(tmpIdx);                         // 表からローカル変数を捨てる
  curCnt = tmpCnt;                             // スタックの深さを戻す
  // curScope = curScope - 1;     // C言語と同じスコープルールにするなら
  return sta;
}    

// break 文
static int getBreak(void) {
  if (depth <= 0) error("ループ外で break を使用");
  chkTok(';', "break の次に ';' がない");
  return syNewNode(SyBRK, SyNULL, SyNULL);
}

// continue 文
static int getCont(void) {
    if (depth <= 0) error("ループ外で continue を使用");
  chkTok(';', "continue の次に ';' がない");
  return syNewNode(SyCNT, SyNULL, SyNULL);
}

// return 文
static int getReturn(void) {
  int t = ntGetType(funcIdx);                  // 関数の型
  int d = ntGetDim(funcIdx);                   // 関数の型の配列次元数
  int exp = SyNULL;                            // 式
  if (t!=TyINTR && (t!=TyVOID || d!=0)) {      // 現在の関数がvoid型以外なら
    struct watch* w = newWatch();              //   式が続くはず
    getExpr(w);                                // 式(w)を読み込み
    chkCmpat(w, t, d);                         //   関数の型と比較し型が合って
    exp = w->tree;
    freeWatch(w);                              // w は役割を終えた
  }
  chkTok(';', "return の次に ';' がない");
  return syNewNode(SyRET, exp, SyNULL);
}

static int lastStat = LxNONTOK;     // 最後の文はどの種類のトークンで始まったか

// 文
static int getStat(void) {
  int sta = SyNULL;
  int ltok = tok;                              // 最後に読んだ文(returnがあるか)
  if      (isTok(LxIF))       sta=getIf();     // if 文を見つけた場合
  else if (isTok(LxWHILE))    sta=getWhile();  // while 文を見つけた場合
  else if (isTok(LxDO))       sta=getDoWhile();// do - while 文を見つけた場合
  else if (isTok(LxFOR))      sta=getFor();    // for 文を見つけた場合
  else if (isTok(LxRETURN))   sta=getReturn(); // return 文を見つけた場合
  else if (isTok(LxBREAK))    sta=getBreak();  // break 文を見つけた場合
  else if (isTok(LxCONTINUE)) sta=getCont();   // continue 文を見つけた場合
  else if (isTok(';'))    ;                    // 空文を見つけた場合
  else if (isTok('{')) {                       // ブロックを見つけた場合
    // curScope = curScope + 1;                // C言語と同じスコープルールなら
    sta = getBlock();                          //   ブロックを読み込む
    // curScope = curScope - 1;                // C言語と同じスコープルールなら
  } else {                                     // どれでもなければ式文
    struct watch *w = newWatch();
    getExpr(w);                                // 式(w)を読み込む
    sta = w->tree;
    freeWatch(w);                              // w は役割を終えた
    chkTok(';', "式文の後ろに ';' がない");
  }
  lastStat = ltok;                             // 最後に読んだ文(returnがあるか)
  return sta;
}

// ブロックを読み込む
static int getBlock(void) {
  int lval   = SyNULL;
  int tmpIdx = ntGetSize();                    // ブロック終了時にローカル変数
  int tmpCnt = curCnt;                         //   を捨てるために憶えておくもの
  while (tok!='}' && tok!=EOF) {               // '}' が来るまで処理を続ける
    int rval;
    if (isType()) {                            // 型が来た場合
      rval = getLVars();                       //   ローカル変数の宣言
      chkTok(';', "変数宣言が ';' で終わっていない");
    } else {                                   // 型以外なら
      rval = getStat();                        //   文を読み込む
    }
    lval = syCatNode(lval, rval);              // リストを作る
  }
  chkTok('}', "ブロックが '}' で終了していない");
  ntUndefName(tmpIdx);                         // 表からローカル変数を捨てる
  curCnt = tmpCnt;                             // スタックの深さを戻す
  if (lval!=SyNULL && syGetType(lval)==SySEMI) // 意味のあるブロックなら
    sySetType(lval, SyBLK);                    //   リストを { } で括る
  return lval;
}

// プロトタイプ宣言と引数を比較する
static void chkParam(int idx) {
  if (ntGetType(idx)!=curType || ntGetDim(idx)!=curDim)
    error("引数が以前の宣言と異なる");
}

// 関数宣言の引数リスト( 'f( ... )'の ... 部分)を読み込む
static void getParams(int idx) {
  int lastTok = ',';                           // '...'の前は ',' でなきゃ
  for(curCnt=-1;isType();curCnt=curCnt-1){     // 型名の間,オフセットを変化
    getType();                                 // 型を読む
    if (idx>=0) chkParam(idx-curCnt);          // プロトタイプ宣言があれば比較
    getName(false);                            // 仮引数名を登録
    lastTok = tok;                             // ',' があったか記録する
    if (!isTok(',')) break;                    // ',' の間繰り返す
  }
  if (lastTok==',' && isTok(LxDOTDOTDOT)) {    // '...' があった場合
    curType = TyDOTDOTDOT;                     // 型は便宜的に DOTDOTDOT
    curDim  = 0;
    if (idx>=0) chkParam(idx-curCnt);          // プロトタイプ宣言があれば比較
    ntDefName("", curScope, TyDOTDOTDOT,       // '...'を関数引数として表に登録
	      0, curCnt, false);
  }
  chkTok(')', "引数リストが ')' で終わっていない");
}


// 関数を読み込む(プロトタイプ宣言、定義両方を処理する)
static void getFunc(void) {
  funcIdx = ntGetSize() - 1;       // 処理中の関数(return型のチェックに使用)
  int idx = ntSrcGlob(funcIdx);              // 同じ名前のものはないか
  if (idx>=0 && (ntGetScope(idx)==ScCOMM || ntGetScope(idx)==ScGVAR))
    error("2重定義(以前は変数)");
  if (idx>=0 &&
      (ntGetType(idx)!=curType||ntGetDim(idx)!=curDim||ntGetPub(idx)!=pubFlag))
    error("関数の型が以前の宣言と異なっている");
  if (curType==TyINTR && curDim!=0)
    error("interrupt型の配列は認められない");
  getTok();                                  // '(' を読み飛ばす
  lastStat = LxNONTOK;                       // 最後の文(returnで終わっているか)
  curScope = ScLVAR;                         // 仮引数、ローカル変数のスコープ
  int prmIdx = ntGetSize();                  // 仮引数の先頭を記憶
  getParams(idx);                            // 仮引数リストを読み込む
  int prmCnt = ntGetSize()-prmIdx;           // 仮引数の個数を計算
  if (ntGetType(funcIdx)==TyINTR && prmCnt!=0) // 割込み関数は引数を持てない
    error("interrupt関数は引数を持てない");
  ntSetCnt(funcIdx, prmCnt);                 // 関数に引数の個数を記録
  if (idx>=0 && ntGetCnt(idx)!=ntGetCnt(funcIdx))
    error("引数の数が以前の宣言と異なっている");
  int locIdx = ntGetSize();                  // ローカル変数の先頭を記録
  if (isTok('{')) {                          // 関数本体がある場合
    if (idx>=0 && ntGetScope(idx)!=ScPROT) error("関数の2重定義");
    if (idx>=0) ntSetScope(idx, ScFUNC);     // 以前の宣言を定義に変更
    ntSetScope(funcIdx, ScFUNC);             // 今回の宣言を定義に変更
    maxCnt = 0;                              // 最大の変数番号
    curCnt = 0;                              // 次出現変数の番号
    getBlock();                              // 関数本体を読み込む
    if ((ntGetType(funcIdx)!=TyVOID ||       // void型の関数,
	 ntGetDim(funcIdx)!=0)      &&       //
	ntGetType(funcIdx)!=TyINTR  &&       // interupt型以外の関数が
	lastStat!=LxRETURN)                  // return文で終わっていない
      error("関数が値を返していない");
    //syPrintTree();                         // ### デバッグ用 ###
    if (optFlag) optTree(syGetRoot());       // 木を最適化する
    //syPrintTree();                         // ### デバッグ用 ###
    genFunc(funcIdx, maxCnt, krnFlag);       //   コード生成
    syClear(0);                              // コード生成終了で木を消去する
  } else {                                   // プロトタイプ宣言の場合
    chkTok(';', "プロトタイプ宣言が ';' で終わっていない");
    genProto(funcIdx);                       // プロトタイプ宣言を出力
  }                                          // (トランスレータ版だけで必要)
  if (idx>=0) {                              // 既に名前表にあれば
    ntUndefName(funcIdx);                    //   全体を削除
  } else {                                   // そうでなければ
    ntUndefName(locIdx);                     // シグネチャーだけ残す
    for (int i=prmIdx; i<locIdx; i=i+1)      // その際、仮引数は検索対象に
      ntSetScope(i, ScVOID);                 // ならないスコープに変更する．
  }
}

/*
 * グローバル変数の処理
 */
static int getGArrayInit(int dim);           // 再帰呼出があるので宣言必要

// 初期化に使用される定数式を読み込む
static int getCnst(int typ) {
  struct watch *w = newWatch();
  getAsExpr(w);                              // 初期化式にはカンマ式不可
  chkCmpat(w, typ, 0);                       // 初期化(代入)できるかチェック
  int tree = w->tree;                        // 定数式の木を取り出す
  freeWatch(w);                              // 式(w)は役目を終えた
  optTree(tree);                             // 定数式を計算する
  int ty = syGetType(tree);
  if (ty!=SyCNST && ty!=SyLABL && ty!=SySTR)
    error("定数式が必要");
  return tree;
}

// array(n1 [, n2] ...) を読み込む */
static int getArray0(int dim) {
  if (dim<=0) error("array の次元が配列の次元を超える");
  int node = getCnst(TyINT);                 // 整数定数式を読み込む
  if (isTok(',')) {                          // ',' が続くなら
    int lVal = getArray0(dim - 1);           //   ','の右側を先に読み
    node = syCatNode(lVal, node);            //   リストの左につなぐ
  }
  return node;
}

static int getArray(int dim) {
  chkTok('(', "array の次に '(' がない");
  int node = getArray0(dim);
  chkTok(')', "array の後に ')' がない");
  int d = 0;
  if (curType==TyCHAR || curType==TyBOOL)    // 配列ならバイト単位にする
    d = dim;
  return syNewNode(SyARRY, node, d);         // 構造体と配列と同じ扱い
}

// 構造体初期化('{ ... }'を読み込む
static int getStructInit0() {
  int node = SyNULL;
  int i=-curType+1;                          // i が構造体フィールドを指す
  do {
    if (i>=ntGetSize()||ntGetScope(i)!=ScVOID)
      error("構造体初期化がフィールドより多い");
    int n = 0;
    if (ntGetType(i)<=0) {                   // フィールドが構造体の場合
      chkTok(LxNUL, "入れ子構造体の初期化は null だけ");
      n = syNewNode(SyCNST, 0, SyNULL);
    } else if (ntGetDim(i)>0) {              // フィールドが配列の場合
      if (isTok(LxSTRING)) {                 // 入力が文字列なら
	if (ntGetDim(i)!=1||ntGetType(i)!=TyCHAR)
	  error("文字列で初期化できない");
	int lab = genStr(lxGetStr());        // .Lx STRING "..." を出力
	n = syNewNode(SySTR, lab, SyNULL);
      } else if (isTok(LxNUL)) {             // 配列を null で初期化
	n = syNewNode(SyCNST, 0, SyNULL);
      } else error("構造体フィールドの配列初期化は文字列か null だけ");
    } else if (ntGetType(i)==TyINT) {        // フィールドが整数型の場合
      n = getCnst(TyINT);                    //   整数定数式を読み込む
    } else if (ntGetType(i)==TyBOOL) {       // フィールドが論理型の場合
      n = getCnst(TyBOOL);                   //   論理型定数式を読み込む
    } else if (ntGetType(i)==TyCHAR) {       // フィールドが文字型の場合
      n = getCnst(TyCHAR);                   //   文字型定数式を読み込む
    } else error("バグ...getStructInit0");
    node = syCatNode(node, n);
    i=i+1;                                   // i を進める
  } while (isTok(','));                      // ',' が続く間繰り返す
  if (i<ntGetSize() && ntGetScope(i)==ScVOID)
    error("構造体の初期化がフィールドより少ない");
  return node;
}

// 構造体の初期化
static int getStructInit() {
  int node = SyNULL;
  if (isTok(LxNUL)) {                        // null による初期化の場合
    node = syNewNode(SyCNST, 0, SyNULL);     // 0 を木に登録
  } else {                                   // '{ ... }' による初期化の場合
    chkTok('{', "構造体の初期化が '{' で始まっていない");
    node = getStructInit0();                 // 初期化の内容を読み込む
    node = syNewNode(SyLIST, node, SyNULL);
    chkTok('}', "構造体の初期化が '}' で終わっていない");
  }
  return node;
}

// 配列初期化('{ ... }'の ... を読み込む
static int getGArrayInit0(int dim) {
  int node = SyNULL;                         // 最初はリスト要素が空
  if (dim<=1) {                              // 最後の１次元の場合
    if (curType<=0) {                        //   型が構造体型なら
      do {
	int r = getStructInit();             //     構造体の初期化を読み込む
	node = syCatNode(node, r);           //     リストにつなぐ
      } while (isTok(','));                  //      ',' が続く間、繰り返す
    } else if (curType==TyINT  ||
	       curType==TyCHAR ||
	       curType==TyBOOL) {            //   型が基本型なら
      do {
	int n = getCnst(curType);            //     定数式を読み込む
	node = syCatNode(node, n);           //     リストにつなぐ
      } while (isTok(','));                  //     ',' が続く間、繰り返す
    } else error("バグ...getGArrayInit0");
  } else {                                   // 2次元以上の配列の場合
    do {
      int r = getGArrayInit(dim-1);          //   1次元低い配列の読み込み
      node = syCatNode(node, r);             //   リストにつなぐ
    } while (isTok(','));                    //   ',' が続く間繰り返す
  }
  int d = 0;
  if (curType==TyCHAR || curType==TyBOOL)    // バイト単位の配列にするもの
    d = dim;
  return syNewNode(SyLIST, node, d);         // リストを返す
}

// 大域配列の初期化
static int getGArrayInit(int dim) {
  int node = 0;
  if (isTok('{')) {                          // '{ ... ' の初期化の場合
    node = getGArrayInit0(dim);              // 初期化の本体(リスト)を読み込む
    chkTok('}', "初期化で '}'が不足");       // '}' をチェック
  } else if (isTok(LxSTRING)) {              // 文字列による初期化の場合
    if (curType!=TyCHAR || dim!=1) error("文字列による初期化の型");
    int l = genStr(lxGetStr());              // .L STRING "..." を出力
    node = syNewNode(SySTR, l, SyNULL);      // 文字列を木に登録
  } else if (isTok(LxARRAY)) {               // 'array( ... ) の場合
    node = getArray(dim);                    // array の括弧の中を読み込む
  } else if (isTok(LxNUL)) {                 // null による初期化の場合
    node = syNewNode(SyCNST, 0, SyNULL);     // 定数 0 を木に登録
  } else error("配列の初期化");              // 上のどれでもない入力
  return node;
}

// 大域変数
static void getGVar(void) {
  int curIdx = ntGetSize()-1;                // 処理中の変数
  int idx = ntSrcGlob(curIdx);               // 2重定義の可能性があるか？
  if (isTok('=')) {                          // '='が続けば初期化部分がある
    ntSetScope(curIdx, ScGVAR);              // 初期化された大域変数
    if (curDim>0) {                          // 配列の初期化なら
      getGArrayInit(curDim);                 //   配列の初期化部分 '{ ... }'
    } else if (curType==TyINT ||
	       curType==TyCHAR ||
	       curType==TyBOOL) {            // 基本型の初期化なら
      getCnst(curType);                      //   整数定数式を入力する
    } else if (curType<=0) {                 // 構造体初期化なら
      getStructInit();                       //   構造体の初期化部分 '{ ... }'
    } else error("バグ...getGVar");
    genData(curIdx);                         // 初期化済みデータを生成
    syClear(0);                              // データ生成終了で木を消去する
    if (idx>=0) {                            // 既に登録されていた場合
      if (ntGetScope(idx)!=ScCOMM) error("2重定義"); // コモン以外は2重定義
      ntSetScope(idx, ScGVAR);               // 初期化済データに変更
      ntUndefName(curIdx);                   // 既に登録されているので削除
    }
  } else {                                   // 初期化部分が無い場合
    if (idx>=0) {
      if (ntGetScope(idx)==ScCOMM || ntGetScope(idx)==ScGVAR)
	ntUndefName(curIdx);                 // 既に登録されている
      else
	error("2重定義(以前は関数)");
    } else {
      genBss(curIdx);                        // 始めての宣言なら出力する
    }
  }
  if (idx>=0) {                              // 型に矛盾がないかチェック
    if (ntGetType(idx)!=curType||ntGetDim(idx)!=curDim||ntGetPub(idx)!=pubFlag)
      error("変数の型が以前の宣言と異なっている");
  }
}

// 大域変数宣言
static void getGVars(void) {
  getGVar();                                 // 大域変数１件
  while (isTok(',')) {                       // ',' が続いている
    getName(pubFlag);                        // 変数名を表に登録
    getGVar();                               // 大域変数１件
  }
  chkTok(';', "大域変数の宣言が ';' で終わっていない");
}

// C-- プログラムを読む
static void getProg(void) {
  pubFlag = false;                           // 'public'をまだ読んでいない
  if (isTok(LxPUBLIC)) {                     // 'public'修飾子があった
    pubFlag = true;                          //   それを記録し次トークンの処理
  }
  if (isTok(LxSTRUCT)) {                     // 'struct'なら構造体宣言
    curScope = ScSTRC;                       // スコープは構造体(-1)にする
    curType=TySTRUC;                         // 型は STRUC にする
    if (pubFlag) error("\'public struct\'は使えない");
    getName(false);                          // 構造体名を表に登録
    getStruct();                             // 構造体宣言の本体を読み込む
  } else if (isTok(LxTYPEDEF)) {             // 'typedef'なら
    curScope = ScSTRC;                       // スコープは構造体(-1)にする
    curType = TyREF;                         // 何か参照型
    if (pubFlag) error("\'public typedef\'は使えない");
    getName(false);                          // 新たな型名を表に登録
    chkTok(';', "typedef宣言が';'で終わっていない");
  } else {
    curScope = ScPROT;                       // プロトタイプ宣言と仮定する
    getType();                               // 型を読み込む
    getName(pubFlag);                        // プロトタイプ宣言として登録
    if (tok=='(') {                          // '('が続けば関数(仮定は正解)
      getFunc();                             // 関数定義かプロトタイプ宣言
    } else {                                 // 大域変数宣言だと判明
      ntSetScope(ntGetSize()-1, ScCOMM);     // 仮定が外れたので訂正
      curScope = ScCOMM;                     // 仮定が外れたので訂正
      if (curType==TyVOID && curDim==0)      // 変数だとすればvoid型はエラー
	error("void型変数は使用できない");
      if (curType==TyINTR)                   // 変数ならinterrupt型はエラー
	error("interrupt型変数は使用できない");
      getGVars();                            // グローバル変数宣言
    }
  }
}

// フラグを操作する
void snSetOptFlag(boolean f) { optFlag = f; };
void snSetKrnFlag(boolean f) { krnFlag = f; };

// ソースプログラムを読む
void snGetSrc(void) {
  genOn();                                   // コード生成を許可する
  getTok();                                  // 最初の tok を読み込む
  while (tok!=EOF)                           // EOF になるまで
    getProg();                               //   C-- プログラムを処理
}

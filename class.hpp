#ifndef HEADER_H
#define HEADER_H

//#include <windows.h>  // for MS Windows
//#include <GL/glut.h>  // GLUT, include glu.h and gl.h
#include <GLUT/GLUT.h>
#include <math.h>
#include <iostream>
#include <map>
#include <string>
#include <vector>
using namespace std;


/* ビリヤード台に対応したクラス
   クラス内の関数を一通り実行するとビリヤード台が描画される。
   本来クラスを使う必要はないが、自分の学習のためあえてクラスを用いて実装した */
class Table{

 private:
   float size_cloth[3] = {1.0, 0.5, 0.5}; //clothの各辺の比率
   float size_leg[3] = {0.5, 2.0, 0.5};   //脚の各辺の比率
   float frame_width = 0.4;               //frameの幅
   float frame_height = 0.2;              //frameの高さ
   float bigframe_width = 0.1;            //bigframe(frameの外側をさらに囲う板の)厚さ
   float r = 0.15;                        //ポケットの半径
   float edge_width = frame_width + r; 
   double coord_corner[4+75+1][3] = {{0.0, 0.0, 0.0},  {edge_width, 0.0, 0.0},  {edge_width, 0.0, edge_width},  {0.0, 0.0, edge_width}}; //ポケット周辺の図形の各点の座標を格納する配列
   int i, type;
   float color_cloth[4];
   float ft;
   GLfloat black[4] = { 0.0, 0.0, 0.0, 1.0f }; //脚の色
   GLfloat brown[4] = { 0.31, 0.18, 0.11, 1.0f }; //frameの色
   GLfloat lightpos[4] = { -1000.0, 200.0, -500.0, 1.0 }; //光源の位置
   GLfloat spotDirrection[3] = { 0.0, 0.0, 0.0 }; //スポットライトを向ける方向

/* ポケット以外のパーツは、全て全辺1.0の立方体を
      表示したい位置に平行移動(glTranslatef)　→　xyz各方向に拡大or縮小(glScalef)
   した上で描画している。
   rectangularがこの操作を行う関数であり、clothを描く関数(drawing_cloth)やframeを描く関数(drawing_frame)はrectangularを複数回呼び出して描画を行う。
   各パーツにおいて、変数 trans_(パーツ名) は平行移動後の中心の座標、size_(パーツ名) は拡大(縮小)の倍率を表している。
*/
public:
   void before_display(int type_of_table, float ft, float color[]);
   void start_display();

   void rectangular(float trans[], float size[], float color[]);
   void drawing_cloth();
   void drawing_frame();

   void set_corner_coord(); 
   void corner(float trans[], int angle, float color[]);
   void drawing_corner();

   void drawing_legs();
   void end_display();
};

#endif
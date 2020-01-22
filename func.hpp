#include "class.hpp"

Table table;

GLUtesselator *tess;


void TessErr(GLenum error_code)
{
	cout << gluErrorString(error_code) << "\n";
	exit(1);
}

void MakeTess()
{  
	tess = gluNewTess();
	if(tess == NULL){
		cerr << "Can't make tessellator\n";
		exit(1);
	}
	gluTessCallback(tess, GLU_TESS_BEGIN, (void (__stdcall*)(void))glBegin);
	gluTessCallback(tess, GLU_TESS_END, (void (__stdcall*)(void))glEnd);
	gluTessCallback(tess, GLU_TESS_ERROR, (void (__stdcall*)(void))TessErr);
	gluTessCallback(tess, GLU_TESS_VERTEX, (void (__stdcall*)(void))glVertex3dv);
}



/* ------------------------------ここからクラス内関数-------------------------------------- */
/* 標準入力に合わせて変数を設定 */
void Table::before_display(int type_of_table, float size_of_cloth, float color[]){
   type = type_of_table; //OdesseyかRegentか
   ft = size_of_cloth;
   size_cloth[0] *= ft; //7ftだったらclothの大きさをを7倍、8ftだったら8倍、9ftだったら9倍にする
   size_cloth[2] *= ft;
   if(ft !=8) size_cloth[2] += 0.1; //実際の机の採寸西タフガイ、8ftの場合だけy座標を0.1増やす

   for(i = 0; i < 4 ; i++){ //clothの色を標準入力に対応したものに変更
      color_cloth[i] = color[i];
   }
}

void Table::start_display(){
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glMatrixMode(GL_MODELVIEW); 
};

void Table::rectangular(float trans[], float size[], float color[]){
   glLoadIdentity();
   gluLookAt(7.0,5.0,10.0, 0.0,0.0,0.0, 0.0,1.0,0.0); //机が見やすいように視点を調整
   glLightfv(GL_LIGHT0, GL_POSITION, lightpos); //光源の設定
   glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spotDirrection); //光源の向きの設定
   glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color); //質感の設定	
   glTranslatef(trans[0],trans[1],trans[2]); //平行移動
   glScalef(size[0], size[1], size[2]); //xyzの各方向に拡大or縮小
   glutSolidCube(1.0); //一辺1.0の立方体
}

void Table::drawing_cloth(){
   float trans_cloth[3] = { 0.0f, (size_leg[1]+size_cloth[1]) / 2.0 , 0.0f};
   rectangular(trans_cloth, size_cloth, color_cloth);
};

/* frameを4分割し、各フレームの大きさと位置を計算した上で描画 */
void Table::drawing_frame(){
   float size_frame1[3] = {frame_width, frame_height, size_cloth[2] - 2 * (frame_width + r) };
   float trans_frame1[3] = {(size_cloth[0] - size_frame1[0]) /2, size_leg[1] /2 + size_cloth[1] + frame_height /2, 0.0f };
   rectangular(trans_frame1, size_frame1, brown);
   trans_frame1[0] *= (-1);
   rectangular(trans_frame1, size_frame1, brown);

   float size_frame2[3] = {size_cloth[0] - 2 * (frame_width + r), frame_height, frame_width};
   float trans_frame2[3] = {0.0f, size_leg[1] /2 + size_cloth[1] + size_frame2[1] /2, (size_cloth[2] - size_frame2[2]) /2 };
   rectangular(trans_frame2, size_frame2, brown);
   trans_frame2[2] *= (-1);
   rectangular(trans_frame2, size_frame2, brown);

   float size_bigframe1[3] = {bigframe_width, size_cloth[1] + frame_height, size_cloth[2] - bigframe_width * 2};
   float trans_bigframe1[3] = {(size_cloth[0] + bigframe_width) /2, (size_leg[1] + size_cloth[1] + frame_height) /2, 0.0f };
   rectangular(trans_bigframe1, size_bigframe1, brown);
   trans_bigframe1[0] *= (-1);
   rectangular(trans_bigframe1, size_bigframe1, brown);

   float size_bigframe2[3] = {size_cloth[0] + bigframe_width * 2, size_cloth[1] + frame_height, bigframe_width};
   float trans_bigframe2[3] = {0.0f, (size_leg[1] + size_cloth[1] + frame_height) /2, (size_cloth[2] + bigframe_width) /2 };
   rectangular(trans_bigframe2, size_bigframe2, brown);
   trans_bigframe2[2] *= (-1);
   rectangular(trans_bigframe2, size_bigframe2, brown);
}

/* 正方形からくり抜くポケットの座標を計算する関数。
   ポケットは扇形のため、円を100分割したうちの75個を座標として用いる。 */
void Table::set_corner_coord(){
   double rate;
   int PART = 100;
   int three_quarter = PART * 0.75;
   for (i = 0; i < three_quarter ; i++) {
      rate = (double)i / PART;
      coord_corner[i+4][0] = edge_width - r + (-1.0) * r * sin(2.0 * M_PI * rate);
      coord_corner[i+4][1] = 0;
      coord_corner[i+4][2] = edge_width - r + r * cos(2.0 * M_PI * rate);
   }
   for (i = 0; i < 3; i++){ 
      coord_corner[79][i] = coord_corner[2][i]; 
   }
}

/* tessellationを利用し、正方形からポケット部分をくりぬいた図形を描画する関数 */
void Table::corner(float trans[], int angle, float color[]){
   glLoadIdentity();
   glDisable(GL_LIGHTING);
   glDisable(GL_LIGHT0);

   glColor3f(color[0],color[1],color[2]);
   gluLookAt(7.0,5.0,10.0, 0.0,0.0,0.0, 0.0,1.0,0.0); 
   glTranslatef(trans[0], trans[1], trans[2]);
   glRotatef(angle, 0, 1, 0);
	gluTessProperty(tess, GLU_TESS_WINDING_RULE, GLU_TESS_WINDING_ODD);
	gluTessBeginPolygon(tess, 0);
		gluTessBeginContour(tess);
			gluTessVertex(tess, coord_corner[0], coord_corner[0]);
			gluTessVertex(tess, coord_corner[1], coord_corner[1]);
			gluTessVertex(tess, coord_corner[2], coord_corner[2]);
			gluTessVertex(tess, coord_corner[3], coord_corner[3]);
		gluTessEndContour(tess);

		gluTessBeginContour(tess);
         for (i = 0; i < 76; i++) {
            gluTessVertex(tess, coord_corner[i+4], coord_corner[i+4]);
         }
		gluTessEndContour(tess);
	gluTessEndPolygon(tess);

   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
}

/* corner関数を呼び出しポケット周辺を描画 */
void Table::drawing_corner(){
   float trans_corner_1[3] = { (-1.0) * size_cloth[0] /2,  size_leg[1] /2.0 + size_cloth[1] + frame_height, (-1.0) * size_cloth[2]/2.0 };
   float trans_corner_2[3] = { trans_corner_1[0],  trans_corner_1[1], (-1) * trans_corner_1[2] };
   float trans_corner_3[3] = { (-1) * trans_corner_1[0], trans_corner_1[1], (-1) * trans_corner_1[2] };
   float trans_corner_4[3] = { (-1) * trans_corner_1[0], trans_corner_1[1],        trans_corner_1[2] };
   corner(trans_corner_1, 0, brown);
   corner(trans_corner_2, 90, brown);
   corner(trans_corner_3, 180, brown);
   corner(trans_corner_4, 270, brown);
}

void Table::drawing_legs(){
   float coord_leg_x = (size_cloth[0] - size_leg[0]) /2.0 - frame_width ;
   float coord_leg_z = (size_cloth[2] - size_leg[2]) / 2.0 - frame_width;
   float trans_leg1[3] = {        coord_leg_x, 0.0f,        coord_leg_z};
   float trans_leg2[3] = { -1.0 * coord_leg_x, 0.0f,        coord_leg_z};
   float trans_leg3[3] = { -1.0 * coord_leg_x, 0.0f, -1.0 * coord_leg_z};
   float trans_leg4[3] = {        coord_leg_x, 0.0f, -1.0 * coord_leg_z};

   rectangular(trans_leg1, size_leg, black);
   rectangular(trans_leg2, size_leg, black);
   rectangular(trans_leg3, size_leg, black);
   rectangular(trans_leg4, size_leg, black);

   if(ft != 7 and type != 0){ //机のタイプがRegentかつ8ft以上の場合、脚が6本となるため追加で描画
      trans_leg1[0] = 0.0f;
      trans_leg3[0] = 0.0f;
      rectangular(trans_leg1, size_leg, black);
      rectangular(trans_leg3, size_leg, black);
   }
   if(type == 0){ //机のタイプがOdesseyの場合、y方向に脚が連結されるため、その部分を描画
      float size_odyssey_leg[3] = {size_leg[0], size_leg[0], size_cloth[2] - 2.0 * (frame_width + size_leg[2])};
      float trans_odyssey_leg[3] = {trans_leg1[0], -1.0 * size_leg[1] /2 + size_odyssey_leg[1] /2, 0.0f};
      rectangular(trans_odyssey_leg, size_odyssey_leg, black);
      trans_odyssey_leg[0] *= -1.0;
      rectangular(trans_odyssey_leg, size_odyssey_leg, black);
   }
   
};

void Table::end_display(){
    glutSwapBuffers();
};
/* ------------------------------ここまでクラス内関数-------------------------------------- */



void initGL() {
   glClearColor(0.5f, 0.5f, 0.5f, 0.0f); // 背景のカラーを設定（デフォルトではグレー）
   MakeTess();
   glClearDepth(1.0f);                   // 背景の透明度の設定
   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   glEnable(GL_DEPTH_TEST); 
   glDepthFunc(GL_LEQUAL);    
   glShadeModel(GL_SMOOTH);   
}

/* windowへの描画処理をここでまとめている */
void display(void){
   table.start_display();
   table.drawing_cloth();
   table.drawing_frame();
   table.set_corner_coord();
   table.drawing_corner();
   table.drawing_legs();
   table.end_display();
}

void reshape(GLsizei width, GLsizei height) { 
   if (height == 0) height = 1;           
   GLfloat aspect = (GLfloat)width / (GLfloat)height;
 
   glViewport(0, 0, width, height);
 
   glMatrixMode(GL_PROJECTION); 
   glLoadIdentity();             

   gluPerspective(45.0, aspect, 1.0, 1000.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

void idle(void)
{
 glutPostRedisplay();
}
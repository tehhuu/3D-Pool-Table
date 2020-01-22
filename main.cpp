#include "class.hpp"
#include "func.hpp"

string color_names[] = {
   "green", "royal_blue", "burgundy", "black", "red", "purple", 
   "slate_blue", "electric_blue", "source", "navy_blue", "charcoa", "gray"
   };

/* color_names[]内の各色に対応するベクトルを格納 */
float color_rgb[][4] = {
   { 0.00f, 1.00f, 0.00f, 1.0f },
   { 0.00f, 0.00f, 1.00f, 1.0f },
   { 0.70f, 0.09f, 0.19f, 1.0f },
   { 0.00f, 0.00f, 0.00f, 1.0f },
   { 1.00f, 0.10f, 0.10f, 1.0f },
   { 0.70f, 0.00f, 0.70f, 1.0f },
   { 0.03f, 0.17f, 0.36f, 1.0f },
   { 0.39f, 0.60f, 0.76f, 1.0f },
   { 0.09f, 0.19f, 0.19f, 1.0f },
   { 0.10f, 0.10f, 0.19f, 1.0f },
   { 0.26f, 0.27f, 0.36f, 1.0f },
   { 0.80f, 0.80f, 0.80f, 1.0f },
};

char title[] = "3D Shapes"; //表示ウインドウの名前


int main(int argc, char** argv) {
   float ft;
   int type;
   cout << "select the type of table(0 : Odyssey or 1 : Regent)"<<endl; //タイプを入力から受け取る。
	cin >> type;
   cout << "select the size of cloth(7 or 8 or 9)"<<endl; //大きさを入力から受け取る。
	cin >> ft;
   if(type > 1 or 7 > ft or 9 < ft){
      cout << "Error : Choose the correct number"<<endl;  //予想外の入力の場合はエラーを表示。
      exit(0);
   }
   map <string, int> colors;
   string color;
	cout << "select the color of cloth (Choose from below colors)"<<endl;　//clothの色を入力から受け取る。
   cout << "[ ";
   for(int i = 0 ; i < sizeof(color_names)/sizeof(*color_names) ; i++){
      cout << color_names[i] << "  ";
      colors[color_names[i]] = i;    //各々の色に番号をふる 
   }
   cout << "]" << endl;
	cin >> color;
   table.before_display(type, ft, color_rgb[colors[color]]);

   glutInit(&argc, argv);            // Initialize GLUT
   glutInitDisplayMode(GLUT_RGBA| GLUT_DOUBLE | GLUT_DEPTH);
   glutInitWindowSize(640, 480);   // Set the window's initial width & height
   glutInitWindowPosition(50, 50); // Position the window's initial top-left corner
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
   glutCreateWindow(title);          // Create window with the given title
   glutDisplayFunc(display);       // Register callback handler for window re-paint event
   glutReshapeFunc(reshape);       // Register callback handler for window re-size event
   glutIdleFunc(idle);
   initGL();                       // Our own OpenGL initialization
   glutMainLoop();                 // Enter the infinite event-processing loop
   return 0;
}

#include <math.h>
#include <GL/glut.h>
#include<cstdlib>


#define VIEW_WIDTH 4
#define VIEW_HEIGHT 4

double angle = 0;

double windowWidth = 800.0;
double windowHeight = 600.0;

bool translateOn = false;
bool rotateOn = false;

double rotSpeed = 0;

float transX = 0,transY = 0,transZ = 0;
float axisX = 0,axisY = 0,axisZ = 0;
float rotAngle = 0;

GLfloat vertices[] = {
	1.214124,0.000000,1.589309,
	0.375185,1.154701,1.589309,
	-0.982247,0.713644,1.589309,
	-0.982247,-0.713644,1.589309,
	0.375185,-1.154701,1.589309,
	1.964494,0.000000,0.375185,
	0.607062,1.868345,0.375185,
	-1.589309,1.154701,0.375185,
	-1.589309,-1.154701,0.375185,
	0.607062,-1.868345,0.375185,
	1.589309,1.154701,-0.375185,
	-0.607062,1.868345,-0.375185,
	-1.964494,0.000000,-0.375185,
	-0.607062,-1.868345,-0.375185,
	1.589309,-1.154701,-0.375185,
	0.982247 ,0.713644,-1.589309,
	-0.375185,1.154701,-1.589309,
	-1.214124,0.000000,-1.589309,
	-0.375185,-1.154701,-1.589309,
	0.982247,-0.713644,-1.589309
};
GLbyte colors[60];
GLint indices[] = {
	0 ,1 ,2 ,3 ,4,
	0 ,5, 10, 6, 1,
	1 ,6, 11, 7, 2,
	2 ,7, 12, 8, 3,
	3 ,8, 13, 9, 4,
	4 ,9, 14, 5, 0,
	15, 10, 5, 14, 19,
	16, 11, 6, 10 ,15,
	17, 12, 7, 11 ,16,
	18, 13, 8, 12 ,17,
	19, 14, 9, 13, 18,
	19, 18, 17, 16, 15
};

void init(void);
void initMenu();
void initView();
void initColor();
void init(void){
	glutInitDisplayMode( GLUT_DEPTH | GLUT_SINGLE | GLUT_RGB);
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	int orthoWidth = 5.0;
	glOrtho(-orthoWidth, orthoWidth, -orthoWidth, orthoWidth, -orthoWidth, orthoWidth);
	glMatrixMode(GL_MODELVIEW);
	//initMenu();
	initView();
	initColor();

}

void menuHandler(int value){
	switch(value){
	case 1:
		//todo
		break;
	case 2:
		//todo
		break;
	case 3:
		exit(0);
		break;
	default:
		break;
	}
}

void initMenu(){
	glutCreateMenu(menuHandler);
	glutAddMenuEntry("Wire Mode",1);
	glutAddMenuEntry("Not Wire Mode",2);
	glutAddMenuEntry("Exit",3);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}


void initView(){
	
}

void initColor(){
	for(int i = 0 ; i < 60 ; i++){
		colors[i] = rand()%200+44;
	}
}

void drawPolygon(){
	for(int i = 0 ; i < 12 ; i++){
		glBegin(GL_POLYGON);
		for(int j = 0 ; j < 5 ; j++ ){
			int vertexIndex = indices[i * 5 + j];
			glColor3b(colors[vertexIndex*3+0],colors[vertexIndex*3+1],colors[vertexIndex*3+2]);
			glVertex3f(vertices[vertexIndex*3+0],vertices[vertexIndex*3+1],vertices[vertexIndex*3+2]);
		}
		glEnd();
	}
}

void display(){
	glClearColor(0.0,0.0,0.0,1.0);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	//glCullFace(GL_FRONT_AND_BACK);
	//glEnableClientState(GL_VERTEX_ARRAY);
	//glEnableClientState(GL_COLOR_ARRAY);
	glLoadIdentity();

	glTranslatef(transX,transY,transZ);
	glTranslatef(0,0,10);
	//gluLookAt(0,0,-20.0,0,0,0,0,1,0);
	if((axisX!=0||axisY!=0||axisZ!=0)){
		glRotated(rotAngle,axisX,axisY,axisZ);
	}


	//glutSolidCube(1.0);
	
	glEnable(GL_CULL_FACE);

	drawPolygon();
	
	//glDisableClientState(GL_COLOR_ARRAY);
	//glDisableClientState(GL_VERTEX_ARRAY);
	glPopMatrix();
	glutSwapBuffers();
	glFlush();
}

void reshape(int m, int n){
	glViewport(0,0,m,n);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//int orthoWidth = 5.0;
	//glOrtho(-orthoWidth, orthoWidth, -orthoWidth, orthoWidth, -orthoWidth, orthoWidth);
	gluPerspective(90,windowWidth/windowHeight,0.1f,100.0);
	glRotated(180,0,1,0);
	glMatrixMode(GL_MODELVIEW);	

	windowWidth = m;
	windowHeight = n;
}

void startTranslate(){
	translateOn = true;	
}
void stopTranslate(){
	translateOn = false;
}
void startRotate(){
	rotateOn = true;
}
void stopRotate(){
	rotateOn = false;
}

void mouse(int key,int state,int x,int y){
	if(key == GLUT_RIGHT_BUTTON){
		switch(state){
		case GLUT_DOWN:
			startTranslate();
			break;
		case GLUT_UP:
			stopTranslate();
			break;
		}
	}
	if(key == GLUT_LEFT_BUTTON){
		switch(state){
		case GLUT_DOWN:
			startRotate();
			break;
		case GLUT_UP:
			stopRotate();
			break;
		default:
			break;
		}
	}
}

void motion(int x,int y){
	static double lastX = x,lastY = y;
	static double transRatio = 0.1;
	static double rotRatio = 0.0001;
	double curX = x;
	double curY = y;

	double deltaX = curX - lastX;
	double deltaY = curY - lastY;

	if(translateOn == true){
		transX -= deltaX * transRatio;
		transY -= deltaY * transRatio;
	}



	if(rotateOn == true){
		rotAngle = int(rotAngle + ( deltaX * deltaX + deltaY * deltaY ) * rotRatio) % 360;

		static int lastTime = 0;
		int currentTime = glutGet(GLUT_ELAPSED_TIME);
		int elapsedTime = currentTime - lastTime;		

		rotSpeed = ( deltaX * deltaX + deltaY * deltaY )/double(elapsedTime) * 0.008;

		axisX = -deltaY;
		axisY = +deltaX;
		axisZ = 0;

		lastTime = currentTime;
	}

	lastX = curX;
	lastY = curY;
}

void idle(){
	static int lastTime = 0;
	int currentTime = glutGet(GLUT_ELAPSED_TIME);
	int elapsedTime = currentTime - lastTime;
	if(elapsedTime > 20){
		rotAngle = ( rotAngle + rotSpeed * elapsedTime ) > 360 ? rotAngle + rotSpeed * elapsedTime : rotAngle + rotSpeed * elapsedTime - 360 ;

		lastTime = currentTime;
		glutPostRedisplay();
	}
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitWindowSize(windowWidth, windowHeight);
	
	init();

	glutCreateWindow("Virtual Tracing Ball --by kelvin zhang");
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutMainLoop();
	system("pause");
}

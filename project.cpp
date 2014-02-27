#include <iostream>
#include <cmath>
#include<cstdlib>
#include<ctime>
#include<cstdio>
#include<string.h>
#include<vector>
#include <GL/glut.h>
using namespace std;

#define PI 3.141592653589
#define DEG2RAD(deg) (deg * PI / 180)


// Function Declarations
void drawScene();
void update(int value);
void drawBox(float len);
void drawBox1(float len);
void drawBox2(float len);
void drawBox3(float len);
void drawBox4(float len);
void drawBox5(float len);
void drawBall(float rad);
void drawLaser(float theta);
void drawellipse();
void drawellipse1();
void drawTriangle();
void initRendering();
double distance(double x1, double y1, double x2, double y2);
void handleResize(int w, int h);
void handleKeypress1(unsigned char key, int x, int y);
void handleKeypress2(int key,int x,int y);
void handleKeypress3(int key,int x,int y);
void dragwithmouse(int x,int y);
void handleKeypress4(int key,int x,int y);
void handleMouseclick(int button, int state, int x, int y);

// Global Variables
float ball_x = 1.0f;
float ball_y = 0.0f;
float ball_velx = 0.01f;
float ball_vely = 0.02f;
float ball_rad = 0.1f;
float box_len = 4.0f;
float tri_x = 0.0f;
float tri_y = 0.0f;
float theta = 90.0f; 
float box_r_x=0.0f;
float box_r_y=0.0f;
float box_g_x=0.0f;
float box_g_y=0.0f;
float cannon_x=0.0f;
float cannon_y=-1.5f;
float ellipse_a=0.25f;
float ellipse_b=0.08f;
float ellipse_x=-1.75f;
float ellipse1_x=1.75f;
float p=0.01f;
int togcan=0;
int toggreen=0;
int togred=0;
int rotcan=0;
float mousex;
float mousey;
time_t current;
time_t past;
int score=0;
int paused=0;
int gameover=0;
int i=0;
int j=0;
int peg=1;
int count=0;
typedef struct spider
{
	float x;
	float y;
	float colour;
	int noob;
}spider;
vector <spider> sp;

typedef struct laser
{
	float x;
	float y;
	float theta;
}laser;

vector <laser> la;


int main(int argc, char **argv) {

	// Initialize GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

	int w = glutGet(GLUT_SCREEN_WIDTH);
	int h = glutGet(GLUT_SCREEN_HEIGHT);
	int windowWidth = w * 2 / 3;
	int windowHeight = h * 2 / 3;
	srand (static_cast <unsigned> (time(0)));

	glutInitWindowSize(windowWidth, windowHeight);
	glutInitWindowPosition((w - windowWidth) / 2, (h - windowHeight) / 2);

	glutCreateWindow("Spider_Game");  // Setup the window
	initRendering();

	// Register callbacks
	glutMotionFunc(dragwithmouse);
	glutDisplayFunc(drawScene);
	glutIdleFunc(drawScene);
	glutKeyboardFunc(handleKeypress1);

	//	glutMouseFunc(handleMouseclick);
	glutReshapeFunc(handleResize);
	glutMouseFunc(handleMouseclick);
	glutTimerFunc(10, update, 0);

	glutMainLoop();
	return 0;
}

// Function to draw objects on the screen
void drawScene() {
	glClearColor(1.0,1.0,1.0,1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();

	// Draw Box
	glTranslatef(0.0f, 0.0f, -5.0f);
	glColor3f(0.0f, 0.0f, 0.0f);
	drawBox(box_len);

	// Draw Box 1
	glColor3f(0.0f, 0.0f, 0.0f);
	drawBox1(box_len);

	//Draw Box 2 and 3
	glPushMatrix();
	glTranslatef(box_r_x, box_r_y, 0.0f);
	glColor3f(1.0f, 0.0f, 0.0f);
	drawBox2(box_len);

	glPopMatrix();

	glPushMatrix();
	glTranslatef(ellipse_x,-1.5f,0.0f);
	glColor3f(1.0f,0.0f,0.0f);
	drawellipse();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(box_g_x, box_g_y, 0.0f);
	glColor3f(0.0f, 1.0f, 0.0f);
	drawBox3(box_len);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(ellipse1_x,-1.5f,0.0f);
	glColor3f(0.0f,1.0f,0.0f);
	drawellipse1();
	glPopMatrix();

	//Draw cannon
	glPushMatrix();
	glTranslatef(cannon_x, cannon_y, 0.0f);
	glRotatef(theta-90.0f, 0.0f, 0.0f, 1.0f);
	glColor3f(0.0f, 0.0f, 1.0f);
	drawBox4(box_len);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(cannon_x, cannon_y, 0.0f);
	glRotatef(theta-90.0f, 0.0f, 0.0f, 1.0f);
	glColor3f(0.0f, 0.0f, 1.0f);
	drawBox5(box_len);
	glPopMatrix();
	int local=0;
	//Score
	for(size_t k=0;k<sp.size();k++)
	{
		if((sp[k].y<=1.5f)&&(sp[k].noob==1))
			{
			local++;
			count=max(count,local);
			}
	}	

	glPushMatrix(); 
	char text[11]; 
	sprintf(text,"Score: %d",score-5*count); 
	glTranslatef(0.0f,0.0f,0.0f ); 
	glColor3f(1.0f,0.0f,1.0f);
	glRasterPos2f(2.5f,0.0f); 
	for(unsigned int i=0;i<strlen(text);i++) 
	{
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,text[i]); 
	}
	glPopMatrix();
	
	// Draw Ball
	time(&current);
	if(paused==0)
	{
	if(difftime(current,past)>=3.0)
	{
		spider limb;
		limb.x=((rand() % 4 + (-2))*1.0)*(rand()%45)/50;
		limb.y=box_len/2*1.0;
		limb.colour=rand()%3*1.0;
		limb.noob=0;
		sp.push_back(limb);
		past=current;
	}}
	
	
	for(size_t i=0;i<sp.size();i++)
	{
		glPushMatrix();
		glTranslatef(sp[i].x, sp[i].y, 0.0f);
		if(sp[i].colour==0.0)
			glColor3f(1.0f, 0.0f, 0.0f);
		if(sp[i].colour==1.0)
			glColor3f(0.0f, 1.0f, 0.0f);
		if(sp[i].colour==2.0)
			glColor3f(0.0f, 0.0f, 0.0f);
		drawBall(ball_rad);



		glPopMatrix();

	}




	//Draw Laser
	for(size_t i=0;i<la.size();i++)
	{	
		


		glPushMatrix();
		glTranslatef(la[i].x, la[i].y, 0.0f);
		glRotatef(la[i].theta-90.0f, 0.0f, 0.0f, 1.0f);
		glColor3f(1.0f, 0.0f, 0.0f);
		drawLaser(la[i].theta);
		glPopMatrix();

	}


	glPopMatrix();

	glutSwapBuffers();
}

// Function to handle all calculations in the scene
// updated evry 10 milliseconds
void update(int value) {
	if(paused==0)
	{



		for(size_t i=0;i<la.size();i++)
		{
			if(la[i].y+0.4f*(sin(DEG2RAD(la[i].theta)))>box_len/2)
			{
				la.erase(la.begin()+i);
				continue;
			}



			if((la[i].x+0.4f*(cos(DEG2RAD(la[i].theta))))>2.0f || (la[i].x+0.4f*(cos(DEG2RAD(la[i].theta))))<-2.0f)
			{
				la[i].x=la[i].x+0.4f*(cos(DEG2RAD(la[i].theta)));
				la[i].y=la[i].y+0.4f*(sin(DEG2RAD(la[i].theta)));
				la[i].theta=180.0f-la[i].theta;

			}

			else
			{
				la[i].x+=0.01f*(cos(DEG2RAD(la[i].theta)));
				la[i].y+=0.01f*(sin(DEG2RAD(la[i].theta)));
				for(size_t j=0;j<sp.size();j++)
				{
					if(((la[i].x+0.4f*(cos(DEG2RAD(la[i].theta)))-sp[j].x)*(la[i].x+0.4f*(cos(DEG2RAD(la[i].theta)))-sp[j].x)+(la[i].y+0.4f*(sin(DEG2RAD(la[i].theta)))-sp[j].y)*(la[i].y+0.4f*(sin(DEG2RAD(la[i].theta)))-sp[j].y)<=(0.1f)*(0.1f)))
					{
						if(sp[i].colour==2.0)
						{
							score++;
							
						}
						sp.erase(sp.begin()+j);
					}

				}

			}
		}


		for(size_t i=0;i<sp.size();i++)
		{

			if((sp[i].x<=cannon_x+0.1f)&&(sp[i].x>=cannon_x-0.1f)&&(sp[i].y-ball_rad<=-1.49f))
			{
				exit(0);
			}
			if((sp[i].x>=box_r_x-2.0f)&&(sp[i].x<=box_r_x-1.5f)&&(sp[i].colour==0.0)&&(sp[i].y>=-1.5f)&&(sp[i].y<=-1.4f))
			{
				sp.erase(sp.begin()+i);
				score++;
			}
			if((sp[i].x>=box_r_x-2.0f)&&(sp[i].x<=box_r_x-1.5f)&&(sp[i].colour!=0.0)&&(sp[i].y<=-1.5f)&&(sp[i].y<=-1.4f))
			{
				sp.erase(sp.begin()+i);
				score--;
			}
			if((sp[i].x<=box_g_x+2.0f)&&(sp[i].x>=box_g_x+1.5f)&&(sp[i].colour==1.0)&&(sp[i].y<=-1.5f)&&(sp[i].y<=-1.4f))
			{
				sp.erase(sp.begin()+i);
				score++;
			}
			if((sp[i].x<=box_g_x+2.0f)&&(sp[i].x>=box_g_x+1.5f)&&(sp[i].colour!=1.0)&&(sp[i].y<=-1.5f)&&(sp[i].y<=-1.4f))
			{
				sp.erase(sp.begin()+i);
				score--;
			}




		
		time(&current);
		if((difftime(current,past)>20.0f))
		{
			p=p*2;
			past=current;
		}
		
		

			if(sp[i].y>=-1.5f)
			{
				sp[i].y-=p;
			}
			if(sp[i].y<=-1.5f)
			{
				sp[i].noob=1;
			}


		}	
				

		


	}





	glutTimerFunc(10, update, 0);
}

void drawBox(float len) {

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_QUADS);
	glVertex2f(-len / 2, -len / 2);
	glVertex2f(len/2, -len / 2);
	glVertex2f(len/2, len/2);
	glVertex2f(-len / 2, len/2);
	glEnd();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void drawBox1(float len) {

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_QUADS);
	glVertex2f(-len / 2, -len / 2);
	glVertex2f(len / 2, -len / 2);
	glVertex2f(len / 2, -1.5f);
	glVertex2f(-len / 2,-1.5f);
	glEnd();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void drawBox2(float len) {

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_QUADS);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex2f(-len/2, -len/2);
	glVertex2f(-1.5f, -len / 2);
	glVertex2f(-1.5f, -1.5f);
	glVertex2f(-len/2,-1.5f);
	glEnd();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void drawBox3(float len) {

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_QUADS);
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex2f(1.5f, -len / 2);
	glVertex2f(len/2, -len / 2);
	glVertex2f(len/2, -1.5f);
	glVertex2f(1.5f,-1.5f);
	glEnd();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void drawBox4(float len)
{
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_QUADS);
	glVertex2f(-0.2f , -0.3f);
	glVertex2f(0.2f, -0.3f);
	glVertex2f(0.1f, 0.0f);
	glVertex2f(-0.1f,0.0f);
	glEnd();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void drawBox5(float len)
{
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_QUADS);
	glVertex2f(-0.03f , 0.0f);
	glVertex2f(0.03f, 0.0f);
	glVertex2f(0.03f, 0.2f);
	glVertex2f(-0.03f,0.2f);
	glEnd();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}


void drawellipse()
{
	glBegin(GL_TRIANGLE_FAN);
	for(int i=0 ; i<360 ; i++) {
		//glVertex2f(rad * cos(DEG2RAD(i)), rad * sin(DEG2RAD(i)));
		glVertex2f(ellipse_a * cos(DEG2RAD(i)), ellipse_b * sin(DEG2RAD(i)));

	}
	glEnd();

}
void drawellipse1()
{
	glBegin(GL_TRIANGLE_FAN);
	for(int i=0 ; i<360 ; i++) {
		// glVertex2f(rad * cos(DEG2RAD(i)), rad * sin(DEG2RAD(i)));
		glVertex2f(ellipse_a * cos(DEG2RAD(i)), ellipse_b * sin(DEG2RAD(i)));

	}
	glEnd();
}
void drawBall(float rad) {

	glBegin(GL_TRIANGLE_FAN);
	for(int i=0 ; i<360 ; i++) {
		glVertex2f(rad * cos(DEG2RAD(i)), rad * sin(DEG2RAD(i)));
		//glVertex2f(ellipse_a * cos(DEG2RAD(i)), ellipse_b * sin(DEG2RAD(i)));

	}

	glEnd();
}

void drawLaser(float theta)
{
	glBegin(GL_LINES);
	glVertex2f(0.0f,0.0f);
	glVertex2f(0.0f,0.4f);
	//glVertex2f(0.5*cos(DEG2RAD(theta)),0.5*(sin(DEG2RAD(theta))));
	glEnd();

}

// Initializing some openGL 3D rendering options
void initRendering() {

	glEnable(GL_DEPTH_TEST);        // Enable objects to be drawn ahead/behind one another
	glEnable(GL_COLOR_MATERIAL);    // Enable coloring
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);   // Setting a background color
}

// Function called when the window is resized
void handleResize(int w, int h) {

	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (float)w / (float)h, 0.1f, 200.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void handleKeypress1(unsigned char key, int x, int y) {
	
	if ((key == 27)||(key =='q')) {
		exit(0);     // escape key is pressed
	}
	if(paused==0)
	{
	if(key == 'r')
	{	
		peg=1;
		glutSpecialFunc(handleKeypress2);
	}
	if(key == 'g')
	{	
		peg=1;
		glutSpecialFunc(handleKeypress3);
	}
	if(key == 'b')
	{
		peg=2;
		glutSpecialFunc(handleKeypress4);
	}
	if((key == ' ')&&(peg==2))
	{
		if(difftime(current,past)>=0.8)
		{
			laser p;
			p.x=cannon_x;
			p.y=cannon_y;
			p.theta=theta;
			la.push_back(p);
			past=current;
		}
	}
	}



	if(key == 'p')
	{
		if(paused==0)
		{
			paused=1;
		}
		else
		{
			paused=0;
		}
	}
}

void handleKeypress2(int key,int x,int y) {
	if(paused==0)
	{
	int bag;
	if ((key == GLUT_KEY_LEFT)&&(box_r_x-box_len/2>-2.0f))
	{
		bag=0;

		for(size_t i=0;i<sp.size();i++)
		{
			//      cout<<sp[i].y<<" "<<sp[i].x<<" "<<cannon_x<<endl;
			//      cout<<abs(sp[i].x-cannon_x)<<" "<<0.15+ball_rad<<endl;
			if((sp[i].y<=-1.50f)&&(-sp[i].x+(box_r_x-2.0f)<=ball_rad)&&((box_r_x-2.0f)-sp[i].x>0))
			{
				bag=1;

				break;
			}




		}
		if(bag==0)
		{
			box_r_x-=0.05;

			ellipse_x-=0.05;

		}

	}

	if ((key == GLUT_KEY_RIGHT)&&(box_r_x-1.5f<2.0f))
	{

		
			bag=0;

			for(size_t i=0;i<sp.size();i++)
			{
				//      cout<<sp[i].y<<" "<<sp[i].x<<" "<<cannon_x<<endl;
				//      cout<<abs(sp[i].x-cannon_x)<<" "<<0.15+ball_rad<<endl;
				if((sp[i].y<=-1.50f)&&(sp[i].x-(box_r_x-1.5f)<=ball_rad)&&(-(box_r_x-1.5f)+sp[i].x>0))
				{
					bag=1;

					break;
				}




			}
			if(bag==0)
			{	

				box_r_x+=0.05;
				ellipse_x+=0.05;

			}
		
	}}

}

void handleKeypress3(int key,int x,int y) {
	if(paused==0)
	{
	int bag;
	if ((key == GLUT_KEY_LEFT))
	{	if(1.5f+box_g_x>-box_len/2)
		{

			bag=0;

			for(size_t i=0;i<sp.size();i++)
			{
				//      cout<<sp[i].y<<" "<<sp[i].x<<" "<<cannon_x<<endl;
				//      cout<<abs(sp[i].x-cannon_x)<<" "<<0.15+ball_rad<<endl;
				if((sp[i].y<=-1.50f)&&(-sp[i].x+(box_g_x+1.5f)<=ball_rad)&&((box_g_x+1.5f)-sp[i].x>0))
				{
					bag=1;

					break;
				}




			}
			if(bag==0)
			{
				box_g_x-=0.05;
				ellipse1_x-=0.05;

			}

		}
	}

	if ((key == GLUT_KEY_RIGHT)&&(box_g_x+box_len/2<box_len/2))
	{
		bag=0;

		for(size_t i=0;i<sp.size();i++)
		{
			//      cout<<sp[i].y<<" "<<sp[i].x<<" "<<cannon_x<<endl;
			//      cout<<abs(sp[i].x-cannon_x)<<" "<<0.15+ball_rad<<endl;
			if((sp[i].y<=-1.50f)&&(sp[i].x-(box_g_x+2.0f)<=ball_rad)&&(-(box_g_x+2.0f)+sp[i].x>0))
			{
				bag=1;

				break;
			}




		}
		if(bag==0)
		{
			box_g_x+=0.05;
			ellipse1_x+=0.05;

		}

	}}

}






void handleKeypress4(int key,int x,int y) {
	if(paused==0)
	{
	int tag;

	if ((key == GLUT_KEY_LEFT)&&(cannon_x>-1.9f))
	{   
		tag=0;

		for(size_t i=0;i<sp.size();i++)
		{
			//	cout<<sp[i].y<<" "<<sp[i].x<<" "<<cannon_x<<endl;
			//	cout<<abs(sp[i].x-cannon_x)<<" "<<0.15+ball_rad<<endl;
			if((sp[i].y<=-1.50f)&&(-sp[i].x+cannon_x<=0.1f+ball_rad)&&(cannon_x-sp[i].x>0))
			{
				tag=1;

				break;
			}




		}
		if(tag==0)
		{
			cannon_x-=0.05;

		}


	}



	if ((key == GLUT_KEY_RIGHT)&&(cannon_x<1.9f))
	{	
		tag=0;

		for(size_t i=0;i<sp.size();i++)
		{
			//      cout<<sp[i].y<<" "<<sp[i].x<<" "<<cannon_x<<endl;
			//      cout<<abs(sp[i].x-cannon_x)<<" "<<0.15+ball_rad<<endl;
			if((sp[i].y<=-1.50f)&&(sp[i].x-cannon_x<=0.1f+ball_rad)&&(sp[i].x-cannon_x>0))
			{
				tag=1;

				break;
			}




		}
		if(tag==0)
		{
			cannon_x+=0.05;
		}

	}





	if (key == GLUT_KEY_DOWN)
	{
		if(theta + 7.5<150.0f)
			theta += 15;
		
	}
	else if (key == GLUT_KEY_UP)
	{
		if(theta - 7.5>30.0f)
			theta -= 15;
	}}
}

double distance(double x1, double y1, double x2, double y2)
{
	double x = x1 - x2;
	double y = y1 - y2;
	double dist;

	dist = pow(x,2)+pow(y,2);           //calculating distance by euclidean formula
	dist = sqrt(dist);                  //sqrt is function in math.h

	return dist;
}



void handleMouseclick(int button, int state, int x, int y) {
	if(paused==1&& gameover==1)
		return;
	GLint viewport[4];
		GLdouble modelview[16];
		GLdouble projection[16];
		GLfloat winX, winY, winZ;
		GLdouble posX, posY, posZ;

		glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
		glGetDoublev( GL_PROJECTION_MATRIX, projection );
		glGetIntegerv( GL_VIEWPORT, viewport );

		winX = (float)x;
		winY = (float)viewport[3] - (float)y;

		glReadPixels( x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );

		gluUnProject( winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);
		cout<<"posX : "<<posX<<"  posY : "<<posY<<endl;

		mousex=posX*50.12;
		mousey=posY*50.12;
		cout<<mousex<<"   "<<mousey<<endl;
	if (state == GLUT_DOWN)
	{
		if (button == GLUT_LEFT_BUTTON)
		{
			if(distance(mousex,mousey,cannon_x,cannon_y)<0.2f)
			{
				togcan=1;
				toggreen=0;
				togred=0;
				rotcan=0;

			}
			else if(distance(mousex,mousey,box_g_x+1.75f,box_g_y-1.75f)<0.25f)
			{
				togcan=0;
				toggreen=1;
				togred=0;
				rotcan=0;
			}
			else if(distance(mousex,mousey,box_r_x-1.75f,box_r_y-1.75f)<0.25f)
			{
				togcan=0;
				toggreen=0;
				togred=1;
				rotcan=0;
			}
			else
			{
				togcan=0;
				toggreen=0;
				togred=0;
				rotcan=0;
			}
		}
		if(button == GLUT_RIGHT_BUTTON)
		{
			togcan=0;
			toggreen=0;
			togred=0;
			if(distance(mousex,mousey,cannon_x,cannon_y)<0.2f)
			{
				rotcan=1;
			}
			else
				rotcan=0;
		}
	}
	glutPostRedisplay();
}







void dragwithmouse(int x,int y)
{
	if(paused==1&& gameover==1)
		return;
	float move;
	GLint viewport[4];
		GLdouble modelview[16];
		GLdouble projection[16];
		GLfloat winX, winY, winZ;
		GLdouble posX, posY, posZ;

		glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
		glGetDoublev( GL_PROJECTION_MATRIX, projection );
		glGetIntegerv( GL_VIEWPORT, viewport );

		winX = (float)x;
		winY = (float)viewport[3] - (float)y;

		glReadPixels( x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );

		gluUnProject( winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);


		mousex=posX*50.12;
		mousey=posY*50.12;
	if(togred)
	{
		move=(floor)(2*(mousex-box_r_x+1.75));
		if(move>0 && abs(mousey-box_r_y+1.75)>0.2f)
		{
			box_r_x+=(move*0.1);
			ellipse_x=(move*0.1);
			int flag=0;
			if(box_r_x+0.1f>box_len/2 )
				flag=1;
			for(size_t i=0;i<sp.size();i++)
			{
				if(distance(sp[i].x,sp[i].y,box_r_x-1.75f,box_r_y-1.75f)<0.4f && sp[i].x>=box_r_x-1.75f)
					flag=1;
			}
			if(flag==1){

				box_r_x-=(move*0.1);
				ellipse_x-=(move*0.1);
			}
		}
		else if(move<0 && distance(mousex,mousey,box_r_x-1.75f,box_r_y-1.75f)<0.4)
		{
			box_r_x+=(move*0.1);
			ellipse_x+=(move*0.1);
			int flag=0;
			if(box_r_x-0.1<-box_len/2 )
				flag=1;
			for(size_t i=0;i<sp.size();i++)
			{
				if(distance(sp[i].x,sp[i].y,box_r_x-1.75f,box_r_y-1.75f)<0.4f && sp[i].x<=box_r_x-1.75f)
					flag=1;
			}
			if(flag==1)
			{
				box_r_x-=(move*0.1);
				ellipse_x-=(move*0.1);

			}
		}
	}
	else if(toggreen)
	{
		move=(floor)(2*(mousex+box_g_x-1.75f));
		if(move>0 && distance(mousex,mousey,box_g_x,box_g_y)<0.4)
		{
			box_g_x+=(move*0.1);
			ellipse1_x+=(move*0.1);
			int flag=0;
			if(box_g_x+0.1>box_len/2 )
				flag=1;
			for(size_t i=0;i<sp.size();i++)
			{
				if(distance(sp[i].x,sp[i].y,box_g_x-1.75f,box_g_y-1.75f)<0.4f && sp[i].x>=box_g_x-1.75f)
					flag=1;
			}
			if(flag==1)
			{
				box_g_x-=(move*0.1);
				ellipse1_x-=(move*0.1);
			}
		}
		else if(move<0 && distance(mousex,mousey,box_g_x+1.75f,box_g_y-1.75f)<0.4)
		{
			box_g_x+=(move*0.1);
			ellipse1_x+=(move*0.1);
			int flag=0;
			if(box_g_x-0.1<-box_len/2 )
				flag=1;
			for(size_t i=0;i<sp.size();i++)
			{
				if(distance(sp[i].x,sp[i].y,box_g_x+1.75f,box_g_y-1.75f)<0.4f && sp[i].x<=box_g_x+1.75f)
					flag=1;
			}
			if(flag==1)
			{
				box_g_x-=(move*0.1);
				ellipse1_x-=(move*0.1);
			}
		}
	}
	else if(togcan)
	{
		move=(floor)(2*(mousex-cannon_x));
		if(move>0 && distance(mousex,mousey,cannon_x,cannon_y)<0.8)
		{
			cannon_x+=(move*0.1);
			int flag=0;
			if(cannon_x+0.1>box_len/2 )
				flag=1;
			for(size_t i=0;i<sp.size();i++)
			{
				if(distance(sp[i].x,sp[i].y,cannon_x,cannon_y)<0.8f && sp[i].x>=cannon_x)
					flag=1;
			}
			if(flag==1)
				cannon_x-=(move*0.1);
		}
		else if(move<0 && distance(mousex,mousey,cannon_x,cannon_y)<0.7)
		{
			cout<<"dasfsdf"<<endl;;
			cannon_x+=(move*0.1);
			int flag=0;
			if(cannon_x-0.1<-box_len/2 )
				flag=1;
			for(size_t i=0;i<sp.size();i++)
			{
				if(distance(sp[i].x,sp[i].y,cannon_x,cannon_y)<0.8f && sp[i].x<=cannon_x)
					flag=1;
			}
			if(flag==1)
				cannon_x-=(move*0.1);
		}
	}
	if(rotcan && distance(mousex,mousey,cannon_x,cannon_y)<0.8)
	{
			cout<<"dasfsdf"<<endl;;
		if(cannon_x-mousex>0)
		{
			if(theta + 7.5<150.0f)
				theta += 7.5;
		}
		else
		{

			if(theta - 7.5>30.0f)
				theta -=7.5;
		}
	}

}

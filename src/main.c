#include <stdlib.h>
#include <stdio.h>
#include <GL/gl.h>
#include <GL/freeglut.h>

#define WINDOW_TITLE "Exercice 1"

void initialize(int, char*[]);
void initWindow(int, char*[]);
void resizeFunction(int, int);
void renderFunction(void);
void keyboard(unsigned char, int, int);
void initData(char*);
double interpolatePos(double, double, double, double, double);

int currentWidth = 800;
int currentHeight = 600;
int windowHandle = 0;

double display_size = 0.7;
int display_number = 1;

GLfloat angle_h = 0.0;
int prev_x = -1;

double * datas;
double * x;
double * y;
double iso = 2.5;
int size = 0;

int main(int argc,char* argv[]){
	initialize(argc, argv);

	initData("src/datas");

	glutMainLoop();

	if (datas){
		free(datas);
	}

	exit(EXIT_SUCCESS);
}

void initData(char* path){
	FILE * fp = fopen(path, "r");
	char ch[20];
	fgets(ch, sizeof(ch), fp);
	size = atoi(ch);
	int nb_value = size*size;
	datas = malloc(sizeof(double) * nb_value);
	x = malloc(sizeof(double) * nb_value);
	y = malloc(sizeof(double) * nb_value);

	int i = 0;
	while(fgets(ch, sizeof(ch), fp) && i < nb_value){
		datas[i] = atof(ch);
		i++;
	}
	double step = (display_size*2) / (size-1);
	for (int i = 0 ; i < size ; i++){
		for (int j = 0 ; j < size ; j++){
			x[i*size+j] = step * i - display_size;
 			y[i*size+j] = step * j - display_size;
		}
	}
}

int chooseCase(double v, double hg, double hd, double bg, double bd){
	int higher = 0 ;
	int lower = 0;
	hg > v ? higher++ : lower++;
	hd > v ? higher++ : lower++;
	bg > v ? higher++ : lower++;
	bd > v ? higher++ : lower++;
	if(higher == 0 || lower == 0){
		return 1;
	}else if(higher == 1 || lower == 1){
		return 2;
	}else if(higher == lower){
		if ((hg > v && bd > v)||(bg > v && hd > v)){
			return 3;
		}else{
			return 4;
		}
	}
	return 0;

}

// 1 points isolé
void handleCase2(double v, int i, int j){
	int id_hg = i*size+(j+1);
	int id_hd = (i+1)*size+(j+1);
	int id_bg = i*size+j;
	int id_bd = (i+1)*size+j;
	glBegin(GL_LINES);
	if ((datas[id_hg]-v > 0 && datas[id_hd]-v < 0) || (datas[id_hg]-v < 0 && datas[id_hd]-v > 0)){
		glVertex2f(
				interpolatePos(x[id_hg], x[id_hd], iso, datas[id_hg], datas[id_hd]),
				interpolatePos(y[id_hg], y[id_hd], iso, datas[id_hg], datas[id_hd])
			  );
	}else{
		glVertex2f(
				interpolatePos(x[id_bg], x[id_bd], iso, datas[id_bg], datas[id_bd]),
				interpolatePos(y[id_bg], y[id_bd], iso, datas[id_bg], datas[id_bd])
			  );
	}
	if ((datas[id_hg]-v > 0 && datas[id_bg]-v < 0) || (datas[id_hg]-v < 0 && datas[id_bg]-v > 0)){
		glVertex2f(
				interpolatePos(x[id_hg], x[id_bg], iso, datas[id_hg], datas[id_bg]),
				interpolatePos(y[id_hg], y[id_bg], iso, datas[id_hg], datas[id_bg])
			  );
	}else{
		glVertex2f(
				interpolatePos(x[id_hd], x[id_bd], iso, datas[id_hd], datas[id_bd]),
				interpolatePos(y[id_hd], y[id_bd], iso, datas[id_hd], datas[id_bd])
			  );
	}
	glEnd();
}

// 2 points oposé
void handleCase3(double v, int i, int j){
	int id_hg = i*size+(j+1);
	int id_hd = (i+1)*size+(j+1);
	int id_bg = i*size+j;
	int id_bd = (i+1)*size+j;
	glBegin(GL_LINES);
	if (datas[id_hg] > v){
		//angle hg
		glVertex2f(
				interpolatePos(x[id_hg], x[id_hd], iso, datas[id_hg], datas[id_hd]),
				interpolatePos(y[id_hg], y[id_hd], iso, datas[id_hg], datas[id_hd])
			  );
		glVertex2f(
				interpolatePos(x[id_hg], x[id_bg], iso, datas[id_hg], datas[id_bg]),
				interpolatePos(y[id_hg], y[id_bg], iso, datas[id_hg], datas[id_bg])
			  );
		//angle bd
		glVertex2f(
				interpolatePos(x[id_bd], x[id_hd], iso, datas[id_bd], datas[id_hd]),
				interpolatePos(y[id_bd], y[id_hd], iso, datas[id_bd], datas[id_hd])
			  );
		glVertex2f(
				interpolatePos(x[id_bd], x[id_bg], iso, datas[id_bd], datas[id_bg]),
				interpolatePos(y[id_bd], y[id_bg], iso, datas[id_bd], datas[id_bg])
			  );
	}else{
		//
		glVertex2f(
				interpolatePos(x[id_hd], x[id_hg], iso, datas[id_hd], datas[id_hg]),
				interpolatePos(y[id_hd], y[id_hg], iso, datas[id_hd], datas[id_hg])
			  );
		glVertex2f(
				interpolatePos(x[id_hd], x[id_bd], iso, datas[id_hd], datas[id_bd]),
				interpolatePos(y[id_hd], y[id_bd], iso, datas[id_hd], datas[id_bd])
			  );
		//
		glVertex2f(
				interpolatePos(x[id_bg], x[id_hg], iso, datas[id_bg], datas[id_hg]),
				interpolatePos(y[id_bg], y[id_hg], iso, datas[id_bg], datas[id_hg])
			  );
		glVertex2f(
				interpolatePos(x[id_bg], x[id_bd], iso, datas[id_bg], datas[id_bd]),
				interpolatePos(y[id_bg], y[id_bd], iso, datas[id_bg], datas[id_bd])
			  );
	}
	glEnd();

}

// 2 points
void handleCase4(double v, int i, int j){
	int id_hg = i*size+(j+1);
	int id_hd = (i+1)*size+(j+1);
	int id_bg = i*size+j;
	int id_bd = (i+1)*size+j;
	glBegin(GL_LINES);
	if ((datas[id_hg]-v > 0 && datas[id_hd]-v > 0) || (datas[id_hg]-v < 0 && datas[id_hd]-v < 0)){
		glVertex2f(
				interpolatePos(x[id_hg], x[id_bg], iso, datas[id_hg], datas[id_bg]),
				interpolatePos(y[id_hg], y[id_bg], iso, datas[id_hg], datas[id_bg])
			  );
		glVertex2f(
				interpolatePos(x[id_hd], x[id_bd], iso, datas[id_hd], datas[id_bd]),
				interpolatePos(y[id_hd], y[id_bd], iso, datas[id_hd], datas[id_bd])
			  );
	}else{
		glVertex2f(
				interpolatePos(x[id_hg], x[id_hd], iso, datas[id_hg], datas[id_hd]),
				interpolatePos(y[id_hg], y[id_hd], iso, datas[id_hg], datas[id_hd])
			  );
		glVertex2f(
				interpolatePos(x[id_bg], x[id_bd], iso, datas[id_bg], datas[id_bd]),
				interpolatePos(y[id_bg], y[id_bd], iso, datas[id_bg], datas[id_bd])
			  );
	}
	glEnd();
}

void renderFunction(void){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	glRotatef(angle_h, 0, 0, 1);

	// Carré extérieur
	glBegin(GL_LINE_LOOP);
	glVertex2f(-display_size,-display_size);
	glVertex2f(display_size,-display_size);
	glVertex2f(display_size,display_size);
	glVertex2f(-display_size,display_size);
	glEnd();
	// Label iso
	char ch[20];
	sprintf(ch, "%.2f", iso);
	glRasterPos2f(0, display_size+0.15);
	glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, ch);

	// each value has a dot and text
	for(int i = 0 ; i < size*size; i++){
		glBegin(GL_POINTS);
		glVertex2f(x[i], y[i]);
		glEnd();
		if (display_number){
			glRasterPos2f(x[i], y[i]);
			sprintf(ch, "%.2f", datas[i]);
			glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_10, ch);
		}
	}

	for (int i = 0; i < size-1 ;i ++){
		for (int j = 0; j < size-1 ; j++){
			int c = chooseCase(
					iso,
					datas[i*size+(j+1)],
					datas[(i+1)*size+(j+1)],
					datas[i*size+j],
					datas[(i+1)*size+j]
					);
			switch (c) {
				case 1:
					//nothing todo
					break;
				case 2:
					handleCase2(iso, i, j);
					break;
				case 3:
					handleCase3(iso, i, j);
					break;
				case 4:
					handleCase4(iso, i, j);
					break;
				default:
					printf("error -> %d,%d \n",i,j);
			}
		}
	}

	glutSwapBuffers();
	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y){
	switch(key){
		case '4':
			angle_h++;
			break;
		case '6':
			angle_h--;
			break;
		case '5':
			display_number = (display_number+1)%2;
			break;
		case '+':
			iso += 0.005;
			break;
		case '-':
			iso -= 0.005;
			break;
		case 27: // Esc
			exit(1);
	}
	glutPostRedisplay();
}

void motion(int x, int y){
	if (prev_x!=-1){
		angle_h += x-prev_x;
		glutPostRedisplay();
	}
	prev_x = x;
}

void initialize(int argc, char* argv[]){
	initWindow(argc, argv);
	fprintf(stdout, "OpenGL Version: %s\n", glGetString(GL_VERSION));

	glClearColor(0.1f, 0.1f, 0.1f, 0.0f);
}

void initWindow(int argc, char* argv[]){
	glutInit(&argc, argv);
	/*
	   glutInitContextVersion(4, 0);
	   glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	   glutInitContextProfile(GLUT_CORE_PROFILE);

	   glutSetOption(
	   GLUT_ACTION_ON_WINDOW_CLOSE,
	   GLUT_ACTION_GLUTMAINLOOP_RETURNS
	   );
	   */
	glutInitWindowSize(currentWidth, currentHeight);


	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

	windowHandle = glutCreateWindow(WINDOW_TITLE);

	if(windowHandle < 1) {
		fprintf(stderr, "ERROR: Could not create a new rendering window.\n");
		exit(EXIT_FAILURE);
	}

	glutReshapeFunc(resizeFunction);
	glutDisplayFunc(renderFunction);
	glutKeyboardFunc(keyboard);
	glutMotionFunc(motion);
}

void resizeFunction(int width, int height){
	currentWidth = width;
	currentHeight = height;
	glViewport(0, 0, currentWidth, currentHeight);
}

double interpolatePos(double c1, double c2, double iso, double v1, double v2){
	return c1 + ((iso-v1)/(v2-v1)) * (c2-c1);
}

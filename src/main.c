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
void drawIsoWith(double);

int currentWidth = 800;
int currentHeight = 600;
int windowHandle = 0;

double display_size = 0.7;
int display_number = 1;

GLfloat angle_h = 0.0;
GLfloat angle_v = 0.0;
int prev_x = -1;
int prev_y = -1;

double * datas;
double maxi;
double mini;
double * x;
double * y;
int nb_iso = 5;
int size = 0;

double display_Z_correction = 0.5;

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
	fgets(ch, sizeof(ch), fp);
	mini = atof(ch);
	maxi = atof(ch);
	do {
		datas[i] = atof(ch);
		mini = mini < datas[i] ? mini : datas[i];
		maxi = maxi > datas[i] ? maxi : datas[i];
		i++;
	 } while(fgets(ch, sizeof(ch), fp) && i < nb_value);
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
		glVertex3f(
				interpolatePos(x[id_hg], x[id_hd], v, datas[id_hg], datas[id_hd]),
				interpolatePos(y[id_hg], y[id_hd], v, datas[id_hg], datas[id_hd]),
				v/maxi - display_Z_correction
			  );
	}else{
		glVertex3f(
				interpolatePos(x[id_bg], x[id_bd], v, datas[id_bg], datas[id_bd]),
				interpolatePos(y[id_bg], y[id_bd], v, datas[id_bg], datas[id_bd]),
				v/maxi - display_Z_correction
			  );
	}
	if ((datas[id_hg]-v > 0 && datas[id_bg]-v < 0) || (datas[id_hg]-v < 0 && datas[id_bg]-v > 0)){
		glVertex3f(
				interpolatePos(x[id_hg], x[id_bg], v, datas[id_hg], datas[id_bg]),
				interpolatePos(y[id_hg], y[id_bg], v, datas[id_hg], datas[id_bg]),
				v/maxi - display_Z_correction
			  );
	}else{
		glVertex3f(
				interpolatePos(x[id_hd], x[id_bd], v, datas[id_hd], datas[id_bd]),
				interpolatePos(y[id_hd], y[id_bd], v, datas[id_hd], datas[id_bd]),
				v/maxi - display_Z_correction
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
		glVertex3f(
				interpolatePos(x[id_hg], x[id_hd], v, datas[id_hg], datas[id_hd]),
				interpolatePos(y[id_hg], y[id_hd], v, datas[id_hg], datas[id_hd]),
				v/maxi - display_Z_correction
			  );
		glVertex3f(
				interpolatePos(x[id_hg], x[id_bg], v, datas[id_hg], datas[id_bg]),
				interpolatePos(y[id_hg], y[id_bg], v, datas[id_hg], datas[id_bg]),
				v/maxi - display_Z_correction
			  );
		//angle bd
		glVertex3f(
				interpolatePos(x[id_bd], x[id_hd], v, datas[id_bd], datas[id_hd]),
				interpolatePos(y[id_bd], y[id_hd], v, datas[id_bd], datas[id_hd]),
				v/maxi - display_Z_correction
			  );
		glVertex3f(
				interpolatePos(x[id_bd], x[id_bg], v, datas[id_bd], datas[id_bg]),
				interpolatePos(y[id_bd], y[id_bg], v, datas[id_bd], datas[id_bg]),
				v/maxi - display_Z_correction
			  );
	}else{
		//
		glVertex3f(
				interpolatePos(x[id_hd], x[id_hg], v, datas[id_hd], datas[id_hg]),
				interpolatePos(y[id_hd], y[id_hg], v, datas[id_hd], datas[id_hg]),
				v/maxi - display_Z_correction
			  );
		glVertex3f(
				interpolatePos(x[id_hd], x[id_bd], v, datas[id_hd], datas[id_bd]),
				interpolatePos(y[id_hd], y[id_bd], v, datas[id_hd], datas[id_bd]),
				v/maxi - display_Z_correction
			  );
		//
		glVertex3f(
				interpolatePos(x[id_bg], x[id_hg], v, datas[id_bg], datas[id_hg]),
				interpolatePos(y[id_bg], y[id_hg], v, datas[id_bg], datas[id_hg]),
				v/maxi - display_Z_correction
			  );
		glVertex3f(
				interpolatePos(x[id_bg], x[id_bd], v, datas[id_bg], datas[id_bd]),
				interpolatePos(y[id_bg], y[id_bd], v, datas[id_bg], datas[id_bd]),
				v/maxi - display_Z_correction
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
		glVertex3f(
				interpolatePos(x[id_hg], x[id_bg], v, datas[id_hg], datas[id_bg]),
				interpolatePos(y[id_hg], y[id_bg], v, datas[id_hg], datas[id_bg]),
				v/maxi - display_Z_correction
			  );
		glVertex3f(
				interpolatePos(x[id_hd], x[id_bd], v, datas[id_hd], datas[id_bd]),
				interpolatePos(y[id_hd], y[id_bd], v, datas[id_hd], datas[id_bd]),
				v/maxi - display_Z_correction
			  );
	}else{
		glVertex3f(
				interpolatePos(x[id_hg], x[id_hd], v, datas[id_hg], datas[id_hd]),
				interpolatePos(y[id_hg], y[id_hd], v, datas[id_hg], datas[id_hd]),
				v/maxi - display_Z_correction
			  );
		glVertex3f(
				interpolatePos(x[id_bg], x[id_bd], v, datas[id_bg], datas[id_bd]),
				interpolatePos(y[id_bg], y[id_bd], v, datas[id_bg], datas[id_bd]),
				v/maxi - display_Z_correction
			  );
	}
	glEnd();
}

void renderFunction(void){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	glRotatef(angle_h, 0, 1, 0);
	glRotatef(angle_v, 1, 0, 0);

	// Carré extérieur
	glBegin(GL_LINE_LOOP);
	glVertex2f(-display_size, -display_size);
	glVertex2f( display_size, -display_size);
	glVertex2f( display_size,  display_size);
	glVertex2f(-display_size,  display_size);
	glEnd();
	// Label iso
	char ch[20];
	sprintf(ch, "%d", nb_iso);
	glRasterPos2f(0, display_size+0.15);
	glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, ch);

	// each value has a dot and text
	for(int i = 0 ; i < size*size; i++){
		glBegin(GL_POINTS);
		glVertex3f(x[i], y[i], datas[i]/4*0.5);
		glEnd();
		if (display_number){
			glRasterPos2f(x[i], y[i]);
			sprintf(ch, "%.2f", datas[i]);
			glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_10, ch);
		}
	}

	double step = (maxi - mini) / nb_iso;
	double color_step = 3.0f / nb_iso;
	double c = 0;
	for (double i = mini+0.000001 ; i < maxi ; i += step){
		c < 1 ? glColor3f(0, 1-c, 0) :
			c < 1.5 ? glColor3f(0, 0, (c-1)*2):
			c < 2 ? glColor3f(0, 0, 1-(c-1.5)*2):
			glColor3f(c-2, 0, 0);
		drawIsoWith(i);
		c+=color_step;
	}
	glColor3f(1, 1, 1);

	glutSwapBuffers();
	glutPostRedisplay();
}

void drawIsoWith(double value){
	for (int i = 0; i < size-1 ;i ++){
		for (int j = 0; j < size-1 ; j++){
			int c = chooseCase(
					value,
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
					handleCase2(value, i, j);
					break;
				case 3:
					handleCase3(value, i, j);
					break;
				case 4:
					handleCase4(value, i, j);
					break;
				default:
					fprintf(stderr, "error -> i:%d, j:%d \n",i,j);
			}
		}
	}
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
			nb_iso += 1;
			break;
		case '-':
			nb_iso -= 1;
			nb_iso = nb_iso < 0 ? 0: nb_iso;
			break;
		case 27: // Esc
			exit(1);
		default:
			printf("%d\n", key);
	}
	glutPostRedisplay();
}

void passiveMotion(int x, int y){
	prev_x = x;
	prev_y = y;
}

void motion(int x, int y){
	if (prev_x!=-1){
		angle_h += x-prev_x;
		glutPostRedisplay();
	}

	if (prev_y!=-1){
		angle_v += y-prev_y;
		glutPostRedisplay();
	}
	prev_x = x;
	prev_y = y;
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
	glutPassiveMotionFunc(passiveMotion);
}

void resizeFunction(int width, int height){
	currentWidth = width;
	currentHeight = height;
	glViewport(0, 0, currentWidth, currentHeight);
}

double interpolatePos(double c1, double c2, double iso, double v1, double v2){
	return c1 + ((iso-v1)/(v2-v1)) * (c2-c1);
}

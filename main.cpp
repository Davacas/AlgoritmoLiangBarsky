/*
PROGRAMA QUE REALIZA EL ALGORITMO DE LIANG-BARSKY PARA RECORTE DE LÍNEAS CON 
ENTRADA DE DATOS DEL USUARIO Y DESPLIEGUE DE LOS RESULTADOS MEDIANTE UNA VENTANA
GRÁFICA HECHA CON GLUT Y OPENGL.

DESARROLLADO POR DANIEL VARGAS CASTRO PARA LA MATERIA DE COMPUTACIÓN GRÁFICA DE LA FACULTAD 
DE INGENIERÍA DE LA UNIVERSIDAD NACIONAL AUTÓNOMA DE MÉXICO DURANTE EL SEMESTRE 2018-1.
*/

#include "Main.h"
#include <iostream>
#include <vector>

//Estructura que define un punto mediante sus coordenadas x e y.
typedef struct {
	float x;
	float y;
}punto;

float xmin, xmax, ymin, ymax; //Dimensiones de la ventana de recorte
float t_temp, x_temp, y_temp; //Para cálculos temporales
int cant_lineas = 0, i = 0, cant_lineas_final = 0, puntos; //Contadores
punto punto_ini_temp, punto_fin_temp; //Puntos temporales
std::vector <punto *> lineas, lineas_final; //Vectores de puntos que representan lineas

//FUNCIÓN PARA SOLICITAR DATOS AL USUARIO.
void pedirDatos(void) {
	std::cout << "\n\t---- RECORTE DE LINEAS POR ALGORITMO DE LIANG-BARSKY ----\n\n";
	//SOLICITAMOS Y ALMACENAMOS LOS DATOS DEL USUARIO
	std::cout << "*** Antes de empezar, ten en cuenta que el programa solo mostrara\n";
	std::cout << "correctamente valores entre 50 y -50 para los ejes X y Y. ***\n\n";
	std::cout << "Para la ventana de recorte, teclea los siguientes datos.\n";
	std::cout << "Coordenada en x del punto inferior izquierdo: ";
	std::cin >> xmin;
	std::cout << "Coordenada en y del punto inferior izquierdo: ";
	std::cin >> ymin;
	std::cout << "Coordenada en x del punto superior derecho: ";
	std::cin >> xmax;
	std::cout << "Coordenada en y del punto superior derecho: ";
	std::cin >> ymax;
	std::cout << "Ingresa la cantidad de lineas a dibujar: ";
	std::cin >> cant_lineas;
	std::cout << "\n";

	//PARA LA CANTIDAD DE LINEAS QUE EL USUARIO QUIERA DIBUJAR...
	for (i = 0; i < cant_lineas; i++) {
		lineas.push_back(new punto[2]); //ALMACENAMOS UNA LINEA, COMPUESTA A SU VEZ DE DOS PUNTOS
		std::cout << "Ingresa la coordenada x inicial de la linea " << i + 1 << ": ";
		std::cin >> lineas[i][0].x;
		std::cout << "Ingresa la coordenada y inicial de la linea " << i + 1 << ": ";
		std::cin >> lineas[i][0].y;
		std::cout << "Ingresa la coordenada x final de la linea " << i + 1 << ": ";
		std::cin >> lineas[i][1].x;
		std::cout << "Ingresa la coordenada y final de la linea " << i + 1 << ": ";
		std::cin >> lineas[i][1].y;
		std::cout << "\n";
	}

	return;
}

//FUNCIÓN QUE REALIZA EL ALGORITMO DE LIANG BARSKY SI ES NECESARIO
void algoritmoLB(void) {
	std::cout << "La coordenada inferior izquierda de la ventana de recorte es: ";
	std::cout << "(" << xmin << ", " << ymin << ")\n";
	std::cout << "La coordenada superior derecha de la ventana de recorte es: ";
	std::cout << "(" << xmax << ", " << ymax << ")\n\n";
	for (i = 0; i < cant_lineas; i++) {
		//VERIFICAMOS SI LA LÍNEA SE ENCUENTRA DENTRO DEL ÁREA DE RECORTE. SI LO ESTÁ, SE CONSERVA INTACTA
		if (lineas[i][0].x > xmin && lineas[i][0].x < xmax && lineas[i][0].y > ymin && lineas[i][0].y < ymax){
			//Si el punto inicial está dentro de la ventana de recorte, lo almacenamos temporalmente.
			punto_ini_temp.x = lineas[i][0].x;
			punto_ini_temp.y = lineas[i][0].y;
			//Incrementamos la variable puntos, indicando que se ha almacenado un punto final.
			puntos ++;
		}
		if (lineas[i][1].x > xmin && lineas[i][1].x < xmax && lineas[i][1].y > ymin && lineas[i][1].y < ymax){
			//Si el punto final está dentro de la ventana de recorte, lo almacenamos temporalmente.
			punto_fin_temp.x = lineas[i][1].x;
			punto_fin_temp.y = lineas[i][1].y;
			//Incrementamos la variable puntos, indicando que se ha almacenado un punto final.
			puntos ++;
		}
		//SI YA TENEMOS DOS PUNTOS, NO NECESITAMOS HACER RECORTE. ALMACENAMOS AMBOS PUNTOS DADOS EN EL VECTOR DE LINEAS FINAL.
		if (puntos == 2) {
			lineas_final.push_back(new punto[2]);
			std::cout << "Se mantiene la linea " << i + 1 << ".\n";
			//Almacenamos los puntos temporales inicial y final en el vector de lineas final.
			lineas_final[cant_lineas_final][0] = punto_ini_temp;
			lineas_final[cant_lineas_final][1] = punto_fin_temp;
			//Imprimimos las coordenadas que acabamos de obtener.
			std::cout << "Sus coordenadas finales son: " ;
			std::cout <<"("<< punto_ini_temp.x << ", " << punto_ini_temp.y << "), (" << punto_fin_temp.x << ", " << punto_fin_temp.y << ").\n\n";
			cant_lineas_final ++;
		}
		//SI AÚN NO TENEMOS DOS PUNTOS, DEBEMOS CALCULAR LOS PUNTOS RESTANTES PARA HACER UNA LÍNEA, SI ES QUE EXISTEN.
		//PARA ELLO EJECUTAMOS EL ALGORITMO DE LIANG BARSKY.
		else {
			//CALCULAMOS Y VERIFICAMOS UN PUNTO IZQUIERDO
			t_temp = (float)(xmin - lineas[i][0].x) / (float)(lineas[i][1].x - lineas[i][0].x);
			//Si el valor de t recién calculado está entre 0 y 1, tenemos un posible punto de recorte.
			if (t_temp >= 0 && t_temp <= 1) {
				//Calculamos un valor temporal para y.
				y_temp = lineas[i][0].y + t_temp*(lineas[i][1].y - lineas[i][0].y);
				//Si el valor de y calculado está dentro de la ventana de recorte y la coordenada
				//de x inferior izquierda dada está entre los puntos inicial y final de la recta, 
				//almacenamos temporalmente ambos valores como punto de recorte.
				if (xmin >= lineas[i][0].x && xmin <= lineas[i][1].x && y_temp >= ymin && y_temp <= ymax) {
					punto_ini_temp.x = xmin;
					punto_ini_temp.y = y_temp;
					//Incrementamos la variable puntos, indicando que obtuvimos un punto válido.
					puntos++;
				}
			}
			//CALCULAMOS Y VERIFICAMOS UN PUNTO DERECHO
			t_temp = (float)(xmax - lineas[i][0].x) / (float)(lineas[i][1].x - lineas[i][0].x);
			//Si el valor de t recién calculado está entre 0 y 1, tenemos un posible punto de recorte.
			if (t_temp >= 0 && t_temp <= 1) {
				//Calculamos un valor temporal para y.
				y_temp = lineas[i][0].y + t_temp*(lineas[i][1].y - lineas[i][0].y);
				//Si el valor de y calculado está dentro de la ventana de recorte y la coordenada
				//de x superior derecha dada está entre los puntos inicial y final de la recta, 
				//almacenamos temporalmente ambos valores como punto de recorte.
				if (xmax >= lineas[i][0].x && xmax <= lineas[i][1].x && y_temp >= ymin && y_temp <= ymax) {
					punto_fin_temp.x = xmax;
					punto_fin_temp.y = y_temp;
					//Incrementamos la variable puntos, indicando que obtuvimos un punto válido.
					puntos++;
				}
			}
			//CALCULAMOS Y VERIFICAMOS PUNTO INFERIOR
			t_temp = (float)(ymin - lineas[i][0].y) / (float)(lineas[i][1].y - lineas[i][0].y);
			//Si el valor de t recién calculado está entre 0 y 1, tenemos un posible punto de recorte.
			if (t_temp >= 0 && t_temp <= 1) {
				//Calculamos un valor temporal para x.
				x_temp = lineas[i][0].x + t_temp*(lineas[i][1].x - lineas[i][0].x);
				//Si el valor de x calculado está dentro de la ventana de recorte y la coordenada
				//de y inferior izquierda dada está entre los puntos inicial y final de la recta, 
				//almacenamos temporalmente ambos valores como punto de recorte.
				if (ymin >= lineas[i][0].y && ymin <= lineas[i][1].y && x_temp >= xmin && x_temp <= xmax) {
					punto_ini_temp.x = x_temp;
					punto_ini_temp.y = ymin;
					//Incrementamos la variable puntos, indicando que obtuvimos un punto válido.
					puntos++;
				}
			}
			//CALCULAMOS Y VERIFICAMOS PUNTO SUPERIOR
			t_temp = (float)(ymax - lineas[i][0].y) / (float)(lineas[i][1].y - lineas[i][0].y);
			//Si el valor de t recién calculado está entre 0 y 1, tenemos un posible punto de recorte.
			if (t_temp >= 0 && t_temp <= 1) {
				//Calculamos un valor temporal para x.
				x_temp = lineas[i][0].x + t_temp*(lineas[i][1].x - lineas[i][0].x);
				//Si el valor de x calculado está dentro de la ventana de recorte y la coordenada
				//de y superior derecha dada está entre los puntos inicial y final de la recta, 
				//almacenamos temporalmente ambos valores como punto de recorte.
				if (ymax >= lineas[i][0].y && ymax <= lineas[i][1].y && x_temp >= xmin && x_temp <= xmax) {
					punto_fin_temp.x = x_temp;
					punto_fin_temp.y = ymax;
					//Incrementamos la variable puntos, indicando que obtuvimos un punto válido.
					puntos++;
				}
			}
			//SI CON TODO LO ANTERIOR HEMOS CONSEGUIDO DOS PUNTOS PARA LA LÍNEA, LA AGREGAMOS COMO LÍNEA A DIBUJAR
			//El algoritmo puede resultar en 4 puntos de recorte si la línea traza una 
			//diagonal a la ventana de recorte, por eso condideramos esta opción.
			if (puntos == 2 || puntos == 4) {
				std::cout << "Se recorta la linea " << i + 1 << ".\n";
				//Creamos una nueva línea dentro del vector de lineas finales.
				lineas_final.push_back(new punto[2]);
				//Le asignamos ambos puntos temporales a la nueva linea creada.
				lineas_final[cant_lineas_final][0] = punto_ini_temp;
				lineas_final[cant_lineas_final][1] = punto_fin_temp;
				std::cout << "Sus coordenadas finales son: ";
				std::cout << "(" << punto_ini_temp.x << ", " << punto_ini_temp.y << "), (" << punto_fin_temp.x << ", " << punto_fin_temp.y << ").\n\n";
				cant_lineas_final++;
			}
			//SI NO SE OBTUVIERON 2 PUNTOS, SE DESCARTA LA LÍNEA
			else std::cout << "Se descarta la linea " << i + 1 << ".\n";
		}
		puntos = 0;
	}

	return;
}

//FUNCIÓN PARA DIBUJAR LAS LÍNEAS FINALES
void dibujarLineas(void) {
	//DIBUJAMOS EL ÁREA DE RECORTE
	//con un grosor de 2 puntos y un color gris.
	glLineWidth(2.0);
	glColor3f(0.25, 0.25, 0.25);
	//Dibujamos la ventana a partir de los puntos ingresados.
	glBegin(GL_LINE_STRIP);
	glVertex2f(xmin, ymin);
	glVertex2f(xmin, ymax);
	glVertex2f(xmax, ymax);
	glVertex2f(xmax, ymin);
	glVertex2f(xmin, ymin);
	glEnd();

	//DIBUJAMOS CADA LINEA FINAL
	//con un color blanco y grosor de 1.
	glColor3f(1.0, 1.0, 1.0);
	glLineWidth(1.0);
	//Para cada una de las lineas dentro del vector de líneas final
	//Dibujamos una lína a partir de las coordenadas almacenadas en dicho vector.
	for (i = 0; i < cant_lineas_final; i ++) {
		glBegin(GL_LINES);
		glVertex2f(lineas_final[i][0].x, lineas_final[i][0].y);
		glVertex2f(lineas_final[i][1].x, lineas_final[i][1].y);
		glEnd();
	}
	return;
}

//FUNCIÓN QUE SE LANZA CUANDO SE REDIMENSIONA LA PANTALLA
void reshape(int w, int h)
{
	if (!h)
		return;

	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//Definimos que la ventana de recorte sólo dibuje dentro de las coordenadas (-50. -50) y (50, 50)
	glOrtho(-50, 50, -50, 50, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	return;
}

//FUNCIÓN PARA REALIZAR DIBUJOS
void display(void)
{ 
	//Mandamos llamar la función que dibuja las líneas.
	dibujarLineas();

	glutSwapBuffers();

	std::cout << "\nRecorte terminado. Ya puedes cerrar el programa.\n";

	return;
} 

//Función que se lanza cuando presionamos una tecla del teclado.
void keyboard(unsigned char key, int x, int y)
{
	switch (key) //Salimos del programa cuando presionamos escape.
	{
		case 27: 
			exit(0);
			break;
	}
	glutPostRedisplay();
	return;
}    

//FUNCIÓN MAIN
int main(int argc, char **argv)
{ 
	//Pedimos los datos al usuario y con base en ellos, ejecutamos el algoritmo.
	pedirDatos();
	algoritmoLB();

	//Inicializamos GLUT y OpenGL.
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB| GLUT_DEPTH);
	glutInitWindowSize (480, 480);
	glutInitWindowPosition (0, 0);
	glutCreateWindow ("Algoritmo de Liang-Barsky");
	glutDisplayFunc(display); 
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	//Empieza el dibujo del resultado del algoritmo.
	glutMainLoop();
	 
	 return 0;
}
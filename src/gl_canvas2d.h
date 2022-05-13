#ifndef __CANVAS_2D__H__
#define __CANVAS_2D__H__

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include <GL/glut.h>
#include <GL/freeglut_ext.h> //callback da wheel do mouse.

#define PI_2 6.28318530717
#define PI   3.14159265359

#define Y_CANVAS_CRESCE_PARA_CIMA 1

//funcoes para desenho de ponto e linha
void cv_point(float x, float y); //coordeandas do ponto

void cv_line( float x1, float y1, float x2, float y2 ); //coordenadas da linha x1, y1, x2, y2

//desenha um retangulo alinhado nos eixos x e y
void cv_rect( float x1, float y1, float x2, float y2 ); //coordenadas do retangulo x1, y1, x2, y2

void cv_rectFill( float x1, float y1, float x2, float y2 ); //coordenadas do retangulo x1, y1, x2, y2

//desenha um poligono CONVEXO. Para um retangulo, deve-se passar 4 vertices
void cv_polygon(float vx[], float vy[], int n_elems);
void cv_polygonFill(float vx[], float vy[], int n_elems);

//centro e raio do circulo
void cv_circle( float x, float y, float radius, int div );

void cv_circleFill( float x, float y, float radius, int div );

//especifica a cor de desenho e de limpeza de tela
void cv_color_rgb (float r, float g, float b);
void cv_color_rgba(float r, float g, float b, float alpha);
void cv_color_index(int index);

void cv_clear(float r, float g, float b);

//desenha texto na coordenada (x,y)
void cv_text(float x, float y, const char *t);

//coordenada de offset para desenho de objetos.
void cv_translate(float x, float y);

//funcao de inicializacao da Canvas2D. Recebe a largura, altura, e um titulo para a janela
void cv_init(int *w, int *h, const char *title);

//funcao para executar a Canvas2D
void cv_run();

#endif

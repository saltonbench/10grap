#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "gmath.h"
#include "matrix.h"
#include "ml6.h"

//lighting functions
color get_lighting( double *normal, double *view, color alight, double light[2][3], double *areflect, double *dreflect, double *sreflect) {
  color i;
  normalize(normal), normalize(light[LOCATION]), normalize(view);
  color ambient = calculate_ambient(alight, areflect);
  color diffuse = calculate_diffuse(light, dreflect, normal);
  color specular = calculate_specular(light, sreflect, view, normal);
  i.red = ambient.red + diffuse.red + specular.red;
  i.green = ambient.green + diffuse.green + specular.green;
  i.blue = ambient.blue + diffuse.blue + specular.blue;
  limit_color(&i);
  return i;
  
}

color calculate_reflections(double light[3], double *reflect, double dot_prod){
  color a;
  a.red = light[RED] * reflect[RED] * dot_prod;
  a.green = light[GREEN] * reflect[GREEN] * dot_prod;
  a.blue = light[BLUE] * reflect[BLUE] * dot_prod;
  limit_color(&a);
  return a;
}
color calculate_ambient(color alight, double *areflect ) {
  double light[3] = {alight.red, alight.green, alight.blue};
  color a = calculate_reflections(light, areflect, 1);
  return a;
}

color calculate_diffuse(double light[2][3], double *dreflect, double *normal ) {
  double cos_theta = dot_product(light[LOCATION], normal);
  color d = calculate_reflections(light[COLOR], dreflect, cos_theta);
  return d;
}

double calculate_cos_alpha(double light[3], double *view, double *normal, int specular_exp){
  double reflect[3] = {2 * dot_product(normal, light) * normal[RED] - light[RED],
		   2 * dot_product(normal, light) * normal[GREEN] - light[GREEN],
		       2 * dot_product(normal, light) * normal[BLUE] - light[BLUE]};
  return pow(dot_product(reflect, view), specular_exp);
}

color calculate_specular(double light[2][3], double *sreflect, double *view, double *normal ) {
  color s = {0, 0, 0};
  if (dot_product(normal, light[LOCATION]) > 0){
    double cos_alpha = calculate_cos_alpha(light[LOCATION], view, normal, 4);
    s = calculate_reflections(light[COLOR], sreflect, cos_alpha);
  }
  return s;
}


//limit each component of c to a max of 255
void limit_color( color * c ) {
  c->red > 255 ? c->red = 255 : 0;
  c->green > 255 ? c->green = 255 : 0;
  c->blue > 255 ? c->blue = 255 : 0;
}


//vector functions
//normalize vetor, should modify the parameter
void normalize( double *vector ) {
  double magnitude = sqrt(pow(vector[0], 2) + pow(vector[1], 2) + pow(vector[2], 2));
  vector[0] /= magnitude, vector[1] /= magnitude, vector[2] /= magnitude;
}

//Return the dot porduct of a . b
double dot_product( double *a, double *b ) {
  return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

double *calculate_normal(struct matrix *polygons, int i) {

  double A[3];
  double B[3];
  double *N = (double *)malloc(3 * sizeof(double));

  A[0] = polygons->m[0][i+1] - polygons->m[0][i];
  A[1] = polygons->m[1][i+1] - polygons->m[1][i];
  A[2] = polygons->m[2][i+1] - polygons->m[2][i];

  B[0] = polygons->m[0][i+2] - polygons->m[0][i];
  B[1] = polygons->m[1][i+2] - polygons->m[1][i];
  B[2] = polygons->m[2][i+2] - polygons->m[2][i];

  N[0] = A[1] * B[2] - A[2] * B[1];
  N[1] = A[2] * B[0] - A[0] * B[2];
  N[2] = A[0] * B[1] - A[1] * B[0];

  return N;
}

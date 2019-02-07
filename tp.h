#ifndef _TP_H
#define _TP_H

#define cant(l) l -> cant
#define iter(l) l -> iter
#define escala(l) l -> escala
#define of_x(l) l -> of_x
#define of_y(l) l -> of_y
#define list_ecs(l, n) (l -> list_ec)[n]
#define coef_ec(l, n, k) list_ecs(l, n)[k]
#define ANCHO 400
#define ALTO 400
#define OFFSET 26
typedef struct _datos_vista
{
    int escala;
    int of_x;
    int of_y;
} datos_vista;

typedef struct _bmp_header
{
    short FileType;      //Tipo de archivo, siempre 424Dh ("BM")
    int FileSize;        //TamaÃ±o del archivo en bytes, no comprimido = 0
    int Reserved;        //Siempre 0
    int BitmapOffset;    //Posicion de datos de imagenes A partir de bytes
    int Size;            //Tamaño de la segunda cabecera (incluida en esta) siempre 12
    short Ancho;         //Ancho de la imagen (pixeles)
    short Alto;          //Alto de la imagen (pixeles)
    short Planos;        //Cantidad de planos, siempre 1
    short Prof;          //Paleta de colores, nosotros 24
} bmp_header;

typedef struct _datos {
    int cant;
    long iter;
    float ** list_ec;
} datos;

typedef struct _punto {
    float x;
    float y;
} punto;

datos_vista *leer_datos_vista(void);

datos * leer_datos (void);

void base_image (char *nombre);

void pto_random(punto *pto, datos_vista *vista);

float * ec_random (datos *ret);

long despl (punto *pto, datos_vista *vista);

void marcar_pxl (FILE *fichero, punto *pto, datos_vista *vista);

void lib_dat (datos *ret);

void solve(float *ec, punto *p);
#endif /* tp.h */

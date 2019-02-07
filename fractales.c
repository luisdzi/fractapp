#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "tp.h"


datos_vista *leer_datos_vista(void)
{
    // reservamos el espacio de memoria que vamos a retornar
    datos_vista *ret = malloc (sizeof (datos_vista));
    // tomamos los valores de escala, desplazamiento x e y
    scanf ("%d %d %d",  &escala(ret), &of_x(ret), &of_y(ret));
    return ret;
}

datos * leer_datos (void)
{
    int i, j;
    float *ec = NULL;
    // reservamos el espacio de memoria que vamos a retornar
    datos *ret = malloc (sizeof (datos));
    // tomamos la cant de ec y nro de iteraciones.
    scanf ("%d %ld",  &cant(ret) , &iter(ret));
    // reservamos el espacio para los puntadores a las ecuaciones
    ret -> list_ec = malloc (sizeof(float *) * cant(ret));
    // recorremos los punteros a las ecuaciones
    for (i = 0; i < cant(ret); i++)
    {
        // vamos reservando el espacio para los coef de las ec i, utilizamos la macro list_ecs()
        list_ecs(ret, i) =  malloc (sizeof(float) * 6);
        // tomamos los coeficientes de la ec i, utilizamos la macro coef_ec()
        for (j = 0; j < 6; j++)
            scanf ("%f", &coef_ec(ret, i, j));
    }
    return ret;
}

void base_image (char *nombre)
{
    int i;
    FILE * fichero = NULL;
    
    // declaramos e inicializamos la estructura con los datos de la cabecera
    bmp_header hd = { 0x4d42, 0, 0, OFFSET, 12, ANCHO, ALTO, 1, 24};
    
    // arreglo usado como pixel de color de fondo
    char pxb[3] = {0xff, 0xff, 0xff};

    //Elimino cualquier copia ya existente
    remove (nombre);

    //Creo el archivo en modo binario de escritura
    fichero = fopen(nombre, "wb");

    //Verifico que lo hayamos abierto correctamente
    if (fichero == NULL) {
        printf ("Error al crear el fichero \n");
        abort ();
    }
    
    //Escribimos en el archivo con los datos de la cabecera
    fwrite (&(hd.FileType), 1, sizeof(hd.FileType), fichero);
    fwrite (&(hd.FileSize), 1, sizeof(hd.FileSize), fichero);
    fwrite (&(hd.Reserved), 1, sizeof(hd.Reserved), fichero);
    fwrite (&(hd.BitmapOffset), 1, sizeof(hd.BitmapOffset), fichero);
    fwrite (&(hd.Size), 1, sizeof(hd.Size), fichero);
    fwrite (&(hd.Ancho), 1, sizeof(hd.Ancho), fichero);
    fwrite (&(hd.Alto), 1, sizeof(hd.Alto), fichero);
    fwrite (&(hd.Planos), 1, sizeof(hd.Planos), fichero);
    fwrite (&(hd.Prof), 1, sizeof(hd.Prof), fichero);

    //Creamos el fondo de la imagen
    for(i = 0; i < ( ANCHO * ALTO); i++)
        fwrite (pxb, 1, 3, fichero);

    fclose (fichero);
}
/* para generar las coordenadas, utilizamos un numero random,
 * utilizamos el modulo para acotarlo en valores utiles
 * luego desplazamos y escalamos a numero mas pequeños,
 * segun los datos ingresados.
 */
void pto_random(punto *pto, datos_vista *vista)
{
    pto -> x = (rand() % ANCHO - 50) / escala(vista);
    pto -> y = (rand() % ALTO - 50) / escala(vista);
}

// seleccionamos una ecuacion de manera aleatoria de las ingresada
float * ec_random (datos *ret)
{
    // elegimos un numero de ecuacion de manera aleatoria
    int ec = rand()%cant(ret);
    // retornamos la ecuacion elegida
    return list_ecs(ret, ec);
}

long despl (punto *pto, datos_vista *vista)
{
    
    long x, y;
    /* dado que los pixeles son posiciones enteras,
     * volvemos al valor orignal, mediante un escalado,
     * y redondeamos el valor
     */
    
    x = (long)(round((pto -> x) * escala(vista)));
    y = (long)(round((pto -> y) * escala(vista)));
    /** Desplazamiento jugar  **/
    x += (long)(of_x(vista));
    y += (long)(of_y(vista));
    return (OFFSET + (ANCHO * y + x) * 3);
}

void marcar_pxl (FILE *fichero, punto *pto, datos_vista *vista)
{
    // color del pixel que va a ser marcado
    char pxp[3] = {0x00, 0x00, 0x00};
    // posicionamos el fichero mediante la funcion desplazamiento
    fseek (fichero, despl(pto, vista), SEEK_SET);
    // grabamos el pixel en la posicion correspondiente
    fwrite (pxp, 1, 3, fichero);
}

void lib_dat (datos *ret)
{
    int i;
    for (i = 0; i < cant(ret); i++)
        free (list_ecs(ret, i));
    free (ret -> list_ec);
    free (ret);
}

int main(void)
{
    long k;
    
    // nombre que le daremos al archivo generado
    char nombre[] = "fractal.bmp";
    
    // puntero a los coeficientes de la ecuacion
    float * ecuacion = NULL;
    
    /* puntero que contiene los datos de escala y offset
     * para que la imagen quede centrada y de tamaño correspondiente
     * leemos los datos por stdin
     */
    datos_vista *vista = leer_datos_vista();
    
    /* puntero a la estructura que contiene todos los datos ingresados,
     * leemos los datos ingresados por stdin
     */
    datos *ret = leer_datos();
    
    /* puntero a la estructura que contiene las coordenadas del punto,
     * reservamos el espacio
     */
    punto *pto = malloc(sizeof(punto));
    
    // puntero para manejar el archivo en el q trabajamos
    FILE * fichero = NULL;
    
    // plantamos la semilla basada en el tiempo para generar numeros aleatorios
    srand(time(NULL));
    
    // creamos la cabecera de la imagen
    base_image (nombre);

    // generamos el primer punto en forma aleatoria
    pto_random (pto, vista);
    
    // abrimos el archivo en modo binario de lecto-escritura
    fichero = fopen(nombre, "rb+");
    
    // chequeamos q no haya error al abrir el archivo
    if (fichero == NULL) {
        printf ("Error al abrir el fichero\n");
        abort ();
    }
    
    // procedemos a realizar la creacion del fractal
    for (k = 0; k < iter(ret); k++){
        marcar_pxl (fichero, pto, vista);
        ecuacion = ec_random (ret);
        solve (ecuacion, pto);
    }
    // cerramos el fichero y liberamos la memoria involucrada
    fclose (fichero);
    free (pto);
    free(vista);
    lib_dat (ret);
    return 0;
}

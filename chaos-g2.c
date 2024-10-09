#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#include <g2.h>
#include <g2_gd.h>
#include <g2_X11.h>

#define MAX_X	800
#define MAX_Y	500

int grdev;			/* the virtual graphics device we'll use */

void mapa_logistico()
{
    double x, a;
    int i, cx, cy;
    extern int grdev;

    /*
       The logistic map is an oscillator computed as per
       the following formula:

       dx / dt = a x (1 - x), 0 =< a <= 4

       we'll see how the values of x change as we change a
     */
    x = 0.3;

    /* Let us start at a = 2.95 */
    for (a = 2.95; a += 2.5e-3; a <= 3.95) {
    	/* for each value of a */
	for (i = 1; i <= 200; i++)
	    /* let x evolve 200 times */
	    x = a * x * (1 - x);
	for (i = 1; i <= 300; i++) {
	    /* and now plot the 300 next */
	    x = a * x * (1 - x);
	    cx = floor(MAX_X * (a - 2.95));
	    cy = floor(MAX_Y * x);
	    g2_plot(grdev, cx, cy);
	}
	a += 2.5e-3;
    }

}

double f(y, c)
double y, c;
{
    return (c * y + 2 * (1 - c) * (y * y) / (1 + y * y));
}

Gumowski_and_Mira()
{
    int i, j, cx, cy;
    double c, x, y, xx;
    int cnt;
    extern int grdev;


    g2_string(grdev, 30, 180, "MAPA DE GUMOWSKI & MIRA");
    g2_string(grdev, 30, 160, "=======================");
    g2_string(grdev, 20, 140, "x\' <- y");
    g2_string(grdev, 20, 120, "y\' <- -x + 2 F(y)");
    g2_string(grdev, 20, 100, "F(y) <- c.y + 2.(1 - c).y.y / (1 + y.y)");
    g2_string(grdev, 20, 80, "-1 <= c <= 1, -20 <= x, y <= 20");
    g2_string(grdev, 20, 60, "Valor de c: ");
    scanf("%f", &c);
    g2_string(grdev, 20, 40, "Valor inicial de x: ");
    scanf("%f", &x);
    g2_string(grdev, 20, 20, "Valor inicial de y: ");
    scanf("%f", &y);


    cnt = 0;
    do {
	cnt++;
	xx = y;
	y = -x + 2 * f(y, c);
	x = xx;
	cx = floor(x * 50) + (MAX_X >> 1);
	cy = floor(y * 50) + (MAX_Y >> 1);
	g2_plot(grdev, cx, cy);
    }
    while (cnt < 100000);
    putchar('\007');
    getchar();
}


atractor_de_Henon()
{
    double a, b, c, x, xx, y;
    int cx, cy;
    int cnt;
    extern int grdev;

    a = 3.1678;
    b = 0.3;
    y = x = 1;
    cnt = 0;
    do {
	cnt++;
	xx = y;
	y = b * x + a * y - (y * y);
	x = xx;
	cx = floor((y * 100) + 50);
	cy = MAX_Y - floor((x * 62.5) + 30);
	g2_plot(grdev, cx, cy);
    }
    while (cnt < 100000);
    putchar('\007');
    getchar();
}

main()
{
    char ch;
    extern int grdev;
    int xdev, pdev;

/*     grdev = g2_open_vd(); 
     xdev = g2_open_X11(MAX_X, MAX_Y); */
    pdev = g2_open_gd("map.png", MAX_X, MAX_Y, g2_gd_png);

/*     g2_attach(grdev, xdev);
     g2_attach(grdev, pdev); 
*/
    grdev = pdev;
/*     g2_set_auto_flush(grdev, 1); */

    mapa_logistico();
/*     getchar();
     
     do {
     mapa_logistico();
     ch = getchar();
     atractor_de_Henon();
     ch = getchar(); 
     Gumowski_and_Mira();
     ch = getchar();
     } while ((ch = getchar()) != 'f'); */
    g2_close(grdev);
}

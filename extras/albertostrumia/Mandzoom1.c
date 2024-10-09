/***********************************************
*	Mandzoom1.c
*
*  Generates a RAW format file of an image 
*  of Mandelbrot set (grayscale)
*
*  C source file by Alberto Strumia
*
************************************************/


#include<stdio.h>	/* loads the standard I/O header of ANSI library */

/* definition of constants */
#define Radius 10	/* convergence domain radius */
#define Cx 1.80	/* real coordinate of image center */
#define Cy 0	/*  imaginary coordinate of image center  */
#define Side 0.07	/* half side of the complex square domain */
#define M 700	/* number of pixels of each side of the image */
#define Num 255	/* max iteration number */

/* altenrative values: Cx 1.8616, 1.8622; Side 0.0015, 0.000030; Num 400, 700 */


/* main program */
main()
{
	/* declaration of variables */
	
	int p, q, n, w;	/* integer variables */
	double x, y, xx, yy, Incx, Incy;	/* double precision variables */
	
	
	/* data file */
	FILE *fp;
	fp = fopen("Mand1.pgm","w");	/* open data file */

    	fprintf(fp, "P2\n%d %d\n256\n", M, M, Num);

	/* scanning of the complex domain */
	for (p = 1; p <= M; p++)
		{
			Incy = Cy - Side + 2*Side/M*p;
			printf("%i %%\n", p*100/M);	/* screen message */

			for (q = 1; q <= M; q++)
				{
				Incx = Cx - Side + 2*Side/M*q;
				x = 0;
				y = 0;
				w = 200;	/* background color */
		
			/* escape method */
				for ( n = 1; n <= Num; ++n)
					{
					xx = x*x - y*y - Incx;
					yy = 2*x*y - Incy;
				
					x = xx;
					y = yy;
				
							if ( x*x + y*y > Radius )	/* divergence condition */
									{
									w = n;
									n = Num;
									}
					}

						fprintf(fp, "%d\n", w );	/* wrtie the escape speeds into the data file */
					
				}
		}
	
	fclose(fp);	/* close the file */
}

/* end of main program */

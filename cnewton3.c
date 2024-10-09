/*          ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
            บ                                                               บ
		  บ                                                    3          บ
		  บ      cnewton = MAP OF NEWTON'S METHOD FOR SOLVING Z  =  1     บ
            บ                                                               บ
            ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
*/
#include <stdio.h>
#include <math.h>
#ifndef DOS
#  include <grx20.h>
#  include <grxkeys.h>
#else
#  include <graph.h>
#endif

int maxcol = 639;
int maxrow = 349;

int max_colors = 16;

GrColor *egacolors;


int max_iterations = 64;
int max_size = 4;
float Xmax = 3.5,   	    /* the reange of values to compute */ 
      Xmin = -3.5, 
      Ymax = 2.50, 
      Ymin = -2.50;


void _setvideomode(int i)
{

    GrSetDriver("xwin");


    GrSetMode(GR_width_height_color_graphics, 640, 350, (GrColor) 256);

    egacolors = GrAllocEgaColors();

    GrSetRGBcolorMode();

}

void _clearscreen(int color)
{

    GrClearScreen(GrWhite());

}

int _color;


void _setcolor(int i)
{

//    _color = egacolors[i];
    _color = i;
}

void _setpixel_w(int col, int row)
{

    GrPlot(col, row, _color);

}

void getch()
{

    GrKeyRead();

}

main()
{

    double deltaX, deltaY, X, Y, Xsquare, Xold, Yold,
	Ysquare, Ytemp, temp1, temp2, denom, theta;
    int color, row, col, i;

    _setvideomode(16);

    _clearscreen(0);

    deltaX = (Xmax - Xmin) / (maxcol * 1.0);	/* compute value increment on X */

    deltaY = (Ymax - Ymin) / (maxrow * 1.0);	/* compute value increment on Y */

    for (col = 0; col <= maxcol; col++)
    {
	for (row = 0; row <= maxrow; row++)
	{
	    X = Xmin + col * deltaX;	/* compute new X */

	    Y = Ymax - row * deltaY;	/* compute new Y */

	    Ysquare = Xsquare = 0;  	/* reset temporaries */

	    Yold = Xold = 42;

	    for (i = 0; i < max_iterations; i++)
	    {

		Xsquare = X * X;
		Ysquare = Y * Y;

		denom =
		    3 * ((Xsquare - Ysquare) * (Xsquare - Ysquare) +
			 4 * Xsquare * Ysquare);

		if (denom == 0)
		    denom = .00000001;

		X = .6666667 * X + (Xsquare - Ysquare) / (denom * 1.0);

		Y = .6666667 * Y - 2 * X * Y / (denom * 1.0);

		if ((Xold == X) && (Yold == Y))
		    break;

		Xold = X;
		Yold = Y;
	    }

	    if (X > 0)
		color = i * 4;	/* scale up to 256 Blue*/
	    else
	    {
		if ((X < -.3) && (Y > 0))
		    color = (i * 4) << 8;   /* scale up to 256 Green */
		else
		    color = (i *4) << 16;   /* scale up to 256 Red */
	    }

	    _setcolor(color);
	    _setpixel_w(col, row);

	}

    }

    getch();

}

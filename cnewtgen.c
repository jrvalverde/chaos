/*+-------------------------------------------------------------+
  |                                                             |
  |   newtgen = PROGRAM TO MAP THE GENERALIZED NEWTON'S METHOD  |
  |                                                             |
  +-------------------------------------------------------------+*/

#include <stdio.h>
#ifdef MSDOS
#include <dos.h>
#include <conio.h>
#endif
#include <math.h>
#include <complex.h>
#include <grx20.h>
#include <grxkeys.h>

void cls(int color);
void plot(int x, int y, int color);
void setMode(int mode);

/* define double complex type  (C99)*/
typedef complex double cmplx;

#define cmplx(r,i) ((double)(r) + ((double)(i))*I)

char strings[80];
int col,row,i,j,flag=0,flag2=0;
int max_iterations = 64;
int max_size = 4;
float Xmax= 2.0, Xmin= -2.0, Ymax=1.20, Ymin=-1.20,deltaX,deltaY;
cmplx z,zz,c,one=1.0,point_two=.2,old_z;
cmplx f,f_prime,z_power;
float arguments[10]; 
float temp;
const int maxcol = 639;
const int maxrow = 349;
const int max_colors = 16;
#ifndef MSDOS
#ifdef OFFSCREEN
char screen[maxcol][maxrow];
int kbhit(), getch();
#else
GrColor *egacolors;
int kbhit();
int getch();
#endif
#endif

int main()
{
     int color, row, col, degree;

     for (i=0; i<10; i++)
     {
          arguments[i] = 0;
    	  printf("\nEnter %dth coefficient: ", i);
          scanf("%f", &arguments[i]);
     }
     setMode(16);
	for (i=9; i>=0; i--)
 	{
	 	if (arguments[i] != 0)
	 	{
			degree = i;
			break;
		}
	}
     cls(7);
     deltaX = (Xmax - Xmin)/(maxcol + 1);
     deltaY = (Ymax - Ymin)/(maxrow + 1);
     old_z = cmplx(42,42);
        for (col=0; col<=maxcol; col++)
        {
          if (kbhit() != 0) break;
          for (row=0; row<=maxrow; row++)
	  {
               z = cmplx(Xmin + col * deltaX,Ymax - row * deltaY);
               i = 0;
               while (i<max_iterations)
               {
                    f = 0;
		    f_prime = 0;
                    z_power = 1;
                    for(j=0; j<degree; j++)
                    { 
                         f = f + z_power * arguments[j];
                         f_prime = f_prime + (z_power * arguments[j+1]) *
                         	(float)(j+1);
                         z_power = z*z_power;
                    }
                    f = f + z_power * arguments[degree];
                    z = z - f/f_prime; 
                    if (z == old_z)
                         break;
                    old_z = z;
                    i++;
               }
               color = egacolors[i%16];
               plot(col, row, color);
          }
     }
     getch();
}

/* +---------------------------------------------------------+
   |                                                         |
   |                 cls() = Clears the Screen               |
   |                                                         |
   +---------------------------------------------------------+*/

#ifdef MSDOS
void cls(int color)
{
     union REGS reg;
     reg.x.ax = 0x0600;
     reg.x.cx = 0;
     reg.x.dx = 0x184F;
     reg.h.bh = color;
     int86(0x10,&reg,&reg);
}
#else
#ifdef OFFSCREEN
void cls(int color)
{
    int i, j;
    for (i = 0; i < maxcol; i++)
	for (j = 0; j < maxrow; j++)
	    screen[i][j] = color;
}
#else
void cls(int color)
{
    GrClearScreen(GrWhite());
}
#endif
#endif

/*  +---------------------------------------------------------+
    |                                                         |
    |         plot() = Plots a point at (x,y) in color        |
    |    for Enhanced Graphics Adapter, using Turbo C port    |
    |    output functions                                     |
    +---------------------------------------------------------+*/

#ifdef MSDOS
void plot(int x, int y, int color)
{
     #define dataOut(reg, value)   {outp(0x3CE, reg);\
                                    outp(0x3CF, value);}
      #define EGAaddress 0xA0000000L

      unsigned char mask, dummy;
      char far *color_address;

      color_address = (char far *) (EGAaddress +
           ((long)y * 80L + ((long)x / 8L)));
      mask = 0x80 >> (x % 8);
      dummy = *color_address;
      dataOut(0,color);
      dataOut(1,0x0F);
      dataOut(3,0);
      dataOut(8,mask);
      *color_address &= 0xFF;
      dataOut(0,0);
      dataOut(1,0);
      dataOut(3,0);
      dataOut(8,0xFF);
}
#else
#ifdef OFFSCREEN
void plot(int x, int y, int color)
{
    screen[x][y] = color;
}
#else
void plot(int x, int y, int color)
{
    GrPlot(x, y, color);
}
#endif
#endif

/*  +---------------------------------------------------------+
    |                                                         |
    |               setMode() = Sets Video Mode               |
    |                                                         |
    +---------------------------------------------------------+*/

#ifdef MSDOS
union REGS reg;
void setMode(int mode)
{
     reg.h.ah = 0;
     reg.h.al = mode;
     int86 (0x10,&reg,&reg);
}
#else
#ifdef OFFSCREEN
void setMode(int mode)
{
    return;
}
#else
void setMode(int mode)
{
    GrSetDriver("xwin");

    GrSetMode(GR_width_height_color_graphics, 640, 350, (GrColor) 256);

    egacolors = GrAllocEgaColors();
    /* RGB colors are in 0xRRGGBB format */

    GrSetRGBcolorMode();

}
#endif
#endif

#ifndef MSDOS
#ifdef OFFSCREEN
int getch()
{
    return 0;
}
#else
int getch()
{
    /* since it is called only once at the end of the program,
       screen could be saved to a file here */

    GrKeyRead();
    return 0;
}
#endif
int kbhit()
{
    return 0;
}
#endif

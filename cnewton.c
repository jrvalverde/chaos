/*          +---------------------------------------------------------------+
            |                                                               |
            |                                                               |
            |              newton = PROGRAM TO MAP THE NEWTON METHOD        |
            |                                                               |
            +---------------------------------------------------------------+
*/
#include <stdio.h>
#include <math.h>
#ifdef MSDOS
#   include <dos.h>
#   include <process.h>
#   include <conio.h>
#else
#   include <grx20.h>
#   include <grxkeys.h>
#endif
#include <complex.h>

/* forward declaration of auxiliary functions */
void cls(int color);	    	    	    	/* clear screen */
void move_cursor(int type, int color);	    	/* move cursor (unused) */
void plot(int x, int y, int color); 	    	/* draw a point */
void setMode(int mode);     	    	    	/* set drawing mode */
void writChar(char ch, int color, int page);	/* write a character (unused) */

/* define double complex type  (C99)*/
typedef complex double cmplx;

#define cmplx(r,i) ((double)(r) + ((double)(i))*I)

const int maxcol = 639;
const int maxrow = 349;
const int max_colors = 16;  	// unused

#ifndef MSDOS
GrColor *egacolors;
int kbhit();
int getch();
#endif


int max_iterations = 64;
double  Xmax = 2.0, 	/* interval range of values to compute */
    	Xmin = -2.0, 
	Ymax = 1.20, 
	Ymin = -1.20, 
	deltaX, 
	deltaY;
cmplx z, 
      c, 
      old_z, 
      z_power, 
      z_temp, 
      roots[10], 
      z_sq, 
      z_fourth, 
      z_eighth;

int degree;

int main()
{
    int i, j, color, row, col, flag;
    double a, b, threshold = 0.2, twopi = 6.2831853;

    setMode(16);
    cls(7);
    
    /* get parameters */
    printf("Enter degree of equation (3 - 10): ");
    scanf("%d", &degree);
    printf
	("\nEnter '0' for cyclical colors;\n'1' for colors associated with root: ");
    scanf("%d", &flag);

    if (degree < 3)
	degree = 3;
    if (degree > 10)
	degree = 10;
    if (flag < 0)
	flag = 0;
    if (flag > 1)
	flag = 1;


    a = 1.0 / (double) degree;
    b = 1 - a;
    for (i = 0; i < degree; i++) {
	roots[i] = cmplx(cos(i * twopi / (double) degree),
			 sin(i * twopi / (double) degree));
        printf("%d = %2.3f + %2.3fi\n", i, creal(roots[i]), cimag(roots[i]));
    }
    deltaX = (Xmax - Xmin) / (maxcol + 1);
    deltaY = (Ymax - Ymin) / (maxrow + 1);
    for (col = 0; col <= maxcol; col++) {
	if (kbhit() != 0)
	    break;
	for (row = 0; row <= maxrow; row++) {
	    z = cmplx(Xmin + col * deltaX, Ymax - row * deltaY);
	    old_z = cmplx(10000, 10000);
	    i = 0;
	    while ((z != old_z) && (i < 64)) {
/*              z.print("z");
                old_z.print("old z");  */
		old_z = z;
		z = z * 0.8 + cmplx(0.2, 0) / (z * z * z * z);
		z_temp = ~z;
		z_sq = z_temp * z_temp;
		if (degree > 3)
		    z_fourth = z_sq * z_sq;
		if (degree > 7)
		    z_eighth = z_fourth * z_fourth;
		switch (degree) {
		case 3:
		    z_power = z_temp * z_sq;
		    break;
		case 4:
		    z_power = z_fourth;
		    break;
		case 5:
		    z_power = z_temp * z_fourth;
		    break;
		case 6:
		    z_power = z_sq * z_fourth;
		    break;
		case 7:
		    z_power = z_temp * z_sq * z_fourth;
		    break;
		case 8:
		    z_power = z_eighth;
		case 9:
		    z_power = z_temp * z_eighth;
		    break;
		case 10:
		    z_power = z_sq * z_eighth;
		}
		z = z * b + (z_power) * a;
		i++;
	    }
//                      printf("Start new z\n");
//                      z.print("z");
	    if (flag == 1) {
		color = egacolors[15];
		for (j = 0; j < degree; j++) {
		    if (z == roots[j])
//			color = egacolors[1 + j % 8 + 8 * (i % 2)];
    	    	    	color = ((j * 256) / degree) + (i * 4);
		}
	    } else
		color = egacolors[i % 16];
//    	    	color = (i * 4) + (i << 10) + (i << 18);
	    plot(col, row, color);
	}
    }
    printf("Done\n");
    getch();
}


/*             +---------------------------------------------------------+
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
    int86(0x10, &reg, &reg);
}
#else
void cls(int color)
{

    GrClearScreen(GrWhite());

}
#endif

/*             +---------------------------------------------------------+
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
				  ((long) y * 80L + ((long) x / 8L)));
    mask = 0x80 >> (x % 8);
    dummy = *color_address;
    dataOut(0, color);
    dataOut(1, 0x0F);
    dataOut(3, 0);
    dataOut(8, mask);
    *color_address &= 0xFF;
    dataOut(0, 0);
    dataOut(1, 0);
    dataOut(3, 0);
    dataOut(8, 0xFF);
}
#else
void plot(int x, int y, int color)
{
    GrPlot(x, y, color);
}
#endif

/*             +---------------------------------------------------------+
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
    int86(0x10, &reg, &reg);
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

#ifndef MSDOS
int kbhit()
{
    //GrKeyRead();
    return 0;
}

int getch()
{
    /* since it is called only once at the end of the program,
       screen could be saved to a file here */

    GrKeyRead();
    return 0;
}
#endif

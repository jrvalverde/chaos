#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <g2.h>
#include <g2_gd.h>
#include <g2_X11.h>

#define MAX_X   640
#define MAX_Y   400

int grdev;                      /* the virtual graphics device we'll use */
double left, right, top, bottom;

double population, feedback;
int maximal_iteration;

void hello();
void application();
void compute_and_display();
void goodbye();

#define set_point(x, y) g2_plot(grdev, x, y)

void set_universal_point(double x, double y)
{
    double devx, devy;
    
    devx = ((x - left) * MAX_X ) / (right - left);
    devy = ((y - bottom) * MAX_Y) / (top - bottom);
    g2plot(grdev, round(devx), round(devy));
}

void text_mode()
{
    /* machine specific */
}

void graphics_mode()
{
    /* machine specific */
}

void enter_graphics()
{
    /* various actions to initialise the graphics
       such as graphics_mode, etc... */
    graphics_mode();
}

void exit_graphics()
{
    /* various actions to end the graphics, e.g. */
    while (! button());     /* button is machine specific */
    text_mode();
}

/* --------- end graphics procedures ------------*/

void main()
{
    hello();
    application();
    compute_and_display();
    goodbye();    
}

double f(double p, double k)
{
    /*
     * computes value of sick population percentage in next
     * iteration.
     *
     * p is the proportion of sick children
     * (1 -p) is therefore the proportion of well children
     * k is the infection rate and reflects the virulence of the disease
     *	 (new infections expected each day)
     *
     * Thus, we have
     *	z = k * p * (1 - p)     - percent of newly infected children
     *
     *  f(p) = p + ( k * p * (1 - p)) - total percent of infected kids
     */
    return p + (k * p * (1 - p));
}

void epidemy()
{
    int i;
    extern int maximal_iteration;
    extern double population, feedback;
    
    for ( i = 0; i < maximal_iteration; i++) {
    	population = f(population, feedback);
	printf("After %d iterations p has the value %g\n", i+1, population);
    }
}

void hello()
{
    printf("Calculation of measles-values\n\n");
    printf("Start:\n");
}

void application()
{
    extern int maximal_iteration;
    extern double population, feedback;
    float p, f;

    printf("Initial population p (0 to 1): ");
    scanf("%f", &p);
    population = p;
    printf("Feedback parameter k (0 to 3): ");
    scanf("%f", &f);
    feedback = f;
    printf("Max. iteration number: ");
    scanf("%d", &maximal_iteration);
    printf("population = %g feedback = %g max_iter = %d\n", population, feedback, maximal_iteration);
}

void compute_and_display()
{
    enter_graphics();
    epidemy();
    exit_graphics();
}

void goodbye()
{
    printf("Stopping: ");
}

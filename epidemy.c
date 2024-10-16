#include <stdio.h>
#include <stdlib.h>

double population, feedback;
int maximal_iteration;

void hello();
void application();
void compute_and_display();
void goodbye();

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
    epidemy();
}

void goodbye()
{
    printf("Stopping: ");
}

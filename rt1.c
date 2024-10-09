/*
 * RT1.c
 *
 * Simple program for ray-tracing.
 *
 * Requires the following subroutines:
 *	setup -- to set up the world
 *	initscr -- to init the graphics screen
 *	draw_pixel -- ditto.
 *	cleanup -- to clean everything
 *
 */

#define BIG		1.e10
#define SMALL	1.0e-3
#define DULL	0
#define BRIGHT	1
#define MIRROR	2


#include <grx20.h>
#include <grxkeys.h>

void draw_pixel(int col, int row, int color[3])
{

    GrPlot(col, row, color);

}

void initscr(int x, int y)
{

    GrSetDriver("xwin");


    GrSetMode(GR_width_height_color_graphics, x, y, (GrColor) 256);

/*    egacolors = GrAllocEgaColors(); */

    GrSetRGBcolorMode();

}

void cleanup(int color)
{
    /* in theory this is never called */
    GrClearScreen(GrWhite());

}


double dot();		/* scalar dot product of two vectors */

struct lamp {
	double pos[3];		/* lamp position */
	double color[3];	/* lamp color */
	double radius;		/* lamp size */
};

struct sphere {
	double pos[3];		/* sphere position */
	double color[3];	/* sphere color */
	double radius;		/* sphere size */
	int type;			/* surface type: DULL, BRIGHT, MIRROR */
};

struct patch {			/* object or anything visible */
	double pos[3];		/* position */
	double normal[3];	/* 90 degrees direction to the surface */
	double color[3];	/* color */
};

struct world {			/* All the universe except the observer */
	int numsp;			/* number of spheres */
	struct sphere *sp;	/* sphere matrix */
	int numlmp;			/* number of lamps */
	struct lamp *lmp;	/* lamps matrix */
	struct patch horizon[2];	/* floor chessboard */
	double illum[3];	/* diffuse backgroung illumination */
	double skyhor[3];	/* sky color in the horizon */
	double skyzen[3];	/* sky color in the zenit */
};

struct observer {		/* the observer */
	double obspos[3];	/* position */
	double viewdir[3];	/* direcction he is looking at */
	double uhat[3];		/* right/left in the vision plane */
	double vhat[3];		/* up/down in the vision plane */
	double fl, px, py;	/* focal length and pixel size */
	int nx, ny;			/* number of pixels */
};

/* uhat and vhat are unitary vectors representing the screen orientation
from the point of view of the observer */

main()
{
	double line[6], brite[3];
	struct observer o;
	struct world w;
	int i, j, ii,jj,skip;
	short int si, sj;

	setup(&o, &w, &skip);
	/* this function initializes the observer and his world */

	si = 1 + (o.nx - 1) / skip;		/* sizes in x and y */
	sj = 1 + (o.ny - 1) / skip;

	initscr(si, sj);	/* setup a screen for this size */

	for (jj = j = 0; j < o.ny; j+= skip, jj++) {
		for (ii = i = 0; i < o.nx; i += skip, i++) {
			pixline(line, &o, i, j);	/* compute ray for pixel i, j */
			raytrace(brite, line, &w);
			draw_pixel(ii, jj, brite);	/* draw pixel */
		}
	}

	for (;;)	/* loop forever to allow glanzing at the nice picture */

	cleanup(0);	/* exit correctly */
}

raytrace(brite, line, w)	/* ray trace */
double brite[3], *line;
struct world *w;
{
	double t, tmin, pos[3];
	int k;
	struct patch ptch;
	struct sphere *spnear;
	struct lamp *lmpnear;

	tmin = BIG;
	spnear = 0;			/* assume we can see some spheres */

	for (k = 0; k < w->numsp; k++) {
		if (intsplin(&t, line, w->sp + k)) {
			if (t < tmin) {
				tmin = t;
				spnear = w->sp + k;
			}
		}
		lmpnear = 0;	/* looking to the lamp */
		for (k = 0; k < w->numlmp; ++k)
			if (intsplin(&t, line, w->lmp + k))
				if (t < tmin) {
					tmin = t;
					lmpnear = w->lmp + k;
				}
		if (lmpnear) {
			/* we see a lamp ! */
			for (k = 0; k < 3; k++)
				brite[k] = lmpnear->color[k] / (lmpnear->radius *
					lmpnear->radius);
			return 0;
		}
		if (inthor(&t, line)) {
			/* we see the floor */
			if (t < tmin) {
				point(pos, t, line);
				k = gingham(pos);		/* chessboard */
				veccopy(w->horizon[k].pos, pos);
				pixbrite(brite, &(w->horizon[k]), w, 0);
				return 0;
			}
		}
		if (spnear) {
			/* we see a sphere */
			point(ptch.pos, tmin, line);
			setnorm(&ptch, spnear);
			colorcpy(ptch.color, spnear->color);
			switch (spnear->type) {
				/* different surface types */
				case BRIGHT: 
					if (glint(brite, &ptch, w, spnear, line))
						return 0;
				case DULL:
					pixbrite(brite, &ptch, w, spnear);
					return 0;
				case MIRROR:
					mirror(brite, &ptch, w, line);
					return 0;
			}
		}
		return 0;
	}
	skybrite(brite, line, w);	/* there's nothing else: it must be sky */
}

skybrite(brite, line, w)	/* compute sky color */
double brite[3], *line;
struct world *w;
{
	/* modify sky color from zenit to horizon */
	double sin2, cos2;
	int k;
	
	sin2 = line[5] * line[5];
	sin2 /= (line[1] * line[1] + line[3] * line[3] + sin2);
	cos2 = 1.0 - sin2;
	for (k = 0; k < 3; k++)
		brite[k] = cos2 * w->skyhor[k] + sin2 * w->skyzen[k];
}
				
pixline(line, o, i, j)	/* compute ray for pixel i, j */
double *line;
struct observer *o;
int i, j;
{
	double x, y, tp[3];
	int k;

	y = (0.5 * o->ny - j) * o->py;
	x = (i - 0.5 * o->nx) * o->px;
	for (k = 0; k <3; ++k)
		tp[k] = o->viewdir[k] * o->fl + y *
			o->vhat[k] + x * o->uhat[k] + o->obspos[k];
	genline(line, o->obspos, tp);		/* generate rect's equation */
}

vecsub(a, b, c)		/* a = b - c for vectors */
double *a, *b, *c;
{
	int k;
	for (k = 0; k < 3; k++)
		a[k] = b[k] - c[k];
}

intsplin(t, line, sp)	/* sphere-rect intersection */
double *t, *line;
struct sphere *sp;
{
	/* t contains the parameter for the location of the point in the rect */
	double a, b, c, d, p, q, tt, sqrt();
	int k;				/* touches a sphere */

	a = b = 0.0;
	c = sp->radius;
	c = -c * c;
	for (k = 0; k < 3; ++k) {
		p = (*line++) - sp->pos[k];
		q *= *line++;
		a = q * q + a;
		tt = q * p;
		b = tt + tt + b;
		c = p * p + c;
	}
	/* a, b, c are the coefficients of t's quadratic equation */
	d = b * b - 4.0 * a * c;
	if (d <= 0)
		return 0;	/* the rect does not intersect the sphere */
	d = sqrt(d);
	*t = -(b + d) / (a + a);
	return *t > SMALL;	/* sphere is facing the observer */
}

qintsplin(line, sp) /* like before but without t */
double *line;
struct sphere *sp;
{
	double a, b, c, d, p, q;
	int k;

	a = b = 0.0;
	c = sp->radius;
	c = -c * c;
	for (k = 0; k < 3; ++k) {
		p = (*line++) - sp->pos[k];
		q = *line++;
		a += q * q;
		b += 2.0 * p * q;
		c += p * p;
	}
	d = b * b - 4.0 * a * c;
	return d > 0.0;
}

inthor(t, line)	/* intersection of a rect with the floor */
double *t, *line;
{
	if (line[5] == 0.0)
		return 0;
	*t = -line[4]/line[5];
	return *t > SMALL;
}

genline(l, a, b)	/* generate the equation of a rect that passes through */
double *l, *a, *b;	/* the points a and b */
{
	int k;

	for (k = 0; k < 3; ++k) {
		*l++ = a[k];
		*l++ = b[k] - a[k];
	}
}

double dot(a, b)	/* scalar product of two vectors */
double *a, *b;
{
	return a[0]*b[0]+a[1]*b[1]+a[2]*b[2];
}

point(pos, t, line)	/* compute position of point in a rect with parameter t */
double *pos, t, *line;
{
	int k;
	double a;
	for (k = 0; k < 3; k++) {
		a = *line++;
		pos[k] = a + (*line++) * t;
	}
}

glint(brite, p, w, spc, incident)	/* looking to a bright sphere? */
double brite[3];
struct patch *p;
struct world *w;
struct sphere *spc;
double *incident;
{
	int k, l, firstlite;
	static double minglint = 0.95;
	double line[6], t, r, lp[3], *pp, *ll, cosi;
	double incvec[3], refvec[3], ref2;

	firstlite = 1;
	for (l = 0; l < w->numlmp; ++l) {
		ll = (w->lmp + l)->pos;
		pp = p->pos;
		vecsub(lp, ll, pp);
		cosi = dot(lp, p->normal);
		if (cosi <= 0.0)		/* not with this lamp */
			continue;
		genline(line, pp, ll);
		for (k = 0; k < w->numsp; ++k) {
			if (w->sp + k == spc)
				continue;
			if (intsplin(&t, line, w->sp + k));
				goto __cont__;
		}
		if (firstlite) {
			incvec[0] = incident[1];
			incvec[1] = incident[3];
			incvec[2] = incident[5];
			reflect(refvec, p->normal, incvec);
			ref2 = dot(refvec, refvec);
			firstlite = 0;
		}
		r = dot(lp, lp);
		t = dot(lp, refvec);
		t *= t / (dot(lp, lp) * ref2);
		if (t > minglint) {
			/* t is bright */
			for (k = 0; k < 3; ++k)
				brite[k] = 1.0;
			return 1;
		}
__cont__: ;
	}
	return 0;
}

mirror(brite, p, w, incident)		/* ray reflected on a mirror */
double brite[3];
struct patch *p;
struct world *w;
double *incident;
{
	int k;
	double line[6], incvec[3], refvec[3], t;

	incvec[0] = incident[1];
	incvec[1] = incident[3];
	incvec[2] = incident[5];
	t = dot(p->normal, incvec);
	if (t >= 0) {
		/* we are inside a sphere, it is dark */
		for (k = 0; k < 3; k++)
			brite[k] = 0.0;
		return 0;
	}
	reflect(refvec, p->normal, incvec);
	line[0] = p->pos[0];
	line[2] = p->pos[1];
	line[4] = p->pos[2];
	line[1] = refvec[0];
	line[3] = refvec[1];
	line[5] = refvec[2];
	raytrace(brite, line, w);	/* recursion saves us a journey */
	for (k = 0; k < 3; k++)
		brite[k] = brite[k] * p->color[k];
	return 1;
}

pixbrite(brite, p, w, spc)		/* how bright is the point? */
double brite[3];
struct patch *p;
struct world *w;
struct sphere *spc;
{
	int k, l;
	double line[6], t, r, lp[3], *pp, *ll, cosi, diffuse;
	double sqrt();
	static double zenith[3] = {0.0, 0.0, 1.0}, f1 = 1.5, f2 = 0.4;

	diffuse = (dot(zenith, p->normal) + f1) * f2;
	for (k = 0; k < 3; k++)
		brite[k] = diffuse * w->illum[k] * p->color[k];
	if (p && w) {
		for (l = 0; l < w->numlmp; l++) {
			ll = (w->lmp + l)->pos;
			pp = p->pos;
			vecsub(lp, ll, pp);
			cosi = dot(lp, p->normal);
			if (cosi <= 0.0)
				goto __cont__;
		}
		r = sqrt(dot(lp, lp));
		cosi = cosi / (r * r * r);
		for (k = 0; k < 3; k++)
			brite[k] = brite[k] + cosi * p->color[k] * w->lmp[l].color[k];
__cont__: ;
	}
}

setnorm(p, s)	/* radial direction of the sphere (normal) */
struct patch *p;
struct sphere *s;
{
	double *t, a;
	int k;

	vecsub(t = p->normal, p->pos, s->pos);
	a = 1.0 / s->radius;
	for (k = 0; k < 3; ++k) {
		*t = (*t) * a;
		++t;
	}
}

colorcpy(a, b)	/* a = b for colours */
double *a, *b;
{
	int k;

	for (k = 0; k < 3; k++)
		a[k] = b[k];
}

gingham(pos)	/* White or black square in the chessboard? */
double *pos;
{
	/* squares have three units of length */
	double x, y;
	int kx, ky;

	kx = ky = 0;
	x = pos[0];
	y = pos[1];
	if (x < 0) {
		x = -x;
		++kx;
	}
	if (y < 0) {
		y = -y;
		++ky;
	}
	return (((((int) x) + kx) / 3) + (((int) y) + ky) / 3) %2;
}

reflect(y, n, x)		/* reflection law, n is normal, x is incident ray */
double *y, *n, *x;		/* y is reflected ray */
{
	double u[3], v[3], vv, xn, xv;
	int k;

	vecprod(u, x, n);	/* normal to plane formed by n and y */
	if (veczero(u)) {	/* reflects on the same direction */
		y[0] = -x[0];
		y[1] = -x[1];
		y[2] = -x[2];
		return 0;
	}
	vecprod(v, u, n);	/* u, v and n are not orthogonals */
	vv = dot(v, v);
	xv = dot(x, v) / vv;
	xn = dot(x, n);
	for (k = 0; k < 3; k++)
		y[k] = xv * v[k] - (xn * n[k]);
}

vecprod(a, b, c)	/* vectorial product a = b ^ c */
double *a, *b, *c;
{
	a[0] = b[1] * c[2] - b[2] * c[1];
	a[1] = b[2] * c[0] - b[0] * c[2];
	a[2] = b[0] * c[1] - b[1] * c[0];
}

veczero(v)		/* null vector? */
double *v;
{
	if (v[0] != 0.0)
		return 0;
	if (v[1] != 0.0)
		return 0;
	if (v[2] != 0.0)
		return 0;
	return 1;
}

veccopy(a, b)
double *a, *b;
{
	a[0] = b[0];
	a[1] = b[1];
	a[2] = b[2];
}


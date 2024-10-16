chaos:	chaos-grx.c
	gcc -o chaos chaos-grx.c -lgrx -lgd -lpng -ltiff -lX11 -lm

chaos2:	chaos-g2.c
	gcc -o chaos2 chaos-g2.c -lg2 -lgd -lpng -ltiff -lX11 -lm

cnewton3: cnewton3.c
	gcc -o cnewton3 cnewton3.c -lgrx -lgd -lpng -ltiff -lX11 -lm

cnewton: cnewton.c
	gcc -std=c99 -o cnewton cnewton.c -lgrx -lgd -lpng -ltiff -lX11 -lm

cnewtgen: cnewtgen.c
	gcc -std=c99 -o cnewtgen cnewtgen.c -lgrx -lgd -lpng -ltiff -lX11 -lm

cmplx: cmplx.cc
	gcc -o cmplx cmplx.cc -lstdc++

#
#
#

epidemy: epidemy.c
	gcc -o epidemy epidemy.c -lgrx -lgd -lpng -ltiff -lX11 -lm

epidemy2: epidemy-g2.c
	gcc -o epidemy2 epidemy-g2.c -lg2 -lgd -lpng -ltiff -lX11 -lm

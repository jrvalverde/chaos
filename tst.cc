#include <iostream>
#include <complex>

using namespace std;

main ()
{
  complex xx;
  complex yy = complex (1, 2.718);
  xx = log (yy / 3);
  cout << 1 + xx;
}

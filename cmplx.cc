
#include <cmath>
#include <iostream>
#include <iomanip>

using namespace std;


class cmplx {
  private:
    float real;			// Real Part
    float imag;			//  Imaginary Part


  public:
     cmplx(float, float);
     cmplx(const cmplx &);
    cmplx operator +(cmplx);
    cmplx operator -(cmplx);
    cmplx operator *(cmplx);
    cmplx operator /(cmplx);
    cmplx getconjugate();
    cmplx getreciprocal();
    float getmodulus();
    void setdata(float, float);
    void getdata();
    float getreal();
    float getimaginary();
    bool operator ==(cmplx);
    void operator =(cmplx);
    friend ostream & operator <<(ostream & s, cmplx & c);
};
//                                        CONSTRUCTOR
cmplx::cmplx(float r = 0.0f, float im = 0.0f)
{
    real = r;
    imag = im;
}

//                                 COPY CONSTRUCTOR
cmplx::cmplx(const cmplx & c)
{
    this->real = c.real;
    this->imag = c.imag;
}


void cmplx::operator =(cmplx c)
{
    real = c.real;
    imag = c.imag;
}


cmplx cmplx::operator +(cmplx c)
{
    cmplx tmp;
    tmp.real = this->real + c.real;
    tmp.imag = this->imag + c.imag;
    return tmp;
}

cmplx cmplx::operator -(cmplx c)
{
    cmplx tmp;
    tmp.real = this->real - c.real;
    tmp.imag = this->imag - c.imag;
    return tmp;
}

cmplx cmplx::operator *(cmplx c)
{
    cmplx tmp;
    tmp.real = (real * c.real) - (imag * c.imag);
    tmp.imag = (real * c.imag) - (imag * c.real);
    return tmp;
}

cmplx cmplx::operator /(cmplx c)
{
    float div = (c.real * c.real) + (c.imag * c.imag);
    cmplx tmp;
    tmp.real = (real * c.real) + (imag * c.imag);
    tmp.real /= div;
    tmp.imag = (imag * c.real) - (real * c.imag);
    tmp.imag /= div;
    return tmp;
}

cmplx cmplx::getconjugate()
{
    cmplx tmp;
    tmp.real = this->real;
    tmp.imag = this->imag * -1;
    return tmp;
}

cmplx cmplx::getreciprocal()
{
    cmplx t;
    t.real = real;
    t.imag = imag * -1;
    float div;
    div = (real * real) + (imag * imag);
    t.real /= div;
    t.imag /= div;
    return t;
}

float cmplx::getmodulus()
{
    float z;
    z = (real * real) + (imag * imag);
    z = sqrt(z);
    return z;
}

void cmplx::setdata(float r, float i)
{
    real = r;
    imag = i;
}

void cmplx::getdata()
{
    cout << "Enter Real:";
    cin >> this->real;
    cout << "Enter Imaginary:";
    cin >> this->imag;

}

float cmplx::getreal()
{
    return real;
}

float cmplx::getimaginary()
{
    return imag;
}

bool cmplx::operator ==(cmplx c)
{
    return (real == c.real) && (imag == c.imag) ? 1 : 0;
}

ostream & operator <<(ostream & s, cmplx & c)
{
    s << "Real Part = " << c.real << endl
	<< "Imaginary Part = " << c.imag << endl;
    s << "z = " << c.real << setiosflags(ios::showpos)
	<< c.imag << "i" << endl << resetiosflags(ios::showpos);
    return s;
}



int main()
{
    cmplx a(10.0f, -2.f);	// Calls Constructor
    cout << a << endl;		// Calls the overloaded << operator
    cmplx b(-2);		// Calls Constructor
    cmplx c = b;		// Calls Copy Constructor
    c = a;			// calls overloaded = operator
    b.getdata();		// Calls Getdata()
    c.getdata();
    if (b == c)			// calls overloaded == operator
	cout << "b == c";
    else
	cout << "b != c";


    cout << endl << c.getmodulus() << endl;	// calls getmodulus function()
    cmplx d;
    d = a + b;			// Calls overloaded +
    cout << d << endl;
    d = a - b;			// Calls overloaded -
    cout << d << endl;
    d = a * b;			// calls overloaded *
    cout << d << endl;
    d = a / b;			// calls overloaded /
    cout << d << endl;

    return 0;
}

class ComplexNumber:
    def __init__(self, re, im):
        self.re = re;
        self.im = im;

    def __repr__(self):
        return f"{self.re}+{self.im}i";

    def __abs__(self):
        import math;
        return math.sqrt(math.pow(self.re, 2) + math.pow(self.im, 2));

    def inc(self, re_koef, im_koef):
        self.re += re_koef;
        self.im += im_koef;

    def inc_by_complex(self, complx):
        self.re += complx.re;
        self.im += complx.im;

    def __add__(self,other):
        return ComplexNumber(self.re + other.re, self.im + other.im);

    def __sub__(self,other):
        return ComplexNumber(self.re - other.re, self.im - other.im);

if __name__ == "__main__":
    a = ComplexNumber(1,2);
    b = ComplexNumber(2,2);
    print(f"{a},{b}");
    c = a + b;
    print(c);
    print(abs(c));
    c.inc(1,2);
    print(c);
    c.inc_by_complex(a);
    print(c);
    d = c - a - b;
    print(d);
    d.inc(-2,-4);
    print(d);

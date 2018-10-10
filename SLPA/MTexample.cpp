# 5 "MTexample.cpp"
#include <iostream>
#include <fstream>
#include "MersenneTwister.h"

using namespace std;

void func(){
 MTRand mtrand1;
 for(int i=0;i<10;i++){
  int h = mtrand1.randInt(3);
  cout << "One in the range [0,3]:  " << h << endl;
 }
}

int mainexam( int argc, char * const argv[] ){



 MTRand mtrand1;



 double a = mtrand1();



 double b = mtrand1.rand();

 cout << "Two real numbers in the range [0,1]:  ";
 cout << a << ", " << b << endl;





 unsigned long c = mtrand1.randInt();

 cout << "An integer in the range [0," << 0xffffffffUL;
 cout << "]:  " << c << endl;



 int d = mtrand1.randInt( 42 );

 cout << "An integer in the range [0,42]:  " << d << endl;




 double e = mtrand1.randExc();

 cout << "A real number in the range [0,1):  " << e << endl;




 double f = mtrand1.randDblExc();

 cout << "A real number in the range (0,1):  " << f << endl;




 double g = mtrand1.rand( 2.5 );
 double h = mtrand1.randExc( 10.0 );
 double i = 12.0 + mtrand1.randDblExc( 8.0 );

 cout << "A real number in the range [0,2.5]:  " << g << endl;
 cout << "One in the range [0,10.0):  " << h << endl;
 cout << "And one in the range (12.0,20.0):  " << i << endl;






 cout << "A few grades from a class with a 52 pt average ";
 cout << "and a 9 pt standard deviation:" << endl;
 for( int student = 0; student < 20; ++student )
 {
  double j = mtrand1.randNorm( 52.0, 9.0 );
  cout << ' ' << int(j);
 }
 cout << endl;
# 100 "MTexample.cpp"
 MTRand mtrand2a( 1973 );

 double k1 = mtrand2a();

 MTRand mtrand2b( 1973 );

 double k2 = mtrand2b();

 cout << "These two numbers are the same:  ";
 cout << k1 << ", " << k2 << endl;



 mtrand2a.seed( 1776 );
 mtrand2b.seed( 1941 );

 double l1 = mtrand2a();
 double l2 = mtrand2b();

 cout << "Re-seeding gives different numbers:  ";
 cout << l1 << ", " << l2 << endl;
# 130 "MTexample.cpp"
 MTRand::uint32 seed[ MTRand::N ];
 for( int n = 0; n < MTRand::N; ++n )
  seed[n] = 23 * n;
 MTRand mtrand3( seed );

 double m1 = mtrand3();
 double m2 = mtrand3();
 double m3 = mtrand3();

 cout << "We seeded this sequence with 19968 bits:  ";
 cout << m1 << ", " << m2 << ", " << m3 << endl;
# 157 "MTexample.cpp"
 MTRand mtrand4;



 MTRand::uint32 randState[ MTRand::SAVE ];

 mtrand4.save( randState );



 ofstream stateOut( "state.dat" );
 if( stateOut )
 {
  stateOut << mtrand4;
  stateOut.close();
 }

 unsigned long n1 = mtrand4.randInt();
 unsigned long n2 = mtrand4.randInt();
 unsigned long n3 = mtrand4.randInt();

 cout << "A random sequence:       "
      << n1 << ", " << n2 << ", " << n3 << endl;



 mtrand4.load( randState );

 unsigned long o4 = mtrand4.randInt();
 unsigned long o5 = mtrand4.randInt();
 unsigned long o6 = mtrand4.randInt();

 cout << "Restored from an array:  "
      << o4 << ", " << o5 << ", " << o6 << endl;

 ifstream stateIn( "state.dat" );
 if( stateIn )
 {
  stateIn >> mtrand4;
  stateIn.close();
 }

 unsigned long p7 = mtrand4.randInt();
 unsigned long p8 = mtrand4.randInt();
 unsigned long p9 = mtrand4.randInt();

 cout << "Restored from a stream:  "
      << p7 << ", " << p8 << ", " << p9 << endl;



 MTRand mtrand5( mtrand3 );

 double q1 = mtrand3();
 double q2 = mtrand5();

 cout << "These two numbers are the same:  ";
 cout << q1 << ", " << q2 << endl;

 mtrand5 = mtrand4;

 double r1 = mtrand4();
 double r2 = mtrand5();

 cout << "These two numbers are the same:  ";
 cout << r1 << ", " << r2 << endl;



 MTRand mtrand6;
 double s = mtrand6();
 double t = mtrand6.randExc(0.5);
 unsigned long u = mtrand6.randInt(10);




 cout << "Your lucky number for today is "
      << s + t * u << endl;


 func();




 return 0;
}

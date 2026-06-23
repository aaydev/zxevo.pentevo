from sympy import *
import math

#w1=184054.140601213
#w2=191540.313016980
#q=1.02654691835174

w1=30000*2*math.pi
w2=w1
q=1.0



print('w1={}\nw2={}\nq={}\n\n'.format(w1,w2,q))

A=1/w1 + 1/(w2*q)
B=1/(q*w1*w2) + 1/(w2*w2)
C=1/(w1*w2*w2)

print('A={}\nB={}\nC={}\n\n'.format(A,B,C))

r1 = 1.2e3
r2 = 47e3
r3 = 68e3
c3 = 10e-12

r4,c1,c2 = symbols('r4 c1 c2',positive=True)

eq1 = Eq( (c1*r1*r2 + c3*r1*r3 + c3*r1*r4 + c3*r2*r3 + c3*r2*r4 + c3*r3*r4)/(r1 + r2), A )
eq2 = Eq( c3*(c1*r1*r2*r3 + c1*r1*r2*r4 + c1*r1*r3*r4 + c2*r1*r3*r4 + c2*r2*r3*r4)/(r1 + r2), B )
eq3 = Eq( c1*c2*c3*r1*r2*r3*r4/(r1 + r2), C )

s=solve( [eq1,eq2,eq3], [c1,c2,r4], dict=True )

print(s)


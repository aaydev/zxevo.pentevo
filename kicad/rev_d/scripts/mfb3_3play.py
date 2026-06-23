from sympy import *

A,B,C = symbols('A B C')
r1,r2,r3,r4,c1,c2,c3 = symbols('r1 r2 r3 r4 c1 c2 c3')

eq1 = Eq( (c1*r1*r2 + c3*r1*r3 + c3*r1*r4 + c3*r2*r3 + c3*r2*r4 + c3*r3*r4)/(r1 + r2), A )
eq2 = Eq( c3*(c1*r1*r2*r3 + c1*r1*r2*r4 + c1*r1*r3*r4 + c2*r1*r3*r4 + c2*r2*r3*r4)/(r1 + r2), B )
eq3 = Eq( c1*c2*c3*r1*r2*r3*r4/(r1 + r2), C )

s=solve( [eq1,eq2,eq3], [c1,c2,r4], dict=True, manual=True, simplify=False )

print(s)


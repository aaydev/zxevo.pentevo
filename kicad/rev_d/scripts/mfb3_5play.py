from sympy import *
import math

#r1 = 1.2e3
#r2 = 47e3
#r3 = 68e3
#c3 = 10e-12

r1,r2,r3,r4,c1,c2,c3 = symbols('r1,r2,r3,r4,c1,c2,c3')

A,B,C,D,E,F,G,H,K,L,M = symbols('A,B,C,D,E,F,G,H,K,L,M')

eq1 = Eq( c1*E+r4*F, D )
eq2 = Eq( c1*H + c1*r4*K + c2*r4*L, G )
eq3 = Eq( c1*c2*r4, M )

s=solve( [eq1,eq2,eq3], [r4,c1,c2], dict=True, manual=True, simplify=False )

print(s)
print('')

for ss in s:
    print('\n===')
    print('r4={}\n'.format(ss[r4]))
    print('c1={}\n'.format(ss[c1]))
    print('c2={}\n'.format(ss[c2]))



D = A*(r1+r2)-c3*c1*c2
E = r1*r2
F = c3*(r1+r2+r3)
G = B*(r1+r2)/c3
H = r1*r2*r3
K = r1*(r2+r3)
L = r3*(r1+r2)
M = C*(r1+r2)/(c3*r1*r2*r3)


A  = (c1*r1*r2 + c3*r1*r3 + c3*r1*r4 + c3*r2*r3 + c3*r2*r4 + c3*r3*r4)/(r1 + r2)
B  = c3*(c1*r1*r2*r3 + c1*r1*r2*r4 + c1*r1*r3*r4 + c2*r1*r3*r4 + c2*r2*r3*r4)/(r1 + r2)
C  = c1*c2*c3*r1*r2*r3*r4/(r1 + r2)


from sympy import *
import math


r1,r2,r3,r4,c1,c2,c3 = symbols('r1,r2,r3,r4,c1,c2,c3')

A,B,C,D,EE,F,G,H,K,L,M = symbols('A,B,C,D,EE,F,G,H,K,L,M')

eq1 = Eq( c1*EE+r4*F, D )
eq2 = Eq( c1*H + c1*r4*K + c2*r4*L, G )
eq3 = Eq( c1*c2*r4, M )

s=solve( [eq1,eq2,eq3], [r4,c1,c2], dict=True, manual=True, simplify=False )

print(s)
print('')

print('num solutions: {}'.format(len(s)))
print('')

for ss in s:
    print('\n===')
    print('r4={}\n'.format(ss[r4]))
    print('c1={}\n'.format(ss[c1]))
    print('c2={}\n'.format(ss[c2]))




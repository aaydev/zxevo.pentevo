from sympy import *
from math import *

A,B,C = symbols('A B C')

beta,gamma,kappa = symbols('beta gamma kappa')

# this set seems to be correct
kappa=sympify('2**(1/6)*sqrt(3)*A/sqrt(3*2**(2/3)*A*C/(9*A*B*C - 2*B**3 - 27*C**2 + 3*sqrt(3)*sqrt(4*A**3*C**3 - A**2*B**2*C**2 - 18*A*B*C**3 + 4*B**3*C**2 + 27*C**4))**(1/3) - 2**(2/3)*B**2/(9*A*B*C - 2*B**3 - 27*C**2 + 3*sqrt(3)*sqrt(4*A**3*C**3 - A**2*B**2*C**2 - 18*A*B*C**3 + 4*B**3*C**2 + 27*C**4))**(1/3) + 2**(1/3)*B - (9*A*B*C - 2*B**3 - 27*C**2 + 3*sqrt(3)*sqrt(4*A**3*C**3 - A**2*B**2*C**2 - 18*A*B*C**3 + 4*B**3*C**2 + 27*C**4))**(1/3)) - 3*sqrt(6)*C/(3*2**(2/3)*A*C/(9*A*B*C - 2*B**3 - 27*C**2 + 3*sqrt(3)*sqrt(4*A**3*C**3 - A**2*B**2*C**2 - 18*A*B*C**3 + 4*B**3*C**2 + 27*C**4))**(1/3) - 2**(2/3)*B**2/(9*A*B*C - 2*B**3 - 27*C**2 + 3*sqrt(3)*sqrt(4*A**3*C**3 - A**2*B**2*C**2 - 18*A*B*C**3 + 4*B**3*C**2 + 27*C**4))**(1/3) + 2**(1/3)*B - (9*A*B*C - 2*B**3 - 27*C**2 + 3*sqrt(3)*sqrt(4*A**3*C**3 - A**2*B**2*C**2 - 18*A*B*C**3 + 4*B**3*C**2 + 27*C**4))**(1/3))**(3/2)')
beta=sympify('3*C*(18*A*B*C - 4*B**3 - 54*C**2 + 6*sqrt(3)*sqrt(C**2*(4*A**3*C - A**2*B**2 - 18*A*B*C + 4*B**3 + 27*C**2)))**(1/3)/(3*2**(2/3)*A*C - 2**(2/3)*B**2 + (2**(1/3)*B - (9*A*B*C - 2*B**3 - 27*C**2 + 3*sqrt(3)*sqrt(C**2*(4*A**3*C - A**2*B**2 - 18*A*B*C + 4*B**3 + 27*C**2)))**(1/3))*(9*A*B*C - 2*B**3 - 27*C**2 + 3*sqrt(3)*sqrt(C**2*(4*A**3*C - A**2*B**2 - 18*A*B*C + 4*B**3 + 27*C**2)))**(1/3))')
gamma=sympify('2**(5/6)*sqrt(3)*sqrt((3*2**(2/3)*A*C - 2**(2/3)*B**2 + (2**(1/3)*B - (9*A*B*C - 2*B**3 - 27*C**2 + 3*sqrt(3)*sqrt(C**2*(4*A**3*C - A**2*B**2 - 18*A*B*C + 4*B**3 + 27*C**2)))**(1/3))*(9*A*B*C - 2*B**3 - 27*C**2 + 3*sqrt(3)*sqrt(C**2*(4*A**3*C - A**2*B**2 - 18*A*B*C + 4*B**3 + 27*C**2)))**(1/3))/(9*A*B*C - 2*B**3 - 27*C**2 + 3*sqrt(3)*sqrt(C**2*(4*A**3*C - A**2*B**2 - 18*A*B*C + 4*B**3 + 27*C**2)))**(1/3))/6')

#print('\n=== beta ===')
#pprint(beta)
#print('\n=== gamma ===')
#pprint(gamma)
#print('\n=== kappa ===')
#pprint(kappa)


r1,r2,r3,r4,c1,c2,c3 = symbols('r1 r2 r3 r4 c1 c2 c3')

r1 = 1.2e3
r2 = 47e3
r3 = 68e3
r4 = 180e3
c1 = 0.0047e-6
c2 = 220e-12
c3 = 10e-12


AA,BB,CC = symbols('AA BB CC')
AA = (c1*r1*r2 + c3*r1*r3 + c3*r1*r4 + c3*r2*r3 + c3*r2*r4 + c3*r3*r4)/(r1 + r2)
BB = c3*(c1*r1*r2*r3 + c1*r1*r2*r4 + c1*r1*r3*r4 + c2*r1*r3*r4 + c2*r2*r3*r4)/(r1 + r2)
CC = c1*c2*c3*r1*r2*r3*r4/(r1 + r2)

w1= (1/beta ).evalf(subs={A:AA,B:BB,C:CC})
w2= (1/gamma).evalf(subs={A:AA,B:BB,C:CC})
q = (1/kappa).evalf(subs={A:AA,B:BB,C:CC})

print('from values:')
print('f1={}\nf2={}\nq={}'.format(w1/(2*pi),w2/(2*pi),q))

#print('w1={}'.format( (1/(beta)).evalf(subs={A:AA,B:BB,C:CC})))
#print('w2={}'.format( (1/(gamma)).evalf(subs={A:AA,B:BB,C:CC})))
#print('q={}'.format( (1/kappa).evalf(subs={A:AA,B:BB,C:CC})))

print('A={}\nB={}\nC={}'.format(AA,BB,CC))


print('\nfrom freq/q:')
f1=Float(30000.0)
f2=Float(30000.0)
qq=Float(1.0)

ww1=N(f1*2*pi,50)
ww2=N(f2*2*pi,50)

AAA = (1/ww1 + 1/(ww2*qq))
BBB = (1/(q*ww1*ww2) + 1/(ww2*ww2))
CCC = 1/(ww1*ww2*ww2)

print('A={}\nB={}\nC={}'.format(AAA,BBB,CCC))

w1= (1/beta ).evalf(subs={A:AAA,B:BBB,C:CCC},n=50)
w2= (1/gamma).evalf(subs={A:AAA,B:BBB,C:CCC},n=50)
q = (1/kappa).evalf(subs={A:AAA,B:BBB,C:CCC},n=50)

print('\nagain:')
print('f1={}\nf2={}\nq={}'.format(w1/(2*pi),w2/(2*pi),q))


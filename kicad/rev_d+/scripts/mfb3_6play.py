from sympy import *
#import math

prec=30

r1,r2,r3,r4,c1,c2,c3 = symbols('r1,r2,r3,r4,c1,c2,c3')

A,B,C = symbols('A,B,C')

print('')

DD = sympify('(A-c3*r3)*(r1+r2)')
print('D={}'.format(DD))
EEE = sympify('r1*r2')
print('E={}'.format(EEE))
FF = sympify('c3*(r1+r2+r3)')
print('F={}'.format(FF))
GG = sympify('B*(r1+r2)/c3')
print('G={}'.format(GG))
HH = sympify('r1*r2*r3')
print('H={}'.format(HH))
KK = sympify('r1*(r2+r3)')
print('K={}'.format(KK))
LL = sympify('r3*(r1+r2)')
print('L={}'.format(LL))
MM = sympify('C*(r1+r2)/(c3*r1*r2*r3)')
print('M={}'.format(MM))

D,EE,F,G,H,K,L,M = symbols('D,EE,F,G,H,K,L,M')
r4 = sympify('-(-3*(D**2*K - 2*D*F*H + EE*F*G)/(F**2*K) + (-2*D*K + F*H)**2/(F**2*K**2))/(3*(sqrt(-4*(-3*(D**2*K - 2*D*F*H + EE*F*G)/(F**2*K) + (-2*D*K + F*H)**2/(F**2*K**2))**3 + (27*(D**2*H - D*EE*G + EE**2*L*M)/(F**2*K) - 9*(-2*D*K + F*H)*(D**2*K - 2*D*F*H + EE*F*G)/(F**3*K**2) + 2*(-2*D*K + F*H)**3/(F**3*K**3))**2)/2 + 27*(D**2*H - D*EE*G + EE**2*L*M)/(2*F**2*K) - 9*(-2*D*K + F*H)*(D**2*K - 2*D*F*H + EE*F*G)/(2*F**3*K**2) + (-2*D*K + F*H)**3/(F**3*K**3))**(1/3)) - (sqrt(-4*(-3*(D**2*K - 2*D*F*H + EE*F*G)/(F**2*K) + (-2*D*K + F*H)**2/(F**2*K**2))**3 + (27*(D**2*H - D*EE*G + EE**2*L*M)/(F**2*K) - 9*(-2*D*K + F*H)*(D**2*K - 2*D*F*H + EE*F*G)/(F**3*K**2) + 2*(-2*D*K + F*H)**3/(F**3*K**3))**2)/2 + 27*(D**2*H - D*EE*G + EE**2*L*M)/(2*F**2*K) - 9*(-2*D*K + F*H)*(D**2*K - 2*D*F*H + EE*F*G)/(2*F**3*K**2) + (-2*D*K + F*H)**3/(F**3*K**3))**(1/3)/3 - (-2*D*K + F*H)/(3*F*K)')
c1 = sympify('D/EE - F*(-(-3*(D**2*K - 2*D*F*H + EE*F*G)/(F**2*K) + (-2*D*K + F*H)**2/(F**2*K**2))/(3*(sqrt(-4*(-3*(D**2*K - 2*D*F*H + EE*F*G)/(F**2*K) + (-2*D*K + F*H)**2/(F**2*K**2))**3 + (27*(D**2*H - D*EE*G + EE**2*L*M)/(F**2*K) - 9*(-2*D*K + F*H)*(D**2*K - 2*D*F*H + EE*F*G)/(F**3*K**2) + 2*(-2*D*K + F*H)**3/(F**3*K**3))**2)/2 + 27*(D**2*H - D*EE*G + EE**2*L*M)/(2*F**2*K) - 9*(-2*D*K + F*H)*(D**2*K - 2*D*F*H + EE*F*G)/(2*F**3*K**2) + (-2*D*K + F*H)**3/(F**3*K**3))**(1/3)) - (sqrt(-4*(-3*(D**2*K - 2*D*F*H + EE*F*G)/(F**2*K) + (-2*D*K + F*H)**2/(F**2*K**2))**3 + (27*(D**2*H - D*EE*G + EE**2*L*M)/(F**2*K) - 9*(-2*D*K + F*H)*(D**2*K - 2*D*F*H + EE*F*G)/(F**3*K**2) + 2*(-2*D*K + F*H)**3/(F**3*K**3))**2)/2 + 27*(D**2*H - D*EE*G + EE**2*L*M)/(2*F**2*K) - 9*(-2*D*K + F*H)*(D**2*K - 2*D*F*H + EE*F*G)/(2*F**3*K**2) + (-2*D*K + F*H)**3/(F**3*K**3))**(1/3)/3 - (-2*D*K + F*H)/(3*F*K))/EE')
c2 = sympify('EE*M/(D*(-(-3*(D**2*K - 2*D*F*H + EE*F*G)/(F**2*K) + (-2*D*K + F*H)**2/(F**2*K**2))/(3*(sqrt(-4*(-3*(D**2*K - 2*D*F*H + EE*F*G)/(F**2*K) + (-2*D*K + F*H)**2/(F**2*K**2))**3 + (27*(D**2*H - D*EE*G + EE**2*L*M)/(F**2*K) - 9*(-2*D*K + F*H)*(D**2*K - 2*D*F*H + EE*F*G)/(F**3*K**2) + 2*(-2*D*K + F*H)**3/(F**3*K**3))**2)/2 + 27*(D**2*H - D*EE*G + EE**2*L*M)/(2*F**2*K) - 9*(-2*D*K + F*H)*(D**2*K - 2*D*F*H + EE*F*G)/(2*F**3*K**2) + (-2*D*K + F*H)**3/(F**3*K**3))**(1/3)) - (sqrt(-4*(-3*(D**2*K - 2*D*F*H + EE*F*G)/(F**2*K) + (-2*D*K + F*H)**2/(F**2*K**2))**3 + (27*(D**2*H - D*EE*G + EE**2*L*M)/(F**2*K) - 9*(-2*D*K + F*H)*(D**2*K - 2*D*F*H + EE*F*G)/(F**3*K**2) + 2*(-2*D*K + F*H)**3/(F**3*K**3))**2)/2 + 27*(D**2*H - D*EE*G + EE**2*L*M)/(2*F**2*K) - 9*(-2*D*K + F*H)*(D**2*K - 2*D*F*H + EE*F*G)/(2*F**3*K**2) + (-2*D*K + F*H)**3/(F**3*K**3))**(1/3)/3 - (-2*D*K + F*H)/(3*F*K)) - F*(-(-3*(D**2*K - 2*D*F*H + EE*F*G)/(F**2*K) + (-2*D*K + F*H)**2/(F**2*K**2))/(3*(sqrt(-4*(-3*(D**2*K - 2*D*F*H + EE*F*G)/(F**2*K) + (-2*D*K + F*H)**2/(F**2*K**2))**3 + (27*(D**2*H - D*EE*G + EE**2*L*M)/(F**2*K) - 9*(-2*D*K + F*H)*(D**2*K - 2*D*F*H + EE*F*G)/(F**3*K**2) + 2*(-2*D*K + F*H)**3/(F**3*K**3))**2)/2 + 27*(D**2*H - D*EE*G + EE**2*L*M)/(2*F**2*K) - 9*(-2*D*K + F*H)*(D**2*K - 2*D*F*H + EE*F*G)/(2*F**3*K**2) + (-2*D*K + F*H)**3/(F**3*K**3))**(1/3)) - (sqrt(-4*(-3*(D**2*K - 2*D*F*H + EE*F*G)/(F**2*K) + (-2*D*K + F*H)**2/(F**2*K**2))**3 + (27*(D**2*H - D*EE*G + EE**2*L*M)/(F**2*K) - 9*(-2*D*K + F*H)*(D**2*K - 2*D*F*H + EE*F*G)/(F**3*K**2) + 2*(-2*D*K + F*H)**3/(F**3*K**3))**2)/2 + 27*(D**2*H - D*EE*G + EE**2*L*M)/(2*F**2*K) - 9*(-2*D*K + F*H)*(D**2*K - 2*D*F*H + EE*F*G)/(2*F**3*K**2) + (-2*D*K + F*H)**3/(F**3*K**3))**(1/3)/3 - (-2*D*K + F*H)/(3*F*K))**2)')

replacer={D:DD,EE:EEE,F:FF,G:GG,H:HH,K:KK,L:LL,M:MM}

r4_expr=r4.subs(replacer)
c1_expr=c1.subs(replacer)
c2_expr=c2.subs(replacer)

#breakpoint()

print('\nr4={}'.format(r4_expr))
print('\nc1={}'.format(c1_expr))
print('\nc2={}'.format(c2_expr))

rr4,cc1,cc2=symbols('rr4 cc1 cc2')
#pprint(Eq(rr4,r4_expr))
#pprint(Eq(cc1,c1_expr))
#pprint(Eq(cc2,c2_expr))

#print('\nsimplified r4={}'.format(simplify(r4_expr)))
#print('\nsimplified c1={}'.format(simplify(c1_expr)))
#print('\nsimplified c2={}'.format(simplify(c2_expr)))


rr1 = Float(1.2e3)
rr2 = Float(47e3)
rr3 = Float(68e3)
cc3 = Float(10e-12)


def R4C1C2(r4_expr,c1_expr,c2_expr,w1,w2,q,rr1,rr2,rr3,cc3,prec):

    AA=N(1/w1 + 1/(w2*q),n=prec)
    BB=N(1/(q*w1*w2) + 1/(w2*w2),n=prec)
    CC=N(1/(w1*w2*w2),n=prec)

    rr4 = r4_expr.evalf(subs={A:AA,B:BB,C:CC,r1:rr1,r2:rr2,r3:rr3,c3:cc3},n=prec)
    cc1 = c1_expr.evalf(subs={A:AA,B:BB,C:CC,r1:rr1,r2:rr2,r3:rr3,c3:cc3},n=prec)
    cc2 = c2_expr.evalf(subs={A:AA,B:BB,C:CC,r1:rr1,r2:rr2,r3:rr3,c3:cc3},n=prec)

    return (rr4,cc1,cc2)

f1=30000
f2=f1
q=1.0

w1=N(f1*2*pi,n=prec)
w2=N(f2*2*pi,n=prec)
q=Float(q)

rrr4,ccc1,ccc2=R4C1C2(r4_expr,c1_expr,c2_expr,w1,w2,q,rr1,rr2,rr3,cc3,prec)

print('\nfrom r1={},r2={},r3={},c3={}; f1={},f2={},q={}'.format(rr1,rr2,rr3,cc3,f1,f2,q))
print('r4={},c1={},c2={}'.format(rrr4,ccc1,ccc2))


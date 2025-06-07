from sympy import *
#import math

prec=20

r1,r2,r3,r4,c1,c2,c3 = symbols('r1,r2,r3,r4,c1,c2,c3')

A,B,C = symbols('A,B,C')

#A  = sympify('(c1*r1*r2 + c3*r1*r3 + c3*r1*r4 + c3*r2*r3 + c3*r2*r4 + c3*r3*r4)/(r1 + r2)')
#print('A={}'.format(A))
#B  = sympify('c3*(c1*r1*r2*r3 + c1*r1*r2*r4 + c1*r1*r3*r4 + c2*r1*r3*r4 + c2*r2*r3*r4)/(r1 + r2)')
#print('B={}'.format(B))
#C  = sympify('c1*c2*c3*r1*r2*r3*r4/(r1 + r2)')
#print('C={}'.format(C))

print('')

#D = sympify('A*(r1+r2)-c3*r1*r2').subs('A',A)
#print('D={}'.format(D))
#E = sympify('r1*r2')
#print('E={}'.format(E))
#F = sympify('c3*(r1+r2+r3)')
#print('F={}'.format(F))
#G = sympify('B*(r1+r2)/c3').subs('B',B)
#print('G={}'.format(G))
#H = sympify('r1*r2*r3')
#print('H={}'.format(H))
#K = sympify('r1*(r2+r3)')
#print('K={}'.format(K))
#L = sympify('r3*(r1+r2)')
#print('L={}'.format(L))
#M = sympify('C*(r1+r2)/(c3*r1*r2*r3)').subs('C',C)
#print('M={}'.format(M))

DD = sympify('A*(r1+r2)-c3*r1*r2')
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

##replacer=[('D',D),('E',E),('F',F),('G',G),('H',H),('K',K),('L',L),('M',M)]
#replacer={'D':D,'E':E,'F':F,'G':G,'H':H,'K':K,'L':L,'M':M}
#r4 = sympify('-(-3*(D**2*K - 2*D*F*H + E*F*G)/(F**2*K) + (-2*D*K + F*H)**2/(F**2*K**2))/(3*(sqrt(-4*(-3*(D**2*K - 2*D*F*H + E*F*G)/(F**2*K) + (-2*D*K + F*H)**2/(F**2*K**2))**3 + (27*(D**2*H - D*E*G + E**2*L*M)/(F**2*K) - 9*(-2*D*K + F*H)*(D**2*K - 2*D*F*H + E*F*G)/(F**3*K**2) + 2*(-2*D*K + F*H)**3/(F**3*K**3))**2)/2 + 27*(D**2*H - D*E*G + E**2*L*M)/(2*F**2*K) - 9*(-2*D*K + F*H)*(D**2*K - 2*D*F*H + E*F*G)/(2*F**3*K**2) + (-2*D*K + F*H)**3/(F**3*K**3))**(1/3)) - (sqrt(-4*(-3*(D**2*K - 2*D*F*H + E*F*G)/(F**2*K) + (-2*D*K + F*H)**2/(F**2*K**2))**3 + (27*(D**2*H - D*E*G + E**2*L*M)/(F**2*K) - 9*(-2*D*K + F*H)*(D**2*K - 2*D*F*H + E*F*G)/(F**3*K**2) + 2*(-2*D*K + F*H)**3/(F**3*K**3))**2)/2 + 27*(D**2*H - D*E*G + E**2*L*M)/(2*F**2*K) - 9*(-2*D*K + F*H)*(D**2*K - 2*D*F*H + E*F*G)/(2*F**3*K**2) + (-2*D*K + F*H)**3/(F**3*K**3))**(1/3)/3 - (-2*D*K + F*H)/(3*F*K)')
#c1 = sympify('D/E - F*(-(-3*(D**2*K - 2*D*F*H + E*F*G)/(F**2*K) + (-2*D*K + F*H)**2/(F**2*K**2))/(3*(sqrt(-4*(-3*(D**2*K - 2*D*F*H + E*F*G)/(F**2*K) + (-2*D*K + F*H)**2/(F**2*K**2))**3 + (27*(D**2*H - D*E*G + E**2*L*M)/(F**2*K) - 9*(-2*D*K + F*H)*(D**2*K - 2*D*F*H + E*F*G)/(F**3*K**2) + 2*(-2*D*K + F*H)**3/(F**3*K**3))**2)/2 + 27*(D**2*H - D*E*G + E**2*L*M)/(2*F**2*K) - 9*(-2*D*K + F*H)*(D**2*K - 2*D*F*H + E*F*G)/(2*F**3*K**2) + (-2*D*K + F*H)**3/(F**3*K**3))**(1/3)) - (sqrt(-4*(-3*(D**2*K - 2*D*F*H + E*F*G)/(F**2*K) + (-2*D*K + F*H)**2/(F**2*K**2))**3 + (27*(D**2*H - D*E*G + E**2*L*M)/(F**2*K) - 9*(-2*D*K + F*H)*(D**2*K - 2*D*F*H + E*F*G)/(F**3*K**2) + 2*(-2*D*K + F*H)**3/(F**3*K**3))**2)/2 + 27*(D**2*H - D*E*G + E**2*L*M)/(2*F**2*K) - 9*(-2*D*K + F*H)*(D**2*K - 2*D*F*H + E*F*G)/(2*F**3*K**2) + (-2*D*K + F*H)**3/(F**3*K**3))**(1/3)/3 - (-2*D*K + F*H)/(3*F*K))/E')
#c2= sympify('E*M/(D*(-(-3*(D**2*K - 2*D*F*H + E*F*G)/(F**2*K) + (-2*D*K + F*H)**2/(F**2*K**2))/(3*(sqrt(-4*(-3*(D**2*K - 2*D*F*H + E*F*G)/(F**2*K) + (-2*D*K + F*H)**2/(F**2*K**2))**3 + (27*(D**2*H - D*E*G + E**2*L*M)/(F**2*K) - 9*(-2*D*K + F*H)*(D**2*K - 2*D*F*H + E*F*G)/(F**3*K**2) + 2*(-2*D*K + F*H)**3/(F**3*K**3))**2)/2 + 27*(D**2*H - D*E*G + E**2*L*M)/(2*F**2*K) - 9*(-2*D*K + F*H)*(D**2*K - 2*D*F*H + E*F*G)/(2*F**3*K**2) + (-2*D*K + F*H)**3/(F**3*K**3))**(1/3)) - (sqrt(-4*(-3*(D**2*K - 2*D*F*H + E*F*G)/(F**2*K) + (-2*D*K + F*H)**2/(F**2*K**2))**3 + (27*(D**2*H - D*E*G + E**2*L*M)/(F**2*K) - 9*(-2*D*K + F*H)*(D**2*K - 2*D*F*H + E*F*G)/(F**3*K**2) + 2*(-2*D*K + F*H)**3/(F**3*K**3))**2)/2 + 27*(D**2*H - D*E*G + E**2*L*M)/(2*F**2*K) - 9*(-2*D*K + F*H)*(D**2*K - 2*D*F*H + E*F*G)/(2*F**3*K**2) + (-2*D*K + F*H)**3/(F**3*K**3))**(1/3)/3 - (-2*D*K + F*H)/(3*F*K)) - F*(-(-3*(D**2*K - 2*D*F*H + E*F*G)/(F**2*K) + (-2*D*K + F*H)**2/(F**2*K**2))/(3*(sqrt(-4*(-3*(D**2*K - 2*D*F*H + E*F*G)/(F**2*K) + (-2*D*K + F*H)**2/(F**2*K**2))**3 + (27*(D**2*H - D*E*G + E**2*L*M)/(F**2*K) - 9*(-2*D*K + F*H)*(D**2*K - 2*D*F*H + E*F*G)/(F**3*K**2) + 2*(-2*D*K + F*H)**3/(F**3*K**3))**2)/2 + 27*(D**2*H - D*E*G + E**2*L*M)/(2*F**2*K) - 9*(-2*D*K + F*H)*(D**2*K - 2*D*F*H + E*F*G)/(2*F**3*K**2) + (-2*D*K + F*H)**3/(F**3*K**3))**(1/3)) - (sqrt(-4*(-3*(D**2*K - 2*D*F*H + E*F*G)/(F**2*K) + (-2*D*K + F*H)**2/(F**2*K**2))**3 + (27*(D**2*H - D*E*G + E**2*L*M)/(F**2*K) - 9*(-2*D*K + F*H)*(D**2*K - 2*D*F*H + E*F*G)/(F**3*K**2) + 2*(-2*D*K + F*H)**3/(F**3*K**3))**2)/2 + 27*(D**2*H - D*E*G + E**2*L*M)/(2*F**2*K) - 9*(-2*D*K + F*H)*(D**2*K - 2*D*F*H + E*F*G)/(2*F**3*K**2) + (-2*D*K + F*H)**3/(F**3*K**3))**(1/3)/3 - (-2*D*K + F*H)/(3*F*K))**2)')

#DD,EE,FF,GG,HH,KK,LL,MM = symbols('DD,EE,FF,GG,HH,KK,LL,MM')
D,EE,F,G,H,K,L,M = symbols('D,EE,F,G,H,K,L,M')
##r4 = sympify('-(-3*(DD**2*KK - 2*DD*FF*HH + EE*FF*GG)/(FF**2*KK) + (-2*DD*KK + FF*HH)**2/(FF**2*KK**2))/(3*(sqrt(-4*(-3*(DD**2*KK - 2*DD*FF*HH + EE*FF*GG)/(FF**2*KK) + (-2*DD*KK + FF*HH)**2/(FF**2*KK**2))**3 + (27*(DD**2*HH - DD*EE*GG + EE**2*LL*MM)/(FF**2*KK) - 9*(-2*DD*KK + FF*HH)*(DD**2*KK - 2*DD*FF*HH + EE*FF*GG)/(FF**3*KK**2) + 2*(-2*DD*KK + FF*HH)**3/(FF**3*KK**3))**2)/2 + 27*(DD**2*HH - DD*EE*GG + EE**2*LL*MM)/(2*FF**2*KK) - 9*(-2*DD*KK + FF*HH)*(DD**2*KK - 2*DD*FF*HH + EE*FF*GG)/(2*FF**3*KK**2) + (-2*DD*KK + FF*HH)**3/(FF**3*KK**3))**(1/3)) - (sqrt(-4*(-3*(DD**2*KK - 2*DD*FF*HH + EE*FF*GG)/(FF**2*KK) + (-2*DD*KK + FF*HH)**2/(FF**2*KK**2))**3 + (27*(DD**2*HH - DD*EE*GG + EE**2*LL*MM)/(FF**2*KK) - 9*(-2*DD*KK + FF*HH)*(DD**2*KK - 2*DD*FF*HH + EE*FF*GG)/(FF**3*KK**2) + 2*(-2*DD*KK + FF*HH)**3/(FF**3*KK**3))**2)/2 + 27*(DD**2*HH - DD*EE*GG + EE**2*LL*MM)/(2*FF**2*KK) - 9*(-2*DD*KK + FF*HH)*(DD**2*KK - 2*DD*FF*HH + EE*FF*GG)/(2*FF**3*KK**2) + (-2*DD*KK + FF*HH)**3/(FF**3*KK**3))**(1/3)/3 - (-2*DD*KK + FF*HH)/(3*FF*KK)')
#r4 = sympify('-(-3*(D**2*K - 2*D*F*H + E*F*G)/(F**2*K) + (-2*D*K + F*H)**2/(F**2*K**2))/(3*(sqrt(-4*(-3*(D**2*K - 2*D*F*H + E*F*G)/(F**2*K) + (-2*D*K + F*H)**2/(F**2*K**2))**3 + (27*(D**2*H - D*E*G + E**2*L*M)/(F**2*K) - 9*(-2*D*K + F*H)*(D**2*K - 2*D*F*H + E*F*G)/(F**3*K**2) + 2*(-2*D*K + F*H)**3/(F**3*K**3))**2)/2 + 27*(D**2*H - D*E*G + E**2*L*M)/(2*F**2*K) - 9*(-2*D*K + F*H)*(D**2*K - 2*D*F*H + E*F*G)/(2*F**3*K**2) + (-2*D*K + F*H)**3/(F**3*K**3))**(1/3)) - (sqrt(-4*(-3*(D**2*K - 2*D*F*H + E*F*G)/(F**2*K) + (-2*D*K + F*H)**2/(F**2*K**2))**3 + (27*(D**2*H - D*E*G + E**2*L*M)/(F**2*K) - 9*(-2*D*K + F*H)*(D**2*K - 2*D*F*H + E*F*G)/(F**3*K**2) + 2*(-2*D*K + F*H)**3/(F**3*K**3))**2)/2 + 27*(D**2*H - D*E*G + E**2*L*M)/(2*F**2*K) - 9*(-2*D*K + F*H)*(D**2*K - 2*D*F*H + E*F*G)/(2*F**3*K**2) + (-2*D*K + F*H)**3/(F**3*K**3))**(1/3)/3 - (-2*D*K + F*H)/(3*F*K)')
r4 = sympify('-(-3*(D**2*K - 2*D*F*H + EE*F*G)/(F**2*K) + (-2*D*K + F*H)**2/(F**2*K**2))/(3*(sqrt(-4*(-3*(D**2*K - 2*D*F*H + EE*F*G)/(F**2*K) + (-2*D*K + F*H)**2/(F**2*K**2))**3 + (27*(D**2*H - D*EE*G + EE**2*L*M)/(F**2*K) - 9*(-2*D*K + F*H)*(D**2*K - 2*D*F*H + EE*F*G)/(F**3*K**2) + 2*(-2*D*K + F*H)**3/(F**3*K**3))**2)/2 + 27*(D**2*H - D*EE*G + EE**2*L*M)/(2*F**2*K) - 9*(-2*D*K + F*H)*(D**2*K - 2*D*F*H + EE*F*G)/(2*F**3*K**2) + (-2*D*K + F*H)**3/(F**3*K**3))**(1/3)) - (sqrt(-4*(-3*(D**2*K - 2*D*F*H + EE*F*G)/(F**2*K) + (-2*D*K + F*H)**2/(F**2*K**2))**3 + (27*(D**2*H - D*EE*G + EE**2*L*M)/(F**2*K) - 9*(-2*D*K + F*H)*(D**2*K - 2*D*F*H + EE*F*G)/(F**3*K**2) + 2*(-2*D*K + F*H)**3/(F**3*K**3))**2)/2 + 27*(D**2*H - D*EE*G + EE**2*L*M)/(2*F**2*K) - 9*(-2*D*K + F*H)*(D**2*K - 2*D*F*H + EE*F*G)/(2*F**3*K**2) + (-2*D*K + F*H)**3/(F**3*K**3))**(1/3)/3 - (-2*D*K + F*H)/(3*F*K)')
c1 = sympify('D/EE - F*(-(-3*(D**2*K - 2*D*F*H + EE*F*G)/(F**2*K) + (-2*D*K + F*H)**2/(F**2*K**2))/(3*(sqrt(-4*(-3*(D**2*K - 2*D*F*H + EE*F*G)/(F**2*K) + (-2*D*K + F*H)**2/(F**2*K**2))**3 + (27*(D**2*H - D*EE*G + EE**2*L*M)/(F**2*K) - 9*(-2*D*K + F*H)*(D**2*K - 2*D*F*H + EE*F*G)/(F**3*K**2) + 2*(-2*D*K + F*H)**3/(F**3*K**3))**2)/2 + 27*(D**2*H - D*EE*G + EE**2*L*M)/(2*F**2*K) - 9*(-2*D*K + F*H)*(D**2*K - 2*D*F*H + EE*F*G)/(2*F**3*K**2) + (-2*D*K + F*H)**3/(F**3*K**3))**(1/3)) - (sqrt(-4*(-3*(D**2*K - 2*D*F*H + EE*F*G)/(F**2*K) + (-2*D*K + F*H)**2/(F**2*K**2))**3 + (27*(D**2*H - D*EE*G + EE**2*L*M)/(F**2*K) - 9*(-2*D*K + F*H)*(D**2*K - 2*D*F*H + EE*F*G)/(F**3*K**2) + 2*(-2*D*K + F*H)**3/(F**3*K**3))**2)/2 + 27*(D**2*H - D*EE*G + EE**2*L*M)/(2*F**2*K) - 9*(-2*D*K + F*H)*(D**2*K - 2*D*F*H + EE*F*G)/(2*F**3*K**2) + (-2*D*K + F*H)**3/(F**3*K**3))**(1/3)/3 - (-2*D*K + F*H)/(3*F*K))/EE')
c2= sympify('EE*M/(D*(-(-3*(D**2*K - 2*D*F*H + EE*F*G)/(F**2*K) + (-2*D*K + F*H)**2/(F**2*K**2))/(3*(sqrt(-4*(-3*(D**2*K - 2*D*F*H + EE*F*G)/(F**2*K) + (-2*D*K + F*H)**2/(F**2*K**2))**3 + (27*(D**2*H - D*EE*G + EE**2*L*M)/(F**2*K) - 9*(-2*D*K + F*H)*(D**2*K - 2*D*F*H + EE*F*G)/(F**3*K**2) + 2*(-2*D*K + F*H)**3/(F**3*K**3))**2)/2 + 27*(D**2*H - D*EE*G + EE**2*L*M)/(2*F**2*K) - 9*(-2*D*K + F*H)*(D**2*K - 2*D*F*H + EE*F*G)/(2*F**3*K**2) + (-2*D*K + F*H)**3/(F**3*K**3))**(1/3)) - (sqrt(-4*(-3*(D**2*K - 2*D*F*H + EE*F*G)/(F**2*K) + (-2*D*K + F*H)**2/(F**2*K**2))**3 + (27*(D**2*H - D*EE*G + EE**2*L*M)/(F**2*K) - 9*(-2*D*K + F*H)*(D**2*K - 2*D*F*H + EE*F*G)/(F**3*K**2) + 2*(-2*D*K + F*H)**3/(F**3*K**3))**2)/2 + 27*(D**2*H - D*EE*G + EE**2*L*M)/(2*F**2*K) - 9*(-2*D*K + F*H)*(D**2*K - 2*D*F*H + EE*F*G)/(2*F**3*K**2) + (-2*D*K + F*H)**3/(F**3*K**3))**(1/3)/3 - (-2*D*K + F*H)/(3*F*K)) - F*(-(-3*(D**2*K - 2*D*F*H + EE*F*G)/(F**2*K) + (-2*D*K + F*H)**2/(F**2*K**2))/(3*(sqrt(-4*(-3*(D**2*K - 2*D*F*H + EE*F*G)/(F**2*K) + (-2*D*K + F*H)**2/(F**2*K**2))**3 + (27*(D**2*H - D*EE*G + EE**2*L*M)/(F**2*K) - 9*(-2*D*K + F*H)*(D**2*K - 2*D*F*H + EE*F*G)/(F**3*K**2) + 2*(-2*D*K + F*H)**3/(F**3*K**3))**2)/2 + 27*(D**2*H - D*EE*G + EE**2*L*M)/(2*F**2*K) - 9*(-2*D*K + F*H)*(D**2*K - 2*D*F*H + EE*F*G)/(2*F**3*K**2) + (-2*D*K + F*H)**3/(F**3*K**3))**(1/3)) - (sqrt(-4*(-3*(D**2*K - 2*D*F*H + EE*F*G)/(F**2*K) + (-2*D*K + F*H)**2/(F**2*K**2))**3 + (27*(D**2*H - D*EE*G + EE**2*L*M)/(F**2*K) - 9*(-2*D*K + F*H)*(D**2*K - 2*D*F*H + EE*F*G)/(F**3*K**2) + 2*(-2*D*K + F*H)**3/(F**3*K**3))**2)/2 + 27*(D**2*H - D*EE*G + EE**2*L*M)/(2*F**2*K) - 9*(-2*D*K + F*H)*(D**2*K - 2*D*F*H + EE*F*G)/(2*F**3*K**2) + (-2*D*K + F*H)**3/(F**3*K**3))**(1/3)/3 - (-2*D*K + F*H)/(3*F*K))**2)')

replacer={D:DD,EE:EEE,F:FF,G:GG,H:HH,K:KK,L:LL,M:MM}

r4_expr=r4.subs(replacer)
c1_expr=c1.subs(replacer)
c2_expr=c2.subs(replacer)

print('\nr4={}'.format(r4_expr))
print('\nc1={}'.format(c1_expr))
print('\nc2={}'.format(c2_expr))


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


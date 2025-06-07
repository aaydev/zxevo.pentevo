from sympy import *
import math

r1,r2,r3,r4,c1,c2,c3 = symbols('r1,r2,r3,r4,c1,c2,c3')

A,B,C,D,E,F,G,H,K,L,M = symbols('A,B,C,D,E,F,G,H,K,L,M')


A  = sympify('(c1*r1*r2 + c3*r1*r3 + c3*r1*r4 + c3*r2*r3 + c3*r2*r4 + c3*r3*r4)/(r1 + r2)')
B  = sympify('c3*(c1*r1*r2*r3 + c1*r1*r2*r4 + c1*r1*r3*r4 + c2*r1*r3*r4 + c2*r2*r3*r4)/(r1 + r2)')
C  = sympify('c1*c2*c3*r1*r2*r3*r4/(r1 + r2)')


D = sympify('A*(r1+r2)-c3*r1*r2')
E = sympify('r1*r2')
F = sympify('c3*(r1+r2+r3)')
G = sympify('B*(r1+r2)/c3')
H = sympify('r1*r2*r3')
K = sympify('r1*(r2+r3)')
L = sympify('r3*(r1+r2)')
M = sympify('C*(r1+r2)/(c3*r1*r2*r3)')

r4 = sympify('-(-3*(D**2*K - 2*D*F*H + E*F*G)/(F**2*K) + (-2*D*K + F*H)**2/(F**2*K**2))/(3*(sqrt(-4*(-3*(D**2*K - 2*D*F*H + E*F*G)/(F**2*K) + (-2*D*K + F*H)**2/(F**2*K**2))**3 + (27*(D**2*H - D*E*G + E**2*L*M)/(F**2*K) - 9*(-2*D*K + F*H)*(D**2*K - 2*D*F*H + E*F*G)/(F**3*K**2) + 2*(-2*D*K + F*H)**3/(F**3*K**3))**2)/2 + 27*(D**2*H - D*E*G + E**2*L*M)/(2*F**2*K) - 9*(-2*D*K + F*H)*(D**2*K - 2*D*F*H + E*F*G)/(2*F**3*K**2) + (-2*D*K + F*H)**3/(F**3*K**3))**(1/3)) - (sqrt(-4*(-3*(D**2*K - 2*D*F*H + E*F*G)/(F**2*K) + (-2*D*K + F*H)**2/(F**2*K**2))**3 + (27*(D**2*H - D*E*G + E**2*L*M)/(F**2*K) - 9*(-2*D*K + F*H)*(D**2*K - 2*D*F*H + E*F*G)/(F**3*K**2) + 2*(-2*D*K + F*H)**3/(F**3*K**3))**2)/2 + 27*(D**2*H - D*E*G + E**2*L*M)/(2*F**2*K) - 9*(-2*D*K + F*H)*(D**2*K - 2*D*F*H + E*F*G)/(2*F**3*K**2) + (-2*D*K + F*H)**3/(F**3*K**3))**(1/3)/3 - (-2*D*K + F*H)/(3*F*K)')

c1 = sympify('D/E - F*(-(-3*(D**2*K - 2*D*F*H + E*F*G)/(F**2*K) + (-2*D*K + F*H)**2/(F**2*K**2))/(3*(sqrt(-4*(-3*(D**2*K - 2*D*F*H + E*F*G)/(F**2*K) + (-2*D*K + F*H)**2/(F**2*K**2))**3 + (27*(D**2*H - D*E*G + E**2*L*M)/(F**2*K) - 9*(-2*D*K + F*H)*(D**2*K - 2*D*F*H + E*F*G)/(F**3*K**2) + 2*(-2*D*K + F*H)**3/(F**3*K**3))**2)/2 + 27*(D**2*H - D*E*G + E**2*L*M)/(2*F**2*K) - 9*(-2*D*K + F*H)*(D**2*K - 2*D*F*H + E*F*G)/(2*F**3*K**2) + (-2*D*K + F*H)**3/(F**3*K**3))**(1/3)) - (sqrt(-4*(-3*(D**2*K - 2*D*F*H + E*F*G)/(F**2*K) + (-2*D*K + F*H)**2/(F**2*K**2))**3 + (27*(D**2*H - D*E*G + E**2*L*M)/(F**2*K) - 9*(-2*D*K + F*H)*(D**2*K - 2*D*F*H + E*F*G)/(F**3*K**2) + 2*(-2*D*K + F*H)**3/(F**3*K**3))**2)/2 + 27*(D**2*H - D*E*G + E**2*L*M)/(2*F**2*K) - 9*(-2*D*K + F*H)*(D**2*K - 2*D*F*H + E*F*G)/(2*F**3*K**2) + (-2*D*K + F*H)**3/(F**3*K**3))**(1/3)/3 - (-2*D*K + F*H)/(3*F*K))/E')

c2= sympify('E*M/(D*(-(-3*(D**2*K - 2*D*F*H + E*F*G)/(F**2*K) + (-2*D*K + F*H)**2/(F**2*K**2))/(3*(sqrt(-4*(-3*(D**2*K - 2*D*F*H + E*F*G)/(F**2*K) + (-2*D*K + F*H)**2/(F**2*K**2))**3 + (27*(D**2*H - D*E*G + E**2*L*M)/(F**2*K) - 9*(-2*D*K + F*H)*(D**2*K - 2*D*F*H + E*F*G)/(F**3*K**2) + 2*(-2*D*K + F*H)**3/(F**3*K**3))**2)/2 + 27*(D**2*H - D*E*G + E**2*L*M)/(2*F**2*K) - 9*(-2*D*K + F*H)*(D**2*K - 2*D*F*H + E*F*G)/(2*F**3*K**2) + (-2*D*K + F*H)**3/(F**3*K**3))**(1/3)) - (sqrt(-4*(-3*(D**2*K - 2*D*F*H + E*F*G)/(F**2*K) + (-2*D*K + F*H)**2/(F**2*K**2))**3 + (27*(D**2*H - D*E*G + E**2*L*M)/(F**2*K) - 9*(-2*D*K + F*H)*(D**2*K - 2*D*F*H + E*F*G)/(F**3*K**2) + 2*(-2*D*K + F*H)**3/(F**3*K**3))**2)/2 + 27*(D**2*H - D*E*G + E**2*L*M)/(2*F**2*K) - 9*(-2*D*K + F*H)*(D**2*K - 2*D*F*H + E*F*G)/(2*F**3*K**2) + (-2*D*K + F*H)**3/(F**3*K**3))**(1/3)/3 - (-2*D*K + F*H)/(3*F*K)) - F*(-(-3*(D**2*K - 2*D*F*H + E*F*G)/(F**2*K) + (-2*D*K + F*H)**2/(F**2*K**2))/(3*(sqrt(-4*(-3*(D**2*K - 2*D*F*H + E*F*G)/(F**2*K) + (-2*D*K + F*H)**2/(F**2*K**2))**3 + (27*(D**2*H - D*E*G + E**2*L*M)/(F**2*K) - 9*(-2*D*K + F*H)*(D**2*K - 2*D*F*H + E*F*G)/(F**3*K**2) + 2*(-2*D*K + F*H)**3/(F**3*K**3))**2)/2 + 27*(D**2*H - D*E*G + E**2*L*M)/(2*F**2*K) - 9*(-2*D*K + F*H)*(D**2*K - 2*D*F*H + E*F*G)/(2*F**3*K**2) + (-2*D*K + F*H)**3/(F**3*K**3))**(1/3)) - (sqrt(-4*(-3*(D**2*K - 2*D*F*H + E*F*G)/(F**2*K) + (-2*D*K + F*H)**2/(F**2*K**2))**3 + (27*(D**2*H - D*E*G + E**2*L*M)/(F**2*K) - 9*(-2*D*K + F*H)*(D**2*K - 2*D*F*H + E*F*G)/(F**3*K**2) + 2*(-2*D*K + F*H)**3/(F**3*K**3))**2)/2 + 27*(D**2*H - D*E*G + E**2*L*M)/(2*F**2*K) - 9*(-2*D*K + F*H)*(D**2*K - 2*D*F*H + E*F*G)/(2*F**3*K**2) + (-2*D*K + F*H)**3/(F**3*K**3))**(1/3)/3 - (-2*D*K + F*H)/(3*F*K))**2)')



rr1 = 1.2e3
rr2 = 47e3
rr3 = 68e3
cc3 = 10e-12

w1=30000*2*math.pi
w2=w1
q=1.0

AA=1/w1 + 1/(w2*q)
BB=1/(q*w1*w2) + 1/(w2*w2)
CC=1/(w1*w2*w2)

DD = D.evalf(subs={r1:rr1,r2:rr2,r3:rr3,c3:cc3,'A':AA,'B':BB,'C':CC})
EE = E.evalf(subs={r1:rr1,r2:rr2,r3:rr3,c3:cc3,'A':AA,'B':BB,'C':CC})
FF = F.evalf(subs={r1:rr1,r2:rr2,r3:rr3,c3:cc3,'A':AA,'B':BB,'C':CC})
GG = G.evalf(subs={r1:rr1,r2:rr2,r3:rr3,c3:cc3,'A':AA,'B':BB,'C':CC})
HH = H.evalf(subs={r1:rr1,r2:rr2,r3:rr3,c3:cc3,'A':AA,'B':BB,'C':CC})
KK = K.evalf(subs={r1:rr1,r2:rr2,r3:rr3,c3:cc3,'A':AA,'B':BB,'C':CC})
LL = L.evalf(subs={r1:rr1,r2:rr2,r3:rr3,c3:cc3,'A':AA,'B':BB,'C':CC})
MM = M.evalf(subs={r1:rr1,r2:rr2,r3:rr3,c3:cc3,'A':AA,'B':BB,'C':CC})

rr4 = r4.evalf(subs={'D':DD,'E':EE,'F':FF,'G':GG,'H':HH,'K':KK,'L':LL,'M':MM})
cc1 = c1.evalf(subs={'D':DD,'E':EE,'F':FF,'G':GG,'H':HH,'K':KK,'L':LL,'M':MM})
cc2 = c2.evalf(subs={'D':DD,'E':EE,'F':FF,'G':GG,'H':HH,'K':KK,'L':LL,'M':MM})


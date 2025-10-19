from sympy import *

A,B,C = symbols('A B C')

beta,gamma,kappa = symbols('beta gamma kappa')

# manual!
ex6 = gamma**6 - B*gamma**4 + A*C*gamma**2 - C*C

s = solve(ex6,gamma,dict=True)

# s[1] is OK

gamma_sol = simplify(s[1][gamma])
print('\ngamma={}'.format(gamma_sol))
beta_sol = simplify( C*gamma_sol**(-2) )
print('\nbeta={}'.format(beta_sol))
kappa_sol = simplify( (A-beta_sol)/gamma_sol )
print('\nkappa={}'.format(kappa_sol))


w1_sol = simplify(1/beta_sol)
print('\nw1={}'.format(w1_sol))
w2_sol = simplify(1/gamma_sol)
print('\nw2={}'.format(w2_sol))
q_sol = simplify(1/kappa_sol)
print('\nq={}'.format(q_sol))

# check initial equations

print('\n1/w1 + 1/(q*w2): {}'.format(simplify(1/w1_sol + 1/(q_sol*w2_sol))))
print('\n1/w2**2 + 1/(q*w1*w2): {}'.format(simplify(1/w2_sol**2 + 1/(q_sol*w1_sol*w2_sol))))
print('\n1/(w1*w2**2): {}'.format(simplify(1/(w1_sol*w2_sol**2))))


prec=50

def ABC(f1_val,f2_val,q_val,prec=50):

    w1_val=Float(f1_val)*Float(2)*pi
    w2_val=Float(f2_val)*Float(2)*pi
    q_val =Float(q_val)

    q,w1,w2=symbols('q,w1,w2')
    A = 1/w1+1/(q*w2)
    B = 1/w2**2+1/(q*w1*w2)
    C = 1/(w1*w2**2)

    A_val = A.evalf(subs={w1:w1_val,w2:w2_val,q:q_val},n=prec)
    B_val = B.evalf(subs={w1:w1_val,w2:w2_val,q:q_val},n=prec)
    C_val = C.evalf(subs={w1:w1_val,w2:w2_val,q:q_val},n=prec)

    return (A_val,B_val,C_val)

f1_val=30000
f2_val=30000
q_val=1
(AA,BB,CC) = ABC(f1_val,f2_val,q_val,prec)
print('\nfor f1={}, f2={}, q={}:\nA={}, B={}, C={}'.format(f1_val,f2_val,q_val,AA,BB,CC))

def F1F2Q(A_val,B_val,C_val,w1_sol,w2_sol,q_sol,prec=50):
    w1_val = w1_sol.evalf(subs={A:A_val,B:B_val,C:C_val},n=prec)
    w2_val = w2_sol.evalf(subs={A:A_val,B:B_val,C:C_val},n=prec)
    q_val  =  q_sol.evalf(subs={A:A_val,B:B_val,C:C_val},n=prec)

    f1_val = N(w1_val/Float(2)/pi,n=prec)
    f2_val = N(w2_val/Float(2)/pi,n=prec)

    return (f1_val,f2_val,q_val)

(ff1,ff2,qq) = F1F2Q(AA,BB,CC,w1_sol,w2_sol,q_sol,prec)
print('\nfor previous A,B,C: f1={}, f2={}, q={}'.format(ff1,ff2,qq))



r1 = Float(1.2e3)
r2 = Float(47e3)
r3 = Float(68e3)
r4 = Float(180e3)
c1 = Float(0.0047e-6)
c2 = Float(220e-12)
c3 = Float(10e-12)

AA = N((c1*r1*r2 + c3*r1*r3 + c3*r1*r4 + c3*r2*r3 + c3*r2*r4 + c3*r3*r4)/(r1 + r2),prec)
BB = N(c3*(c1*r1*r2*r3 + c1*r1*r2*r4 + c1*r1*r3*r4 + c2*r1*r3*r4 + c2*r2*r3*r4)/(r1 + r2),prec)
CC = N(c1*c2*c3*r1*r2*r3*r4/(r1 + r2),prec)

print('\nfor r1={}, r2={}, r3={}, r4={}, c1={}, c2={}, c3={}:\nA={}, B={}, C={}'.format(r1,r2,r3,r4,c1,c2,c3,AA,BB,CC))

(ff1,ff2,qq) = F1F2Q(AA,BB,CC,w1_sol,w2_sol,q_sol,prec)
print('\nfor previous A,B,C: f1={}, f2={}, q={}'.format(ff1,ff2,qq))

(AA,BB,CC) = ABC(ff1,ff2,qq,prec)
print('\nfor f1,f2,q:\nA={}, B={}, C={}'.format(AA,BB,CC))





r1 = Float(1.2e3)
r2 = Float(47e3)
r3 = Float(68e3)
r4 = Float(188992.783958288349467650567878)
c1 = Float(4.59277072415648874927951207664E-9)
c2 = Float(2.16190031268923023889022787715E-10)
c3 = Float(10e-12)

AA = N((c1*r1*r2 + c3*r1*r3 + c3*r1*r4 + c3*r2*r3 + c3*r2*r4 + c3*r3*r4)/(r1 + r2),prec)
BB = N(c3*(c1*r1*r2*r3 + c1*r1*r2*r4 + c1*r1*r3*r4 + c2*r1*r3*r4 + c2*r2*r3*r4)/(r1 + r2),prec)
CC = N(c1*c2*c3*r1*r2*r3*r4/(r1 + r2),prec)

print('\nfor r1={}, r2={}, r3={}, r4={}, c1={}, c2={}, c3={}:\nA={}, B={}, C={}'.format(r1,r2,r3,r4,c1,c2,c3,AA,BB,CC))

(ff1,ff2,qq) = F1F2Q(AA,BB,CC,w1_sol,w2_sol,q_sol,prec)
print('\nfor previous A,B,C: f1={}, f2={}, q={}'.format(ff1,ff2,qq))

(AA,BB,CC) = ABC(ff1,ff2,qq,prec)
print('\nfor f1,f2,q:\nA={}, B={}, C={}'.format(AA,BB,CC))


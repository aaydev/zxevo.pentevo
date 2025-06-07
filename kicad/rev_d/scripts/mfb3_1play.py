from sympy import *

r1,r2,r3,r4,c1,c2,c3 = symbols('r1 r2 r3 r4 c1 c2 c3', real=True, positive=True)

ui,uo,u1,u2,u3 = symbols('ui uo u1 u2 u3')

# currents through appropriate elements.
ir1,ir2,ir3,ir4 = symbols('ir1 ir2 ir3 ir4')
ic1,ic2,ic3 = symbols('ic1 ic2 ic3')

# angular frequency (omega)
w = symbols('w', real=True, positive=True)


# make set of equations to calculate H
eqs=[]
#
# u1 node, currents and voltages
eqs += [ Eq( ir1, (ui-u1)/r1 ) ]
eqs += [ Eq( ir2, (u2-u1)/r2 ) ]
eqs += [ Eq( u1, (ir1+ir2)*(1/(I*w*c1)) ) ]
#
# u2 node
eqs += [ Eq( ir3, (uo-u2)/r3 ) ]
eqs += [ Eq( ir4, (u3-u2)/r4 ) ]
eqs += [ Eq( u2, (ir3+ir4-ir2)*(1/(I*w*c2)) ) ]
#
# u3 node
eqs += [ Eq( ir4, (uo-u3)/(1/(I*w*c3)) ) ]
eqs += [ Eq( u3, 0 ) ]

# solve the set
u_solve = solve( eqs, [u1,u2,u3,ir1,ir2,ir3,ir4,uo], dict=True, domain=S.Complexes )

#print(u_solve)

if len(u_solve)!=1:
    sys.stderr.write("Many or no solutions: {} !\n".format(u_solve))
    exit(1)

u_expr = u_solve[0]

h_expr = u_expr[uo]/ui

print(h_expr)

(n,d)=fraction(h_expr)
d2=(d/n)*(-r3/(r1+r2))
co=simplify(d2).collect(w)

print(co)


w1,w2,q=symbols('w1,w2,q')

#w1 - 1st order, w2,q - 2nd order
hd=(1+I*w/w1)*(1+I*w/(w2*q)-w**2/w2**2)

hdo = collect(expand(hd),w)

print('w**3: {}'.format(hdo.coeff(w,3)))
print('w**2: {}'.format(hdo.coeff(w,2)))
print('w**1: {}'.format(hdo.coeff(w,1)))
print('w**0: {}'.format(hdo.coeff(w,0)))

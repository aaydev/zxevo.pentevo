#!/usr/bin/env python

import sys
from sympy import *




def main():
	u,v,w = symbols('u v w')
	r,c = symbols('r c')
	n,m = symbols('n m')
	omega = symbols('omega')
	i1,i2,i3 = symbols('i1 i2 i3')
	omega_c,q,rc = symbols('omega_c q rc')

	eq1=Eq(i1,(v-w)/(m*r))
	eq2=Eq(i2,(u-w)/(1/(I*omega*n*c)))
	eq3=Eq(i1+i2,i3)
	eq4=Eq(w,i3*(r+1/(I*omega*c)))
	eq5=Eq(u,i3*(1/(I*omega*c)))

#	eq6=Eq(rc,r*c)
	eq7=Eq(omega_c,1/(r*c*sqrt(n*m)))
	eq8=Eq(q,sqrt(n*m)/(m+1))

	s = solve([eq1,eq2,eq3,eq4,eq5,eq7,eq8],[w,u,i1,i2,i3,r,c],dict=True,domain=S.Complexes)

	if len(s)!=1:
		sys.stderr.write('several solutions???\n')
		exit(1)

	sol = s[0]

	uexpr = sol[u]
	h = uexpr/v

	print(h)

	breakpoint()






if __name__=="__main__":
	main()

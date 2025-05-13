#!/usr/bin/env python

import sys
from sympy import *

e24 = [1.0, 1.1, 1.2, 1.3, 1.5, 1.6, 1.8, 2.0, 2.2, 2.4, 2.7, 3.0, 3.3, 3.6, 3.9, 4.3, 4.7, 5.1, 5.6, 6.2, 6.8, 7.5, 8.2, 9.1];
e6  = [1.0,                1.5,                2.2,                3.3,                4.7,                6.8               ];



def calc_mfb_lowpass():

	# as in sloa049, fig.7-1
	r1,r2,r3,c1,c2 = symbols('r1 r2 r3 c1 c2')

	# v is input voltage, u is output, x is voltage at r1/c2/r2/r3 node
	u,v,x = symbols('u v x')

	# currents through appropriate elements.
	# ir1,ir2,ir3 flow into the node x,
	# ic2 flows out of the node x
	ir1,ic2,ir2,ir3 = symbols('ir1,ic2,ir2,ir3')

	# angular frequency (omega)
	w = symbols('w')

	# make set of equations to calculate H
	#
	# current through r1
	h_eq1 = Eq( ir1, (v-x)/r1 )
	#
	# current through c2
	h_eq2 = Eq( ic2, x/(1/(I*w*c2)) )
	#
	# current through r2
	h_eq3 = Eq( ir2, (u-x)/r2 )
	#
	# current through r3/c1
	h_eq4 = Eq( ir3, (u-x)/(r3 + 1/(I*w*c1)) )
	#
	# node x charge conservation
	h_eq5 = Eq( ir1+ir2+ir3, ic2 )
	#
	# opamp NFB condition
	h_eq6 = Eq( u-ir3*(1/(I*w*c1)), 0 )

	# solve the set
	u_solve = solve( [h_eq1, h_eq2, h_eq3, h_eq4, h_eq5, h_eq6], [x,u,ir1,ic2,ir2,ir3], dict=True, domain=S.Complexes )

	if len(u_solve)!=1:
		sys.stderr.write("Many or no solutions!\n")
		exit(1)
	
	u_expr = u_solve[0]

	h_expr = u_expr[u]/v

	print(h_expr)

	breakpoint()




def main():

	calc_mfb_lowpass()


if __name__=="__main__":
	main()


#!/usr/bin/env python

import sys
from sympy import *
import math
import cmath
import random

#e24 = [1.0, 1.1, 1.2, 1.3, 1.5, 1.6, 1.8, 2.0, 2.2, 2.4, 2.7, 3.0, 3.3, 3.6, 3.9, 4.3, 4.7, 5.1, 5.6, 6.2, 6.8, 7.5, 8.2, 9.1];
#e6  = [1.0,                1.5,                2.2,                3.3,                4.7,                6.8               ];



def calc_dac():

    r3,r2,r1,r0,rpd = symbols('r3 r2 r1 r0 rpd', real=True, positive=True)

    eq1 = Eq( 1/(1/r3 + 1/r2 + 1/r1 + 1/r0 + 1/rpd), 75.0 )
    eq2 = Eq( r3 / (1/(1/r2+1/r1+1/r0+1/rpd)), (5-0.8)/0.8 ) # bit[3]=1, bits[2:0]=0,   level (unloaded) = 0.8v
    eq3 = Eq( r2 / (1/(1/r3+1/r1+1/r0+1/rpd)), (5-0.4)/0.4 ) # bit[2]=1, bits[3,1:0]=0, level (unloaded) = 0.4v
    eq4 = Eq( r1 / (1/(1/r3+1/r2+1/r0+1/rpd)), (5-0.2)/0.2 ) # bit[1]=1, bits[3:2,0]=0, level (unloaded) = 0.2v
    eq5 = Eq( r0 / (1/(1/r3+1/r2+1/r1+1/rpd)), (5-0.1)/0.1 ) # bit[0]=1, bits[3:0]=0,   level (unloaded) = 0.1v

    # solve the set
    u_solve = solve( [eq1, eq2, eq3, eq4, eq5], [r3,r2,r1,r0,rpd], dict=True )

    if len(u_solve)!=1:
        sys.stderr.write("Many or no solutions: {} !\n".format(u_solve))
        exit(1)
    
    print(u_solve)

    #u_expr = u_solve[0]

    #h_expr = u_expr[u]/v

    #print(h_expr)



def main():

    calc_dac()


if __name__=="__main__":
    main()




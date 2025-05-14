#!/usr/bin/env python

import sys
from sympy import *

e24 = [1.0, 1.1, 1.2, 1.3, 1.5, 1.6, 1.8, 2.0, 2.2, 2.4, 2.7, 3.0, 3.3, 3.6, 3.9, 4.3, 4.7, 5.1, 5.6, 6.2, 6.8, 7.5, 8.2, 9.1];
e6  = [1.0,                1.5,                2.2,                3.3,                4.7,                6.8               ];



def calc_mfb_lowpass():

    # as in sloa049, fig.7-1
    r1,r2,r3,c1,c2 = symbols('r1 r2 r3 c1 c2', real=True, positive=True)

    # v is input voltage, u is output, x is voltage at r1/c2/r2/r3 node
    u,v,x = symbols('u v x')

    # currents through appropriate elements.
    # ir1,ir2,ir3 flow into the node x,
    # ic2 flows out of the node x
    ir1,ic2,ir2,ir3 = symbols('ir1,ic2,ir2,ir3')

    # angular frequency (omega)
    w = symbols('w', real=True, positive=True)

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
        sys.stderr.write("Many or no solutions: {} !\n".format(u_solve))
        exit(1)
    
    u_expr = u_solve[0]

    h_expr = u_expr[u]/v

    print(h_expr)



    # now make more substitutions
    #
    h = symbols('h')
    k = symbols('k', real=True, negative=True)
    q,wc = symbols('q wc', real=True, positive=True)
    #
    # filter gain
    s_eq1 = Eq( k, -r2/r1 )
    #
    # cutoff angular frequency
    s_eq2 = Eq( wc, 1/sqrt(r2*r3*c1*c2) )
    #
    # quality
    s_eq3  = Eq( q, sqrt(r2*r3*c1*c2)/(r3*c1+r2*c1-r3*c1*k) )
    #s_neq3 = Lt( k, 0 )
    #
    # new H expression
    s_eq4 = Eq( h, h_expr )

    # solve
    h_solve = solve( [s_eq1, s_eq2, s_eq3, s_eq4], [h, r1, r2, r3, c1, c2], dict=True )

    if len(h_solve)!=1:
        sys.stderr.write("Many or no solutions: {} !\n".format(h_solve))
        exit(1)

    h_expr = h_solve[0]
    h_result = h_expr[h]

    init_printing()
    print('')
    #pprint(h_expr)
    print('')
    pprint(Eq(h,h_result))


    rc_solve = solve( [s_eq1, s_eq2, s_eq3], [r3,c2], dict=True )

    rc_expr = rc_solve[0]

    print('')
    pprint(Eq(c2,rc_expr[c2]))
    print('')
    pprint(Eq(r3,rc_expr[r3]))
    print('')

"""
                  2        
            k⋅q⋅wc         
h = ───────────────────────
         2       2         
    - q⋅w  + q⋅wc  + ⅈ⋅w⋅wc

           q⋅(k - 1)       
c₂ = ──────────────────────
     r₂⋅wc⋅(c₁⋅q⋅r₂⋅wc - 1)

     c₁⋅q⋅r₂⋅wc - 1 
r₃ = ───────────────
     c₁⋅q⋅wc⋅(k - 1)

"""



def main():

    #calc_mfb_lowpass()


if __name__=="__main__":
    main()




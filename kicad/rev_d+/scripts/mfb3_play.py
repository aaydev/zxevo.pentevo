#!/usr/bin/env python

import sys
from sympy import *
import math
import cmath
import random

#e24 = [1.0, 1.1, 1.2, 1.3, 1.5, 1.6, 1.8, 2.0, 2.2, 2.4, 2.7, 3.0, 3.3, 3.6, 3.9, 4.3, 4.7, 5.1, 5.6, 6.2, 6.8, 7.5, 8.2, 9.1];
#e6  = [1.0,                1.5,                2.2,                3.3,                4.7,                6.8               ];



def play_mfb3_lowpass():

    # as in mfb3.png

    r1,r2,r3,r4,c1,c2,c3 = symbols('r1 r2 r3 r4 c1 c2 c3', real=True, positive=True)

    # angular frequency (omega)
    w = symbols('w', real=True, positive=True)

    ho,hu = symbols('ho hu')
    hon,hun = symbols('hon hun')

    ho = I*r3
    hu = (c1*c2*c3*r1*r2*r3*r4*w**3 - I*c1*c3*r1*r2*r3*w**2 - I*c1*c3*r1*r2*r4*w**2 - I*c1*c3*r1*r3*r4*w**2 - c1*r1*r2*w - I*c2*c3*r1*r3*r4*w**2 - I*c2*c3*r2*r3*r4*w**2 - c3*r1*r3*w - c3*r1*r4*w - c3*r2*r3*w - c3*r2*r4*w - c3*r3*r4*w + I*r1 + I*r2)

    hu = expand(hu/(I*(r1+r2)))

    huc = hu.as_coefficients_dict(w)

    print('hu={}'.format(huc))

    ps0 = simplify(huc[w**0])
    ps1 = simplify(huc[w**1])
    ps2 = simplify(huc[w**2])
    ps3 = simplify(huc[w**3])

    print('ps0={}\nps1={}\nps2={}\nps3={}'.format(ps0,ps1,ps2,ps3))

    #w1,w2,q = symbols('w1 w2 q')

    #weq1 = Eq( ps1, I*(1/w1 + 1/(w2*q)) )
    #weq2 = Eq( ps2, (-1/(q*w1*w2)-1/(w2*w2)) )
    #weq3 = Eq( ps3, -I/(w1*w2*w2) )

    #w_solve = solve( [weq1,weq2,weq3], [w1,w2,q] )

    #print(w_solve)

def play_mfb3_param():
    
    r1,r2,r3,r4,c1,c2,c3 = symbols('r1 r2 r3 r4 c1 c2 c3')
    A,B,C = symbols('A B C')
    w1,w2,q = symbols('w1 w2 q')
    beta,gamma,kappa = symbols('beta gamma kappa')
    #beta=1/w1 gamma=1/w2 kappa=1/q

    #eq1 = Eq( (c1*r1*r2 + c3*r1*r3 + c3*r1*r4 + c3*r2*r3 + c3*r2*r4 + c3*r3*r4)/(r1 + r2), (1/w1 + 1/(w2*q)) )
    #eq2 = Eq( c3*(c1*r1*r2*r3 + c1*r1*r2*r4 + c1*r1*r3*r4 + c2*r1*r3*r4 + c2*r2*r3*r4)/(r1 + r2), (1/(q*w1*w2) + 1/(w2*w2)) )
    #eq3 = Eq( c1*c2*c3*r1*r2*r3*r4/(r1 + r2), 1/(w1*w2*w2) )

    #eq1 = Eq( A, (1/w1 + 1/(w2*q)) )
    #eq2 = Eq( B, (1/(q*w1*w2) + 1/(w2*w2)) )
    #eq3 = Eq( C, 1/(w1*w2*w2) )

    #p_solve = solve( [eq1,eq2,eq3], [w1,w2,q], dict=True, manual=True, simplify=False )
    #print(p_solve)

    eq1 = Eq( A, (beta + gamma*kappa) )
    eq2 = Eq( B, (kappa*beta*gamma + gamma*gamma) )
    eq3 = Eq( C, (beta*gamma*gamma) )
    bgk = solve( [eq1,eq2,eq3], [beta,gamma,kappa], dict=True, manual=True, simplify=False, domain=S.Complexes )
    print(bgk)

    for b in bgk:
        beta_o  = b[beta]
        gamma_o = b[gamma]
        kappa_o = b[kappa]

        print('\n=====')
        print('orig_beta={}'.format(beta_o))
        print('orig_gamma={}'.format(gamma_o))
        print('orig_kappa={}'.format(kappa_o))
       
        beta_s = simplify(beta_o)
        print('simp_beta={}'.format(beta_s))
        gamma_s = simplify(gamma_o)
        print('simp_gamma={}'.format(gamma_s))
        kappa_s = simplify(kappa_o)
        print('simp_kappa={}'.format(kappa_s))



def main():

    #play_mfb3_lowpass()
    play_mfb3_param()



if __name__=="__main__":
    main()




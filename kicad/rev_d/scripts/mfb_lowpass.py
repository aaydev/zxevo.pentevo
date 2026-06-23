#!/usr/bin/env python

import sys
from sympy import *
import math
import cmath
import random

#e24 = [1.0, 1.1, 1.2, 1.3, 1.5, 1.6, 1.8, 2.0, 2.2, 2.4, 2.7, 3.0, 3.3, 3.6, 3.9, 4.3, 4.7, 5.1, 5.6, 6.2, 6.8, 7.5, 8.2, 9.1];
#e6  = [1.0,                1.5,                2.2,                3.3,                4.7,                6.8               ];



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
    pprint(s_eq1)
    print('')
    pprint(s_eq2)
    print('')
    pprint(s_eq3)

# these formulae calculated symbolically by the function above and are set here in comments
# as reference for further numerical calculations.
"""
         ____________
       ╲╱ c₁⋅c₂⋅r₂⋅r₃ 
q = ─────────────────────
     c₁⋅((1 - k)⋅r₃ + r₂) 

            1             
wc = ───────────────
       ____________
     ╲╱ c₁⋅c₂⋅r₂⋅r₃ 


    -r₂ 
k = ────
     r₁ 

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


def select_pvalues_list(values='e24'):
# returns [1.0, ..., 9.1] list (for e24)

    if values=='e24':
        r = [1.0, 1.1, 1.2, 1.3, 1.5, 1.6, 1.8, 2.0, 2.2, 2.4, 2.7, 3.0, 3.3, 3.6, 3.9, 4.3, 4.7, 5.1, 5.6, 6.2, 6.8, 7.5, 8.2, 9.1]
    elif values=='e12':
        r = [1.0,      1.2,      1.5,      1.8,      2.2,      2.7,      3.3,      3.9,      4.7,      5.6,      6.8,      8.2     ]
    elif values=='e6':
        r = [1.0,                1.5,                2.2,                3.3,                4.7,                6.8               ]
    else:
        sys.stderr.write('wrong argument given!\n')
        raise NameError('stop here')
        sys.exit(1)

    r = [r[-1]*0.1] + r + [r[0]*10.0]

    return r




def select_two_nearest(val,val_exp,pvalues_name='e24'):

    def my_lt(v1,v2):
        return (v1<v2) and (not math.isclose(v1,v2,rel_tol=1e-3))

    def my_gt(v1,v2):
        return (v1>v2) and (not math.isclose(v1,v2,rel_tol=1e-3))


    pvalues = select_pvalues_list(pvalues_name)

    # select range
    if my_lt(val, min(pvalues[1:-1])):
        while my_lt(val, min(pvalues[1:-1])):
            if my_lt(val, max(pvalues[1:-1])*0.1):
                val     = val     * 10.0;
                val_exp = val_exp *  0.1;
            else:
                break;

    elif my_gt(val, max(pvalues[1:-1])):
        while my_gt(val, max(pvalues[1:-1])):
            if my_gt(val, min(pvalues[1:-1])*10.0):
                val     = val     *  0.1;
                val_exp = val_exp * 10.0;
            else:
                break;


    # select nearest values
    for i in range(len(pvalues)-1):
        vdn = pvalues[i];
        vup = pvalues[i+1];
        if vdn<=val and val<=vup:
            break;

    vdn_exp = val_exp
    vup_exp = val_exp

    if vdn<pvalues[1]:
        vdn *= 10.0
        vdn_exp *= 0.1

    if vup>pvalues[-2]:
        vup *= 0.1
        vup_exp *= 10.0

    return [ (vdn,vdn_exp), (vup,vup_exp) ];

def select_one_nearest(val,pvalues_name='e24'):

    lval = select_two_nearest(val,1.0,pvalues_name)

    (vdn,vdn_exp) = lval[0]
    (vup,vup_exp) = lval[1]

    vdn *= vdn_exp
    vup *= vup_exp

    if math.fabs( (vdn-val)/val ) <= math.fabs( (vup-val)/val ):
        return vdn
    else:
        return vup

def get_neighbour_value(val,dir='up',pvalues_name='e24'):
    
    pvalues = select_pvalues_list(pvalues_name)
    exp = 1.0

    found_v = None
    found_idx = None
    for idx,v in enumerate(pvalues[1:-1]):
        if math.isclose(val,v,rel_tol=1e-3):
            found_v = v
            found_idx = idx

            #print(' found {} {}'.format(found_v,found_idx))
            break
    
    if found_v is None:
        raise NameError('found_v is None!')
        sys.exit(1)

    if dir=='up':
        if math.isclose(found_v,pvalues[-2],rel_tol=1e-3):
            next_v = pvalues[1]
            exp = 10.0
        else:
            tmp = pvalues[1:-1]
            #print('  tmp {}'.format(tmp))
            next_v = tmp[found_idx+1]
        
    elif dir=='down':
        if math.isclose(found_v,pvalues[1],rel_tol=1e-3):
            next_v = pvalues[-2]
            exp = 0.1
        else:
            tmp = pvalues[1:-1]
            next_v = tmp[found_idx-1]

    else:
        raise NameError('dir is neither "up" nor "down"!')
        sys.exit(1)

    return (next_v,exp)


def val_exp_le( val1, val1_exp, val2, val2_exp ):

    return (val1_exp < val2_exp) or ( math.isclose(val1_exp, val2_exp, rel_tol=1e-3) and ( (val1 < val2) or math.isclose(val1, val2, rel_tol=1e-3) ) )


def select_values_in_range(vrange, vrange_exp, pvalues='e24'):
# range is [min,max]
# returns list of values already multiplied by exp

    vmin=vrange[0]
    vmax=vrange[1]

    if vmax<=vmin:
        raise NameError('wrong vrange: [0] must be bigger than [1]')
        sys.exit(1)

    # expand interval by nearest preferred values
    vmin_vals = select_two_nearest(vmin,vrange_exp, pvalues_name=pvalues)
    vmax_vals = select_two_nearest(vmax,vrange_exp, pvalues_name=pvalues)
    #print(vmin,vmax,vrange_exp)
    #print(vmin_vals,vmax_vals)


    # start building values list

    vmin_val_0, vmin_exp_0 = vmin_vals[0]
    vmin_val_1, vmin_exp_1 = vmin_vals[1]
    vmax_val_0, vmax_exp_0 = vmax_vals[0]
    vmax_val_1, vmax_exp_1 = vmax_vals[1]

    if math.isclose(vmin_val_1*vmin_exp_1, vmin*vrange_exp, rel_tol=1e-3):
        curr_v, curr_exp = vmin_vals[1]
    else:
        curr_v, curr_exp = vmin_vals[0]

    if math.isclose(vmax_val_0*vmax_exp_0, vmax*vrange_exp, rel_tol=1e-3):
        vmax_v, vmax_exp = vmax_vals[0]
    else:
        vmax_v, vmax_exp = vmax_vals[1]

    values_range = []

    #print('vmaxs: {} {}'.format(vmax_v, vmax_exp))
    while val_exp_le( curr_v, curr_exp, vmax_v, vmax_exp ):
      
        #print('currs: {} {}'.format(curr_v, curr_exp))

        values_range = values_range + [ (curr_v, curr_exp) ]

        next_v, exp_update = get_neighbour_value( curr_v, dir='up', pvalues_name=pvalues )
        
        #print('nexts: {} {}\n'.format(next_v, exp_update))

        curr_v = next_v
        curr_exp *= exp_update

    
    return values_range



def calc_mfb2_parameters(r1,r2,r3,c1,c2):

    wc = 1.0/math.sqrt(c1*c2*r2*r3)
    k  = (-r2)/r1
    q  = 1.0 / (wc * c1 * ((1.0-k)*r3 + r2))

    return (wc,k,q)


def randomize_mfb2(r1,r2,r3,c1,c2, c_precision=0.05, r_precision=0.01, iters=1000):

    (wc0, k0, q0) = calc_mfb2_parameters(r1,r2,r3,c1,c2)
 
    d_wc = 0.0
    d_k  = 0.0
    d_q  = 0.0

    for i in range(iters):
        dr1 = random.uniform(1.0-r_precision,1.0+r_precision)
        dr2 = random.uniform(1.0-r_precision,1.0+r_precision)
        dr3 = random.uniform(1.0-r_precision,1.0+r_precision)

        dc1 = random.uniform(1.0-c_precision,1.0+c_precision)
        dc2 = random.uniform(1.0-c_precision,1.0+c_precision)

        rr1 = r1 * dr1
        rr2 = r2 * dr2
        rr3 = r3 * dr3

        rc1 = c1 * dc1
        rc2 = c2 * dc2

        (wc,k,q) = calc_mfb2_parameters(rr1,rr2,rr3,rc1,rc2)

        d_wc = max(d_wc, abs(wc-wc0))
        d_k  = max(d_k,  abs(k-k0)  )
        d_q  = max(d_q,  abs(q-q0)  )

    return (d_wc/wc0, d_k/k0, d_q/q0)



def calc_filter( wc_set=2*math.pi*30000, q_set=1.0, k_set=(-1.17), r1_range=[20.0,30.0], r1_exp=1e3, r1_div=3.0, r1_add=0.0, r_pvalues='e24', c1_range=[33.0,4700.0], c1_exp=1e-12, c_pvalues='e12'):

    c1_values = select_values_in_range( c1_range, c1_exp, pvalues=c_pvalues )

    r1x3_values = select_values_in_range( [x for x in r1_range], r1_exp, pvalues=r_pvalues )
    print(r1_range,r1_exp,r1x3_values)
    r1_values = [(x/r1_div+r1_add/y,y) for (x,y) in r1x3_values]

    print( 'c1 values for sweep: {}'.format([x*y for (x,y) in c1_values]) )
    print( 'r1 values for sweep: {}'.format([x*y for (x,y) in r1_values]) )

    print( '\ncalculating filter for: q={}, k={}, fc={}'.format(q_set,k_set,wc_set/(2*math.pi)) )


    f_values=[]

    for r1 in [x*y for (x,y) in r1_values]:
        for c1 in [x*y for (x,y) in c1_values]:

            r2_ideal = (-1) * k_set * r1

            c2_ideal = (q_set * (k_set - 1.0)) / (r2_ideal * wc_set * (c1 * q_set * r2_ideal * wc_set - 1.0))

            
            r3_ideal = (c1 * q_set * r2_ideal * wc_set - 1.0) / ( (k_set - 1.0) * q_set * wc_set * c1 )

            if r2_ideal<=0.0 or c2_ideal<=0.0 or r3_ideal<=0.0:
                continue

            print('\ntry c1={:1.1e}, r1={:.1f}:'.format(c1,r1))
            print(' ideal values: r2={:.1f}, r3={:.1f}, c2={:1.4e}'.format(r2_ideal, r3_ideal, c2_ideal))

            r2_real = select_one_nearest(r2_ideal, r_pvalues)
            c2_real = select_one_nearest(c2_ideal, c_pvalues)
            r3_real = select_one_nearest(r3_ideal, r_pvalues)
            print(' nearest values: r2={:.1f}, r3={:.1f}, c2={:1.1e}'.format(r2_real, r3_real, c2_real))

            #wc_real = 1.0/math.sqrt(c1*c2_real*r2_real*r3_real)
            #k_real = (-r2_real)/r1
            #q_real = 1.0 / (wc_real * c1 * ((1.0-k_real)*r3_real + r2_real))
            (wc_real,k_real,q_real) = calc_mfb2_parameters(r1,r2_real,r3_real,c1,c2_real)

            print(' real parameters: q={:.4f}, k={:.4f}, fc={:.1f}'.format(q_real,k_real,wc_real/(2*math.pi)) )

            #f_values += [ (r1,r2_real,r3_real, c1, c2_real, wc_real, k_real, q_real) ]
            f_values += [ {'r1':r1,'r2':r2_real,'r3':r3_real, 'c1':c1, 'c2':c2_real, 'wc':wc_real, 'k':k_real, 'q':q_real} ]


    
    print(f_values)

    # sort values by cutoff freq match
    print('\n\n\n sorted by cutoff freq:')
    for e in  sorted(f_values, key = lambda tup: abs((tup['wc']-wc_set)/wc_set) ):
        print('  c1={:1.1e}, r1={:.1f}, r2={:.1f}, r3={:.1f}, c2={:1.1e}, q={:.4f}, k={:.4f}, fc={:.1f}'.format(e['c1'],e['r1'],e['r2'],e['r3'],e['c2'],e['q'],e['k'],e['wc']/(2*math.pi)) )
        (p_wc, p_k, p_q) = randomize_mfb2(e['r1'],e['r2'],e['r3'],e['c1'],e['c2'])
        print('   percent_fc={:.2f}%, percent_k={:.2f}%, percent_q={:.2f}%'.format(100*p_wc,100*p_k,100*p_q))

    # sort values by q match
    print('\n\n\n sorted by q:')
    for e in  sorted(f_values, key = lambda tup: abs((tup['q']-q_set)/q_set) ):
        print('  c1={:1.1e}, r1={:.1f}, r2={:.1f}, r3={:.1f}, c2={:1.1e}, q={:.4f}, k={:.4f}, fc={:.1f}'.format(e['c1'],e['r1'],e['r2'],e['r3'],e['c2'],e['q'],e['k'],e['wc']/(2*math.pi)) )



def main():

    #calc_mfb_lowpass()

    #calc_filter()

    print(" #### AY ####")
    calc_filter( wc_set=2*math.pi*30000, q_set=1.0, k_set=(-1.75), r1_range=[20.0,30.0], r1_exp=1e3, r1_div=3.0, r_pvalues='e24', c1_range=[33.0,4700.0], c1_exp=1e-12, c_pvalues='e12')

#    print(" #### beep ####")
#    calc_filter( wc_set=2*math.pi*30000, q_set=1.0, k_set=(-0.388), r1_range=[4.7,10.0], r1_exp=1e3, r1_div=1.0, r1_add=750, r_pvalues='e24', c1_range=[33.0,4700.0], c1_exp=1e-12, c_pvalues='e12')


if __name__=="__main__":
    main()




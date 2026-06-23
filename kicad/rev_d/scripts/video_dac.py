#!/usr/bin/env python

import sys
from sympy import *
import math
import matplotlib.pyplot as plt
import numpy as np

#e24 = [1.0, 1.1, 1.2, 1.3, 1.5, 1.6, 1.8, 2.0, 2.2, 2.4, 2.7, 3.0, 3.3, 3.6, 3.9, 4.3, 4.7, 5.1, 5.6, 6.2, 6.8, 7.5, 8.2, 9.1];
#e6  = [1.0,                1.5,                2.2,                3.3,                4.7,                6.8               ];


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


def r_fix(r,r_e24,pvalues_name='e24'):
	# select parallel or series additional resistor for r to be closest to r_e24
	if r>=r_e24: # series
		r_exact = r-r_e24
		(rlo_,rhi_)=select_two_nearest(r_exact,1,pvalues_name)
		rlo = rlo_[0] * rlo_[1]
		rhi = rhi_[0] * rhi_[1]
		if abs(r_exact-rlo)<abs(r_exact-rhi):
			return (r_e24+rlo,rlo,'series')
		else:
			return (r_e24+rhi,rhi,'series')

	else: # parallel (r<r_e24)
		r_exact = 1.0/(1.0/r - 1.0/r_e24)
		(rlo_,rhi_)=select_two_nearest(r_exact,1,pvalues_name)
		rlo = rlo_[0] * rlo_[1]
		rhi = rhi_[0] * rhi_[1]
		r_rlo = 1.0/(1.0/r_e24+1.0/rlo)
		r_rhi = 1.0/(1.0/r_e24+1.0/rhi)
		if abs(r_rlo-r)<abs(r_rhi-r):
			return (r_rlo,rlo,'parallel')
		else:
			return (r_rhi,rhi,'parallel')




def calc_dac():

    r3,r2,r1,r0,rpd = symbols('r3 r2 r1 r0 rpd', real=True, positive=True)

    eq1 = Eq( 1/(1/r3 + 1/r2 + 1/r1 + 1/r0 + 1/rpd), 75.0 )  # output impedance
    eq2 = Eq( r3 / (1/(1/r2+1/r1+1/r0+1/rpd)), (5-0.8)/0.8 ) # bit[3]=1, bits[2:0]=0,   level (unloaded) = 0.8v
    eq3 = Eq( r2 / (1/(1/r3+1/r1+1/r0+1/rpd)), (5-0.4)/0.4 ) # bit[2]=1, bits[3,1:0]=0, level (unloaded) = 0.4v
    eq4 = Eq( r1 / (1/(1/r3+1/r2+1/r0+1/rpd)), (5-0.2)/0.2 ) # bit[1]=1, bits[3:2,0]=0, level (unloaded) = 0.2v
    eq5 = Eq( r0 / (1/(1/r3+1/r2+1/r1+1/rpd)), (5-0.1)/0.1 ) # bit[0]=1, bits[3:0]=0,   level (unloaded) = 0.1v (all on: 1.5v)

    # solve the set
    u_solve = solve( [eq1, eq2, eq3, eq4, eq5], [r3,r2,r1,r0,rpd], dict=True )

    if len(u_solve)!=1:
        sys.stderr.write("Many or no solutions: {} !\n".format(u_solve))
        exit(1)
    
    print(u_solve)

    # ideal result should be:
    #
    # r3 = 468.75
    # r2 = 937.5
    # r1 = 1875
    # r0 = 3750
    # rpd = 107+1/7

def calc_dac_out(r0=3750,r1=1875,r2=937.5,r3=468.75,rpd=(107+1/7),rload=75,val=15):

    assert type(val) is int
    assert 0<=val and val<=15

    if val==0:
        return 0.0

    sup = 0
    sdn = 1/rpd + 1/rload

    if val & 1:
        sup += 1/(r0+5)
    else:
        sdn += 1/(r0-5)

    if val & 2:
        sup += 1/(r1+5)
    else:              
        sdn += 1/(r1-5)

    if val & 4:
        sup += 1/(r2+5)
    else:              
        sdn += 1/(r2-5)

    if val & 8:
        sup += 1/(r3+5)
    else:              
        sdn += 1/(r3-5)

    return 5.0/(sdn*(1/sup+1/sdn))



def draw_dac(r0=3750,r1=1875,r2=937.5,r3=468.75,rpd=(107+1/7),label='unknown'):

    plt.figure(1)
    n = range(0,16)

    v_ideal = [calc_dac_out(val=i) for i in n]
    v_e24   = [calc_dac_out(r0,r1,r2,r3,rpd,val=i) for i in n]

    plt.plot(n,v_ideal,linewidth=2,label='ideal')
    plt.plot(n,v_e24,linewidth=2,label=label)
    plt.legend(title='video DAC')
    plt.xlim(0,15)
    plt.xticks(np.linspace(0,15,num=16))
    plt.xlabel('DAC value')
    plt.ylim(0,0.8)
    plt.yticks(np.linspace(0,0.8,num=17))
    plt.ylabel('Volts')
    plt.grid(True)
    plt.show()


def main():

    #calc_dac()
    #draw_dac()

    ric = 20 
    r3 = 468.75 - ric
    r2 = 937.5 - ric
    r1 = 1875 - ric
    r0 = 3750 -ric
    rpd = 107+1/7

    r0_1 = select_one_nearest(r0,pvalues_name='e24')
    r1_1 = select_one_nearest(r1,'e24')
    r2_1 = select_one_nearest(r2,'e24')
    r3_1 = select_one_nearest(r3,'e24')
    rpd_1 = select_one_nearest(rpd,'e24')

    print(r0_1,r1_1,r2_1,r3_1,rpd_1)

    r0_2 = r_fix(r0,r0_1,pvalues_name='e24')
    r1_2 = r_fix(r1,r1_1,pvalues_name='e24')
    r2_2 = r_fix(r2,r2_1,pvalues_name='e24')
    r3_2 = r_fix(r3,r3_1,pvalues_name='e24')
    rpd_2 = r_fix(rpd,rpd_1,pvalues_name='e24')

    print(r0_2,r1_2,r2_2,r3_2,rpd_2)


    rr0 = r0_1 + ric + r0_2[1]
    rr1 = r1_1 + ric + r1_2[1]
    rr2 = r2_1 + ric
    rr3 = r3_1 + ric + r3_2[1]
    rrpd = 1/(1/rpd_1 + 1/rpd_2[1])

    print(rr0,rr1,rr2,rr3,rrpd)

    draw_dac(r0=rr0,r1=rr1,r2=rr2,r3=rr3,rpd=rrpd,label='2xE24')



if __name__=="__main__":
    main()




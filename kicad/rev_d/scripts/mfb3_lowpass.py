#!/usr/bin/env python

import sys
import math
import cmath
import random

#e24 = [1.0, 1.1, 1.2, 1.3, 1.5, 1.6, 1.8, 2.0, 2.2, 2.4, 2.7, 3.0, 3.3, 3.6, 3.9, 4.3, 4.7, 5.1, 5.6, 6.2, 6.8, 7.5, 8.2, 9.1];
#e6  = [1.0,                1.5,                2.2,                3.3,                4.7,                6.8               ];



# these formulae calculated symbolically by the function above and are set here in comments
# as reference for further numerical calculations.
# see mfb3_*play.py for ways to calculate
"""
r1,r2,r3,c3 -- select arbitrary/as needed,
calculate c1,c2,r4

(see mfb3.png)

w1   - 1st order,
w2,q - 2nd order

1/w1 + 1/(q*w2)       = A = (c1*r1*r2 + c3*r1*r3 + c3*r1*r4 + c3*r2*r3 + c3*r2*r4 + c3*r3*r4)/(r1 + r2)
1/w2**2 + 1/(q*w1*w2) = B = c3*(c1*r1*r2*r3 + c1*r1*r2*r4 + c1*r1*r3*r4 + c2*r1*r3*r4 + c2*r2*r3*r4)/(r1 + r2)
1/(w1*w2**2)          = C = c1*c2*c3*r1*r2*r3*r4/(r1 + r2)

w1 = (2**(1/3)*A*C - 2**(1/3)*B**2/3 + B*(9*A*B*C - 2*B**3 - 27*C**2 + 3*sqrt(3)*sqrt(C**2*(4*A**3*C - A**2*B**2 - 18*A*B*C + 4*B**3 + 27*C**2)))**(1/3)/3 - (18*A*B*C - 4*B**3 - 54*C**2 + 6*sqrt(3)*sqrt(C**2*(4*A**3*C - A**2*B**2 - 18*A*B*C + 4*B**3 + 27*C**2)))**(2/3)/6)/(C*(9*A*B*C - 2*B**3 - 27*C**2 + 3*sqrt(3)*sqrt(C**2*(4*A**3*C - A**2*B**2 - 18*A*B*C + 4*B**3 + 27*C**2)))**(1/3))
w2 = 2**(1/6)*sqrt(3)/sqrt((3*2**(2/3)*A*C - 2**(2/3)*B**2 + (2**(1/3)*B - (9*A*B*C - 2*B**3 - 27*C**2 + 3*sqrt(3)*sqrt(C**2*(4*A**3*C - A**2*B**2 - 18*A*B*C + 4*B**3 + 27*C**2)))**(1/3))*(9*A*B*C - 2*B**3 - 27*C**2 + 3*sqrt(3)*sqrt(C**2*(4*A**3*C - A**2*B**2 - 18*A*B*C + 4*B**3 + 27*C**2)))**(1/3))/(9*A*B*C - 2*B**3 - 27*C**2 + 3*sqrt(3)*sqrt(C**2*(4*A**3*C - A**2*B**2 - 18*A*B*C + 4*B**3 + 27*C**2)))**(1/3))
q  = 2**(5/6)*sqrt(3)*sqrt((3*2**(2/3)*A*C - 2**(2/3)*B**2 + (2**(1/3)*B - (9*A*B*C - 2*B**3 - 27*C**2 + 3*sqrt(3)*sqrt(C**2*(4*A**3*C - A**2*B**2 - 18*A*B*C + 4*B**3 + 27*C**2)))**(1/3))*(9*A*B*C - 2*B**3 - 27*C**2 + 3*sqrt(3)*sqrt(C**2*(4*A**3*C - A**2*B**2 - 18*A*B*C + 4*B**3 + 27*C**2)))**(1/3))/(9*A*B*C - 2*B**3 - 27*C**2 + 3*sqrt(3)*sqrt(C**2*(4*A**3*C - A**2*B**2 - 18*A*B*C + 4*B**3 + 27*C**2)))**(1/3))*(3*2**(2/3)*A*C - 2**(2/3)*B**2 + (2**(1/3)*B - (9*A*B*C - 2*B**3 - 27*C**2 + 3*sqrt(3)*sqrt(C**2*(4*A**3*C - A**2*B**2 - 18*A*B*C + 4*B**3 + 27*C**2)))**(1/3))*(9*A*B*C - 2*B**3 - 27*C**2 + 3*sqrt(3)*sqrt(C**2*(4*A**3*C - A**2*B**2 - 18*A*B*C + 4*B**3 + 27*C**2)))**(1/3))/(6*(A*(3*2**(2/3)*A*C - 2**(2/3)*B**2 + (2**(1/3)*B - (9*A*B*C - 2*B**3 - 27*C**2 + 3*sqrt(3)*sqrt(C**2*(4*A**3*C - A**2*B**2 - 18*A*B*C + 4*B**3 + 27*C**2)))**(1/3))*(9*A*B*C - 2*B**3 - 27*C**2 + 3*sqrt(3)*sqrt(C**2*(4*A**3*C - A**2*B**2 - 18*A*B*C + 4*B**3 + 27*C**2)))**(1/3)) - 3*2**(1/3)*C*(9*A*B*C - 2*B**3 - 27*C**2 + 3*sqrt(3)*sqrt(C**2*(4*A**3*C - A**2*B**2 - 18*A*B*C + 4*B**3 + 27*C**2)))**(1/3)))

r4 = (-(-3*B*r1*r2*(r1 + r2)*(r1 + r2 + r3) + 6*c3*r1*r2*r3*(A - c3*r3)*(r1 + r2)*(r1 + r2 + r3) - 3*r1*(A - c3*r3)**2*(r1 + r2)**2*(r2 + r3))/(c3**2*r1*(r2 + r3)*(r1 + r2 + r3)**2) - (c3*r1*r2*r3*(r1 + r2 + r3) - 2*r1*(A - c3*r3)*(r1 + r2)*(r2 + r3))**2/(c3**2*r1**2*(r2 + r3)**2*(r1 + r2 + r3)**2))/(3*(sqrt(-4*((-3*B*r1*r2*(r1 + r2)*(r1 + r2 + r3) + 6*c3*r1*r2*r3*(A - c3*r3)*(r1 + r2)*(r1 + r2 + r3) - 3*r1*(A - c3*r3)**2*(r1 + r2)**2*(r2 + r3))/(c3**2*r1*(r2 + r3)*(r1 + r2 + r3)**2) + (c3*r1*r2*r3*(r1 + r2 + r3) - 2*r1*(A - c3*r3)*(r1 + r2)*(r2 + r3))**2/(c3**2*r1**2*(r2 + r3)**2*(r1 + r2 + r3)**2))**3 + ((-27*B*r1*r2*(A - c3*r3)*(r1 + r2)**2/c3 + 27*C*r1*r2*(r1 + r2)**2/c3 + 27*r1*r2*r3*(A - c3*r3)**2*(r1 + r2)**2)/(c3**2*r1*(r2 + r3)*(r1 + r2 + r3)**2) - (9*c3*r1*r2*r3*(r1 + r2 + r3) - 18*r1*(A - c3*r3)*(r1 + r2)*(r2 + r3))*(B*r1*r2*(r1 + r2)*(r1 + r2 + r3) - 2*c3*r1*r2*r3*(A - c3*r3)*(r1 + r2)*(r1 + r2 + r3) + r1*(A - c3*r3)**2*(r1 + r2)**2*(r2 + r3))/(c3**3*r1**2*(r2 + r3)**2*(r1 + r2 + r3)**3) + 2*(c3*r1*r2*r3*(r1 + r2 + r3) - 2*r1*(A - c3*r3)*(r1 + r2)*(r2 + r3))**3/(c3**3*r1**3*(r2 + r3)**3*(r1 + r2 + r3)**3))**2)/2 + (-27*B*r1*r2*(A - c3*r3)*(r1 + r2)**2/c3 + 27*C*r1*r2*(r1 + r2)**2/c3 + 27*r1*r2*r3*(A - c3*r3)**2*(r1 + r2)**2)/(2*c3**2*r1*(r2 + r3)*(r1 + r2 + r3)**2) - (9*c3*r1*r2*r3*(r1 + r2 + r3) - 18*r1*(A - c3*r3)*(r1 + r2)*(r2 + r3))*(B*r1*r2*(r1 + r2)*(r1 + r2 + r3) - 2*c3*r1*r2*r3*(A - c3*r3)*(r1 + r2)*(r1 + r2 + r3) + r1*(A - c3*r3)**2*(r1 + r2)**2*(r2 + r3))/(2*c3**3*r1**2*(r2 + r3)**2*(r1 + r2 + r3)**3) + (c3*r1*r2*r3*(r1 + r2 + r3) - 2*r1*(A - c3*r3)*(r1 + r2)*(r2 + r3))**3/(c3**3*r1**3*(r2 + r3)**3*(r1 + r2 + r3)**3))**(1/3)) - (sqrt(-4*((-3*B*r1*r2*(r1 + r2)*(r1 + r2 + r3) + 6*c3*r1*r2*r3*(A - c3*r3)*(r1 + r2)*(r1 + r2 + r3) - 3*r1*(A - c3*r3)**2*(r1 + r2)**2*(r2 + r3))/(c3**2*r1*(r2 + r3)*(r1 + r2 + r3)**2) + (c3*r1*r2*r3*(r1 + r2 + r3) - 2*r1*(A - c3*r3)*(r1 + r2)*(r2 + r3))**2/(c3**2*r1**2*(r2 + r3)**2*(r1 + r2 + r3)**2))**3 + ((-27*B*r1*r2*(A - c3*r3)*(r1 + r2)**2/c3 + 27*C*r1*r2*(r1 + r2)**2/c3 + 27*r1*r2*r3*(A - c3*r3)**2*(r1 + r2)**2)/(c3**2*r1*(r2 + r3)*(r1 + r2 + r3)**2) - (9*c3*r1*r2*r3*(r1 + r2 + r3) - 18*r1*(A - c3*r3)*(r1 + r2)*(r2 + r3))*(B*r1*r2*(r1 + r2)*(r1 + r2 + r3) - 2*c3*r1*r2*r3*(A - c3*r3)*(r1 + r2)*(r1 + r2 + r3) + r1*(A - c3*r3)**2*(r1 + r2)**2*(r2 + r3))/(c3**3*r1**2*(r2 + r3)**2*(r1 + r2 + r3)**3) + 2*(c3*r1*r2*r3*(r1 + r2 + r3) - 2*r1*(A - c3*r3)*(r1 + r2)*(r2 + r3))**3/(c3**3*r1**3*(r2 + r3)**3*(r1 + r2 + r3)**3))**2)/2 + (-27*B*r1*r2*(A - c3*r3)*(r1 + r2)**2/c3 + 27*C*r1*r2*(r1 + r2)**2/c3 + 27*r1*r2*r3*(A - c3*r3)**2*(r1 + r2)**2)/(2*c3**2*r1*(r2 + r3)*(r1 + r2 + r3)**2) - (9*c3*r1*r2*r3*(r1 + r2 + r3) - 18*r1*(A - c3*r3)*(r1 + r2)*(r2 + r3))*(B*r1*r2*(r1 + r2)*(r1 + r2 + r3) - 2*c3*r1*r2*r3*(A - c3*r3)*(r1 + r2)*(r1 + r2 + r3) + r1*(A - c3*r3)**2*(r1 + r2)**2*(r2 + r3))/(2*c3**3*r1**2*(r2 + r3)**2*(r1 + r2 + r3)**3) + (c3*r1*r2*r3*(r1 + r2 + r3) - 2*r1*(A - c3*r3)*(r1 + r2)*(r2 + r3))**3/(c3**3*r1**3*(r2 + r3)**3*(r1 + r2 + r3)**3))**(1/3)/3 - (c3*r1*r2*r3*(r1 + r2 + r3) - 2*r1*(A - c3*r3)*(r1 + r2)*(r2 + r3))/(3*c3*r1*(r2 + r3)*(r1 + r2 + r3))
c1 = -c3*(r1 + r2 + r3)*((-(-3*B*r1*r2*(r1 + r2)*(r1 + r2 + r3) + 6*c3*r1*r2*r3*(A - c3*r3)*(r1 + r2)*(r1 + r2 + r3) - 3*r1*(A - c3*r3)**2*(r1 + r2)**2*(r2 + r3))/(c3**2*r1*(r2 + r3)*(r1 + r2 + r3)**2) - (c3*r1*r2*r3*(r1 + r2 + r3) - 2*r1*(A - c3*r3)*(r1 + r2)*(r2 + r3))**2/(c3**2*r1**2*(r2 + r3)**2*(r1 + r2 + r3)**2))/(3*(sqrt(-4*((-3*B*r1*r2*(r1 + r2)*(r1 + r2 + r3) + 6*c3*r1*r2*r3*(A - c3*r3)*(r1 + r2)*(r1 + r2 + r3) - 3*r1*(A - c3*r3)**2*(r1 + r2)**2*(r2 + r3))/(c3**2*r1*(r2 + r3)*(r1 + r2 + r3)**2) + (c3*r1*r2*r3*(r1 + r2 + r3) - 2*r1*(A - c3*r3)*(r1 + r2)*(r2 + r3))**2/(c3**2*r1**2*(r2 + r3)**2*(r1 + r2 + r3)**2))**3 + ((-27*B*r1*r2*(A - c3*r3)*(r1 + r2)**2/c3 + 27*C*r1*r2*(r1 + r2)**2/c3 + 27*r1*r2*r3*(A - c3*r3)**2*(r1 + r2)**2)/(c3**2*r1*(r2 + r3)*(r1 + r2 + r3)**2) - (9*c3*r1*r2*r3*(r1 + r2 + r3) - 18*r1*(A - c3*r3)*(r1 + r2)*(r2 + r3))*(B*r1*r2*(r1 + r2)*(r1 + r2 + r3) - 2*c3*r1*r2*r3*(A - c3*r3)*(r1 + r2)*(r1 + r2 + r3) + r1*(A - c3*r3)**2*(r1 + r2)**2*(r2 + r3))/(c3**3*r1**2*(r2 + r3)**2*(r1 + r2 + r3)**3) + 2*(c3*r1*r2*r3*(r1 + r2 + r3) - 2*r1*(A - c3*r3)*(r1 + r2)*(r2 + r3))**3/(c3**3*r1**3*(r2 + r3)**3*(r1 + r2 + r3)**3))**2)/2 + (-27*B*r1*r2*(A - c3*r3)*(r1 + r2)**2/c3 + 27*C*r1*r2*(r1 + r2)**2/c3 + 27*r1*r2*r3*(A - c3*r3)**2*(r1 + r2)**2)/(2*c3**2*r1*(r2 + r3)*(r1 + r2 + r3)**2) - (9*c3*r1*r2*r3*(r1 + r2 + r3) - 18*r1*(A - c3*r3)*(r1 + r2)*(r2 + r3))*(B*r1*r2*(r1 + r2)*(r1 + r2 + r3) - 2*c3*r1*r2*r3*(A - c3*r3)*(r1 + r2)*(r1 + r2 + r3) + r1*(A - c3*r3)**2*(r1 + r2)**2*(r2 + r3))/(2*c3**3*r1**2*(r2 + r3)**2*(r1 + r2 + r3)**3) + (c3*r1*r2*r3*(r1 + r2 + r3) - 2*r1*(A - c3*r3)*(r1 + r2)*(r2 + r3))**3/(c3**3*r1**3*(r2 + r3)**3*(r1 + r2 + r3)**3))**(1/3)) - (sqrt(-4*((-3*B*r1*r2*(r1 + r2)*(r1 + r2 + r3) + 6*c3*r1*r2*r3*(A - c3*r3)*(r1 + r2)*(r1 + r2 + r3) - 3*r1*(A - c3*r3)**2*(r1 + r2)**2*(r2 + r3))/(c3**2*r1*(r2 + r3)*(r1 + r2 + r3)**2) + (c3*r1*r2*r3*(r1 + r2 + r3) - 2*r1*(A - c3*r3)*(r1 + r2)*(r2 + r3))**2/(c3**2*r1**2*(r2 + r3)**2*(r1 + r2 + r3)**2))**3 + ((-27*B*r1*r2*(A - c3*r3)*(r1 + r2)**2/c3 + 27*C*r1*r2*(r1 + r2)**2/c3 + 27*r1*r2*r3*(A - c3*r3)**2*(r1 + r2)**2)/(c3**2*r1*(r2 + r3)*(r1 + r2 + r3)**2) - (9*c3*r1*r2*r3*(r1 + r2 + r3) - 18*r1*(A - c3*r3)*(r1 + r2)*(r2 + r3))*(B*r1*r2*(r1 + r2)*(r1 + r2 + r3) - 2*c3*r1*r2*r3*(A - c3*r3)*(r1 + r2)*(r1 + r2 + r3) + r1*(A - c3*r3)**2*(r1 + r2)**2*(r2 + r3))/(c3**3*r1**2*(r2 + r3)**2*(r1 + r2 + r3)**3) + 2*(c3*r1*r2*r3*(r1 + r2 + r3) - 2*r1*(A - c3*r3)*(r1 + r2)*(r2 + r3))**3/(c3**3*r1**3*(r2 + r3)**3*(r1 + r2 + r3)**3))**2)/2 + (-27*B*r1*r2*(A - c3*r3)*(r1 + r2)**2/c3 + 27*C*r1*r2*(r1 + r2)**2/c3 + 27*r1*r2*r3*(A - c3*r3)**2*(r1 + r2)**2)/(2*c3**2*r1*(r2 + r3)*(r1 + r2 + r3)**2) - (9*c3*r1*r2*r3*(r1 + r2 + r3) - 18*r1*(A - c3*r3)*(r1 + r2)*(r2 + r3))*(B*r1*r2*(r1 + r2)*(r1 + r2 + r3) - 2*c3*r1*r2*r3*(A - c3*r3)*(r1 + r2)*(r1 + r2 + r3) + r1*(A - c3*r3)**2*(r1 + r2)**2*(r2 + r3))/(2*c3**3*r1**2*(r2 + r3)**2*(r1 + r2 + r3)**3) + (c3*r1*r2*r3*(r1 + r2 + r3) - 2*r1*(A - c3*r3)*(r1 + r2)*(r2 + r3))**3/(c3**3*r1**3*(r2 + r3)**3*(r1 + r2 + r3)**3))**(1/3)/3 - (c3*r1*r2*r3*(r1 + r2 + r3) - 2*r1*(A - c3*r3)*(r1 + r2)*(r2 + r3))/(3*c3*r1*(r2 + r3)*(r1 + r2 + r3)))/(r1*r2) + (A - c3*r3)*(r1 + r2)/(r1*r2)
c2 = C*(r1 + r2)/(c3*r3*(-c3*(r1 + r2 + r3)*((-(-3*B*r1*r2*(r1 + r2)*(r1 + r2 + r3) + 6*c3*r1*r2*r3*(A - c3*r3)*(r1 + r2)*(r1 + r2 + r3) - 3*r1*(A - c3*r3)**2*(r1 + r2)**2*(r2 + r3))/(c3**2*r1*(r2 + r3)*(r1 + r2 + r3)**2) - (c3*r1*r2*r3*(r1 + r2 + r3) - 2*r1*(A - c3*r3)*(r1 + r2)*(r2 + r3))**2/(c3**2*r1**2*(r2 + r3)**2*(r1 + r2 + r3)**2))/(3*(sqrt(-4*((-3*B*r1*r2*(r1 + r2)*(r1 + r2 + r3) + 6*c3*r1*r2*r3*(A - c3*r3)*(r1 + r2)*(r1 + r2 + r3) - 3*r1*(A - c3*r3)**2*(r1 + r2)**2*(r2 + r3))/(c3**2*r1*(r2 + r3)*(r1 + r2 + r3)**2) + (c3*r1*r2*r3*(r1 + r2 + r3) - 2*r1*(A - c3*r3)*(r1 + r2)*(r2 + r3))**2/(c3**2*r1**2*(r2 + r3)**2*(r1 + r2 + r3)**2))**3 + ((-27*B*r1*r2*(A - c3*r3)*(r1 + r2)**2/c3 + 27*C*r1*r2*(r1 + r2)**2/c3 + 27*r1*r2*r3*(A - c3*r3)**2*(r1 + r2)**2)/(c3**2*r1*(r2 + r3)*(r1 + r2 + r3)**2) - (9*c3*r1*r2*r3*(r1 + r2 + r3) - 18*r1*(A - c3*r3)*(r1 + r2)*(r2 + r3))*(B*r1*r2*(r1 + r2)*(r1 + r2 + r3) - 2*c3*r1*r2*r3*(A - c3*r3)*(r1 + r2)*(r1 + r2 + r3) + r1*(A - c3*r3)**2*(r1 + r2)**2*(r2 + r3))/(c3**3*r1**2*(r2 + r3)**2*(r1 + r2 + r3)**3) + 2*(c3*r1*r2*r3*(r1 + r2 + r3) - 2*r1*(A - c3*r3)*(r1 + r2)*(r2 + r3))**3/(c3**3*r1**3*(r2 + r3)**3*(r1 + r2 + r3)**3))**2)/2 + (-27*B*r1*r2*(A - c3*r3)*(r1 + r2)**2/c3 + 27*C*r1*r2*(r1 + r2)**2/c3 + 27*r1*r2*r3*(A - c3*r3)**2*(r1 + r2)**2)/(2*c3**2*r1*(r2 + r3)*(r1 + r2 + r3)**2) - (9*c3*r1*r2*r3*(r1 + r2 + r3) - 18*r1*(A - c3*r3)*(r1 + r2)*(r2 + r3))*(B*r1*r2*(r1 + r2)*(r1 + r2 + r3) - 2*c3*r1*r2*r3*(A - c3*r3)*(r1 + r2)*(r1 + r2 + r3) + r1*(A - c3*r3)**2*(r1 + r2)**2*(r2 + r3))/(2*c3**3*r1**2*(r2 + r3)**2*(r1 + r2 + r3)**3) + (c3*r1*r2*r3*(r1 + r2 + r3) - 2*r1*(A - c3*r3)*(r1 + r2)*(r2 + r3))**3/(c3**3*r1**3*(r2 + r3)**3*(r1 + r2 + r3)**3))**(1/3)) - (sqrt(-4*((-3*B*r1*r2*(r1 + r2)*(r1 + r2 + r3) + 6*c3*r1*r2*r3*(A - c3*r3)*(r1 + r2)*(r1 + r2 + r3) - 3*r1*(A - c3*r3)**2*(r1 + r2)**2*(r2 + r3))/(c3**2*r1*(r2 + r3)*(r1 + r2 + r3)**2) + (c3*r1*r2*r3*(r1 + r2 + r3) - 2*r1*(A - c3*r3)*(r1 + r2)*(r2 + r3))**2/(c3**2*r1**2*(r2 + r3)**2*(r1 + r2 + r3)**2))**3 + ((-27*B*r1*r2*(A - c3*r3)*(r1 + r2)**2/c3 + 27*C*r1*r2*(r1 + r2)**2/c3 + 27*r1*r2*r3*(A - c3*r3)**2*(r1 + r2)**2)/(c3**2*r1*(r2 + r3)*(r1 + r2 + r3)**2) - (9*c3*r1*r2*r3*(r1 + r2 + r3) - 18*r1*(A - c3*r3)*(r1 + r2)*(r2 + r3))*(B*r1*r2*(r1 + r2)*(r1 + r2 + r3) - 2*c3*r1*r2*r3*(A - c3*r3)*(r1 + r2)*(r1 + r2 + r3) + r1*(A - c3*r3)**2*(r1 + r2)**2*(r2 + r3))/(c3**3*r1**2*(r2 + r3)**2*(r1 + r2 + r3)**3) + 2*(c3*r1*r2*r3*(r1 + r2 + r3) - 2*r1*(A - c3*r3)*(r1 + r2)*(r2 + r3))**3/(c3**3*r1**3*(r2 + r3)**3*(r1 + r2 + r3)**3))**2)/2 + (-27*B*r1*r2*(A - c3*r3)*(r1 + r2)**2/c3 + 27*C*r1*r2*(r1 + r2)**2/c3 + 27*r1*r2*r3*(A - c3*r3)**2*(r1 + r2)**2)/(2*c3**2*r1*(r2 + r3)*(r1 + r2 + r3)**2) - (9*c3*r1*r2*r3*(r1 + r2 + r3) - 18*r1*(A - c3*r3)*(r1 + r2)*(r2 + r3))*(B*r1*r2*(r1 + r2)*(r1 + r2 + r3) - 2*c3*r1*r2*r3*(A - c3*r3)*(r1 + r2)*(r1 + r2 + r3) + r1*(A - c3*r3)**2*(r1 + r2)**2*(r2 + r3))/(2*c3**3*r1**2*(r2 + r3)**2*(r1 + r2 + r3)**3) + (c3*r1*r2*r3*(r1 + r2 + r3) - 2*r1*(A - c3*r3)*(r1 + r2)*(r2 + r3))**3/(c3**3*r1**3*(r2 + r3)**3*(r1 + r2 + r3)**3))**(1/3)/3 - (c3*r1*r2*r3*(r1 + r2 + r3) - 2*r1*(A - c3*r3)*(r1 + r2)*(r2 + r3))/(3*c3*r1*(r2 + r3)*(r1 + r2 + r3)))**2 + (A - c3*r3)*(r1 + r2)*((-(-3*B*r1*r2*(r1 + r2)*(r1 + r2 + r3) + 6*c3*r1*r2*r3*(A - c3*r3)*(r1 + r2)*(r1 + r2 + r3) - 3*r1*(A - c3*r3)**2*(r1 + r2)**2*(r2 + r3))/(c3**2*r1*(r2 + r3)*(r1 + r2 + r3)**2) - (c3*r1*r2*r3*(r1 + r2 + r3) - 2*r1*(A - c3*r3)*(r1 + r2)*(r2 + r3))**2/(c3**2*r1**2*(r2 + r3)**2*(r1 + r2 + r3)**2))/(3*(sqrt(-4*((-3*B*r1*r2*(r1 + r2)*(r1 + r2 + r3) + 6*c3*r1*r2*r3*(A - c3*r3)*(r1 + r2)*(r1 + r2 + r3) - 3*r1*(A - c3*r3)**2*(r1 + r2)**2*(r2 + r3))/(c3**2*r1*(r2 + r3)*(r1 + r2 + r3)**2) + (c3*r1*r2*r3*(r1 + r2 + r3) - 2*r1*(A - c3*r3)*(r1 + r2)*(r2 + r3))**2/(c3**2*r1**2*(r2 + r3)**2*(r1 + r2 + r3)**2))**3 + ((-27*B*r1*r2*(A - c3*r3)*(r1 + r2)**2/c3 + 27*C*r1*r2*(r1 + r2)**2/c3 + 27*r1*r2*r3*(A - c3*r3)**2*(r1 + r2)**2)/(c3**2*r1*(r2 + r3)*(r1 + r2 + r3)**2) - (9*c3*r1*r2*r3*(r1 + r2 + r3) - 18*r1*(A - c3*r3)*(r1 + r2)*(r2 + r3))*(B*r1*r2*(r1 + r2)*(r1 + r2 + r3) - 2*c3*r1*r2*r3*(A - c3*r3)*(r1 + r2)*(r1 + r2 + r3) + r1*(A - c3*r3)**2*(r1 + r2)**2*(r2 + r3))/(c3**3*r1**2*(r2 + r3)**2*(r1 + r2 + r3)**3) + 2*(c3*r1*r2*r3*(r1 + r2 + r3) - 2*r1*(A - c3*r3)*(r1 + r2)*(r2 + r3))**3/(c3**3*r1**3*(r2 + r3)**3*(r1 + r2 + r3)**3))**2)/2 + (-27*B*r1*r2*(A - c3*r3)*(r1 + r2)**2/c3 + 27*C*r1*r2*(r1 + r2)**2/c3 + 27*r1*r2*r3*(A - c3*r3)**2*(r1 + r2)**2)/(2*c3**2*r1*(r2 + r3)*(r1 + r2 + r3)**2) - (9*c3*r1*r2*r3*(r1 + r2 + r3) - 18*r1*(A - c3*r3)*(r1 + r2)*(r2 + r3))*(B*r1*r2*(r1 + r2)*(r1 + r2 + r3) - 2*c3*r1*r2*r3*(A - c3*r3)*(r1 + r2)*(r1 + r2 + r3) + r1*(A - c3*r3)**2*(r1 + r2)**2*(r2 + r3))/(2*c3**3*r1**2*(r2 + r3)**2*(r1 + r2 + r3)**3) + (c3*r1*r2*r3*(r1 + r2 + r3) - 2*r1*(A - c3*r3)*(r1 + r2)*(r2 + r3))**3/(c3**3*r1**3*(r2 + r3)**3*(r1 + r2 + r3)**3))**(1/3)) - (sqrt(-4*((-3*B*r1*r2*(r1 + r2)*(r1 + r2 + r3) + 6*c3*r1*r2*r3*(A - c3*r3)*(r1 + r2)*(r1 + r2 + r3) - 3*r1*(A - c3*r3)**2*(r1 + r2)**2*(r2 + r3))/(c3**2*r1*(r2 + r3)*(r1 + r2 + r3)**2) + (c3*r1*r2*r3*(r1 + r2 + r3) - 2*r1*(A - c3*r3)*(r1 + r2)*(r2 + r3))**2/(c3**2*r1**2*(r2 + r3)**2*(r1 + r2 + r3)**2))**3 + ((-27*B*r1*r2*(A - c3*r3)*(r1 + r2)**2/c3 + 27*C*r1*r2*(r1 + r2)**2/c3 + 27*r1*r2*r3*(A - c3*r3)**2*(r1 + r2)**2)/(c3**2*r1*(r2 + r3)*(r1 + r2 + r3)**2) - (9*c3*r1*r2*r3*(r1 + r2 + r3) - 18*r1*(A - c3*r3)*(r1 + r2)*(r2 + r3))*(B*r1*r2*(r1 + r2)*(r1 + r2 + r3) - 2*c3*r1*r2*r3*(A - c3*r3)*(r1 + r2)*(r1 + r2 + r3) + r1*(A - c3*r3)**2*(r1 + r2)**2*(r2 + r3))/(c3**3*r1**2*(r2 + r3)**2*(r1 + r2 + r3)**3) + 2*(c3*r1*r2*r3*(r1 + r2 + r3) - 2*r1*(A - c3*r3)*(r1 + r2)*(r2 + r3))**3/(c3**3*r1**3*(r2 + r3)**3*(r1 + r2 + r3)**3))**2)/2 + (-27*B*r1*r2*(A - c3*r3)*(r1 + r2)**2/c3 + 27*C*r1*r2*(r1 + r2)**2/c3 + 27*r1*r2*r3*(A - c3*r3)**2*(r1 + r2)**2)/(2*c3**2*r1*(r2 + r3)*(r1 + r2 + r3)**2) - (9*c3*r1*r2*r3*(r1 + r2 + r3) - 18*r1*(A - c3*r3)*(r1 + r2)*(r2 + r3))*(B*r1*r2*(r1 + r2)*(r1 + r2 + r3) - 2*c3*r1*r2*r3*(A - c3*r3)*(r1 + r2)*(r1 + r2 + r3) + r1*(A - c3*r3)**2*(r1 + r2)**2*(r2 + r3))/(2*c3**3*r1**2*(r2 + r3)**2*(r1 + r2 + r3)**3) + (c3*r1*r2*r3*(r1 + r2 + r3) - 2*r1*(A - c3*r3)*(r1 + r2)*(r2 + r3))**3/(c3**3*r1**3*(r2 + r3)**3*(r1 + r2 + r3)**3))**(1/3)/3 - (c3*r1*r2*r3*(r1 + r2 + r3) - 2*r1*(A - c3*r3)*(r1 + r2)*(r2 + r3))/(3*c3*r1*(r2 + r3)*(r1 + r2 + r3)))))

1. select r1,r2,r3,c3,w1,w2,q (-r3/(r1+r2) makes k)
2. calc A,B,C from w1,w2,q
3. calc r4,c1,c2 from A,B,C,r1,r2,r3,c3
4. fit r4,c1,c2 into Exx
5. check resulting w1,w2,q,k
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




def calc_c1c2r4_from_w1w2q_r1r2r3c3(w1,w2,q,r1,r2,r3,c3):

    A = 1.0/w1 + 1.0/(q*w2)       
    B = 1.0/(w2*w2) + 1.0/(q*w1*w2) 
    C = 1.0/(w1*w2*w2)          
    
#    print('A={}'.format(A))
#    print('B={}'.format(B))
#    print('C={}'.format(C))

    try:
        r4 = (-(-3*B*r1*r2*(r1 + r2)*(r1 + r2 + r3) + 6*c3*r1*r2*r3*(A - c3*r3)*(r1 + r2)*(r1 + r2 + r3) - 3*r1*(A - c3*r3)**2*(r1 + r2)**2*(r2 + r3))/(c3**2*r1*(r2 + r3)*(r1 + r2 + r3)**2) - (c3*r1*r2*r3*(r1 + r2 + r3) - 2*r1*(A - c3*r3)*(r1 + r2)*(r2 + r3))**2/(c3**2*r1**2*(r2 + r3)**2*(r1 + r2 + r3)**2))/(3*(math.sqrt(-4*((-3*B*r1*r2*(r1 + r2)*(r1 + r2 + r3) + 6*c3*r1*r2*r3*(A - c3*r3)*(r1 + r2)*(r1 + r2 + r3) - 3*r1*(A - c3*r3)**2*(r1 + r2)**2*(r2 + r3))/(c3**2*r1*(r2 + r3)*(r1 + r2 + r3)**2) + (c3*r1*r2*r3*(r1 + r2 + r3) - 2*r1*(A - c3*r3)*(r1 + r2)*(r2 + r3))**2/(c3**2*r1**2*(r2 + r3)**2*(r1 + r2 + r3)**2))**3 + ((-27*B*r1*r2*(A - c3*r3)*(r1 + r2)**2/c3 + 27*C*r1*r2*(r1 + r2)**2/c3 + 27*r1*r2*r3*(A - c3*r3)**2*(r1 + r2)**2)/(c3**2*r1*(r2 + r3)*(r1 + r2 + r3)**2) - (9*c3*r1*r2*r3*(r1 + r2 + r3) - 18*r1*(A - c3*r3)*(r1 + r2)*(r2 + r3))*(B*r1*r2*(r1 + r2)*(r1 + r2 + r3) - 2*c3*r1*r2*r3*(A - c3*r3)*(r1 + r2)*(r1 + r2 + r3) + r1*(A - c3*r3)**2*(r1 + r2)**2*(r2 + r3))/(c3**3*r1**2*(r2 + r3)**2*(r1 + r2 + r3)**3) + 2*(c3*r1*r2*r3*(r1 + r2 + r3) - 2*r1*(A - c3*r3)*(r1 + r2)*(r2 + r3))**3/(c3**3*r1**3*(r2 + r3)**3*(r1 + r2 + r3)**3))**2)/2 + (-27*B*r1*r2*(A - c3*r3)*(r1 + r2)**2/c3 + 27*C*r1*r2*(r1 + r2)**2/c3 + 27*r1*r2*r3*(A - c3*r3)**2*(r1 + r2)**2)/(2*c3**2*r1*(r2 + r3)*(r1 + r2 + r3)**2) - (9*c3*r1*r2*r3*(r1 + r2 + r3) - 18*r1*(A - c3*r3)*(r1 + r2)*(r2 + r3))*(B*r1*r2*(r1 + r2)*(r1 + r2 + r3) - 2*c3*r1*r2*r3*(A - c3*r3)*(r1 + r2)*(r1 + r2 + r3) + r1*(A - c3*r3)**2*(r1 + r2)**2*(r2 + r3))/(2*c3**3*r1**2*(r2 + r3)**2*(r1 + r2 + r3)**3) + (c3*r1*r2*r3*(r1 + r2 + r3) - 2*r1*(A - c3*r3)*(r1 + r2)*(r2 + r3))**3/(c3**3*r1**3*(r2 + r3)**3*(r1 + r2 + r3)**3))**(1/3)) - (math.sqrt(-4*((-3*B*r1*r2*(r1 + r2)*(r1 + r2 + r3) + 6*c3*r1*r2*r3*(A - c3*r3)*(r1 + r2)*(r1 + r2 + r3) - 3*r1*(A - c3*r3)**2*(r1 + r2)**2*(r2 + r3))/(c3**2*r1*(r2 + r3)*(r1 + r2 + r3)**2) + (c3*r1*r2*r3*(r1 + r2 + r3) - 2*r1*(A - c3*r3)*(r1 + r2)*(r2 + r3))**2/(c3**2*r1**2*(r2 + r3)**2*(r1 + r2 + r3)**2))**3 + ((-27*B*r1*r2*(A - c3*r3)*(r1 + r2)**2/c3 + 27*C*r1*r2*(r1 + r2)**2/c3 + 27*r1*r2*r3*(A - c3*r3)**2*(r1 + r2)**2)/(c3**2*r1*(r2 + r3)*(r1 + r2 + r3)**2) - (9*c3*r1*r2*r3*(r1 + r2 + r3) - 18*r1*(A - c3*r3)*(r1 + r2)*(r2 + r3))*(B*r1*r2*(r1 + r2)*(r1 + r2 + r3) - 2*c3*r1*r2*r3*(A - c3*r3)*(r1 + r2)*(r1 + r2 + r3) + r1*(A - c3*r3)**2*(r1 + r2)**2*(r2 + r3))/(c3**3*r1**2*(r2 + r3)**2*(r1 + r2 + r3)**3) + 2*(c3*r1*r2*r3*(r1 + r2 + r3) - 2*r1*(A - c3*r3)*(r1 + r2)*(r2 + r3))**3/(c3**3*r1**3*(r2 + r3)**3*(r1 + r2 + r3)**3))**2)/2 + (-27*B*r1*r2*(A - c3*r3)*(r1 + r2)**2/c3 + 27*C*r1*r2*(r1 + r2)**2/c3 + 27*r1*r2*r3*(A - c3*r3)**2*(r1 + r2)**2)/(2*c3**2*r1*(r2 + r3)*(r1 + r2 + r3)**2) - (9*c3*r1*r2*r3*(r1 + r2 + r3) - 18*r1*(A - c3*r3)*(r1 + r2)*(r2 + r3))*(B*r1*r2*(r1 + r2)*(r1 + r2 + r3) - 2*c3*r1*r2*r3*(A - c3*r3)*(r1 + r2)*(r1 + r2 + r3) + r1*(A - c3*r3)**2*(r1 + r2)**2*(r2 + r3))/(2*c3**3*r1**2*(r2 + r3)**2*(r1 + r2 + r3)**3) + (c3*r1*r2*r3*(r1 + r2 + r3) - 2*r1*(A - c3*r3)*(r1 + r2)*(r2 + r3))**3/(c3**3*r1**3*(r2 + r3)**3*(r1 + r2 + r3)**3))**(1/3)/3 - (c3*r1*r2*r3*(r1 + r2 + r3) - 2*r1*(A - c3*r3)*(r1 + r2)*(r2 + r3))/(3*c3*r1*(r2 + r3)*(r1 + r2 + r3))
        c1 = -c3*(r1 + r2 + r3)*((-(-3*B*r1*r2*(r1 + r2)*(r1 + r2 + r3) + 6*c3*r1*r2*r3*(A - c3*r3)*(r1 + r2)*(r1 + r2 + r3) - 3*r1*(A - c3*r3)**2*(r1 + r2)**2*(r2 + r3))/(c3**2*r1*(r2 + r3)*(r1 + r2 + r3)**2) - (c3*r1*r2*r3*(r1 + r2 + r3) - 2*r1*(A - c3*r3)*(r1 + r2)*(r2 + r3))**2/(c3**2*r1**2*(r2 + r3)**2*(r1 + r2 + r3)**2))/(3*(math.sqrt(-4*((-3*B*r1*r2*(r1 + r2)*(r1 + r2 + r3) + 6*c3*r1*r2*r3*(A - c3*r3)*(r1 + r2)*(r1 + r2 + r3) - 3*r1*(A - c3*r3)**2*(r1 + r2)**2*(r2 + r3))/(c3**2*r1*(r2 + r3)*(r1 + r2 + r3)**2) + (c3*r1*r2*r3*(r1 + r2 + r3) - 2*r1*(A - c3*r3)*(r1 + r2)*(r2 + r3))**2/(c3**2*r1**2*(r2 + r3)**2*(r1 + r2 + r3)**2))**3 + ((-27*B*r1*r2*(A - c3*r3)*(r1 + r2)**2/c3 + 27*C*r1*r2*(r1 + r2)**2/c3 + 27*r1*r2*r3*(A - c3*r3)**2*(r1 + r2)**2)/(c3**2*r1*(r2 + r3)*(r1 + r2 + r3)**2) - (9*c3*r1*r2*r3*(r1 + r2 + r3) - 18*r1*(A - c3*r3)*(r1 + r2)*(r2 + r3))*(B*r1*r2*(r1 + r2)*(r1 + r2 + r3) - 2*c3*r1*r2*r3*(A - c3*r3)*(r1 + r2)*(r1 + r2 + r3) + r1*(A - c3*r3)**2*(r1 + r2)**2*(r2 + r3))/(c3**3*r1**2*(r2 + r3)**2*(r1 + r2 + r3)**3) + 2*(c3*r1*r2*r3*(r1 + r2 + r3) - 2*r1*(A - c3*r3)*(r1 + r2)*(r2 + r3))**3/(c3**3*r1**3*(r2 + r3)**3*(r1 + r2 + r3)**3))**2)/2 + (-27*B*r1*r2*(A - c3*r3)*(r1 + r2)**2/c3 + 27*C*r1*r2*(r1 + r2)**2/c3 + 27*r1*r2*r3*(A - c3*r3)**2*(r1 + r2)**2)/(2*c3**2*r1*(r2 + r3)*(r1 + r2 + r3)**2) - (9*c3*r1*r2*r3*(r1 + r2 + r3) - 18*r1*(A - c3*r3)*(r1 + r2)*(r2 + r3))*(B*r1*r2*(r1 + r2)*(r1 + r2 + r3) - 2*c3*r1*r2*r3*(A - c3*r3)*(r1 + r2)*(r1 + r2 + r3) + r1*(A - c3*r3)**2*(r1 + r2)**2*(r2 + r3))/(2*c3**3*r1**2*(r2 + r3)**2*(r1 + r2 + r3)**3) + (c3*r1*r2*r3*(r1 + r2 + r3) - 2*r1*(A - c3*r3)*(r1 + r2)*(r2 + r3))**3/(c3**3*r1**3*(r2 + r3)**3*(r1 + r2 + r3)**3))**(1/3)) - (math.sqrt(-4*((-3*B*r1*r2*(r1 + r2)*(r1 + r2 + r3) + 6*c3*r1*r2*r3*(A - c3*r3)*(r1 + r2)*(r1 + r2 + r3) - 3*r1*(A - c3*r3)**2*(r1 + r2)**2*(r2 + r3))/(c3**2*r1*(r2 + r3)*(r1 + r2 + r3)**2) + (c3*r1*r2*r3*(r1 + r2 + r3) - 2*r1*(A - c3*r3)*(r1 + r2)*(r2 + r3))**2/(c3**2*r1**2*(r2 + r3)**2*(r1 + r2 + r3)**2))**3 + ((-27*B*r1*r2*(A - c3*r3)*(r1 + r2)**2/c3 + 27*C*r1*r2*(r1 + r2)**2/c3 + 27*r1*r2*r3*(A - c3*r3)**2*(r1 + r2)**2)/(c3**2*r1*(r2 + r3)*(r1 + r2 + r3)**2) - (9*c3*r1*r2*r3*(r1 + r2 + r3) - 18*r1*(A - c3*r3)*(r1 + r2)*(r2 + r3))*(B*r1*r2*(r1 + r2)*(r1 + r2 + r3) - 2*c3*r1*r2*r3*(A - c3*r3)*(r1 + r2)*(r1 + r2 + r3) + r1*(A - c3*r3)**2*(r1 + r2)**2*(r2 + r3))/(c3**3*r1**2*(r2 + r3)**2*(r1 + r2 + r3)**3) + 2*(c3*r1*r2*r3*(r1 + r2 + r3) - 2*r1*(A - c3*r3)*(r1 + r2)*(r2 + r3))**3/(c3**3*r1**3*(r2 + r3)**3*(r1 + r2 + r3)**3))**2)/2 + (-27*B*r1*r2*(A - c3*r3)*(r1 + r2)**2/c3 + 27*C*r1*r2*(r1 + r2)**2/c3 + 27*r1*r2*r3*(A - c3*r3)**2*(r1 + r2)**2)/(2*c3**2*r1*(r2 + r3)*(r1 + r2 + r3)**2) - (9*c3*r1*r2*r3*(r1 + r2 + r3) - 18*r1*(A - c3*r3)*(r1 + r2)*(r2 + r3))*(B*r1*r2*(r1 + r2)*(r1 + r2 + r3) - 2*c3*r1*r2*r3*(A - c3*r3)*(r1 + r2)*(r1 + r2 + r3) + r1*(A - c3*r3)**2*(r1 + r2)**2*(r2 + r3))/(2*c3**3*r1**2*(r2 + r3)**2*(r1 + r2 + r3)**3) + (c3*r1*r2*r3*(r1 + r2 + r3) - 2*r1*(A - c3*r3)*(r1 + r2)*(r2 + r3))**3/(c3**3*r1**3*(r2 + r3)**3*(r1 + r2 + r3)**3))**(1/3)/3 - (c3*r1*r2*r3*(r1 + r2 + r3) - 2*r1*(A - c3*r3)*(r1 + r2)*(r2 + r3))/(3*c3*r1*(r2 + r3)*(r1 + r2 + r3)))/(r1*r2) + (A - c3*r3)*(r1 + r2)/(r1*r2)
        c2 = C*(r1 + r2)/(c3*r3*(-c3*(r1 + r2 + r3)*((-(-3*B*r1*r2*(r1 + r2)*(r1 + r2 + r3) + 6*c3*r1*r2*r3*(A - c3*r3)*(r1 + r2)*(r1 + r2 + r3) - 3*r1*(A - c3*r3)**2*(r1 + r2)**2*(r2 + r3))/(c3**2*r1*(r2 + r3)*(r1 + r2 + r3)**2) - (c3*r1*r2*r3*(r1 + r2 + r3) - 2*r1*(A - c3*r3)*(r1 + r2)*(r2 + r3))**2/(c3**2*r1**2*(r2 + r3)**2*(r1 + r2 + r3)**2))/(3*(math.sqrt(-4*((-3*B*r1*r2*(r1 + r2)*(r1 + r2 + r3) + 6*c3*r1*r2*r3*(A - c3*r3)*(r1 + r2)*(r1 + r2 + r3) - 3*r1*(A - c3*r3)**2*(r1 + r2)**2*(r2 + r3))/(c3**2*r1*(r2 + r3)*(r1 + r2 + r3)**2) + (c3*r1*r2*r3*(r1 + r2 + r3) - 2*r1*(A - c3*r3)*(r1 + r2)*(r2 + r3))**2/(c3**2*r1**2*(r2 + r3)**2*(r1 + r2 + r3)**2))**3 + ((-27*B*r1*r2*(A - c3*r3)*(r1 + r2)**2/c3 + 27*C*r1*r2*(r1 + r2)**2/c3 + 27*r1*r2*r3*(A - c3*r3)**2*(r1 + r2)**2)/(c3**2*r1*(r2 + r3)*(r1 + r2 + r3)**2) - (9*c3*r1*r2*r3*(r1 + r2 + r3) - 18*r1*(A - c3*r3)*(r1 + r2)*(r2 + r3))*(B*r1*r2*(r1 + r2)*(r1 + r2 + r3) - 2*c3*r1*r2*r3*(A - c3*r3)*(r1 + r2)*(r1 + r2 + r3) + r1*(A - c3*r3)**2*(r1 + r2)**2*(r2 + r3))/(c3**3*r1**2*(r2 + r3)**2*(r1 + r2 + r3)**3) + 2*(c3*r1*r2*r3*(r1 + r2 + r3) - 2*r1*(A - c3*r3)*(r1 + r2)*(r2 + r3))**3/(c3**3*r1**3*(r2 + r3)**3*(r1 + r2 + r3)**3))**2)/2 + (-27*B*r1*r2*(A - c3*r3)*(r1 + r2)**2/c3 + 27*C*r1*r2*(r1 + r2)**2/c3 + 27*r1*r2*r3*(A - c3*r3)**2*(r1 + r2)**2)/(2*c3**2*r1*(r2 + r3)*(r1 + r2 + r3)**2) - (9*c3*r1*r2*r3*(r1 + r2 + r3) - 18*r1*(A - c3*r3)*(r1 + r2)*(r2 + r3))*(B*r1*r2*(r1 + r2)*(r1 + r2 + r3) - 2*c3*r1*r2*r3*(A - c3*r3)*(r1 + r2)*(r1 + r2 + r3) + r1*(A - c3*r3)**2*(r1 + r2)**2*(r2 + r3))/(2*c3**3*r1**2*(r2 + r3)**2*(r1 + r2 + r3)**3) + (c3*r1*r2*r3*(r1 + r2 + r3) - 2*r1*(A - c3*r3)*(r1 + r2)*(r2 + r3))**3/(c3**3*r1**3*(r2 + r3)**3*(r1 + r2 + r3)**3))**(1/3)) - (math.sqrt(-4*((-3*B*r1*r2*(r1 + r2)*(r1 + r2 + r3) + 6*c3*r1*r2*r3*(A - c3*r3)*(r1 + r2)*(r1 + r2 + r3) - 3*r1*(A - c3*r3)**2*(r1 + r2)**2*(r2 + r3))/(c3**2*r1*(r2 + r3)*(r1 + r2 + r3)**2) + (c3*r1*r2*r3*(r1 + r2 + r3) - 2*r1*(A - c3*r3)*(r1 + r2)*(r2 + r3))**2/(c3**2*r1**2*(r2 + r3)**2*(r1 + r2 + r3)**2))**3 + ((-27*B*r1*r2*(A - c3*r3)*(r1 + r2)**2/c3 + 27*C*r1*r2*(r1 + r2)**2/c3 + 27*r1*r2*r3*(A - c3*r3)**2*(r1 + r2)**2)/(c3**2*r1*(r2 + r3)*(r1 + r2 + r3)**2) - (9*c3*r1*r2*r3*(r1 + r2 + r3) - 18*r1*(A - c3*r3)*(r1 + r2)*(r2 + r3))*(B*r1*r2*(r1 + r2)*(r1 + r2 + r3) - 2*c3*r1*r2*r3*(A - c3*r3)*(r1 + r2)*(r1 + r2 + r3) + r1*(A - c3*r3)**2*(r1 + r2)**2*(r2 + r3))/(c3**3*r1**2*(r2 + r3)**2*(r1 + r2 + r3)**3) + 2*(c3*r1*r2*r3*(r1 + r2 + r3) - 2*r1*(A - c3*r3)*(r1 + r2)*(r2 + r3))**3/(c3**3*r1**3*(r2 + r3)**3*(r1 + r2 + r3)**3))**2)/2 + (-27*B*r1*r2*(A - c3*r3)*(r1 + r2)**2/c3 + 27*C*r1*r2*(r1 + r2)**2/c3 + 27*r1*r2*r3*(A - c3*r3)**2*(r1 + r2)**2)/(2*c3**2*r1*(r2 + r3)*(r1 + r2 + r3)**2) - (9*c3*r1*r2*r3*(r1 + r2 + r3) - 18*r1*(A - c3*r3)*(r1 + r2)*(r2 + r3))*(B*r1*r2*(r1 + r2)*(r1 + r2 + r3) - 2*c3*r1*r2*r3*(A - c3*r3)*(r1 + r2)*(r1 + r2 + r3) + r1*(A - c3*r3)**2*(r1 + r2)**2*(r2 + r3))/(2*c3**3*r1**2*(r2 + r3)**2*(r1 + r2 + r3)**3) + (c3*r1*r2*r3*(r1 + r2 + r3) - 2*r1*(A - c3*r3)*(r1 + r2)*(r2 + r3))**3/(c3**3*r1**3*(r2 + r3)**3*(r1 + r2 + r3)**3))**(1/3)/3 - (c3*r1*r2*r3*(r1 + r2 + r3) - 2*r1*(A - c3*r3)*(r1 + r2)*(r2 + r3))/(3*c3*r1*(r2 + r3)*(r1 + r2 + r3)))**2 + (A - c3*r3)*(r1 + r2)*((-(-3*B*r1*r2*(r1 + r2)*(r1 + r2 + r3) + 6*c3*r1*r2*r3*(A - c3*r3)*(r1 + r2)*(r1 + r2 + r3) - 3*r1*(A - c3*r3)**2*(r1 + r2)**2*(r2 + r3))/(c3**2*r1*(r2 + r3)*(r1 + r2 + r3)**2) - (c3*r1*r2*r3*(r1 + r2 + r3) - 2*r1*(A - c3*r3)*(r1 + r2)*(r2 + r3))**2/(c3**2*r1**2*(r2 + r3)**2*(r1 + r2 + r3)**2))/(3*(math.sqrt(-4*((-3*B*r1*r2*(r1 + r2)*(r1 + r2 + r3) + 6*c3*r1*r2*r3*(A - c3*r3)*(r1 + r2)*(r1 + r2 + r3) - 3*r1*(A - c3*r3)**2*(r1 + r2)**2*(r2 + r3))/(c3**2*r1*(r2 + r3)*(r1 + r2 + r3)**2) + (c3*r1*r2*r3*(r1 + r2 + r3) - 2*r1*(A - c3*r3)*(r1 + r2)*(r2 + r3))**2/(c3**2*r1**2*(r2 + r3)**2*(r1 + r2 + r3)**2))**3 + ((-27*B*r1*r2*(A - c3*r3)*(r1 + r2)**2/c3 + 27*C*r1*r2*(r1 + r2)**2/c3 + 27*r1*r2*r3*(A - c3*r3)**2*(r1 + r2)**2)/(c3**2*r1*(r2 + r3)*(r1 + r2 + r3)**2) - (9*c3*r1*r2*r3*(r1 + r2 + r3) - 18*r1*(A - c3*r3)*(r1 + r2)*(r2 + r3))*(B*r1*r2*(r1 + r2)*(r1 + r2 + r3) - 2*c3*r1*r2*r3*(A - c3*r3)*(r1 + r2)*(r1 + r2 + r3) + r1*(A - c3*r3)**2*(r1 + r2)**2*(r2 + r3))/(c3**3*r1**2*(r2 + r3)**2*(r1 + r2 + r3)**3) + 2*(c3*r1*r2*r3*(r1 + r2 + r3) - 2*r1*(A - c3*r3)*(r1 + r2)*(r2 + r3))**3/(c3**3*r1**3*(r2 + r3)**3*(r1 + r2 + r3)**3))**2)/2 + (-27*B*r1*r2*(A - c3*r3)*(r1 + r2)**2/c3 + 27*C*r1*r2*(r1 + r2)**2/c3 + 27*r1*r2*r3*(A - c3*r3)**2*(r1 + r2)**2)/(2*c3**2*r1*(r2 + r3)*(r1 + r2 + r3)**2) - (9*c3*r1*r2*r3*(r1 + r2 + r3) - 18*r1*(A - c3*r3)*(r1 + r2)*(r2 + r3))*(B*r1*r2*(r1 + r2)*(r1 + r2 + r3) - 2*c3*r1*r2*r3*(A - c3*r3)*(r1 + r2)*(r1 + r2 + r3) + r1*(A - c3*r3)**2*(r1 + r2)**2*(r2 + r3))/(2*c3**3*r1**2*(r2 + r3)**2*(r1 + r2 + r3)**3) + (c3*r1*r2*r3*(r1 + r2 + r3) - 2*r1*(A - c3*r3)*(r1 + r2)*(r2 + r3))**3/(c3**3*r1**3*(r2 + r3)**3*(r1 + r2 + r3)**3))**(1/3)) - (math.sqrt(-4*((-3*B*r1*r2*(r1 + r2)*(r1 + r2 + r3) + 6*c3*r1*r2*r3*(A - c3*r3)*(r1 + r2)*(r1 + r2 + r3) - 3*r1*(A - c3*r3)**2*(r1 + r2)**2*(r2 + r3))/(c3**2*r1*(r2 + r3)*(r1 + r2 + r3)**2) + (c3*r1*r2*r3*(r1 + r2 + r3) - 2*r1*(A - c3*r3)*(r1 + r2)*(r2 + r3))**2/(c3**2*r1**2*(r2 + r3)**2*(r1 + r2 + r3)**2))**3 + ((-27*B*r1*r2*(A - c3*r3)*(r1 + r2)**2/c3 + 27*C*r1*r2*(r1 + r2)**2/c3 + 27*r1*r2*r3*(A - c3*r3)**2*(r1 + r2)**2)/(c3**2*r1*(r2 + r3)*(r1 + r2 + r3)**2) - (9*c3*r1*r2*r3*(r1 + r2 + r3) - 18*r1*(A - c3*r3)*(r1 + r2)*(r2 + r3))*(B*r1*r2*(r1 + r2)*(r1 + r2 + r3) - 2*c3*r1*r2*r3*(A - c3*r3)*(r1 + r2)*(r1 + r2 + r3) + r1*(A - c3*r3)**2*(r1 + r2)**2*(r2 + r3))/(c3**3*r1**2*(r2 + r3)**2*(r1 + r2 + r3)**3) + 2*(c3*r1*r2*r3*(r1 + r2 + r3) - 2*r1*(A - c3*r3)*(r1 + r2)*(r2 + r3))**3/(c3**3*r1**3*(r2 + r3)**3*(r1 + r2 + r3)**3))**2)/2 + (-27*B*r1*r2*(A - c3*r3)*(r1 + r2)**2/c3 + 27*C*r1*r2*(r1 + r2)**2/c3 + 27*r1*r2*r3*(A - c3*r3)**2*(r1 + r2)**2)/(2*c3**2*r1*(r2 + r3)*(r1 + r2 + r3)**2) - (9*c3*r1*r2*r3*(r1 + r2 + r3) - 18*r1*(A - c3*r3)*(r1 + r2)*(r2 + r3))*(B*r1*r2*(r1 + r2)*(r1 + r2 + r3) - 2*c3*r1*r2*r3*(A - c3*r3)*(r1 + r2)*(r1 + r2 + r3) + r1*(A - c3*r3)**2*(r1 + r2)**2*(r2 + r3))/(2*c3**3*r1**2*(r2 + r3)**2*(r1 + r2 + r3)**3) + (c3*r1*r2*r3*(r1 + r2 + r3) - 2*r1*(A - c3*r3)*(r1 + r2)*(r2 + r3))**3/(c3**3*r1**3*(r2 + r3)**3*(r1 + r2 + r3)**3))**(1/3)/3 - (c3*r1*r2*r3*(r1 + r2 + r3) - 2*r1*(A - c3*r3)*(r1 + r2)*(r2 + r3))/(3*c3*r1*(r2 + r3)*(r1 + r2 + r3)))))
    except ValueError:
        return (-1,-1,-1)

    return (c1,c2,r4)

def remove_imaginary_or_error(x):

    if abs(x.real)>1e10*abs(x.imag) and x.real>0:
        return x.real

    return -1


def calc_mfb3_parameters_from_values(r1,r2,r3,r4,c1,c2,c3):

    A = (c1*r1*r2 + c3*r1*r3 + c3*r1*r4 + c3*r2*r3 + c3*r2*r4 + c3*r3*r4)/(r1 + r2)
    B = c3*(c1*r1*r2*r3 + c1*r1*r2*r4 + c1*r1*r3*r4 + c2*r1*r3*r4 + c2*r2*r3*r4)/(r1 + r2)
    C = c1*c2*c3*r1*r2*r3*r4/(r1 + r2)

    k = -r3/(r1+r2)
    
    w1 = (2**(1/3)*A*C - 2**(1/3)*B**2/3 + B*(9*A*B*C - 2*B**3 - 27*C**2 + 3*cmath.sqrt(3)*cmath.sqrt(C**2*(4*A**3*C - A**2*B**2 - 18*A*B*C + 4*B**3 + 27*C**2)))**(1/3)/3 - (18*A*B*C - 4*B**3 - 54*C**2 + 6*cmath.sqrt(3)*cmath.sqrt(C**2*(4*A**3*C - A**2*B**2 - 18*A*B*C + 4*B**3 + 27*C**2)))**(2/3)/6)/(C*(9*A*B*C - 2*B**3 - 27*C**2 + 3*cmath.sqrt(3)*cmath.sqrt(C**2*(4*A**3*C - A**2*B**2 - 18*A*B*C + 4*B**3 + 27*C**2)))**(1/3))
    w2 = 2**(1/6)*cmath.sqrt(3)/cmath.sqrt((3*2**(2/3)*A*C - 2**(2/3)*B**2 + (2**(1/3)*B - (9*A*B*C - 2*B**3 - 27*C**2 + 3*cmath.sqrt(3)*cmath.sqrt(C**2*(4*A**3*C - A**2*B**2 - 18*A*B*C + 4*B**3 + 27*C**2)))**(1/3))*(9*A*B*C - 2*B**3 - 27*C**2 + 3*cmath.sqrt(3)*cmath.sqrt(C**2*(4*A**3*C - A**2*B**2 - 18*A*B*C + 4*B**3 + 27*C**2)))**(1/3))/(9*A*B*C - 2*B**3 - 27*C**2 + 3*cmath.sqrt(3)*cmath.sqrt(C**2*(4*A**3*C - A**2*B**2 - 18*A*B*C + 4*B**3 + 27*C**2)))**(1/3))
    q  = 2**(5/6)*cmath.sqrt(3)*cmath.sqrt((3*2**(2/3)*A*C - 2**(2/3)*B**2 + (2**(1/3)*B - (9*A*B*C - 2*B**3 - 27*C**2 + 3*cmath.sqrt(3)*cmath.sqrt(C**2*(4*A**3*C - A**2*B**2 - 18*A*B*C + 4*B**3 + 27*C**2)))**(1/3))*(9*A*B*C - 2*B**3 - 27*C**2 + 3*cmath.sqrt(3)*cmath.sqrt(C**2*(4*A**3*C - A**2*B**2 - 18*A*B*C + 4*B**3 + 27*C**2)))**(1/3))/(9*A*B*C - 2*B**3 - 27*C**2 + 3*cmath.sqrt(3)*cmath.sqrt(C**2*(4*A**3*C - A**2*B**2 - 18*A*B*C + 4*B**3 + 27*C**2)))**(1/3))*(3*2**(2/3)*A*C - 2**(2/3)*B**2 + (2**(1/3)*B - (9*A*B*C - 2*B**3 - 27*C**2 + 3*cmath.sqrt(3)*cmath.sqrt(C**2*(4*A**3*C - A**2*B**2 - 18*A*B*C + 4*B**3 + 27*C**2)))**(1/3))*(9*A*B*C - 2*B**3 - 27*C**2 + 3*cmath.sqrt(3)*cmath.sqrt(C**2*(4*A**3*C - A**2*B**2 - 18*A*B*C + 4*B**3 + 27*C**2)))**(1/3))/(6*(A*(3*2**(2/3)*A*C - 2**(2/3)*B**2 + (2**(1/3)*B - (9*A*B*C - 2*B**3 - 27*C**2 + 3*cmath.sqrt(3)*cmath.sqrt(C**2*(4*A**3*C - A**2*B**2 - 18*A*B*C + 4*B**3 + 27*C**2)))**(1/3))*(9*A*B*C - 2*B**3 - 27*C**2 + 3*cmath.sqrt(3)*cmath.sqrt(C**2*(4*A**3*C - A**2*B**2 - 18*A*B*C + 4*B**3 + 27*C**2)))**(1/3)) - 3*2**(1/3)*C*(9*A*B*C - 2*B**3 - 27*C**2 + 3*cmath.sqrt(3)*cmath.sqrt(C**2*(4*A**3*C - A**2*B**2 - 18*A*B*C + 4*B**3 + 27*C**2)))**(1/3)))

    w1 = remove_imaginary_or_error(w1)
    w2 = remove_imaginary_or_error(w2)
    q  = remove_imaginary_or_error(q)

    return (w1,w2,q,k)


def randomize_mfb3(r1,r2,r3,r4,c1,c2,c3, c_precision=0.05, r_precision=0.01, iters=1000):

    (w1_0, w2_0, q_0, k_0) = calc_mfb3_parameters_from_values(r1,r2,r3,r4,c1,c2,c3)
 
    d_w1 = 0.0
    d_w2 = 0.0
    d_q  = 0.0
    d_k  = 0.0

    for i in range(iters):
        dr1 = random.uniform(1.0-r_precision,1.0+r_precision)
        dr2 = random.uniform(1.0-r_precision,1.0+r_precision)
        dr3 = random.uniform(1.0-r_precision,1.0+r_precision)
        dr4 = random.uniform(1.0-r_precision,1.0+r_precision)

        dc1 = random.uniform(1.0-c_precision,1.0+c_precision)
        dc2 = random.uniform(1.0-c_precision,1.0+c_precision)
        dc3 = random.uniform(1.0-c_precision,1.0+c_precision)

        rr1 = r1 * dr1
        rr2 = r2 * dr2
        rr3 = r3 * dr3
        rr4 = r4 * dr4

        rc1 = c1 * dc1
        rc2 = c2 * dc2
        rc3 = c3 * dc3

        (w1,w2,q,k) = calc_mfb3_parameters_from_values(rr1,rr2,rr3,rr4,rc1,rc2,rc3)

        d_w1 = max(d_w1, abs(w1-w1_0))
        d_w2 = max(d_w2, abs(w2-w2_0))
        d_q  = max(d_q,  abs( q- q_0))
        d_k  = max(d_k,  abs( k- k_0))

    return (d_w1/w1_0, d_w2/w2_0, d_q/q_0, d_k/k_0)



def calc_filter( w1_set=2*math.pi*30000, w2_set=2*math.pi*30000, q_set=1.0, k_set=(-0.4), r1_range=[1.0,20.0], r1_exp=1e3, r2_range=[1.0,20.0], r2_exp=1e3, r_pvalues='e24', c3_range=[56.0,4700.0], c3_exp=1e-12, c_pvalues='e12'):

    c3_values = select_values_in_range( c3_range, c3_exp, pvalues=c_pvalues )

    r1_values = select_values_in_range( [x for x in r1_range], r1_exp, pvalues=r_pvalues )
    r2_values = select_values_in_range( [x for x in r2_range], r2_exp, pvalues=r_pvalues )

    print( 'c3 values for sweep: {}'.format([x*y for (x,y) in c3_values]) )
    print( 'r1 values for sweep: {}'.format([x*y for (x,y) in r1_values]) )
    print( 'r2 values for sweep: {}'.format([x*y for (x,y) in r2_values]) )

    print( '\ncalculating filter for: f1={}, f2={}, q={}, k={}'.format(w1_set/(2*math.pi),w2_set/(2*math.pi),q_set,k_set) )


    f_values=[]

    for r1 in [x*y for (x,y) in r1_values]:
        for r2 in [x*y for (x,y) in r2_values]:
            for c3 in [x*y for (x,y) in c3_values]:

                r3_ideal = (-1) * k_set * (r1+r2)
                if r3_ideal<=0.0:
                    continue

                r3 = select_one_nearest(r3_ideal, r_pvalues)

                print('\ntry c3={:1.1e}, r1={:.1f}, r2={:.1f}:'.format(c3,r1,r2))

                (c1_ideal,c2_ideal,r4_ideal) = calc_c1c2r4_from_w1w2q_r1r2r3c3(w1_set,w2_set,q_set,r1,r2,r3,c3)
                if r4_ideal<=0.0 or c1_ideal<=0.0 or c2_ideal<=0.0:
                    print(' FAILURE!')
                    continue

 

                print(' ideal values: r3={:.1f}, r4={:.1f}, c1={:1.4e}, c2={:1.4e}'.format(r3, r4_ideal, c1_ideal, c2_ideal))
 
                r4 = select_one_nearest(r4_ideal, r_pvalues)
                c1 = select_one_nearest(c1_ideal, c_pvalues)
                c2 = select_one_nearest(c2_ideal, c_pvalues)
                print(' nearest values: r3={:.1f}, r4={:.1f}, c1={:1.4e}, c2={:1.4e}'.format(r3, r4, c1, c2))
 
                (w1_real,w2_real,q_real,k_real) = calc_mfb3_parameters_from_values(r1,r2,r3,r4,c1,c2,c3)
                if w1_real<0 or w2_real<0 or q_real<0:
                    print(' FAILURE!')
                    continue
 
                print(' real parameters: f1={:.1f}, f2={:.1f}, q={:.4f}, k={:.4f}'.format(w1_real/(2*math.pi),w2_real/(2*math.pi),q_real,k_real) )
 
                f_values += [ {'r1':r1,'r2':r2,'r3':r3,'r4':r4, 'c1':c1,'c2':c2,'c3':c3, 'w1':w1_real,'w2':w2_real,'q':q_real,'k':k_real} ]


    
    #print(f_values)

    # sort values by cutoff freq match
    print('\n\n\n sorted by neares cutoff freqs:')
    for e in  sorted(f_values, key = lambda tup: (tup['w1']-w1_set)**2/(w1_set*w1_set) + (tup['w2']-w2_set)**2/(w2_set*w2_set) ):
        print('  r1={:.1f}, r2={:.1f}, r3={:.1f}, r4={:.1f}, c1={:1.1e}, c2={:1.1e}, c3={:1.1e}, f1={:.1f}, f2={:.1f}, q={:.4f}, k={:.4f}'.format(e['r1'],e['r2'],e['r3'],e['r4'],e['c1'],e['c2'],e['c3'],e['w1']/(2*math.pi),e['w2']/(2*math.pi),e['q'],e['k']) )
        (p_w1, p_w2, p_q, p_k) = randomize_mfb3(e['r1'],e['r2'],e['r3'],e['r4'], e['c1'],e['c2'],e['c3'])
        e['p_w1']=p_w1
        e['p_w2']=p_w2
        e['p_q'] =p_q
        e['p_k'] =p_k

        print('   percent_f1={:.2f}%, percent_f2={:.2f}%, percent_q={:.2f}%, percent_k={:.2f}%'.format(100*e['p_w1'],100*e['p_w2'],100*e['p_q'],100*e['p_k']))

    # sort values by q match
    print('\n\n\n sorted by q:')
    for e in  sorted(f_values, key = lambda tup: abs((tup['q']-q_set)/q_set) ):
        print('  r1={:.1f}, r2={:.1f}, r3={:.1f}, r4={:.1f}, c1={:1.1e}, c2={:1.1e}, c3={:1.1e}, f1={:.1f}, f2={:.1f}, q={:.4f}, k={:.4f}'.format(e['r1'],e['r2'],e['r3'],e['r4'],e['c1'],e['c2'],e['c3'],e['w1']/(2*math.pi),e['w2']/(2*math.pi),e['q'],e['k']) )
        print('   percent_f1={:.2f}%, percent_f2={:.2f}%, percent_q={:.2f}%, percent_k={:.2f}%'.format(100*e['p_w1'],100*e['p_w2'],100*e['p_q'],100*e['p_k']))



def main():

    calc_filter()

    #print(" #### AY ####")
    #calc_filter( wc_set=2*math.pi*30000, q_set=1.0, k_set=(-1.75), r1_range=[20.0,30.0], r1_exp=1e3, r1_div=3.0, r_pvalues='e24', c1_range=[33.0,4700.0], c1_exp=1e-12, c_pvalues='e12')

#    print(" #### beep ####")
#    calc_filter( wc_set=2*math.pi*30000, q_set=1.0, k_set=(-0.388), r1_range=[4.7,10.0], r1_exp=1e3, r1_div=1.0, r1_add=750, r_pvalues='e24', c1_range=[33.0,4700.0], c1_exp=1e-12, c_pvalues='e12')


if __name__=="__main__":
    main()




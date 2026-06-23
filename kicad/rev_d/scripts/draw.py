#!/usr/bin/env python

import sys
import math
import cmath
import matplotlib.pyplot as plt
import numpy as np



def draww(q=0.994):

	lo_range=0.1
	hi_range=10.0
	steps = 1001

	lo_log = math.log10(lo_range)
	hi_log = math.log10(hi_range)

	v_log = lo_log
	v_step = (hi_log-lo_log)/steps


	vs=[]
	hh1=[]
	hh2=[]
	hh=[]

	while(v_log<=hi_log):

		v = math.pow(10,v_log)

		h1 = 1.0 / (1 - v*v + (1j/q)*v)
		h2 = 1.0 / (1 + 1j*v)

		vs += [v]
		hh1 += [h1]
		hh2 += [h2]
		hh  += [h1*h2]

		v_log = v_log + v_step


	#print(values)

	x = np.array(vs)
	yy1 = np.array(np.abs(hh1))
	yy2 = np.array(np.abs(hh2))
	yy  = np.array(np.abs(hh ))

	# plot
	fig, (ax,ap) = plt.subplots(2,1)
	
	ax.loglog(x, yy1, linewidth=2.0, color='red')
	ax.loglog(x, yy2, linewidth=2.0, color='green')
	ax.loglog(x, yy , linewidth=2.0, color='blue')

	ax.grid(which="minor", color="0.9")
	ax.grid(which="major", color="0.1")


	ap.semilogx(x,np.angle(hh)*180.0/math.pi)
	ap.grid(which="minor", color="0.9")
	ap.grid(which="major", color="0.1")

	


	plt.show()









def main():

	draww()




if __name__=="__main__":
    main()




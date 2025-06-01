#!/usr/bin/env python


e24=[1.0, 1.1, 1.2, 1.3, 1.5, 1.6, 1.8, 2.0, 2.2, 2.4, 2.7, 3.0, 3.3, 3.6, 3.9, 4.3, 4.7, 5.1, 5.6, 6.2, 6.8, 7.5, 8.2, 9.1];


def calc_rdiv(vo=None, vup=None, vdn=None, rup=None, rdn=None):

	args = {'vo':vo, 'vup':vup, 'vdn':vdn, 'rup':rup, 'rdn':rdn}


	# only 2 'None' args otherwise failure
	# only ints and floats, otherwise failure
	none_nums = 0
	for a in args:
		a_arg = args[a]

		if a_arg==None:
			none_nums += 1
		else:
			if not isinstance(a_arg,float):
				assert isinstance(a_arg,int)

				args[a] = float(a_arg)
				continue
	assert none_nums==2;

	# there might be a conversion from ints to floats, so reload args
	vo =  args['vo']
	vup = args['vup']
	vdn = args['vdn']
	rup = args['rup']
	rdn = args['rdn']

	# there should be at least one resistance value given, otherwise no way to select both
	assert rdn!=None or rup!=None;


	result=args

	# when both resistances are known
	if rup!=None and rdn!=None:

		if vo!=None:
			i = vo/(rup+rdn)
			result['vup'] = i*rup;
			result['vdn'] = i*rdn;
			return result
		elif vup!=None:
			i = vup/rup
			vdn = i*rdn
			vo = vup+vdn
			result['vdn']=vdn
			result['vo'] =vo
			return result
		elif vdn!=None:
			i = vdn/rdn
			vup = i*rup
			vo = vup+vdn
			result['vup']=vup
			result['vo'] =vo
			return result
	# any r unknown
	elif rup==None or rdn==None:
		# restore voltages
		if vo==None:
			vo=vdn+vup
			result['vo']=vo
		elif vup==None:
			vup=vo-vdn;
			result['vup']=vup
		elif vdn==None:
			vdn=vo-vup
			result['vdn']=vdn

		# recalc resistor values
		if rup==None:
			i=vdn/rdn
			rup = vup/i
			result['rup']=rup
			return result
		elif rdn==None:
			i=vup/rup
			rdn = vdn/i
			result['rdn']=rdn
			return result

	# we shouldn't be here
	assert False


def select_nearest(r):

	global e24;


	exp=1.0;

	# select range
	if r < min(e24):
		while r < min(e24):
			if r < max(e24)/10.0:
				r = r*10.0;
				exp = exp * 0.1;
			else:
				break;
	elif r > max(e24):
		while r > max(e24):
			if r > min(e24)*10.0:
				r = r/10.0;
				exp = exp * 10.0;
			else:
				break;


	# select nearest values
	e24_a = [0.91]+e24+[10.0];
	for i in range(len(e24_a)-1):
		rdn = e24_a[i];
		rup = e24_a[i+1];
		if rdn<=r and r<=rup:
			break;

	return (rdn*exp, rup*exp);

def select_lo(r):
	x = select_nearest(r);
	return x[0];

def select_hi(r):
	x = select_nearest(r);
	return x[1];

def r_fix(r,r_e24):
	# select parallel or series additional resistor for r to be closest to r_e24
	if r>=r_e24: # series
		r_exact = r-r_e24
		(rlo,rhi)=select_nearest(r_exact)
		if abs(r_exact-rlo)<abs(r_exact-rhi):
			return (r_e24+rlo,rlo,'series')
		else:
			return (r_e24+rhi,rhi,'series')

	else: # parallel (r<r_e24)
		r_exact = 1.0/(1.0/r - 1.0/r_e24)
		(rlo,rhi)=select_nearest(r_exact)
		r_rlo = 1.0/(1.0/r_e24+1.0/rlo)
		r_rhi = 1.0/(1.0/r_e24+1.0/rhi)
		if abs(r_rlo-r)<abs(r_rhi-r):
			return (r_rlo,rlo,'parallel')
		else:
			return (r_rhi,rhi,'parallel')



def main():
	
	global e24;

	vref=2.5;
	vo=6.0;

	print("\n>>> :: Vref={}, Vout={}\n".format(vref,vo))

	variants=[]

	for rdn in e24:
		r = calc_rdiv(vo=vo, vdn=vref, rdn=rdn)
		rup = r['rup']	

		rup_lo = select_lo(rup);
		rup_hi = select_hi(rup);

		print("rdn_e24={}, rup_exact={}".format(rdn,rup));

		r=calc_rdiv(vdn=vref,rdn=rdn,rup=rup_hi)
		vo_hi = r['vo']
		print("\trup_hi={}, vo={}".format(rup_hi,vo_hi))
		variants += [(rdn,rup_hi,vo_hi)]

		fix=r_fix(rup,rup_hi)
		r=calc_rdiv(vdn=vref,rdn=rdn,rup=fix[0])
		print("\t\trup {} {} {}, vo={}".format(fix[1],fix[2],rup_hi,r['vo']))
		variants += [(rdn,'{} {} {}'.format(fix[1],fix[2],rup_hi),r['vo'])]

		r=calc_rdiv(vdn=vref,rdn=rdn,rup=rup_lo)
		vo_lo = r['vo']
		print("\trup_lo={}, vo={}".format(rup_lo,vo_lo))
		variants += [(rdn,rup_lo,vo_lo)]

		fix=r_fix(rup,rup_lo)
		r=calc_rdiv(vdn=vref,rdn=rdn,rup=fix[0])
		print("\t\trup {} {} {}, vo={}".format(fix[1],fix[2],rup_lo,r['vo']))
		variants += [(rdn,'{} {} {}'.format(fix[1],fix[2],rup_lo),r['vo'])]



	# sort by best approximaation
	variants.sort(key = lambda tup: abs(tup[2]-vo))

	for i in variants:
		print(i)


if __name__=="__main__":
	main()


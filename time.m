
g[l_,t_]:=Block[{k,q},
	q=l;
	For[k=0,k<t,k++,q=(q*q+2.0*q+1.0)/4.0];q]

f[t_]:=g[0.0,t]


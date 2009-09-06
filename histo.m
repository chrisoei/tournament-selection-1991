(* histogram maker *)

hi[l_,b_]:=Block[{min,max,c,j},
	min=Min[l];
	max=Max[l];
	c[_]:=0;
	c[Floor[0.99999*(#-min)/(  (max-min)/b )]]++ & /@ l;
	Table[{min+(j+1/2) (max-min)/b,c[j]},{j,0,b-1}]
]

	

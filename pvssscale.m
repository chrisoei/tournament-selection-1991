
(* Gives the renormalized population vs. niche ranking function F:

pop(NumOfNiches,Niche)=F(Niche/NumOfNiches)/NumOfNiches
*)

op[ww_] := op1[ww]/Integrate[op1[ww], {x, 0, 1}]
 
op1[ww_] := op1[ww] = (ww /. x -> x/2)*Integrate[ww /. x -> y, {y, x/2, 1}]

(* start with 3/2 (1-(1-x^2)) as a seed.  apply op several times to get

F(x)=

In[95]:= Nest[Chop[ExpandAll[op[#]]]&,%,20]

                             3             5               7
Out[95]= 2.4674 x - 1.01468 x  + 0.125181 x  - 0.00735408 x  + 
 
                 9             -6  11             -8  13             -9  15
>    0.00025202 x  - 5.65305 10   x   + 8.94124 10   x   - 1.05055 10   x

*)



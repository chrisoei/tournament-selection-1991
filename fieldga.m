(* Analysis of the mean field equations for tournament section and *)
(* niching in GA's *)


norm[l_]:=l/(Plus@@l)


dpart[l_]:= {l}/; Length[l]==1

dpart[l_]:= Append[dpart[Rest[l]],l]

f[l_]:=First[l] ^2 + 2 First[l] Plus @@ Rest[l]

z[l_]:= Sort[f/@ dpart[Sort[norm[l]]]]


<<histo.m
<<fieldga.m

dat=Rest[NestList[z,Table[Random[],{i,50}],1000]]

f=First/@dat

g=hi[f,10]

gr=ListPlot[g,PlotJoined->True,PlotLabel->"Fluctuations at 50"]

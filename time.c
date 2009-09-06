
# define	TMAX	10000

double g(l,t)
	double l;
	long int t;
{
	while (t>0) {
		l=(l*l+2.0*l+1.0)/4.0;
		t--;
	}
	return(l);
}

double q(t)
	long int t;
{
	return(g(0.0,t)-g(0.0,t-1));
}

double sm(w)
	long int w;
{
	long int k;
	double su;

	su=0;
	for (k=1;k<=w;k++)
		su+=k*q(k);
	return(su);
}

void main()
{
	long int p;

	for (p=1;p<TMAX;p++)
		printf("%lf\n",sm(p));
}



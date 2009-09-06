# include <stdio.h>
# include <math.h>

/* test how tournament selection is incompatible with sharing because of */
/* the feedback noise */

/* NumOfNiche*NumInNiche must be even */

# define	NumOfNiche	400
# define	NumInNiche	50
# define	MaxGen		500
# define	Seed		13223
# define	Shuffles	(10*PoolSize)
# define	FName		"tourn5.t"

# define	PoolSize	(NumOfNiche*NumInNiche)

double ranf()
{
	return(random()/2147483647.0);
}

void initpool(p,in,of)
	int *p,in,of;
{
	int i,j;

	for (i=0;i<of;i++)
		for (j=0;j<in;j++)
			p[i*in+j]=i;
}

void tally(p,n,of,ps)
	int *p, *n, of, ps;
{
	int i;

	for (i=0;i<of;i++) n[i]=0;
	for (i=0;i<ps;i++) n[p[i]]++;
}
	
void shuffle(p,ps)
	int *p,ps;
{
	int i,j,k,l;

	for (i=0;i<Shuffles;i++) {
		j=ps*ranf();
		k=ps*ranf();
		l=p[j]; p[j]=p[k]; p[k]=l;
	}
}

void tournament(pold,pnew,nc,of,ps)
	int *pold, *pnew, *nc, of, ps;
{
	int i, *pn;

	pn=pnew;
	shuffle(pold,ps);
	for (i=0;i<ps;i+=2)
		*(pn++)=nc[pold[i]]>nc[pold[i+1]]?pold[i+1]:pold[i];
	shuffle(pold,ps);
	for (i=0;i<ps;i+=2)
		*(pn++)=nc[pold[i]]>nc[pold[i+1]]?pold[i+1]:pold[i];
	tally(pnew,nc,of,ps);
}	


void prtally(n,of)
	int *n,of;
{
	int i;

	for (i=0;i<of;i++)
		printf("%d ",n[i]);
	printf("\n");
}

void survive(in,of)
	int of,in;
{
	int nichecount[NumOfNiche], pool[PoolSize], newpool[PoolSize], t, i,j;
	int ps;
	FILE *fp;

	ps=of*in;
	srandom(Seed);
	initpool(pool,in,of);
	tally(pool,nichecount,of,ps);

	for (t=1;t<MaxGen;t++) {
		tournament(pool,newpool,nichecount,of,ps);
		for (i=0;i<PoolSize;i++) pool[i]=newpool[i];
		for (j=i=0;i<of;i++)
			nichecount[i] ? j++ : j;
		if (!(fp=fopen(FName,"a"))) {
			printf("Can't open %s\n",FName);
			exit(1);
		}
		fprintf(fp,"%d %d\n",t,j);
		fclose(fp);
	}
}

void main()
{
	int of,in;


	of=NumOfNiche;
	in=NumInNiche;
	survive(in,of);
}


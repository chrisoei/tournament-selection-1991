# include <stdio.h>
# include <math.h>

/* test how tournament selection is incompatible with sharing because of */
/* the feedback noise */

/* NumOfNiche*NumInNiche must be even */

# define	NumOfNiche	50
# define	NumInNiche	50
# define	MaxGen		50
# define	Seed		13223
# define	Shuffles	(10*PoolSize)

# define	PoolSize	(NumOfNiche*NumInNiche)

double ranf()
{
	return(random()/2147483647.0);
}

void initpool(p)
	int *p;
{
	int i,j;

	for (i=0;i<NumOfNiche;i++)
		for (j=0;j<NumInNiche;j++)
			p[i*NumInNiche+j]=i;
}

void tally(p,n)
	int *p, *n;
{
	int i;

	for (i=0;i<NumOfNiche;i++) n[i]=0;
	for (i=0;i<PoolSize;i++) n[p[i]]++;
}
	
void shuffle(p)
	int *p;
{
	int i,j,k,l;

	for (i=0;i<Shuffles;i++) {
		j=PoolSize*ranf();
		k=PoolSize*ranf();
		l=p[j]; p[j]=p[k]; p[k]=l;
	}
}

void tournament(pold,pnew,nc)
	int *pold, *pnew, *nc;
{
	int i, *pn;

	pn=pnew;
	shuffle(pold);
	for (i=0;i<PoolSize;i+=2)
		*(pn++)=nc[pold[i]]>nc[pold[i+1]]?pold[i+1]:pold[i];
	shuffle(pold);
	for (i=0;i<PoolSize;i+=2)
		*(pn++)=nc[pold[i]]>nc[pold[i+1]]?pold[i+1]:pold[i];
	tally(pnew,nc);
}	


void prtally(n)
	int *n;
{
	int i;

	for (i=0;i<NumOfNiche;i++)
		printf("%d ",n[i]);
	printf("\n");
}

void main()

{
	int nichecount[NumOfNiche], pool[PoolSize], newpool[PoolSize], t, i;

	srandom(Seed);
	initpool(pool);
	tally(pool,nichecount);

	for (t=1;t<MaxGen;t++) {
		tournament(pool,newpool,nichecount);
		for (i=0;i<PoolSize;i++) pool[i]=newpool[i];
		prtally(nichecount);
	}
}



/* myniche.c */
/* created 7/20/91 by Christopher K. Oei (coei@complex.ccsr.uiuc.edu */
/* last modified 7/29/91 by Christopher K. Oei */

/* This program tests a new kind of niching mechanism that is compatible */
/* tournament selection. */

# include <stdio.h>
# include <X11/Intrinsic.h>
# include <X11/StringDefs.h> /* needed for XtNheight, etc */
# include <X11/Shell.h> /* needed for topLevelShellClass */
# include <Xm/Xm.h>
# include <Xm/Text.h>
# include <Xm/PushB.h>
# include <Xm/ToggleB.h>
# include <Xm/Label.h>
# include <Xm/BulletinB.h>


# define	CLASSNAME	"Myniche"
# define	MAXARGS		20
# define	PARMS		7
# define	MAXSTRLEN	20
# define	MAXPOOL		9000
# define	MAXLONG		2147483647.0
# define	MAXNICHE	1000
# define	SHUFFLE		10

void quitCB();
void helpCB();
void showCB();
void editCB();
void killhelpCB();
void debugCB();
void controlCB();
void monitorCB();

double ranf();
void shuffle();
# define	SHUFFLE		10
void tourney();
void run();
void displaygen();
void main();

static caddr_t parms[PARMS];
static char *scan_string[PARMS]={"%d","%d","%d","%d","%d","%d","%s"};
static char *helptext;
static int debugflag=0;
static int controlflag=0;
static int monitorflag=0;

static int pool[MAXPOOL],pool2[MAXPOOL];

void main(argc,argv)
	unsigned int argc;
	char **argv;
{
	Widget app_label;
	Widget toplevel,b_board,quit_button,help_button,show_button,
		debug_toggle,control_toggle,monitor_toggle;
	Widget parm_labels[PARMS],parm_edits[PARMS];
	static int parmno[PARMS];
	static char parm_names[PARMS][MAXSTRLEN]={
		"nichesize",
		"numin",
		"numof",
		"numgen",
		"numsample",
		"seed",
		"outfile"};
	static char temp[MAXSTRLEN];
	static Arg wargs[MAXARGS];
	static int spaces[PARMS-1];
	static char outfile[MAXSTRLEN];
	int acount,j;

	for (j=0;j<PARMS-1;j++) parms[j]=(caddr_t)&spaces[j];
	parms[PARMS-1]=(caddr_t)outfile;

	toplevel=XtInitialize(argv[0],CLASSNAME,NULL,0,&argc,argv);

	b_board=XtCreateManagedWidget("board",
		xmBulletinBoardWidgetClass,toplevel,NULL,0);
	
	help_button=XtCreateManagedWidget("help",xmPushButtonWidgetClass,
		b_board,NULL,0);
	show_button=XtCreateManagedWidget("show",xmPushButtonWidgetClass,
		b_board,NULL,0);
	quit_button=XtCreateManagedWidget("quit",xmPushButtonWidgetClass,
		b_board,NULL,0);

	for (j=0;j<PARMS;j++) {
		strcpy(temp,&parm_names[j][0]);
		strcat(temp,"edit");
		parm_edits[j]=XtCreateManagedWidget(temp,
			xmTextWidgetClass,b_board,NULL,0);
		strcpy(temp,&parm_names[j][0]);
		strcat(temp,"label");
		parm_labels[j]=XtCreateManagedWidget(temp,
			xmLabelWidgetClass,b_board,NULL,0);
		parmno[j]=j;
		XtAddCallback(parm_edits[j],XmNvalueChangedCallback,editCB,
			&parmno[j]);
	}

	app_label=XtCreateManagedWidget("app",xmLabelWidgetClass,
		b_board,NULL,0);
	debug_toggle=XtCreateManagedWidget("debugtoggle",
		xmToggleButtonWidgetClass,b_board,NULL,0);
	control_toggle=XtCreateManagedWidget("controltoggle",
		xmToggleButtonWidgetClass,b_board,NULL,0);
	monitor_toggle=XtCreateManagedWidget("monitortoggle",
		xmToggleButtonWidgetClass,b_board,NULL,0);
	XtAddCallback(help_button,XmNactivateCallback,helpCB,&toplevel);
	XtAddCallback(quit_button,XmNactivateCallback,quitCB,NULL);
	XtAddCallback(show_button,XmNactivateCallback,showCB,NULL);
	XtAddCallback(debug_toggle,XmNvalueChangedCallback,debugCB,NULL);
	XtAddCallback(control_toggle,XmNvalueChangedCallback,controlCB,NULL);
	XtAddCallback(monitor_toggle,XmNvalueChangedCallback,monitorCB,NULL);

	XtRealizeWidget(toplevel);
	XtMainLoop();
}

void quitCB(w,client_data,call_data)
	Widget w;
	caddr_t client_data;
	XmAnyCallbackStruct *call_data;
{
	if (call_data->reason==XmCR_ACTIVATE) {
		XtCloseDisplay(XtDisplay(w));
		exit(0);
	}
}

void editCB(w,client_data,call_data)	
	Widget w;
	int *client_data;
	XmAnyCallbackStruct *call_data;
{
	sscanf(XmTextGetString(w),scan_string[*client_data],
			parms[*client_data]);
}

void showCB(w,client_data,call_data)
	Widget w;
	caddr_t client_data;
	XmAnyCallbackStruct *call_data;
{
	int j;
	FILE *fp;
	static char *p[PARMS]={
		"Niche Size: ",
		"# in Niche: ",
		"# of Niches: ",
		"# of Generations: ",
		"# of Samples: ",
		"Random Number Seed: ",
		"Output File: "};

	if (!(fp=fopen(parms[PARMS-1],"w"))) {
		printf("Can't open output file.\n");
		exit(1);
	}
	for (j=0;j<PARMS-1;j++) {
		printf("%s%d\n",p[j],*(int*)parms[j]);
		fprintf(fp,"%s%d\n",p[j],*(int*)parms[j]);
	}
	printf("%s%s\n",p[PARMS-1],(char*)parms[PARMS-1]);
	fprintf(fp,"%s%s\n\n",p[PARMS-1],(char*)parms[PARMS-1]);
	printf("Control Flag: %d\n",controlflag);
	fprintf(fp,"Control Flag: %d\n",controlflag);
	printf("Monitor Flag: %d\n",monitorflag);
	fprintf(fp,"Monitor flag: %d\n",monitorflag);
	run(fp);

	fclose(fp);
}

void helpCB(w,client_data,call_data)
	Widget w;
	Widget *client_data;
	XmAnyCallbackStruct *call_data;
{
	static Widget helpw=NULL,helph;
	Arg args[MAXARGS];
	int ac;

	helpw=XtCreatePopupShell("helpw",topLevelShellWidgetClass,
		*client_data,NULL,0);
	helptext=XmStringCreateLtoR("Sorry, no help available.",
		XmSTRING_DEFAULT_CHARSET);
	ac=0;
	XtSetArg(args[ac],XmNlabelType,XmSTRING); ac++;
	XtSetArg(args[ac],XmNlabelString,helptext); ac++;
	XtSetArg(args[ac],XmNwidth,250); ac++;
	XtSetArg(args[ac],XmNheight,150); ac++;
	helph=XtCreateManagedWidget("helph",xmPushButtonWidgetClass,
		helpw,args,ac);
	XtAddCallback(helph,XmNactivateCallback,killhelpCB,&helpw);
	XtPopup(helpw,XtGrabNone);
}

void killhelpCB(w,client_data,call_data)
	Widget w;
	Widget *client_data;
	XmAnyCallbackStruct *call_data;
{
	XtPopdown(*client_data);
	XtFree(helptext);
}

void debugCB(w,client_data,call_data)
	Widget w;
	caddr_t client_data;
	XmAnyCallbackStruct *call_data;
{
	debugflag=1-debugflag;
}

void controlCB(w,client_data,call_data)
	Widget w;
	caddr_t client_data;
	XmAnyCallbackStruct *call_data;
{
	controlflag=1-controlflag;
}

void monitorCB(w,client_data,call_data)
	Widget w;
	caddr_t client_data;
	XmAnyCallbackStruct *call_data;
{
	monitorflag=1-monitorflag;
}


void run(fp)
	FILE *fp;
{
	int t,j,k,numin,numof;
	static long int psize;

	numin=*(int*)parms[1];
	numof=*(int*)parms[2];
	psize=numin*numof;
	if (psize>=MAXPOOL) {
		printf("population of %d is too big (max=%d)\n",psize,MAXPOOL);
		exit(2);
	}
	
	printf("Initiating pool...\n");
	for (j=0;j<numof;j++)
	for (k=0;k<numin;k++)
		pool[j*numin+k]=j;

	srandom(*(int*)parms[5]);
	shuffle(psize);
	displaygen(fp,psize);
	for (t=0;t<*(int*)parms[3];t++) {
		tourney(psize);
		displaygen(fp,psize);
	}
}

double ranf()
{
	return((double)(random())/MAXLONG);
}

void shuffle(psize)
	long int psize;
{
	long int j,s,r;
	int temp;

	for (j=0;j<psize*SHUFFLE;j++) {
		r=(long int)(psize*ranf());
		s=(long int)(psize*ranf());
		temp=pool[r];
		pool[r]=pool[s];
		pool[s]=temp;
	}
}

void displaygen(fp,psize)
	FILE *fp;
	long int psize;
{
	static int nc[MAXNICHE],nonz;
	long int j;

	if (!debugflag) {
		for (j=0;j<*(int*)parms[2];j++)
			nc[j]=0;
		for (j=0;j<psize;j++)
			nc[pool[j]]++;
		for (nonz=j=0;j<*(int*)parms[2];j++) {
			if (monitorflag) {
				printf("%d ",nc[j]);
				fprintf(fp,"%d ",nc[j]);
			} else
			nc[j] ? nonz++: nonz;
		}
		if (!monitorflag) {
			printf("%d",nonz);
			fprintf(fp,"%d",nonz);
		}
		printf("\n");
		fprintf(fp,"\n");
	}
	else {
		for (j=0;j<psize;j++) {
			printf("%d ",pool[j]);
			fprintf(fp,"%d ",pool[j]);
		}
		printf("\n");
		fprintf(fp,"\n");
	}
}

void tourney(psize)
	long int psize;
{
	long int j;
	static int ss,mn;
	int cnt1,cnt2,t1,t2,tm;
	long int k;

	ss=*(int*)parms[4];
	mn=*(int*)parms[0];
	
	shuffle(psize);
	for (j=0;j<psize/2;j++) {
		t1=pool[2*j];
		t2=pool[2*j+1];
		cnt1=cnt2=0;
		for (k=1;k<=ss;k++) {
			if (j<k) break;
			tm=pool2[j-k];
			if (tm==t1) cnt1++;
			else if (tm==t2) cnt2++;
		}
		if ((!controlflag)||((cnt1<=mn)&&(cnt2<=mn)))
			pool2[j]= ranf()>0.5 ? t1:t2;
		else pool2[j]= cnt1<cnt2 ? t1:t2;
	}
	shuffle(psize);
	for (j=0;j<psize/2;j++) {
		t1=pool[2*j];
		t2=pool[2*j+1];
		cnt1=cnt2=0;
		for (k=1;k<=ss;k++) {
			if (j<k) break;
			tm=pool2[j-k];
			if (tm==t1) cnt1++;
			else if (tm==t2) cnt2++;
		}
		if ((!controlflag)||((cnt1<=mn)&&(cnt2<=mn)))
			pool2[j+psize/2]=ranf()>0.5 ? t1:t2;
		else pool2[j+psize/2]= cnt1<cnt2 ? t1:t2;
	}
	for (j=0;j<psize;j++) pool[j]=pool2[j];
}


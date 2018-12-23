/*lock.h*/

#ifndef _LOCK_H_
#define _LOCK_H_

#define LOCK

#ifndef NLOCKS
#define NLOCKS 		50  		/*No of locks if not defined*/	
#endif


#define LFREE 	1   	/*Lock is free*/
#define LUSED 	2	/*Lock is used*/

#define WRITE 	1	 
#define READ 	2
#define NONE 	3


struct lentry {

	int lstate;		// state of lock
	int ltype;		// Reader or Writer
	int lqhead;		// head of lock queue
	int lqtail;		// tail of lock queue
	int readcnt;		// no of readers holding lock
	int lheld_proc[50];   // process holding lock

};

extern struct lentry locks[];
extern int nextlock;

#define isbadlock(l)	(l<0 || l >=NLOCKS )

int get_hprio_wrt_proc(int );

#endif

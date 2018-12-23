#define EXPDISTSCHED 1
#define LINUXSCHED 2

int sched_class_;
void setschedclass (int sched_class) ;
int getschedclass() ;
int expdist_sched();
int linux_sched();
int max_goodness(int);
int get_exp_proc(int); 

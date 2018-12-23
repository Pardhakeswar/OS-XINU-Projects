/* pagereplace.c - pagereplace policies*/
#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <paging.h>
sc_queue *sc_q;
ag_q * age_q;

/*sc_q daat structures */


void sc_delete(sc_queue* q, node* del){
	if( q->front == q->rear){
		q->SC_curr = NULL;
		q->front = NULL;
		q->rear = NULL;
		return;
	}
	node* prev = q->front;
	while(prev->next != del){
		prev = prev->next;
	}
	prev->next = del->next;
	q->SC_curr = prev->next;
	
	if( del == q->rear){
		q->rear = prev;
	}
	return;
}

void sc_insert(sc_queue *q, int newval){

	node* temp = (node*) getmem(sizeof(node));
	temp->data = newval;

	if(q->front == NULL){
		q->front = temp;
		q->SC_curr = temp;
	}
	else
		q->rear->next = temp;

	q->rear = temp;
	q->rear->next = q->front;
}



int frm_replace_SC(){

	int pid, va_addr, pd_offset, pt_offset;
	int i=0;

	while(1)
	{
		i = sc_q->SC_curr->data;
		if(frm_tab[i].fr_type == FR_PAGE){

			pid = frm_tab[i].fr_pid;

                        va_addr = (frm_tab[i].fr_vpno)*NBPG;
                        int pd_offset = (va_addr >>22) & (0x000003ff);
                        int pt_offset = (va_addr & 0x003ff000) >> 12;

                        pd_t* pd = proctab[pid].pdbr + pd_offset*(sizeof(pd_t));
                        pt_t* pt = pd->pd_base*NBPG + pt_offset*(sizeof(pt_t));

			if(pt->pt_acc == 0){
				sc_delete(sc_q, sc_q->SC_curr);
				return i;	
			}
			else{
				pt->pt_acc = 0;
			}
		}
		sc_q->SC_curr = sc_q->SC_curr->next;
	}
}


/* aging data structures */


void aging_insert(ag_q * q, int frame_no, int age)
{
        aging_q * temp ;
        aging_q* newnode = (aging_q*) getmem(sizeof(aging_q));

	if(age > 255)
        	newnode->age = 255;
	else
		newnode->age =age;

	newnode->frame =frame_no; 
        newnode->next =NULL;

        if(q->head ==NULL){
                q->head =newnode;

        }
        else{

        	temp = q->head;
        	while(temp->next!= NULL){

                	temp = temp->next;
        	}
        	temp->next =newnode;
        }
}


void aging_delete(ag_q *q, int frame_no)
{
        if(q->head == NULL){
                kprintf(" queue is empty\n");
        }

        aging_q *temp =q->head;
        aging_q *prev;

        if(frame_no == q->head->frame){
                //temp =q->head;
                q->head= q->head->next;
        }
        else{
                while(temp->next != NULL && temp->frame != frame_no){
                                prev = temp;
                                temp = temp->next;
                }

		prev->next =temp->next;
        }

}

int find_min_age_frame(ag_q *q){

	aging_q *temp = q->head;
	int frame_age = 256;
	int frame_no =0;

	while(temp){

		if(temp->age < frame_age){

			frame_age = temp->age;
			frame_no =temp->frame;
			
		}
	temp =temp->next;
	}
	return frame_no;
}

int frm_replace_aging()
{	
	kprintf("inside frame replaace aging\n");	
	aging_q *temp =	age_q->head;

	while(temp){

		temp->age = (temp->age)/2;
		temp =temp->next;
	}
	
	int frame_min_age = find_min_age_frame(age_q);
        aging_delete(age_q, frame_min_age);
	
		
	return frame_min_age;
	
}

void print_aging(ag_q *q)
{
	kprintf("inside print aging \n");
	if(q->head == NULL)
		kprintf("queueu is empty" );
	aging_q *temp =q->head;
	while(temp){
		kprintf("%d ------",temp->frame);
		temp =temp->next;
	}
	
	kprintf("\n");
}

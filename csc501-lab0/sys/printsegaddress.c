#include<stdio.h>
//#include<stdlib.h>

extern int  etext, edata, end;
void printsegaddress(){

		kprintf("\n\nvoid printsegaddress()\n\n"
                "current : etext[0x%08x] =0x%08x, "
                "edata[0x%08x] = 0x%08x, "
                "ebss[0x%08x] =0x%08x"

                "\n Preceding :etext[0x%08x]=0x%08x, "    /* preceeding data*/
                "edata[0x%08x] = 0x%08x, "
                "ebss[0x%08x] =0x%08x "

                "\n After :etext[0x%08x]=0x%08x, "    /* After data */
                "edata[0x%08x] = 0x%08x, "
                "ebss[0x%08x] =0x%08x",

                &etext-1, *(&etext-1), &edata-1, *(&edata-1), &end-1, *(&end-1),
		&etext-2, *(&etext-2), &edata-2, *(&edata-2), &end-2, *(&end-2),
		&etext, *(&etext), &edata, *(&edata), &end, *(&end) );

}


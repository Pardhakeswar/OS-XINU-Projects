#include <math.h>
#include <stdio.h>



int RAND_MAX = 077777;


double pow(double x ,int y){

        int i=1;
        double k=x;
        for(i=1;i< y;i++)
        {
                x = x*k;
        }
        return x;
}


double log(double x){

        int i=1;
        double result=0;
        for(i=1;i<=20;i++)
        {
                result =result+(pow(-1,i+1))*(pow(x-1,i))/i;
        }
        return result;

}


double expdev(double lambda){

    double dummy;
    do
        dummy= (double) rand() /RAND_MAX;
    while (dummy == 0.0);
    return -log(1-dummy) / lambda;

}


/*
double pow(double x, int y){
        double result=1;
        while(y){
                result = result *x;
                y--;
        }
        return result;
}

double log(double x){
        int i;
        double sum =0;
        for(i=1;i<=20;i++){
                sum =sum + (pow(x-1,i)/i)*pow(-1,i+1);
        }
        return (sum);
}

double expdev(double lambda) {
    double dummy;
    do
        dummy= (double) rand() / RAND_MAX;
    while (dummy == 0.0);
    return -log(dummy) / lambda;
}
*/

#include <omp.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    
    //define pi
    double pi = 3.14159265358979323846;
    double t = omp_get_wtime();
    int num_threads = 4;
    omp_set_num_threads(num_threads);
    // let the user input the number of steps to be used in the porgram. Defaults to 10000
    long numsteps;
    if (argc ==1){
        numsteps  = 10000;
    } else {
        numsteps = atoi(argv[1]);
    }
    //reduce the numsteps to be a multiple of number of requested threads. Makes the parallelization easier.
    long numsteps_thread = floor((double)numsteps/ (double)num_threads);
 
    numsteps = numsteps_thread*num_threads;
    
    
    double stepsize = 1.0/ (double)numsteps;
    double totalsum = 0;
    //calculate the integral using trapezoid rule.
#pragma omp parallel
    {
        int ID = omp_get_thread_num();
        double stepsize_thread = stepsize;
        double sum_thread = 0;
        int incre = num_threads; long num_steps_thread = numsteps;
        int ii = 0;
        double x = 0;
        for(int i = ID; i <= num_steps_thread-1;i+=incre){
            x = (i+0.5)*stepsize_thread;
            sum_thread+= 1.0/(x*x+1.0);
            ii = i;
        }
        // now after all of the sum_threads are calculated we add them together. This must be synchronized which we do by the critical command. Could also be done by barrier construct.
        
#pragma omp critical
       // printf("sum_thread (%d) %g \n %d \n",ID,sum_thread,ii);
        totalsum += sum_thread;
        
    }
    
    totalsum = totalsum*4.0*stepsize;
    
    double error = pi - totalsum;
    
    printf("Error in Calculation is about: %g \n",error);
    t= omp_get_wtime() -t;
    printf("Time elapse: %f \n", t);
}
#include<stdio.h>
#include<math.h>
#define G 9.81
#define L 1
#define M 1
#define DT 0.001

double force(double theta, double massa)
{
    return G*sin(theta);
}
//VELOCITY-VERLET
double theta(double theta_i, double omega_i, double alpha_i, double (*f)(double)){

    return theta_i + omega_i*DT + 0.5*alpha_i*pow(DT,2);
}

double forza(double posizione){

    return -M*G*sin(posizione/L);
}

double alpha(double theta){
    return -G*sin(theta)/L;
}

double omega(double omega_i, double alpha_i, double alpha){

    return omega_i + 0.5*(alpha_i + alpha)*DT;
}

double alpha(){

    
}

int main(){



    return 0;
}
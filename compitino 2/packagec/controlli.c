#include "controlli.h"
#include<stdio.h>
#include <math.h>


int controllo_theta(float theta)
{
   if (theta<0 || theta>3.14159265358979323846/2)
   {
        printf("Valore di theta non accettabile, inserire un valore compreso tra 0 e pi/2");
        return 1;
   }
   return 0;

}

int controllo_omega(float g, float theta, float omega, float L)
{
    float omegaMax=sqrt(2 * g * cos(theta) / L);
    if(abs(omega) > omegaMax)
    {
        printf("Valore di omega non accettabile, il massimo valore accettabile per l'angolo che hai inserito e' %f", omegaMax);
        return 1;
    }
    return 0;
}



int controllo_step(float step)
{
    if(step>=3.40282346*pow(10,38))
    {
        printf("Numero step troppo elevato, overflow!");
        return 1;
    }
    return 0;
}
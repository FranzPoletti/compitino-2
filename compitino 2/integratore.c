#include "integratore.h"
#include<math.h>

void velocity_verlet(float *p, float theta_i, float omega_i, float alpha_i, float DT, float M, float L, float G, float (*f)(float, float, float))
{
    float theta_successivo = theta_i + omega_i * DT + 0.5 * alpha_i * pow(DT, 2);
    float alpha_successivo = f(theta_successivo, M, G) / M;
    float omega_successivo = omega_i + 0.5 * (alpha_i + alpha_successivo) * DT;
    p[2] = theta_successivo;
    p[3] = omega_successivo;
    p[4] = alpha_successivo;
}
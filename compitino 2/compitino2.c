#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

// #define G 9.81
// #define L 1
// #define M 1
// #define DT 0.001
#define RIGHE 18

struct parametri{

    char T[20];
    char dT[20];
    char Theta0[20];
    char omega0[20];
    char L[20];
    char M[20];
    char g[20];
    char ts[20];
    char out[50];

    float Tval;
    float dTval;
    float theta0val;
    float omega0val;
    float Lval;
    float Mval;
    float gval;
    int tsval;

};

typedef struct parametri par;


// VELOCITY-VERLET
void velocity_verlet(float *p, float theta_i, float omega_i, float alpha_i, float DT, float M, float L, float G, float (*f)(float, float, float))
{
    float theta_successivo = theta_i + omega_i * DT + 0.5 * alpha_i * pow(DT, 2);
    float alpha_successivo = f(theta_successivo, M, G) / M;
    float omega_successivo = omega_i + 0.5 * (alpha_i + alpha_successivo) * DT;
    p[2] = theta_successivo;
    p[3] = omega_successivo;
    p[4] = alpha_successivo;
}

float forza(float theta, float M, float G)
{

    return -M * G * sin(theta);
}

/*float alpha(float theta){
    return -G*sin(theta)/L;
}

float omega(float omega_i, float alpha_i, float alpha){

    return omega_i + 0.5*(alpha_i + alpha)*DT;
}*/

int main(int argc, char *argv[])
{

    float riga_output[5];
    par input;
     
    ///char nome_variabili[100];
    char *filename = "input.csv";

    FILE *in_file;
    in_file = fopen(filename, "r");
    
    char riga[16];

    char stringa[20];
    float valore;

    float parametri[8];
    char *fileoutput;

    for (int i = 0; i < RIGHE; i++)
    {
        fscanf(in_file, "%s %f", &stringa, &valore);
        printf("%s %f\n", stringa, valore);

        if (strcmp(stringa, "T") == 0)
        {
            input.T[0] = stringa[0];
            input.Tval = valore;
        }
        else if (strcmp(stringa, "dT") == 0)
        {
            input.dT[0] = stringa[0];
            input.dTval = valore;
        }
        else if (strcmp(stringa, "theta0") == 0)
        {
            input.Theta0[0] = stringa[0];
            input.theta0val = valore;
        }
        else if (strcmp(stringa, "omega0") == 0)
        {
            input.omega0[0] = stringa[0];
            input.omega0val = valore;
        }
        else if (strcmp(stringa, "L") == 0)
        {
            input.L[0] = stringa[0];
            input.Lval = valore;
        }
        else if (strcmp(stringa, "M") == 0)
        {
            input.M[0] = stringa[0];
            input.Mval = valore;
        }
        else if (strcmp(stringa, "g") == 0)
        {
            input.g[0] = stringa[0];
            input.gval = valore;
        }
        else if (strcmp(stringa, "ts") == 0)
        {
            input.ts[0] = stringa[0];
            input.tsval = valore;
        }
        else if (strcmp(stringa, "out_pendolo0_1.csv") == 0)
        {
            input.out[0] = stringa[0];
        }
    }

    FILE *out_file;
    out_file = fopen(input.out, "w");

    riga_output[0] = 0;                                                                                 // passo di integrazione
    riga_output[1] = 0;                                                                                 // istante di tempo
    riga_output[2] = input.theta0val;                                                                    // angolo
    riga_output[3] = input.omega0val;                                                                      // velocità angolare
    riga_output[4] = forza(riga_output[2], input.Mval, input.gval) / (input.Mval * input.Lval); // accelerazione angolare


    printf("\n%f\n", riga_output[4]);

    printf("\n Parametri: \n");
    for (int i = 0; i < 8; i++)
    {
        printf("%f \n", parametri[i]);
    }

    fprintf(out_file, "%s %s %f\n", "#HDR", "T", input.T);
    fprintf(out_file, "%s %s %f\n", "#HDR", "DT", input.dT);
    fprintf(out_file, "%s %s %f\n", "#HDR", "theta0", input.Theta0);
    fprintf(out_file, "%s %s %f\n", "#HDR", "omega0", input.omega0);
    fprintf(out_file, "%s %s %f\n", "#HDR", "L", input.L);
    fprintf(out_file, "%s %s %f\n", "#HDR", "M", input.M);
    fprintf(out_file, "%s %s %f\n", "#HDR", "g", input.g);
    fprintf(out_file, "%s %s %s %s %s\n", "#passo", "tempo", "theta", "w", "alpha");

    fprintf(out_file, "%f %f %f %f %f\n", riga_output[0], riga_output[1], riga_output[2], riga_output[3], riga_output[4]);



    for (int i = 1; i <= (int)input.Tval; i++)
    {
        riga_output[0] = i;
        riga_output[1] = i * input.dTval;
        velocity_verlet(riga_output, riga_output[2], riga_output[3], riga_output[4], input.dTval, input.Mval, input.Lval, input.gval, forza);

        if (i % input.tsval == 0)
        {
            printf("Passo: %d \n", i);
            fprintf(out_file, "%f %f %f %f %f\n", riga_output[0], riga_output[1], riga_output[2], riga_output[3], riga_output[4]);

            for (int j = 0; j < 5; j++)
            {

                printf("%.10f \n", riga_output[j]);
            }
        }
    }
    fclose(in_file);
    fclose(out_file);

    return 0;
}
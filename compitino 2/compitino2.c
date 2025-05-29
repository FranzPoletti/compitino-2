#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "integratore.h"

// #define G 9.81
// #define L 1
// #define M 1
// #define DT 0.001
#define RIGHE 20

struct parametri
{

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
/*void velocity_verlet(float *p, float theta_i, float omega_i, float alpha_i, float DT, float M, float L, float G, float (*f)(float, float, float))
{
    float theta_successivo = theta_i + omega_i * DT + 0.5 * alpha_i * pow(DT, 2);
    float alpha_successivo = f(theta_successivo, M, G) / M;
    float omega_successivo = omega_i + 0.5 * (alpha_i + alpha_successivo) * DT;
    p[2] = theta_successivo;
    p[3] = omega_successivo;
    p[4] = alpha_successivo;
}*/

float forza(float theta, float M, float G)
{
    return -M * G * sin(theta);
}

int main(int argc, char *argv[])
{

    par input;

    int control = 0;
    if (argc == 3)
    {
        input.theta0val = atof(argv[1]);
        input.omega0val = atof(argv[2]);
        control = 1;
        printf("Valori iniziali letti da riga di comando: theta0 = %f, omega0 = %f\n", input.theta0val, input.omega0val);
    }
    else
    {
        printf("Valori non letti dalla linea di comando, verranno ricavati dal file di input\n");
    }

    float riga_output[5];

    /// char nome_variabili[100];
    char *filename = "input.csv";

    FILE *in_file;
    in_file = fopen(filename, "r");

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
            strcpy(input.T, stringa);            
            input.Tval = valore;
        }
        else if (strcmp(stringa, "dT") == 0)
        {
            strcpy(input.dT, stringa);            
            input.dTval = valore;
        }
        else if (control == 0 && strcmp(stringa, "theta0") == 0 )
        {
            printf("ciaooo");
            strcpy(input.Theta0, stringa);            
            input.theta0val = valore;
        }
        else if (control == 0 && strcmp(stringa, "omega0") == 0)
        {
            strcpy(input.omega0, stringa);            
            input.omega0val = valore;
        }
        else if (strcmp(stringa, "L") == 0)
        {
            strcpy(input.L, stringa);            
            input.Lval = valore;
        }
        else if (strcmp(stringa, "M") == 0)
        {
            strcpy(input.M, stringa);            
            input.Mval = valore;
        }
        else if (strcmp(stringa, "g") == 0)
        {
            strcpy(input.g, stringa);            
            input.gval = valore;
        }
        else if (strcmp(stringa, "ts") == 0)
        {
            strcpy(input.ts, stringa);            
            input.tsval = valore;
        }
        else if (strcmp(stringa, "out_pendolo0_1.csv") == 0)
        {
            strcpy(input.out, stringa);            
            printf("%s", input.out);
        }
    }

    FILE *out_file;
    out_file = fopen(input.out, "w");

    riga_output[0] = 0;                                                                         // passo di integrazione
    riga_output[1] = 0;                                                                         // istante di tempo
    riga_output[2] = input.theta0val;                                                           // angolo
    riga_output[3] = input.omega0val;                                                           // velocità angolare
    riga_output[4] = forza(riga_output[2], input.Mval, input.gval) / (input.Mval * input.Lval); // accelerazione angolare

    fprintf(out_file, "%s %s %f\n", "#HDR", "T", input.Tval);
    fprintf(out_file, "%s %s %f\n", "#HDR", "DT", input.dTval);
    fprintf(out_file, "%s %s %f\n", "#HDR", "theta0", input.theta0val);
    fprintf(out_file, "%s %s %f\n", "#HDR", "omega0", input.omega0val);
    fprintf(out_file, "%s %s %f\n", "#HDR", "L", input.Lval);
    fprintf(out_file, "%s %s %f\n", "#HDR", "M", input.Mval);
    fprintf(out_file, "%s %s %f\n", "#HDR", "g", input.gval);
    fprintf(out_file, "%s %s %s %s %s\n", "#passo", "tempo", "theta", "w", "alpha");

    fprintf(out_file, "%f %f %f %f %f\n", riga_output[0], riga_output[1], riga_output[2], riga_output[3], riga_output[4]);

    for (int i = 1; i <= (int)input.Tval; i++)
    {
        riga_output[0] = i;
        riga_output[1] = i * input.dTval;
        velocity_verlet(riga_output, riga_output[2], riga_output[3], riga_output[4], input.dTval, input.Mval, input.Lval, input.gval, forza);

        if (i % input.tsval == 0)
        {
            //printf("Passo: %d \n", i);
            fprintf(out_file, "%f %f %f %f %f\n", riga_output[0], riga_output[1], riga_output[2], riga_output[3], riga_output[4]);

            /*for (int j = 0; j < 5; j++)
            {

                printf("%.10f \n", riga_output[j]);
            }*/
        }
    }
    fclose(in_file);
    fclose(out_file);

    return 0;
}
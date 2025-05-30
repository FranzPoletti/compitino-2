#include <stdio.h>
#include "packagec/integratore.h"
#include "packagec/controlli.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define RIGHE 30

//Contiene i parametri del sistema fisico che si vuole analizzare
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

//Calcola la forza che agisce sul pendolo
float forza(float theta, float M, float G)
{
    return -M * G * sin(theta);
}

int main(int argc, char *argv[])
{

    par input;
    //variabile di controllo per verificare se l'utente ha già inserito da terminale i valori di omega0 e theta0
    int control = 0;
    //controllo numero di parametri passati da terminale
    if (argc == 3)
    {
        //se presenti, vangono salvati
        input.theta0val = atof(argv[1]);
        input.omega0val = atof(argv[2]);
        control = 1;
        printf("Valori iniziali letti da riga di comando: theta0 = %f, omega0 = %f\n", input.theta0val, input.omega0val);
        printf("i valori soddisfano? inserire 0 se soddisfano, 1 se non soddisfano\n");
        //Variabile necessaria per conferma utente
        int conferma;
        scanf("%d", &conferma);
        if(conferma!=0)
        {
            return 1;
        }
    }
    //se non letti da terminale, procede con un messaggio per l'utente
    else
    {
        printf("Valori non letti dalla linea di comando, verranno ricavati dal file di input\n");
    }

    //si inizializza un vettore per salvare i valori da stampare in output
    float riga_output[5];

    //si apre il fiel di input 
    char *filename = "input.csv";

    FILE *in_file;
    in_file = fopen(filename, "r");

    //variabili in cui vengono provvisoriamente salvati i valori in input
    char stringa[30];
    char label[10];
    float valore;

    //si inizializza una varibaile per il file di output 
    char *fileoutput;


    //ciclo che scorre le righe del file preso in input
    for (int i = 0; i < RIGHE; i++)
    {
        //si legge una riga alla volta 
        fgets(stringa, 30, in_file);   
        //si salvano i valori necessari nelle variabili dichiarate in precedenza      
        sscanf(stringa, "%s %f", &label, &valore);

        //si eseguono confronti tra stringhe per il salvataggio nella corretta variabile della struct  
        if (strcmp(label, "T") == 0)
        {
            strcpy(input.T, label);            
            input.Tval = valore;
        }
        else if (strcmp(label, "dT") == 0)
        {
            strcpy(input.dT, label);            
            input.dTval = valore;
        }
        else if (control == 0 && strcmp(label, "theta0") == 0 )
        {
            strcpy(input.Theta0, label);            
            input.theta0val = valore;
        }
        else if (control == 0 && strcmp(label, "omega0") == 0)
        {
            strcpy(input.omega0, label);            
            input.omega0val = valore;
        }
        else if (strcmp(label, "L") == 0)
        {
            strcpy(input.L, label);            
            input.Lval = valore;
        }
        else if (strcmp(label, "M") == 0)
        {
            strcpy(input.M, label);            
            input.Mval = valore;
        }
        else if (strcmp(label, "g") == 0)
        {
            strcpy(input.g, label);            
            input.gval = valore;
        }
        else if (strcmp(label, "ts") == 0)
        {
            strcpy(input.ts, label);            
            input.tsval = valore;
        }
        else if (strcmp(label, "out_pendolo0_1.csv") == 0)
        {
            strcpy(input.out, label);            
            printf("%s", input.out);
        }
        if(strcmp(label, "#")>0)
        {
            continue;
        }
    }
    // si controllano i valori theta0, omega0 e T, eventualmente il programma viene chiuso oppure si manda un messaggio di conferma all'utente
    if(controllo_omega(input.gval, input.theta0val, input.omega0val, input.Lval) || controllo_theta(input.theta0val) || controllo_step(input.Tval))
    {
        return 1;
    }
    else
    {
        printf("\nParametri inseriti correttamente!");
    }

    //si crea file di output 
    FILE *out_file;
    out_file = fopen(input.out, "w");

    //si popola vettore per l'output della prima riga, tali valori vengono poi utilizzati per procedere nella simulazione 

    riga_output[0] = 0;                                                                         // passo di integrazione
    riga_output[1] = 0;                                                                         // istante di tempo
    riga_output[2] = input.theta0val;                                                           // angolo
    riga_output[3] = input.omega0val;                                                           // velocità angolare
    riga_output[4] = forza(riga_output[2], input.Mval, input.gval) / (input.Mval * input.Lval); // accelerazione angolare

    //si stampano gli header necessari
    fprintf(out_file, "%s %s %f\n", "#HDR", "T", input.Tval);
    fprintf(out_file, "%s %s %f\n", "#HDR", "DT", input.dTval);
    fprintf(out_file, "%s %s %f\n", "#HDR", "theta0", input.theta0val);
    fprintf(out_file, "%s %s %f\n", "#HDR", "omega0", input.omega0val);
    fprintf(out_file, "%s %s %f\n", "#HDR", "L", input.Lval);
    fprintf(out_file, "%s %s %f\n", "#HDR", "M", input.Mval);
    fprintf(out_file, "%s %s %f\n", "#HDR", "g", input.gval);
    fprintf(out_file, "%s   %s    %s      %s      %s\n", "#passo", "tempo", "theta", "w", "alpha");

    //si stampa la prima riga 
    fprintf(out_file, "%f %f %f %f %f\n", riga_output[0], riga_output[1], riga_output[2], riga_output[3], riga_output[4]);

    //per ogni istante si esegue l'algoritmo velocity-verlet e di volta in volta viene stampata una riga di output
    for (int i = 1; i <= (int)input.Tval; i++)
    {
        riga_output[0] = i;
        riga_output[1] = i * input.dTval;
        velocity_verlet(riga_output, riga_output[2], riga_output[3], riga_output[4], input.dTval, input.Mval, input.Lval, input.gval, forza);

        if (i % input.tsval == 0)
        {
            fprintf(out_file, "%f %f %f %f %f\n", riga_output[0], riga_output[1], riga_output[2], riga_output[3], riga_output[4]);
        }
    }

    //si chiudono entrambi i files 
    fclose(in_file);
    fclose(out_file);

    //il programma termina 
    return 0;
}
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

////////////////////////////////////////////////////////////////////////

struct atom
{
    char number[10];
    char type[10];
    char residue[10];
    char chain[10];   
    char residuenumber[10];
    char x[10];
    char y[10];
    char z[10];      
};

struct atom a[1000000];
struct atom b[1000000];

int make_AtomStruct(char* file, atom* protein);
int calc_DistanceAB();

////////////////////////////////////////////////////////////////////////

int main (int argc, char* argv[])
{
    make_AtomStruct(argv[1], a);
    make_AtomStruct(argv[2], b);
    calc_DistanceAB();
    return 0;
}

////////////////////////////////////////////////////////////////////////

int make_AtomStruct(char* file, atom* protein)
{
    FILE *pdb;
    pdb = fopen(file, "r");
    if(!pdb) return 0;

    char line[100];
    int i = 0;
    
    while (fgets(line, 100, pdb))
    {
	if(!strncmp(line, "ATOM", 4))
	{
	    sscanf(line, "%*s %s %s %s %s %s %s %s %s %*s %*s %*s",
		   protein[i].number, protein[i].type, protein[i].residue, protein[i].chain, protein[i].residuenumber, protein[i].x, protein[i].y, protein[i].z);
            
	    i++;
	}
    }
    
    fclose(pdb);
}

////////////////////////////////////////////////////////////////////////

int calc_DistanceAB()
{ 
    for (int i = 0; i < atoi(a[i].number); i++)
    {
	for (int j = 0; j < atoi(b[j].number); j++)
	{
	    float distance = sqrt(pow(atof(a[i].x)-atof(b[j].x), 2) + pow(atof(a[i].y)-atof(b[j].y), 2) + pow(atof(a[i].z)-atof(b[j].z), 2));
            
	    if(distance < 10)
            {                
                printf("INTERFACE: %s%s(%s%s) ::: %s%s(%s%s)\tDISTANCE: %.3f\n",
                       a[i].type, a[i].number, a[i].residue, a[i].residuenumber, b[j].type, b[j].number, b[j].residue, b[j].residuenumber, distance);  
            }  
        }
    }
}

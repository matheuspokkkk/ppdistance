#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

////////////////////////////////////////////////////////////////////////

struct atoms
{
    int number;
    char type[10];
    char residue[10];
    int residuenumber;
    double x;
    double y;
    double z;      
};
struct atoms protein[4][1000000];

int make_AtomStruct(atoms* wprotein, char* file);
int calc_DistanceABInterface(atoms* aprotein, atoms* bprotein, int maxdistanceInterface, int maxdistanceContacts);

////////////////////////////////////////////////////////////////////////

int main(int argc, char* argv[])
{
    for(int i = 0; i < 2; i++)
        make_AtomStruct(protein[i], argv[i+1]);

    for(int i = 0; i < 2; i+=2)
	calc_DistanceABInterface(protein[i], protein[i+1], atoi(argv[3]), atoi(argv[4]));
    
    return 0;
}

////////////////////////////////////////////////////////////////////////

int make_AtomStruct(atoms* wprotein, char* file)
{
    FILE *pdb;
    pdb = fopen(file, "r");
    if(!pdb) return 0;

    char line[100];
    int i = 0;
    
    while(fgets(line, 100, pdb))
    {
        if(!strncmp(line, "ATOM", 4))
	{
	    sscanf(line, "%*s %d %s %s %*s %d %lf %lf %lf %*s %*s %*s",
		   &wprotein[i].number,
		   wprotein[i].type,
		   wprotein[i].residue,
		   &wprotein[i].residuenumber,
		   &wprotein[i].x,
		   &wprotein[i].y,
		   &wprotein[i].z);
            
	    i++;
        }
    }
    
    fclose(pdb);
}

////////////////////////////////////////////////////////////////////////

int calc_DistanceABInterface(atoms* aprotein, atoms* bprotein, int maxdistanceInterface, int maxdistanceContacts)
{     
    float distance = 0;
    int i, j, k = 0;
    
    FILE *interface;
    interface = fopen("interface.in", "a");
    fprintf(interface, "r ");
    
    for(i = 0; aprotein[i].number; i++)
    {
        for(j = 0; bprotein[j].number; j++)
	{	  
	    if(((!strcmp(aprotein[i].residue, "GLY") && !strcmp(aprotein[i].type, "CA")) || !strcmp(aprotein[i].type, "CB")) &&
	       ((!strcmp(bprotein[j].residue, "GLY") && !strcmp(bprotein[j].type, "CA")) || !strcmp(bprotein[j].type, "CB")))
	    {
	        distance = sqrt(pow(aprotein[i].x-bprotein[j].x, 2) +
				pow(aprotein[i].y-bprotein[j].y, 2) +
				pow(aprotein[i].z-bprotein[j].z, 2));				      
					    
	        if(distance < maxdistanceInterface)
		{    
		     fprintf(interface, "%d %d ", aprotein[i].residuenumber, bprotein[j].residuenumber);		     
		     
		     protein[2][k].number = aprotein[i].number;
		     protein[2][k].residuenumber = aprotein[i].residuenumber;
		     protein[2][k].x = aprotein[i].x;
		     protein[2][k].y = aprotein[i].y;
		     protein[2][k].z = aprotein[i].z;
		     
		     protein[3][k].number = bprotein[j].number;
		     protein[3][k].residuenumber = bprotein[j].residuenumber;
		     protein[3][k].x = bprotein[j].x;
		     protein[3][k].y = bprotein[j].y;
		     protein[3][k].z = bprotein[j].z;;
		     
		     k++;
                }
	    }
        }
    }
    
    fprintf(interface, "\n\nq\n");
    fclose(interface);
    
    for(i = 0; protein[2][i].number; i++)
    {
        for(j = 0; protein[3][j].number; j++)
	{
	    distance = sqrt(pow(protein[2][i].x-protein[3][j].x, 2) +
			    pow(protein[2][i].y-protein[3][j].y, 2) +
			    pow(protein[2][i].z-protein[3][j].z, 2));				      
					    
	    if(distance < maxdistanceContacts)
	    {
		printf("%d %d\n", protein[2][i].residuenumber,
				  protein[3][j].residuenumber);
	    }
	}
    }
}

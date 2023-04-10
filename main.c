#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void ajouterVol(FILE *voles);
void removeVol(FILE *voles);
void displayVols(FILE *voles);
int isStringEqual(char strs[20][30], char *str);
void copyFile(FILE *originalFile, FILE *fileCopy);
void sortVols(FILE *voles);


int main()
{

    FILE *vols = NULL;
    int choice;

    printf("1- Ajouter Vole\n");
    printf("2- Supprimer Vole\n");
    printf("3- Afficher Vole\n");
    printf("4- Trie Voles\n");


    scanf("%d", &choice);

    switch(choice)
    {
        case 1:
            ajouterVol(vols);
            break;
        case 2:
            removeVol(vols);
            break;
        case 3:
            displayVols(vols);
            break;
        case 4:
            sortVols(vols);
            break;
        default:
            printf("Choose another number !!");
    }


    return 0;
}


void ajouterVol(FILE *voles)
{
    voles = fopen("vols.txt", "a");
    FILE *sizeOfFile = fopen("size.txt", "w+");
    char vileDepart[20], vileArrive[20], heureDepart[6], heureArrive[6];
    int size;

    scanf("%s", vileDepart);
    scanf("%s", heureDepart);
    scanf("%s", vileArrive);
    scanf("%s", heureArrive);

    fprintf(voles, "%s %s %s %s", vileDepart, heureDepart, vileArrive, heureArrive);
    fputc('\n', voles);

    fscanf(sizeOfFile, "%d", &size);
    size++;
    fprintf(sizeOfFile, "%d", size);

    fclose(voles);
    fclose(sizeOfFile);
}


void removeVol(FILE *voles)
{
    voles = fopen("vols.txt", "r");
    FILE *tempFile = fopen("newVols.txt", "w");
    FILE *sizeOfFile = fopen("size.txt", "r");
    char vileDepart[20], vileArrive[20], heureDepart[6], heureArrive[6];
    char voleD[20], voleA[20], ch, vole[100], voleToRemove[100];
    int hDepart, mDepart, hArrive, mArrive;
    int size;

    scanf("%s", vileDepart);
    scanf("%s", heureDepart);
    scanf("%s", vileArrive);
    scanf("%s", heureArrive);

    sprintf(voleToRemove, "%s %s %s %s", vileDepart, heureDepart, vileArrive, heureArrive);

    int i = 0;
    while(!feof(voles))
    {
        fscanf(voles, "%s %d %c %d %s %d %c %d \n", voleD, &hDepart, &ch, &mDepart, voleA, &hArrive, &ch, &mArrive);
        sprintf(vole, "%s %d%c%d %s %d%c%d", voleD, hDepart, ch, mDepart, voleA, hArrive, ch, mArrive);

        printf("cmp: %d\n", strcmp(vole, voleToRemove));
        if(!strcmp(vole, voleToRemove))
        {
            size--;
            if(size < 0)
                size = 0;
            sizeOfFile = fopen("size.txt", "w");
            fprintf(sizeOfFile, "%d", size);
            printf("remove vole\n");
            //chlef 10:15 tenes 12:20
        }
        else {
            fprintf(tempFile, "%s\n", vole);
            printf("add to temp file\n");
        }
        printf("i = %d\n", i);
        i++;
    }
    fclose(voles);
    fclose(tempFile);
    fclose(sizeOfFile);

    copyFile(tempFile, voles);

}


void displayVols(FILE *voles)
{
    voles = fopen("vols.txt", "r");
    char c;
    while((c = fgetc(voles)) != EOF)
    {
        printf("%c", c);
    }
    fclose(voles);
}


int isStringEqual(char strs[20][30], char *str)
{
    int i = 0;

    while(strcmp(strs[i], "\0"))
    {
        if(!strcmp(strs[i], str))
            return 1;
        i++;
    }

    return 0;
}


void copyFile(FILE *originalFile, FILE *fileCopy)
{
    //fseek(originalFile, 0, SEEK_SET);
    //fseek(fileCopy, 0, SEEK_SET);

    originalFile = fopen("newVols.txt", "r");
    fileCopy = fopen("vols.txt", "w");

    char c;
    c = fgetc(originalFile);
    while(c != EOF)
    {
        fputc(c, fileCopy);
        c = fgetc(originalFile);
    }

    fclose(originalFile);
    fclose(fileCopy);

}


void sortVols(FILE *voles)
{
    voles = fopen("vols.txt", "r");
    FILE *newVols = fopen("newVols.txt", "w");
    FILE *sizeOfFile = fopen("size.txt", "r");
    char arrStr[20][30];
    char ch, voleD[20], voleA[20], vol[100], v[100];
    int i, j, k, minHeure, minMinute, changed;
    int hDepart, mDepart, hArrive, mArrive;

    int size;
    fscanf(sizeOfFile, "%d", &size);

    i = size;
    k = 0;
    strcpy(arrStr[0], "!!!");
    while(i--)
    {
        j = size;
        changed = 0;
        minHeure = 24;
        minMinute = 60;
        fseek(voles, 0, SEEK_SET);
        while(j--)
        {
            fscanf(voles, "%s %d %c %d %s %d %c %d", voleD, &hDepart, &ch, &mDepart, voleA, &hArrive, &ch, &mArrive);
            sprintf(v, "%s %d%c%d %s %d%c%d", voleD, hDepart, ch, mDepart, voleA, hArrive, ch, mArrive);

            if(hDepart < minHeure && !isStringEqual(arrStr, v))
            {
                strcpy(vol, "");
                strcpy(vol, v);
                minHeure = hDepart;
                changed = 1;
            }
            else if(hDepart == minHeure && !isStringEqual(arrStr, v))
            {
                if(mDepart < minMinute)
                {
                    strcpy(vol, "");
                    strcpy(vol, v);
                    minMinute = mDepart;
                    changed = 1;
                }
            }
        }

        if(changed == 1)
        {
            strcpy(arrStr[k], vol);
            k++;
        }
    }


    for(int l = 0; l < k; l++)
    {
        fprintf(newVols, "%s\n", arrStr[l]);
    }
    fclose(voles);
    fclose(newVols);

    copyFile(newVols, voles);


    fclose(sizeOfFile);
}






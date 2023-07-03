#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <assert.h>

#define MAXCOLS  256
#define MAXROWS  256

/* Data type: Labyrinth - ASCII map, costs, direction */

// TODO Sourcecode und insbesondere Rekursion verinnerlichen und verstehen
// TODO Optimaler Weg ermitteln
// TODO sicherstellen, dass Input richtig ist (zum Beispiel trimmen)

typedef struct labyrinth {
    char lab[MAXCOLS + 2][MAXROWS];
    long costs[MAXCOLS][MAXROWS];
    int maxrow;
    int startx;
    int starty;
    int treasurex;
    int treasurey;
} LabCell, *Lab_p;
typedef struct coords {
    int i_Reihe;
    int j_Spalte;
    coords *next;
} Coords, *Coords_p;

bool loadLab(char *filename, Lab_p lab_struct_p);

void printLab(Lab_p lab_struct_p);

bool isFree(Lab_p lab_struct_p, int i, int j);

void pause(Lab_p lab_struct_p);

bool takeStep(Lab_p lab_struct_p, int i, int j);
LabCell lab_struct;
int steps = 1;
int main(int argc, char **argv) {

    Lab_p lab_struct_p = &lab_struct;
    if(!loadLab(argv[1], lab_struct_p)){
        printf("Das Labyrinth entspricht leider nicht den Vorgaben oder der Pointer zeigt auf kein Labyrinth.");
        return 0;
    }
    //printf("Startposition: [%d, %d]", lab_struct_p->starty, lab_struct_p->startx);
    printLab(lab_struct_p);
    printf("\n\n\n");
    if(!takeStep(lab_struct_p, lab_struct_p->starty, lab_struct_p->startx)) {
        printf("Das Labyrinth konnte nicht gelöst werden.\n");
        return 0;
    }
    system("clear");
    for (int i = 0; i < MAXROWS; ++i) {
        for (int j = 0; j < MAXCOLS; ++j) {
            if(lab_struct_p->lab[i][j]=='.') steps++;
        }
    }
    printf("\n\n\nGelöstes Labyrinth:\n");
    printLab(lab_struct_p);
    printf("\n\n\nAnzahl Schritte: %d", steps);

    return 0;
}

bool loadLab(char *filename, Lab_p lab_struct_p) {
    FILE *file;
    file = fopen(filename, "r");
    int anzS=0, anzX=0;
    int i = 0;

    if (file == nullptr) {
        return false;
    } else {
        fgets(&lab_struct_p->lab[i][0], 230, file);

        //printf("%s", &lab_struct_p->lab[i][0]);
        i++;
        while (!feof(file)) {
            fgets(&lab_struct_p->lab[i][0], 230, file);
            //printf("%s", &lab_struct_p->lab[i][0]);
            i++;
        }
        fclose(file);
    }
    //Setze Startkoordinaten auf 'S'
    for(int j = 0; j<MAXCOLS; j++){
        for (int k = 0; k < MAXROWS; ++k) {
            if(lab_struct_p->lab[k][j]=='S' or lab_struct_p->lab[k][j]=='s'){
                lab_struct_p->startx=j;
                lab_struct_p->starty=k;
                anzS++;
            }
            if(lab_struct_p->lab[k][j]=='X' or lab_struct_p->lab[k][j]=='x'){
                anzX++;
            }
        }
    }
    if(anzS==1 and anzX==1) return true;
    else return false;
}
void printLab(Lab_p lab_struct_p) {
    int i = 0;
    while (lab_struct_p->lab[i][0] == '#') {
        printf("%s", &lab_struct_p->lab[i][0]);
        i++;
    }
}

bool takeStep(Lab_p lab_struct_p, int i, int j){
    //weg wird für alle Felder die auf dem Weg liegen am Ende der Rekursion von hinten true
    bool weg = false;

    //Endbedingung der Rekursion
    if (lab_struct_p->lab[i][j] == 'X' or lab_struct_p->lab[i][j] == 'x') {
        return true;
    }


    if(lab_struct_p->lab[i][j] != 'S') lab_struct_p->lab[i][j] ='*';
    pause(lab_struct_p);
    /* Felder um akt. Pos werden gecheckt.
     * Wenn frei: Rekursiver Aufruf der Funktion in jenem Feld
     */
    //Oben
    if (isFree(lab_struct_p, i - 1, j)) {
        weg = takeStep(lab_struct_p, i - 1, j);
    }
    if(weg){
        pause(lab_struct_p);
        if(lab_struct_p->lab[i][j]!='S' and lab_struct_p->lab[i][j]!='s')lab_struct_p->lab[i][j] = '.';
        return true;
    }
    //Links
    if (isFree(lab_struct_p, i, j - 1)) {
        weg = takeStep(lab_struct_p, i, j - 1);
    }
    if(weg){
        pause(lab_struct_p);
        if(lab_struct_p->lab[i][j]!='S' and lab_struct_p->lab[i][j]!='s')lab_struct_p->lab[i][j] = '.';
        return true;
    }
    //Unten
    if (isFree(lab_struct_p, i + 1, j)) {
        weg = takeStep(lab_struct_p, i + 1, j);
    }
    if(weg){
        pause(lab_struct_p);
        if(lab_struct_p->lab[i][j]!='S' and lab_struct_p->lab[i][j]!='s')lab_struct_p->lab[i][j] = '.';
        return true;
    }
    //Rechts
    if (isFree(lab_struct_p, i, j + 1)) {
        weg = takeStep(lab_struct_p, i, j + 1);
    }
    if (weg){
        pause(lab_struct_p);
        if(lab_struct_p->lab[i][j]!='S' and lab_struct_p->lab[i][j]!='s')lab_struct_p->lab[i][j] = '.';
    }
    return weg;
}

bool isFree(Lab_p lab_struct_p, int i, int j) {
    char cell = lab_struct_p->lab[i][j];
    if (cell == '#' or cell == '*' or cell == 'S' or cell == '.') return false;
    else return true;
}
void pause(Lab_p lab_struct_p){
    system("clear");
    printLab(lab_struct_p);
    getchar();
}
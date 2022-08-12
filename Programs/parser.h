#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <math.h>
#include <time.h>
#define STD 20 //Length of a word || Standard
#define MED 100 //Length of a sentence || Medium 
#define BIG 500 //Length of a list 
#define MAX 1200 //Size of the Pokedex

int cpf(int atk, int def, int sta, int lvl, int iv); //To calculate Pokemon CP
void pokedex(); //To generate the <dest_pokedex> file
void pokebattler(); //To generate the <dest_pokebattler> file

//This function is called from main() to instantly update all Pokemon information in the <dest_*> files 
void update(){

pokedex();
pokebattler();

}


int cpf(int atk, int def, int sta, int lvl, int iv){
  int cp;
  double multiplier; 

  if(lvl == 40) multiplier = 0.7903;
  if(lvl == 25) multiplier = 0.667934;
  if(lvl == 20) multiplier = 0.5974;

  cp = floor((atk+iv)*sqrt(def+iv)*sqrt(sta+iv)*0.1*pow(multiplier, 2));
  return cp;
}


void pokedex(){

  char pokedexno[MAX][4], pokename[MAX][STD], type1[MAX][STD], type2[MAX][STD], fast[MAX][STD], charge[MAX][STD], bin[STD];
  int atk[MAX], def[MAX], sta[MAX], cp[MAX], rank[MAX];
  int i, j, exit, signal = 0;

//Extracting information from <var_pokedex>
  FILE * fptr = fopen(".//Parse//var_pokedex", "r");

  for(i = 0; exit != EOF; i++){
    if(signal == 0) fscanf(fptr, " %s", pokedexno[i]);
    else strcpy(pokedexno[i], bin);

    fscanf(fptr, " %s", pokename[i]);
    fscanf(fptr, " %s %s", type1[i], type2[i]);
    
    if(isdigit(type2[i][0]) > 0){
      sta[i] = atoi(type2[i]);
      strcpy(type2[i], "N/A");
      fscanf(fptr, " %d %d", &atk[i], &def[i]);
    }
    else fscanf(fptr, " %d %d %d", &sta[i], &atk[i], &def[i]);

    fscanf(fptr, " %s %s %s", bin, bin, bin);

    while(isdigit(bin[0]) == 0){
      exit= fscanf(fptr, " %s", bin);
      if(exit == EOF) break;
    }
    signal = 1;
  }

  fclose(fptr);

//Generating shadow Pokemon if it is listed in <var_shadowlist> 
  FILE * fptr2 = fopen(".//Parse//var_shadowlist", "r");

  char shadowpokename[MAX][STD];
  int exit2, counter = i;

  for(int a = 0; exit2 != EOF; a++){
    exit2 = fscanf(fptr2, " %s", shadowpokename[a]);
    if(exit2 == EOF) break;

    for(int b = 0; b < counter; b++){
      if(strcmp(shadowpokename[a], pokename[b]) == 0){
        char tempname[STD];
        pokename[i][0] = '*';
        strcpy(tempname, pokename[b]);
        strcat(pokename[i], tempname);
        strcpy(pokedexno[i], pokedexno[b]);
        strcpy(type1[i], type1[b]);
        strcpy(type2[i], type2[b]);
        atk[i] = atk[b];
        def[i] = def[b];
        sta[i] = sta[b];
        i++;
      }
    }

  }

  fclose(fptr2);

  for(int a = 0; a < i; a++){
    cp[a] = cpf(atk[a], def[a], sta[a], 40, 15);
  }

  for(int a = 0; a < i; a++){
    rank[a] = 1;
    for(int b = 0; b < i; b++){
      if(cp[a] < cp[b]) rank[a]++;
    }
  }

//Extracting information of the tier of each Pokemon from <var_poketier>
  FILE * fptr3 = fopen(".//Parse//var_poketier", "r");

  char temppokename1[MAX][STD], temptier[MAX][4], tier[MAX][4];
  int exit3;

  for(int a = 0; a < i; a++){
    if(cp[a] > 3500) strcpy(tier[a], "C-");
    else if(cp[a] > 3000) strcpy(tier[a], "D");
    else if(cp[a] > 2500) strcpy(tier[a], "E");
    else if(cp[a] < 1000) strcpy(tier[a], "NO");
    else strcpy(tier[a], "F");
  }

  for(int a = 0; exit3 != EOF; a++){
    exit3 = fscanf(fptr3, " %s %s", temppokename1[a], temptier[a]);
    for(int b = 0; b < i; b++){
      if(strcmp(temppokename1[a], pokename[b]) == 0){
      strcpy(tier[b], temptier[a]);
      break;
      }
    }
  }

  fclose(fptr3);

//Extracting information about the best moveset from <dest_pokebattler>
  FILE * fptr4 = fopen(".//Files//dest_pokebattler", "r");

  char temppokename2[MAX][STD], tempfast[MAX][STD], tempcharge[MAX][STD];
  int tempscore[MAX], pokebattlerrank[MAX] = {0}, score[MAX] = {0}, exit4, norepetition[MAX];

  for(int a = 0; exit4 != EOF; a++){
    exit = fscanf(fptr4, " %s %s %s %d", temppokename2[a], tempfast[a], tempcharge[a], &tempscore[a]);
    if(exit == EOF) break;
    for(int b = 0; b < i; b++){
      if(strcmp(temppokename2[a], pokename[b]) == 0){
        if(norepetition[b] != 1){
          strcpy(fast[b], tempfast[a]);
          strcpy(charge[b], tempcharge[a]);
          score[b] = tempscore[a];
          pokebattlerrank[b] = a+1;
          norepetition[b] = 1;
        }
      }
    }
  }
  
  fclose(fptr4);

  for(int a = 0; a < MAX; a++){
    if(fast[a][0] == '\0') strcpy(fast[a], "N/A");
    if(charge[a][0] == '\0') strcpy(charge[a], "N/A");
  }

  FILE * fptr5 = fopen(".//Parse//var_shinylist", "r");\

  char shinypokename[MAX][STD];
  int shiny[MAX] = {0}, exit5 = 0;

  for(int a = 0; exit5 != EOF; a++){
    exit5 = fscanf(fptr5, " %s", shinypokename[a]);
    for(int b = 0; pokename[b][0] != '\0'; b++){
      if(strcmp(pokename[b], shinypokename[a]) == 0){
        shiny[b] = 1;
        break;
      }
    }
  }

  fclose(fptr5);

//Printing all the information gathered to <dest_pokedex>
  FILE * fptr6 = fopen(".//Files//dest_pokedex", "w");

  for(j = 0; j < i; j++){
    fprintf(fptr5, "%s %-20.20s %-10s %-10s %-6d %-6d %-6d %-8d %-8d %-8s %-8d %-8d %-15.15s %-15.15s %d\n", pokedexno[j], pokename[j], type1[j], type2[j], atk[j], def[j], sta[j], cp[j], rank[j], tier[j], score[j], pokebattlerrank[j], fast[j], charge[j], shiny[j]);
  }
  
  fclose(fptr6);

}


void pokebattler(){
  char chargetype[STD][STD][STD], pokename[MAX][STD], fast[MAX][STD], charge[MAX][STD], speftype[MAX][STD];
  int score[MAX], comparison[MAX], print[MAX], exit, exit2;

//Extracting information about the typing of each Pokemon's charged moves from <static_chargetype>
  FILE * fptr = fopen(".//Files//static_chargetype", "r");

  for(int a = 0; a < STD; a++){
    exit = fscanf(fptr, " %s", chargetype[a][0]);
    if(exit == EOF) break;
    //printf("%s\n", chargetype[a][0]);
    for(int b = 1; chargetype[a][b-1][0] != '*'; b++){
      fscanf(fptr, " %s", chargetype[a][b]);
      //printf("%s\n", chargetype[a][b]);
    }
  }

  fclose(fptr);

//Extracting information about the best Pokemon and their charged moves from <var_pokebattler>, removing Pokemon of the same name having charged moves of the same type and sorting them in descending order
  FILE * fptr2 = fopen(".//Parse//var_pokebattler", "r");

  for(int a = 0; exit2 != EOF; a++){
  exit2 = fscanf(fptr2, " %s %s %s %d", pokename[a], fast[a], charge[a], &score[a]);
  if(exit2 == EOF) break;

    for(int b = 0; chargetype[b][0][0] != '\0'; b++){
      for(int c = 1; chargetype[b][c][0] != '\0'; c++){
        comparison[a] = strcmp(charge[a], chargetype[b][c]);
        if(comparison[a] == 0){
          strcpy(speftype[a], chargetype[b][0]);
          break;
        }
      }
    }

    print[a] = 1;
    for(int x = 0; x < a; x++){
      if(x == a) continue;
      if(strcmp(pokename[a], pokename[x]) == 0){
        if(strcmp(speftype[a], speftype[x]) == 0){
          print[a] = 0;
          break;
        }
      }
    }

  if(print[a] == 0) score[a] = 0;
  }

  fclose(fptr2);

  for(int a = 0; pokename[a][0] != '\0'; a++){
    char temppokename[STD] = {"\0"}, tempfast[STD] = {"\0"}, tempcharge[STD] = {"\0"};
    int tempscore = score[a];
    strcpy(temppokename, pokename[a]);
    strcpy(tempfast, fast[a]);
    strcpy(tempcharge, charge[a]);
    int b = a-1;
    while(tempscore > score[b] && b >= 0){
      score[b+1] = score[b];
      strcpy(pokename[b+1], pokename[b]);
      strcpy(fast[b+1], fast[b]);
      strcpy(charge[b+1], charge[b]);
      b -= 1;
    }
    score[b+1] = tempscore;
    strcpy(pokename[b+1], temppokename);
    strcpy(fast[b+1], tempfast);
    strcpy(charge[b+1], tempcharge);
  }

//Printing the sorted list of Pokemon to <dest_pokebattler> 
  FILE * fptr3 = fopen(".//Files//dest_pokebattler", "w");

    for(int a = 0; score[a] != 0; a++){
    fprintf(fptr3, "%s %s %s %d\n", pokename[a], fast[a], charge[a], score[a]);
    }

  fclose(fptr3);

}
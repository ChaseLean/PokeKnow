//Function to simulate typing
void type(char string[]){
  for(int a = 0; a < strlen(string); a++){
    printf("%c", string[a]);
    fflush(stdout);
    usleep(30000);
  }
}


//Function to prompt user to press "Enter" to exit
void gets(){
  char string[STD];
  fgets(string, STD, stdin);
}


//Function to print the instructions from <utility_instructions>
void instructions(){
  system("clear");

  FILE * fptr = fopen(".//Files//utility_instructions", "r");

  char string[MED];
  int exit;

//Scanning and printing each line of instructions
  for(int a = 0; a < MAX; a++){
    fgets(string, MED, fptr);
    if(string[0] == '*') break;
    printf("%s", string);
  }

//Return prompt
  type("\n\nPress Enter to return to Main Menu.\n");
  gets();
  system("clear");
}


//Function to autocorrect misspelled Pokemon names based on the list in <utility_autocorrect>
void autocorrect(char inputpokename[]){
  char name1[MED][STD], name2[MED][STD], temppokename[STD] = {'*'};
  int shadow = 0;

  for(int a = 0; inputpokename[a] != '\0'; a++){
    switch(inputpokename[a]){
      case '*': shadow = 1; 
      case ' ': case '(': case ')': case '.': case ',': case '/': case '?': case '-': case '_': case '#':
      for(int b = a; inputpokename[b] != '\0'; b++){
        inputpokename[b] = inputpokename[b+1];
      }
      a--;
      break;
    }
  }

  if(strncasecmp(inputpokename, "Shadow", 6) == 0){
    shadow = 1;
    memmove(inputpokename, inputpokename+6, strlen(inputpokename));
  }

  FILE * fptr = fopen(".//Files//utility_autocorrect", "r");
  
  int exit = 0;

  for(int a = 0; exit != EOF; a++){
    exit = fscanf(fptr, " %s %s", name1[a], name2[a]);
    if(exit == EOF) break;
    if(strcasecmp(inputpokename, name1[a]) == 0){
      strcpy(inputpokename, name2[a]);
      break;
    }
  }

  if(shadow == 1){
    strcat(temppokename, inputpokename);
    strcpy(inputpokename, temppokename);
  }
  
  fclose(fptr);
}


//Function to execute the search
void search(int selection){
  system("clear");

  char inputpokename[STD], pokedexno[MAX][4], pokename[MAX][STD], type[MAX][2][STD], tier[MAX][4], fast[MAX][STD], charge[MAX][STD];
  int atk[MAX], def[MAX], sta[MAX], cp[MAX], rank[MAX], typecount[MAX], pokebattlerrank[MAX], score[MAX], shiny[MAX];
  int i, j, exit;

//Prompting the user to enter Pokemon name
  reenter:
  printf("\nEnter Pokemon name: ");
  if(fgets(inputpokename, 20, stdin) == NULL){
    system("clear");
    return;
  }
  else if(inputpokename[0] == '\n') goto reenter;

//Removing "\n" at the end of the input
  inputpokename[strcspn(inputpokename, "\n")] = 0;

//Extracting information from <dest_pokedex>
  FILE * fptr = fopen(".//Files//dest_pokedex", "r");

  exit = 0;
  for(i = 0; exit != EOF; i++){
    exit = fscanf(fptr, " %s %s %s %s %d %d %d %d %d %s %d %d %s %s %d", pokedexno[i], pokename[i], type[i][0], type[i][1], &atk[i], &def[i], &sta[i], &cp[i], &rank[i], tier[i], &score[i], &pokebattlerrank[i], fast[i], charge[i], &shiny[i]);
  }

  fclose(fptr);

//Function to autocorrect user selection
  autocorrect(inputpokename);

//If user input matches data in Pokedex, print info
  for(j = 0; j < i; j++){
    if(strcasecmp(inputpokename, pokename[j]) == 0){
      printf("\n\n(%s) %s\n", pokedexno[j], pokename[j]);
      for(int a = 0; a < strlen(pokename[j])+6; a++){
        printf("-");
      }

//Print this information only if user selects option [2]
      if(selection == 2){
        if(type[j][1][1] == '/') printf("\nType   : %s\n", type[j][0]);
        else printf("\nType   : %s, %s\n", type[j][0], type[j][1]);

        if(shiny[j] == 1) printf("Shiny  : Yes\n\n");
        else printf("Shiny  : No\n\n");

        printf("Max CP  (lvl40)\t: %d\nRaid CP (lvl20)\t: %d - %d\nCP rank\t\t\t: %d\nPVE rank\t\t: %d\n\nAttacker tier\t\t: %s\nBest fast move\t\t: %s\nBest charged move\t: %s\n\n", cp[j], cpf(atk[j], def[j], sta[j], 20, 10), cpf(atk[j], def[j], sta[j], 20, 15), rank[j], pokebattlerrank[j], tier[j], fast[j], charge[j]);
        break;
      }

      if(selection == 3) break;
    }

//Error message if input name cannot be found in Pokedex
    if(j == i-1){
      printf("Pokemon not found.");
      goto reenter;
    }
  }

//Extracting information from <static_poketypelist>
  FILE * fptr2 = fopen(".//Files//static_poketypelist", "r");

//Intializing variables to store Pokemon type, super effective and not very effective relationships
  char basetype[STD][STD], spef[STD][STD][STD], ntef[STD][STD][STD], typespef[2][STD][STD] = {"\0"}, weakness[2][STD][STD] = {"\0"};
  int weakcount = 0, dwcount = 0, exit2 = 0;

//Reading base typing
  for(int a = 0; exit2 != EOF; a++){
    exit2 = fscanf(fptr2, " %s", basetype[a]);
    if(exit2 == EOF) break;

//What is the base typing super effective against?
    for(int b = 0; b < STD; b++){
      fscanf(fptr2, " %s", spef[a][b]);
      if(spef[a][b][0] == '*'){
        spef[a][b][0] = '\0';
        break;
      }
    }

//What is the base typing not very effective against?
    for(int c = 0; c < STD; c++){
      fscanf(fptr2, " %s", ntef[a][c]);
       if(ntef[a][c][0] == '*'){
        ntef[a][c][0] = '\0';
        break;
      }
    }
  }

  fclose(fptr2);

  //Finding the typing that THIS Pokemon's attacks are super effective against and storing it inside <typespef>
  for(int a = 0; a <= 1; a++){
    for(int b = 0; basetype[b][0] != '\0'; b++){
      if(strcmp(type[j][a], basetype[b]) == 0){
        for(int c = 0; c < STD; c++){
          if(spef[b][c][0] == '\0') break;
          strcpy(typespef[a][c], spef[b][c]);
        }
      }
    }
  }

 //Finding the type of attack that THIS Pokemon is weak to and storing it inside <weakness>
  for(int a = 0; a <= 1; a++){
    for(int b = 0; basetype[b][0] != '\0'; b++){
      start:
      for(int c = 0; spef[b][c][0] != '\0'; c++){
        if(strcmp(type[j][a], spef[b][c]) == 0){
          for(int d = 0; ntef[b][d][0] != '\0'; d++){
            if(strcmp(type[j][1-a], ntef[b][d]) == 0){
              b++;
              goto start;
            }
          }
          strcpy(weakness[0][weakcount], basetype[b]);
          weakcount++;
        }
      }
    }
    if(type[j][1][1] == '/') break;
  }

//Indicating if the Pokemon is DOUBLY weak to a certain type by storing it inside weakness[1] instead of weakness[0]
  for(int a = 0; a < weakcount; a++){
    for(int b = 0; b < weakcount; b++){
      if(a == b) continue;
      if(strcmp(weakness[0][a], weakness[0][b]) == 0 && strcmp(weakness[0][a], "N/A") != 0){
        strcpy(weakness[1][dwcount], weakness[0][a]);
        dwcount++;
        strcpy(weakness[0][a], "N/A");
        strcpy(weakness[0][b], "N/A");
      }
    }
  }

//Printing this info only if the user selects option [2]
  if(selection == 2){
    printf("Moves super effective against:\n------------------------------\nType 1: %-9s Type 2: %s\n\n", type[j][0], type[j][1]);
    for(int a = 0; a < STD; a++){
      if(typespef[0][a][0] == '\0' && typespef[1][a][0] == '\0') break;
      if(typespef[0][a][0] != '\0')
      printf("- %-16s", typespef[0][a]);
      else printf("%-18s", typespef[0][a]);
      if(typespef[1][a][0] != '\0') 
      printf("- %s", typespef[1][a]);
      printf("\n");
    }
  }

    printf("\nThis Pokemon is weak to:\n");
    if(selection == 2) printf("------------------------\n");
    for(int a = 0; weakness[1][a][0] != '\0'; a++){
      printf("- %s(2x)\n", weakness[1][a]);
    }
    for(int a = 0; weakness[0][a][0] != '\0'; a++){
      if(strcmp(weakness[0][a], "N/A") != 0) printf("- %s\n", weakness[0][a]);
    }
  

//Running this part only if the user selects option [3]
  if(selection == 3){

//Extracting information from <static_chargetype>
  FILE * fptr3 = fopen(".//Files//static_chargetype", "r");

//Intializing variables to store the specific charged attacks that THIS Pokemon is weak to 
  char chargetype[MED][MED][MED], weaknesslist[2][MED][MED] = {"\0"};
  int exit3 = 0;

//Categorizing each charged move by type
  for(int a = 0; a < 30; a++){
    int condition = 0;
    for(int b = 0; b < 30; b++){
      exit3 = fscanf(fptr3, " %s", chargetype[a][b]);
      if(exit3 == EOF){
        condition = 1; 
        break;
      }
      if(chargetype[a][b][0] == '*') break;
    if(condition == 1) break;
    }
  }

//Finding all the charged moves that THIS Pokemon is weak to
  for(int a = 0; a <= 1; a++){
    int p = 0, q = 0;
    for(int b = 0; weakness[a][b][0] != '\0'; b++){
      for(int c = 0; chargetype[c][0][0] != '\0'; c++){
        if(strcmp(weakness[a][b], chargetype[c][0]) == 0){
          q = 1;
          while(chargetype[c][q][0] != '*'){
            strcpy(weaknesslist[a][p], chargetype[c][q]);
            p++; q++;
          }
        }
      }
    }
  }

  fclose(fptr3);

//extracting information from <dest_pokebattler>
  FILE * fptr4 = fopen(".//Files//dest_pokebattler", "r");

//Initalizing variables to store counter data
  char counter[MED], mocfast[MED], moccharge[MED];
  char counterlist[BIG][MED] = {"\0"}, mocfastlist[BIG][MED] = {"\0"}, mocchargelist[BIG][MED] = {"\0"};

  int num, score, scorelist[BIG] = {0};
  int rank = 1, exit4 = 0;

//Storing counter data
  for(int a = 0; exit4 != EOF; a++){
    exit4 = fscanf(fptr4, " %s %s %s %d", counter, mocfast, moccharge, &score);
    if(exit4 == EOF) break;
    for(int b = 0; b <= 1; b++){
      for(int c = 0; weaknesslist[b][c][0] != '\0'; c++){
        if(strcmp(moccharge, weaknesslist[b][c]) == 0){ 
          if(b == 1) score *= 6; //Assigning a higher weightage to Pokemon with charged attacks that are doubly super effective to THIS Pokemon
          strcpy(counterlist[num], counter);
          strcpy(mocfastlist[num], mocfast);
          strcpy(mocchargelist[num], moccharge);
          scorelist[num] = score;
          num++;
        }
      }
    }
  }

  fclose(fptr4);

//Sorting the counters to THIS Pokemon by score
  for(int a = 0; a < num; a++){
    strcpy(counter, counterlist[a]);
    strcpy(mocfast, mocfastlist[a]);
    strcpy(moccharge, mocchargelist[a]);
    score = scorelist[a];
    int b = a-1;
      while(score > scorelist[b] && b >= 0){
        strcpy(counterlist[b+1], counterlist[b]);
        strcpy(mocfastlist[b+1], mocfastlist[b]);
        strcpy(mocchargelist[b+1], mocchargelist[b]);
        scorelist[b+1] = scorelist[b];
        b--;
      }
      strcpy(counterlist[b+1], counter);
      strcpy(mocfastlist[b+1], mocfast);
      strcpy(mocchargelist[b+1], moccharge);
      scorelist[b+1] = score; 
  }

//Prompting the user to enter the maximum number of counters to show
  int show;
  printf("\n\nHow many counters do you want to show?\nRecommended : 20\nEnter       : ");
  scanf(" %d", &show);
  getchar();

//Printing the counters
  printf("\n\nTop counters:\n\nNO   NAME                 FAST MOVE         CHARGED MOVE\n\n");

  for(int a = 0; counterlist[a][0] != '\0' && a < show; a++){
    printf("%-4d %-20s %-15s\t%s\n", a+1, counterlist[a], mocfastlist[a], mocchargelist[a]);
  }

}

//Making entry in <pokelog> file
  FILE * fptr5 = fopen(".//Files//utility_pokelog", "a");
  time_t now;
  time(&now);
  fprintf(fptr5, "%-20s %-4d %s", inputpokename, selection, ctime(&now));
  fclose(fptr5);

//Return prompt
  printf("\nPress Enter to return to Main Menu.\n");
  gets();
  system("clear");
}


void credits(){
  system("clear");

//Opening <credits> file
  FILE * fptr = fopen(".//Files//utility_credits", "r");

  char string[MED];
  int exit;

//Printing the credits line by line
  for(int a = 0; a < MAX; a++){
    fgets(string, MED, fptr);
    if(string[0] == '*') break;
    printf("%s", string);
  }

//Return prompt
  type("\n\nPress Enter to return to Main Menu.\n");
  gets();
  system("clear");
}
  

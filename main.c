#include ".//Programs//parser.h"
#include ".//Programs//myfunctions.h"

int main(){

  update(); 

  char selection[20]; 
  int exit; 

  type("Welcome to PokeKnow!\n\n[1] Instructions\n[2] Pokemon Search\n[3] Raid Counters\n[4] Credits\n\nEnter your selection: ");

  for(int count = 0; exit != EOF; count++){

    if(count != 0) {
      printf("\nPokeKnow: Pokemon Go Information Center\n\n[1] Instructions\n[2] Pokemon Search\n[3] Raid Counters\n[4] Credits\n\n");
      type("Enter your selection: ");
    }

    reenter:
    if(fgets(selection, 20, stdin) == NULL){
      type("\nThank you and have a nice day!");
      break;
    }
    selection[strcspn(selection, "\n")] = 0;

    if(strcmp(selection, "1") == 0) instructions();
    else if(strcmp(selection, "2") == 0) search(2);
    else if(strcmp(selection, "3") == 0) search(3);
    else if(strcmp(selection, "4") == 0) credits();
    else {
      type("Invalid selection. Please re-enter: ");
      goto reenter;
    }
  }

return 0;
}

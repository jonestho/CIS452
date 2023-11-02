#include "kitchen.h"

// Baker "constructor"
Baker BakerFactory(int32_t* sharedMemPointer, int ID, int ramsay) {
    Baker baker;

    // baker attributes
    baker.memPointer = sharedMemPointer;
    baker.ramsay = ramsay;
    baker.ID = ID;
    baker.currentRecipe = 0;
    
    memcpy(baker.recipe, getRecipes(), sizeof(Recipe) * 5);

    // sets the color for text
    switch (ID) {
    case 0:
        strcpy(baker.color, "\033[31m");
        break;
    case 1:
        strcpy(baker.color, "\033[32m");
        break;
    case 2:
        strcpy(baker.color, "\033[36m");
        break;
    }

    return baker;
}

void enterPantry(Baker* self) {
    printStatus(self, "entering pantry");
    semop(self->memPointer[0], &p, 1);

    self->ifCheckPantry = 0;
    printStatus(self, "leaving pantry");
    semop(self->memPointer[0], &v, 1);
}

void enterFridge(Baker* self) {
    printStatus(self, "entering fridge");
    semop(self->memPointer[1], &p, 1);

    self->ifCheckFridge = 0;
    printStatus(self, "leaving fridge");
    semop(self->memPointer[1], &v, 1);
}


void useMixer(Baker* self) {
    printStatus(self, "using mixer");
    semop(self->memPointer[2], &p, 1);

    printStatus(self, "finished using mixer");
    semop(self->memPointer[2], &v, 1);
}

void useOven(Baker* self) {
    printStatus(self, "using oven");
    semop(self->memPointer[3], &p, 1);

    printStatus(self, "finished using oven");
    semop(self->memPointer[3], &v, 1);
}


// Prints the status of the baker -> made so I could use color
void printStatus(Baker* self, char* status) {
    printf("%sBaker [%d] is [%s]%s\n", self->color, self->ID, status, "\033[0m");
}


// Checks where the baker needs to go for the recipe
void checkRecipe(Baker* self) {
    Recipe recipe = self->recipe[self->currentRecipe];

    if (recipe.flour | recipe.sugar | recipe.bakingSoda | recipe.yeast | recipe.salt | recipe.cinnamon) {
        self->ifCheckPantry = 1;
    }
    if (recipe.eggs | recipe.milk | recipe.butter) {
        self->ifCheckFridge = 1;
    }
}

// Easy way to get recipes
Recipe* getRecipes() {
    Recipe* recipes = (Recipe*)malloc(sizeof(Recipe) * 5);

    /*
        Cookies: Flour, Sugar, Milk, Butter
        Pancakes: Flour, Sugar, Baking soda, Salt, Egg, Milk, Butter
        Homemade pizza dough: Yeast, Sugar, Salt
        Soft Pretzels: Flour, Sugar, Salt, Yeast, Baking Soda, Egg
        Cinnamon rolls: Flour, Sugar, Salt, Butter, Eggs, Cinnamon
    */
   recipes[0] = (Recipe){ 
         .name = "cookies",
         .flour = 1,
         .sugar = 1,
         .milk = 1,
         .butter = 1,
    };
   
   recipes[1] = (Recipe){
         .name = "pancakes",
         .flour = 1,
         .sugar = 1,
         .bakingSoda = 1,
         .salt = 1,
         .eggs = 1,
         .milk = 1,
         .butter = 1,
    };

    recipes[2] = (Recipe){
         .name = "pizza dough",
         .yeast = 1,
         .sugar = 1,
         .salt = 1,
    };

    recipes[3] = (Recipe){
         .name = "soft pretzels",
         .flour = 1,
         .sugar = 1,
         .salt = 1,
         .yeast = 1,
         .bakingSoda = 1,
         .eggs = 1,
    };

    recipes[4] = (Recipe){
         .name = "cinnamon rolls",
         .flour = 1,
         .sugar = 1,
         .salt = 1,
         .butter = 1,
         .eggs = 1,
         .cinnamon = 1,
    };

    return recipes;
}
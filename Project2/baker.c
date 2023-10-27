#include "kitchen.h"

Baker BakerFactory(int32_t* sharedMemPointer, int ID, int ramsay, Recipe recipe) {
    Baker baker;

    // baker attributes
    baker.memPointer = sharedMemPointer;
    baker.ramsay = ramsay;
    baker.ID = ID;

    // baker methods
    baker.enterFridge = enterFridge;
    baker.enterPantry = enterPantry;
    baker.useMixer = useMixer;
    baker.useOven = useOven;

    checkRecipe(&baker, recipe);

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

void* enterPantry(Baker* self) {
    self->printStatus(self, "entering pantry");
    semop(self->memPointer[0], &p, 1);


    self->ifCheckPantry = 0;
    self->printStatus(self, "leaving pantry");
    semop(self->memPointer[0], &v, 1);
}

void* enterFridge(Baker* self) {
    self->printStatus(self, "entering fridge");
    semop(self->memPointer[1], &p, 1);

    self->ifCheckFridge = 0;
    self->printStatus(self, "leaving fridge");
    semop(self->memPointer[1], &v, 1);
}


void* useMixer(Baker* self) {
    self->printStatus(self, "using mixer");
    semop(self->memPointer[2], &p, 1);

    self->printStatus(self, "finished using mixer");
    semop(self->memPointer[2], &v, 1);
}

void* useOven(Baker* self) {
    self->printStatus(self, "using oven");
    semop(self->memPointer[3], &p, 1);

    self->printStatus(self, "finished using oven");
    semop(self->memPointer[3], &v, 1);
}


void* printStatus(Baker* self, char* status) {
    printf("%sBaker [%d] is [%s]%s\n", self->color, self->ID, status, "\033[0m");
}

void checkRecipe(Baker* self, Recipe recipe) {
    if (recipe.ingredients.flour | recipe.ingredients.sugar | recipe.ingredients.bakingSoda | recipe.ingredients.yeast | recipe.ingredients.salt | recipe.ingredients.cinnamon) {
        self->ifCheckPantry = 1;
    }
    if (recipe.ingredients.eggs | recipe.ingredients.milk | recipe.ingredients.butter) {
        self->ifCheckFridge = 1;
    }
}
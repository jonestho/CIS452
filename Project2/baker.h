#pragma once

#include "kitchen.h"

typedef struct {
    char* name;
    int flour;
    int sugar; 
    int yeast;
    int bakingSoda;
    int salt;
    int cinnamon;
    int eggs;
    int milk;
    int butter;
} Recipe;

typedef struct Baker{
    int ramsay;
    char color[6];
    int32_t* memPointer;
    int ifCheckFridge;
    int ifCheckPantry;
    int ID;
    int currentRecipe;
    Recipe recipe[5];
} Baker;

void enterPantry();
void enterFridge();
void useOven();
void useMixer();
void checkRecipe(Baker* self);
void printStatus(Baker* self, char* status);
Recipe* getRecipes();
Baker BakerFactory(int32_t* sharedMemPointer, int ID, int ramsay);

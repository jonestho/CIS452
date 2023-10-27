#pragma once

#include "kitchen.h"

typedef struct Baker{
    void* (*enterPantry)(struct Baker* self);
    void* (*enterFridge)(struct Baker* self);
    void* (*useOven)(struct Baker* self);
    void* (*useMixer)(struct Baker* self);
    void* (*printStatus)(struct Baker* self, char* status);
    int ramsay;
    char color[6];
    int32_t* memPointer;
    int ifCheckFridge;
    int ifCheckPantry;
    int ID;
} Baker;

typedef struct {
    int flour;
    int sugar; 
    int yeast;
    int bakingSoda;
    int salt;
    int cinnamon;
    int eggs;
    int milk;
    int butter;
} Ingredients;

typedef struct {
    int mixer;
    int pantry;
    int fridge;
    int bowl;
    int spoon;
    int oven;
} Utensils;

typedef struct {
    char* name;
    Ingredients ingredients;
} Recipe;

void* enterPantry();
void* enterFridge();
void* useOven();
void* useMixer();
void checkRecipe(Baker* self, Recipe recipe);
Baker BakerFactory(int32_t* sharedMemPointer, int ID, int ramsay, Recipe recipe);


#define IN_FRIDGE 1
#define IN_PANTRY 0

typedef struct {
    void* (*enterPantry)(void*);
    void* (*enterFridge)(void*);
    void* (*useOven)(void*);
    void* (*useMixer)(void*);
} Baker;

typedef struct {
    int flour;
    int sugar;
    int yeast;
    int bakingSoda;
    int salt;
    int cinnamon;
} Ingredients;

typedef struct {
    int mixer;
    int pantry;
    int fridge;
    int bowl;
    int spoon;
    int oven;
} Utensils;

typedef struct Recipe {
    char* name;
    Ingredients ingredients;
};

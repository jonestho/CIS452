
#define IN_PANTRY 0
#define IN_FRIDGE 1
#define CLEAR_COLOR "\033[0m"

typedef struct {
    void* (*enterPantry)(Baker* self);
    void* (*enterFridge)(Baker* self);
    void* (*useOven)(Baker* self);
    void* (*useMixer)(Baker* self);
    void* (*printStatus)(Baker* self, char* status);
    int ramsay;
    char* color;
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

typedef struct {
    char* name;
    Ingredients ingredients;
} Recipe;

void* enterPantry();
void* enterFridge();
void* useOven();
void* useMixer();
Baker initBaker();
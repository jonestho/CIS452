#include "baker.h"
// #include <stdio.h>

Baker initBaker(int32_t* sharedMemPointer, int ID, int ramsay) {
    Baker baker;
    baker.sharedMemPointer = sharedMemPointer;
    baker.ramsay = ramsay;

    baker.enterFridge = enterFridge;
    baker.enterPantry = enterPantry;
    baker.useMixer = useMixer;
    baker.useOven = useOven;

    switch (bakerID) {
    case 0:
        baker.color = "\033[31m";
        break;
    case 1:
        baker.color = "\033[32m";
        break;
    case 2:
        Baker.color = "\033[36m";
        break;
    }

    return baker;
}

void* enterPantry(Baker* self) {
    self->printStatus(self, "entering pantry");


}

void* enterFridge(Baker* self) {
    self->printStatus(self, "entering fridge");

    
}



void* useMixer(Baker* self) {
    self->printStatus(self, "using mixer");



}

void* useOven(Baker* self) {
    self->printStatus(self, "using oven");



}

void* printStatus(Baker* self, char* status) {
    printf("%sBaker [%d] is [%s]%s\n", self->color, self->ID, status, CLEAR_COLOR);
}

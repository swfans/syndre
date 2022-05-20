#ifndef DISPLAY_H
#define DISPLAY_H

#include <SDL.h>
#include <stddef.h>

void display_lock (void);
void display_unlock (void);
const char * AppResourceMapping(short index);

#endif

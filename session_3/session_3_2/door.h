#ifndef DOOR_H
#define DOOR_H

/*
 * ComicParty ESP32 IoT door lock
 * Copyright (C) 2019 Mickey Chan. ALL RIGHTS RESERVED.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 * 
 * Develop for Arduino-ESP32
 * by Mickey Chan (developer AT comicparty.com)
 * 
 */

#include <Arduino.h>

// Variables
extern int reedState;                     // door closed by default
extern bool isDoorOpened;
extern unsigned long lastDoorBounceTime;  // Bounce time for reed sensor

// Functions
void doorSetup();
void handleDoorState();

#endif /* DOOR_H */


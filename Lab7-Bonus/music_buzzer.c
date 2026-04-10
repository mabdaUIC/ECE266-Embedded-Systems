#include "music_buzzer.h"

const Note melody[] = {
    { NOTE_C4, 500 },
    { NOTE_C4, 500 },
    { NOTE_G4, 500 },
    { NOTE_G4, 500 },
    { NOTE_A4, 500 },
    { NOTE_A4, 500 },
    { NOTE_G4, 1000 },
    { NOTE_F4, 500 },
    { NOTE_F4, 500 },
    { NOTE_E4, 500 },
    { NOTE_E4, 500 },
    { NOTE_D4, 500 },
    { NOTE_D4, 500 },
    { NOTE_C4, 1000 },
    { 0, 2000 }
};

const uint32_t MELODY_LENGTH = sizeof(melody) / sizeof(melody[0]);

#ifndef SONG_LIBRARY_H_
#define SONG_LIBRARY_H_

#include <stdint.h>
#include "music.h"   // for NUM_PITCH

// A single note: pitch index (0..NUM_PITCH-1) and duration in milliseconds
typedef struct {
    uint8_t pitch;      // index 0 = C4, 1 = D4, ..., 6 = B4
    uint16_t duration;  // ms
} Note;

// A song: array of notes + length + default tempo factor (optional)
typedef struct {
    const Note *notes;
    uint16_t length;
    const char *name;
} Song;

// Categories of songs
typedef enum {
    CAT_ROCK,
    CAT_CLASSIC,
    CAT_COUNTRY,
    CAT_POP,
    NUM_CATEGORIES
} Category;

// Each category holds an array of song pointers and a count
typedef struct {
    const Song **songs;
    uint8_t count;
    const char *name;
    uint32_t led_color;   // RGB color for category (packed: 0x00RRGGBB)
} CategoryInfo;

// Global song library (defined in song_library.c)
extern const CategoryInfo g_categories[NUM_CATEGORIES];

#endif /* SONG_LIBRARY_H_ */

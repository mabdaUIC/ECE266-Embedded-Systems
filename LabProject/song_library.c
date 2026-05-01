#include "song_library.h"
#include "music_player.h"   // for PITCH_REST

// ====================================================================
// ROCK CATEGORY – original 2 + 4 new ones -> 6 total
// ====================================================================

// Original: "Come Out and Play" (The Offspring)
static const Note rock1_notes[] = {
    {4, 300}, {4, 150}, {3, 150}, {4, 300},   // G G F# G
    {2, 300}, {2, 150}, {1, 150}, {2, 300},   // E E D E
    {4, 300}, {4, 150}, {3, 150}, {4, 300},
    {2, 300}, {2, 150}, {1, 150}, {2, 600}
};

// Original: "Wash It All Away" (Five Finger Death Punch)
static const Note rock2_notes[] = {
    {2, 400}, {1, 400}, {0, 400}, {6, 600},   // E D C B
    {2, 400}, {1, 400}, {0, 400}, {6, 600}
};

// NEW: "Smoke on the Water" (Deep Purple, 1972) – iconic riff
// Pitches: C-F-A, C-F-Bb-A, C-F-A, F-C   (C=0, F=3, A=5, Bb approx. 6)
static const Note rock3_notes[] = {
    {0, 300}, {3, 300}, {5, 300},
    {0, 300}, {3, 300}, {6, 150}, {5, 150},
    {0, 300}, {3, 300}, {5, 300},
    {3, 300}, {0, 600}
};

// NEW: "Back in Black" (AC/DC, 1980) – main riff
// Pitches: E D A, E D A, …   (E=2, D=1, A=5)
static const Note rock4_notes[] = {
    {2, 250}, {1, 250}, {5, 250},
    {2, 250}, {1, 250}, {5, 250},
    {2, 250}, {1, 250}, {5, 500},
    {2, 250}, {1, 250}, {5, 250},
    {2, 250}, {1, 250}, {5, 250},
    {2, 250}, {1, 250}, {5, 500}
};

// NEW: "Sweet Child O' Mine" (Guns N' Roses, 1987) – intro riff
// Approximate: A D C G A G …   (A=5, D=1, C=0, G=4)
static const Note rock5_notes[] = {
    {5, 300}, {1, 300}, {0, 300}, {4, 300},
    {5, 300}, {4, 300},
    {5, 150}, {1, 150}, {0, 150}, {4, 150},
    {5, 150}, {4, 150},
    {5, 300}, {1, 300}, {0, 300}, {4, 300},
    {5, 300}, {4, 600}
};

// NEW: "Iron Man" (Black Sabbath, 1970) – main riff
// C, B, C, D, E, F#(6?), G… simplified: 0 6 0 1 2 6 4 (approximate)
static const Note rock6_notes[] = {
    {0, 400}, {6, 400}, {0, 400}, {1, 400},
    {2, 400}, {6, 400}, {4, 800},
    {0, 400}, {6, 400}, {0, 400}, {1, 400},
    {2, 400}, {6, 400}, {4, 800}
};

// ====================================================================
// CLASSIC CATEGORY – original 1 + 2 new -> 3 total
// ====================================================================

// Original: "Für Elise" (Beethoven)
static const Note classic_notes[] = {
    {2, 250}, {3, 250}, {2, 250}, {3, 250},
    {2, 250}, {0, 250}, {4, 250}, {2, 500},
    {1, 250}, {2, 500}, {0, 500}
};

// NEW: "Ode to Joy" (Beethoven, 1824) – theme
static const Note classic2_notes[] = {
    {2, 300}, {2, 300}, {3, 300}, {4, 300},
    {4, 300}, {3, 300}, {2, 300}, {1, 300},
    {0, 300}, {0, 300}, {1, 300}, {2, 300},
    {2, 450}, {1, 150}, {1, 600}
};

// NEW: "Canon in D" (Pachelbel) – simplified bass line
static const Note classic3_notes[] = {
    {0, 500}, {4, 500}, {5, 500}, {2, 500},
    {1, 500}, {0, 500}, {1, 500}, {4, 500},
    {0, 500}, {4, 500}, {5, 500}, {2, 500},
    {1, 500}, {0, 500}, {1, 500}, {4, 500}
};

// ====================================================================
// COUNTRY CATEGORY – original 2 + 1 new -> 3 total
// ====================================================================

// Original: "Watermelon Crawl" (Tracy Byrd)
static const Note country1_notes[] = {
    {4, 400}, {4, 400}, {2, 400}, {2, 400},
    {0, 400}, {0, 400}, {1, 400}, {1, 800},
    {4, 400}, {4, 400}, {2, 400}, {2, 400},
    {0, 400}, {0, 400}, {1, 400}, {1, 800}
};

// Original: "Hurt" (Johnny Cash)
static const Note country2_notes[] = {
    {0, 500}, {1, 500}, {2, 500}, {2, 500},
    {2, 500}, {1, 500}, {0, 500}, {0, 1000},
    {0, 500}, {1, 500}, {2, 500}, {2, 500},
    {2, 500}, {1, 500}, {0, 500}, {0, 1000}
};

// NEW: "Jolene" (Dolly Parton, 1973) – chorus melody
static const Note country3_notes[] = {
    {0, 300}, {0, 300}, {1, 300}, {2, 300},
    {2, 300}, {2, 300}, {1, 300}, {0, 600},
    {0, 300}, {0, 300}, {1, 300}, {2, 300},
    {2, 300}, {2, 300}, {1, 300}, {0, 600}
};

// ====================================================================
// POP CATEGORY – original 2 + 1 new -> 3 total
// ====================================================================

// Original: "Godzilla" (Eminem)
static const Note pop1_notes[] = {
    {4, 200}, {5, 200}, {4, 200}, {5, 200},
    {4, 200}, {2, 400}, {0, 400},
    {4, 200}, {5, 200}, {4, 200}, {5, 200},
    {4, 200}, {2, 400}, {0, 400}
};

// Original: "God Slap" (Eminem / Juice WRLD)
static const Note pop2_notes[] = {
    {5, 300}, {4, 300}, {2, 300}, {0, 300},
    {5, 300}, {4, 300}, {2, 300}, {0, 600}
};

// NEW: "Shape of You" (Ed Sheeran, 2017) – marimba hook
static const Note pop3_notes[] = {
    {1, 200}, {0, 200}, {1, 200}, {0, 200},
    {2, 200}, {0, 200}, {2, 200}, {0, 200},
    {3, 400}, {2, 200}, {1, 200}, {0, 400}
};

// ====================================================================
// SONG DEFINITIONS
// ====================================================================

// Rock
static const Song rock1_song = { rock1_notes, sizeof(rock1_notes)/sizeof(Note), "Come Out and Play" };
static const Song rock2_song = { rock2_notes, sizeof(rock2_notes)/sizeof(Note), "Wash It All Away" };
static const Song rock3_song = { rock3_notes, sizeof(rock3_notes)/sizeof(Note), "Smoke on the Water" };
static const Song rock4_song = { rock4_notes, sizeof(rock4_notes)/sizeof(Note), "Back in Black" };
static const Song rock5_song = { rock5_notes, sizeof(rock5_notes)/sizeof(Note), "Sweet Child O' Mine" };
static const Song rock6_song = { rock6_notes, sizeof(rock6_notes)/sizeof(Note), "Iron Man" };

// Classic
static const Song classic_song  = { classic_notes, sizeof(classic_notes)/sizeof(Note), "Fur Elise" };
static const Song classic2_song = { classic2_notes, sizeof(classic2_notes)/sizeof(Note), "Ode to Joy" };
static const Song classic3_song = { classic3_notes, sizeof(classic3_notes)/sizeof(Note), "Canon in D" };

// Country
static const Song country1_song = { country1_notes, sizeof(country1_notes)/sizeof(Note), "Watermelon Crawl" };
static const Song country2_song = { country2_notes, sizeof(country2_notes)/sizeof(Note), "Hurt" };
static const Song country3_song = { country3_notes, sizeof(country3_notes)/sizeof(Note), "Jolene" };

// Pop
static const Song pop1_song = { pop1_notes, sizeof(pop1_notes)/sizeof(Note), "Godzilla" };
static const Song pop2_song = { pop2_notes, sizeof(pop2_notes)/sizeof(Note), "God Slap" };
static const Song pop3_song = { pop3_notes, sizeof(pop3_notes)/sizeof(Note), "Shape of You" };

// ====================================================================
// CATEGORY LISTS
// ====================================================================

static const Song* rock_songs[]    = { &rock1_song, &rock2_song, &rock3_song, &rock4_song, &rock5_song, &rock6_song };
static const Song* classic_songs[] = { &classic_song, &classic2_song, &classic3_song };
static const Song* country_songs[] = { &country1_song, &country2_song, &country3_song };
static const Song* pop_songs[]     = { &pop1_song, &pop2_song, &pop3_song };

const CategoryInfo g_categories[NUM_CATEGORIES] = {
    [CAT_ROCK]    = { rock_songs,    sizeof(rock_songs)/sizeof(rock_songs[0]),    "Rock",    0x00FF0000 }, // Red
    [CAT_CLASSIC] = { classic_songs, sizeof(classic_songs)/sizeof(classic_songs[0]), "Classic", 0x0000FF00 }, // Green
    [CAT_COUNTRY] = { country_songs, sizeof(country_songs)/sizeof(country_songs[0]), "Country", 0x00FFFF00 }, // Yellow
    [CAT_POP]     = { pop_songs,     sizeof(pop_songs)/sizeof(pop_songs[0]),     "Pop",     0x000000FF }  // Blue
};

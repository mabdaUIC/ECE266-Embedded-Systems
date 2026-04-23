#include "song_library.h"
#include "music_player.h"   // for PITCH_REST

// ---------- Rock: "Come Out and Play" (The Offspring) ----------
static const Note rock1_notes[] = {
    {4, 300}, {4, 150}, {3, 150}, {4, 300},   // G G F# G
    {2, 300}, {2, 150}, {1, 150}, {2, 300},   // E E D E
    {4, 300}, {4, 150}, {3, 150}, {4, 300},
    {2, 300}, {2, 150}, {1, 150}, {2, 600}
};

// Rock: "Wash It All Away" (Five Finger Death Punch)
static const Note rock2_notes[] = {
    {2, 400}, {1, 400}, {0, 400}, {6, 600},   // E D C B
    {2, 400}, {1, 400}, {0, 400}, {6, 600}
};

// ---------- Classic: "Für Elise" (Beethoven) ----------
static const Note classic_notes[] = {
    {2, 250}, {3, 250}, {2, 250}, {3, 250},   // E D# E D#
    {2, 250}, {0, 250}, {4, 250}, {2, 500},   // E C G E
    {1, 250}, {2, 500}, {0, 500}              // D E C
};

// ---------- Country: "Watermelon Crawl" (Tracy Byrd) ----------
static const Note country1_notes[] = {
    {4, 400}, {4, 400}, {2, 400}, {2, 400},
    {0, 400}, {0, 400}, {1, 400}, {1, 800},
    {4, 400}, {4, 400}, {2, 400}, {2, 400},
    {0, 400}, {0, 400}, {1, 400}, {1, 800}
};

// Country: "Hurt" (Johnny Cash)
static const Note country2_notes[] = {
    {0, 500}, {1, 500}, {2, 500}, {2, 500},
    {2, 500}, {1, 500}, {0, 500}, {0, 1000},
    {0, 500}, {1, 500}, {2, 500}, {2, 500},
    {2, 500}, {1, 500}, {0, 500}, {0, 1000}
};

// ---------- Pop: "Godzilla" (Eminem) ----------
static const Note pop1_notes[] = {
    {4, 200}, {5, 200}, {4, 200}, {5, 200},
    {4, 200}, {2, 400}, {0, 400},
    {4, 200}, {5, 200}, {4, 200}, {5, 200},
    {4, 200}, {2, 400}, {0, 400}
};

// Pop: "God Slap" (Eminem / Juice WRLD)
static const Note pop2_notes[] = {
    {5, 300}, {4, 300}, {2, 300}, {0, 300},
    {5, 300}, {4, 300}, {2, 300}, {0, 600}
};

// ---------- Song definitions ----------
static const Song rock1_song = { rock1_notes, sizeof(rock1_notes)/sizeof(Note), "Come Out and Play" };
static const Song rock2_song = { rock2_notes, sizeof(rock2_notes)/sizeof(Note), "Wash It All Away" };
static const Song classic_song = { classic_notes, sizeof(classic_notes)/sizeof(Note), "Fur Elise" };
static const Song country1_song = { country1_notes, sizeof(country1_notes)/sizeof(Note), "Watermelon Crawl" };
static const Song country2_song = { country2_notes, sizeof(country2_notes)/sizeof(Note), "Hurt" };
static const Song pop1_song = { pop1_notes, sizeof(pop1_notes)/sizeof(Note), "Godzilla" };
static const Song pop2_song = { pop2_notes, sizeof(pop2_notes)/sizeof(Note), "God Slap" };

// Category lists
static const Song* rock_songs[] = { &rock1_song, &rock2_song };
static const Song* classic_songs[] = { &classic_song };
static const Song* country_songs[] = { &country1_song, &country2_song };
static const Song* pop_songs[] = { &pop1_song, &pop2_song };

const CategoryInfo g_categories[NUM_CATEGORIES] = {
    [CAT_ROCK]    = { rock_songs,    2, "Rock",    0x00FF0000 }, // Red
    [CAT_CLASSIC] = { classic_songs, 1, "Classic", 0x0000FF00 }, // Green
    [CAT_COUNTRY] = { country_songs, 2, "Country", 0x00FFFF00 }, // Yellow
    [CAT_POP]     = { pop_songs,     2, "Pop",     0x000000FF }  // Blue
};

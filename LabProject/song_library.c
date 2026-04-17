#include "song_library.h"

// ---------- Rock Song: "Come Out and Play" (simplified excerpt) ----------
static const Note rock_notes[] = {
    {4, 400},   // G4
    {4, 200},   // G4
    {5, 400},   // A4
    {4, 400},   // G4
    {2, 400},   // E4
    {2, 400},   // E4
    {4, 800}    // G4
};

// ---------- Classic Song: "Piano melody (Für Elise excerpt)" ----------
static const Note classic_notes[] = {
    {2, 300}, {3, 300}, {2, 300}, {3, 300}, {2, 300}, {0, 300}, {4, 300}, {2, 300}, {1, 600}
};

// ---------- Country Song: "Watermelon Crawl" (simplified) ----------
static const Note country_notes[] = {
    {5, 500}, {4, 500}, {2, 500}, {0, 500}, {4, 500}, {5, 500}, {4, 1000}
};

// ---------- Pop Song: "Godzilla" (Eminem) riff ----------
static const Note pop_notes[] = {
    {4, 200}, {5, 200}, {4, 200}, {5, 200}, {4, 200}, {2, 400}, {0, 400}
};

// ---------- Song definitions ----------
static const Song rock_song = { rock_notes, sizeof(rock_notes)/sizeof(Note), "Come Out and Play" };
static const Song classic_song = { classic_notes, sizeof(classic_notes)/sizeof(Note), "Fur Elise" };
static const Song country_song = { country_notes, sizeof(country_notes)/sizeof(Note), "Watermelon Crawl" };
static const Song pop_song = { pop_notes, sizeof(pop_notes)/sizeof(Note), "Godzilla Riff" };

// ---------- Category song lists ----------
static const Song* rock_songs[] = { &rock_song };
static const Song* classic_songs[] = { &classic_song };
static const Song* country_songs[] = { &country_song };
static const Song* pop_songs[] = { &pop_song };

// ---------- Category information (colors in 0x00RRGGBB) ----------
const CategoryInfo g_categories[NUM_CATEGORIES] = {
    [CAT_ROCK]    = { rock_songs,    1, "Rock",    0x00FF0000 },  // Red
    [CAT_CLASSIC] = { classic_songs, 1, "Classic", 0x0000FF00 },  // Green
    [CAT_COUNTRY] = { country_songs, 1, "Country", 0x00FFFF00 },  // Yellow
    [CAT_POP]     = { pop_songs,     1, "Pop",     0x000000FF }   // Blue
};

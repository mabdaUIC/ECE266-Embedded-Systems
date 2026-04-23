/*
 * Jukebox Main – Interactive Melody Player
 * Uses Lab 7 event scheduler for periodic button checking and music updates.
 */

#include "launchpad.h"
#include "music.h"
#include "music_player.h"
#include "song_library.h"
#include "pwm_led.h"
#include "driverlib/gpio.h"

// ---------- UI States ----------
typedef enum {
    STATE_CATEGORY,     // browsing categories
    STATE_SONG,         // browsing songs within a category
    STATE_PLAYING       // song is playing or paused
} UIState;

static UIState ui_state = STATE_CATEGORY;
static uint8_t current_category = 0;      // 0 = Rock, 1 = Classic, etc.
static uint8_t current_song_index = 0;    // index within category's song list

// Button press tracking for long press detection
static uint32_t button_a_press_time = 0;
static uint32_t button_b_press_time = 0;
static bool button_a_held = false;
static bool button_b_held = false;
#define LONG_PRESS_MS   1000    // 1 second

// Event structures (reused from Lab 7)
Event button_check_event;
Event player_update_event;

// ---------- Low level button reading (active low) ----------
static inline int ReadSW1(void) {
    return !(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4) & GPIO_PIN_4);
}
static inline int ReadSW2(void) {
    return !(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0) & GPIO_PIN_0);
}

// ---------- Debouncing (returns 1 = pressed, 0 = released, -1 = unstable) ----------
static int DebounceSW1(void) {
    static int last = 0;
    static uint32_t last_change = 0;
    int now_state = ReadSW1();
    uint32_t now = EventGetCurrentTime();
    if (now_state != last) {
        last_change = now;
        last = now_state;
    }
    if (now - last_change >= 50)    // stable for 50 ms
        return last;
    return -1;
}
static int DebounceSW2(void) {
    static int last = 0;
    static uint32_t last_change = 0;
    int now_state = ReadSW2();
    uint32_t now = EventGetCurrentTime();
    if (now_state != last) {
        last_change = now;
        last = now_state;
    }
    if (now - last_change >= 50)
        return last;
    return -1;
}

// ---------- Show category color on RGB LED ----------
void ShowCategoryColor(uint8_t cat) {
    uint32_t color = g_categories[cat].led_color;   // packed 0x00RRGGBB
    uint8_t r = (color >> 16) & 0xFF;
    uint8_t g = (color >> 8)  & 0xFF;
    uint8_t b = color & 0xFF;
    // Scale 0 255 to PWM pulse width (max 100)
    int pw_r = (r * 100) / 255;
    int pw_g = (g * 100) / 255;
    int pw_b = (b * 100) / 255;
    PwmLedSet(5000, pw_r, pw_b, pw_g);
}

// ---------- Event: Check buttons (called every 20 ms) ----------
void ButtonCheckTask(Event *event) {
    uint32_t now = EventGetCurrentTime();
    int sw1 = DebounceSW1();
    int sw2 = DebounceSW2();

    // ----- SW1 (Button A) -----
    if (sw1 == 1) {                         // button pressed
        if (!button_a_held) {
            button_a_press_time = now;
            button_a_held = true;
        }
    } else if (sw1 == 0) {                  // button released
        if (button_a_held) {
            uint32_t duration = now - button_a_press_time;
            if (duration >= LONG_PRESS_MS) {
                // Long press A: Play/Pause
                if (ui_state == STATE_PLAYING) {
                    PlayerState ps = MusicPlayerGetState();
                    if (ps == PLAYER_PLAYING)
                        MusicPlayerPause();
                    else if (ps == PLAYER_PAUSED)
                        MusicPlayerResume();
                }
            } else {
                // Short press A
                if (ui_state == STATE_CATEGORY) {
                    // Next category
                    current_category = (current_category + 1) % NUM_CATEGORIES;
                    ShowCategoryColor(current_category);
                } else if (ui_state == STATE_SONG) {
                    // Next song in current category
                    const CategoryInfo *cat = &g_categories[current_category];
                    if (cat->count > 0)
                        current_song_index = (current_song_index + 1) % cat->count;
                }
            }
            button_a_held = false;
        }
    }

    // ----- SW2 (Button B) -----
    if (sw2 == 1) {
        if (!button_b_held) {
            button_b_press_time = now;
            button_b_held = true;
        }
    } else if (sw2 == 0) {
        if (button_b_held) {
            uint32_t duration = now - button_b_press_time;
            if (duration >= LONG_PRESS_MS) {
                // Long press B: Stop and return to category selection
                MusicPlayerStop();
                ui_state = STATE_CATEGORY;
                ShowCategoryColor(current_category);
            } else {
                // Short press B
                if (ui_state == STATE_CATEGORY) {
                    // Enter song selection for current category
                    const CategoryInfo *cat = &g_categories[current_category];
                    if (cat->count > 0) {
                        ui_state = STATE_SONG;
                        current_song_index = 0;
                        ShowCategoryColor(current_category);
                    }
                } else if (ui_state == STATE_SONG) {
                    // Play selected song
                    const CategoryInfo *cat = &g_categories[current_category];
                    const Song *song = cat->songs[current_song_index];
                    MusicPlayerPlaySong(song);
                    ui_state = STATE_PLAYING;
                }
            }
            button_b_held = false;
        }
    }

    // Re schedule this event for 20 ms later
    EventSchedule(&button_check_event, now + 20);
}

// ---------- Event: Update music player (called every 10 ms) ----------
void PlayerUpdateTask(Event *event) {
    MusicPlayerUpdate();

    // If song finished, go back to song selection
    if (ui_state == STATE_PLAYING && MusicPlayerGetState() == PLAYER_STOPPED) {
        ui_state = STATE_SONG;
        ShowCategoryColor(current_category);
    }

    // Re schedule for 10 ms later
    EventSchedule(&player_update_event, event->time + 10);
}

// ---------- Main ----------
    int main(void) {
    LaunchPadInit();            // Sets up 50 MHz clock, GPIO, UART, event scheduler
    IntMasterEnable();          // Enable global interrupts (SysTick already running)

    // Configure push buttons (PF0 and PF4) as inputs with pull ups
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    // Unlock PF0 (required because it can be NMI)
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= 0x01;
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0;
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4 | GPIO_PIN_0);
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4 | GPIO_PIN_0,
                     GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    // Initialize music modules
    MusicInitLed();
    MusicInitBuzzer();
    MusicPlayerInit();

    // Set up event driven tasks
    EventInit(&button_check_event, ButtonCheckTask);
    EventInit(&player_update_event, PlayerUpdateTask);
    EventSchedule(&button_check_event, 20);   // check buttons every 20 ms
    EventSchedule(&player_update_event, 10);  // update player every 10 ms

    // Start in category selection
    ui_state = STATE_CATEGORY;
    current_category = 0;
    ShowCategoryColor(current_category);      // Red = Rock

    // Main loop – sleep until next event, then execute callbacks
    while (1) {
        asm(" wfi");          // Wait For Interrupt (low power)
        EventExecute();       // Run any scheduled events
    }
}

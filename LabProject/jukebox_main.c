/*
 * jukebox_main.c – Interactive Jukebox with 7-Segment Display & Volume Knob
 *
 * Uses two pushbuttons (SW1, SW2) to browse song categories and tracks,
 * plays songs via speaker, shows status on RGB LED and a 4-digit
 * 7-segment display (Grove module on J6, TM1637).
 * Volume is controlled by a potentiometer on PE3 (J8 Grove connector).
 */

#include "launchpad.h"
#include "music.h"
#include "music_player.h"
#include "song_library.h"
#include "pwm_led.h"
#include "driverlib/gpio.h"
#include "seg7.h"
#include "volume_control.h"          // <-- NEW

// UI States
typedef enum {
    STATE_CATEGORY,
    STATE_SONG,
    STATE_PLAYING
} UIState;

static UIState ui_state = STATE_CATEGORY;
static uint8_t current_category = 0;      // 0 = Rock, 1 = Classic, 2 = Country, 3 = Pop
static uint8_t current_song_index = 0;    // index within the chosen category

// Button tracking
static uint32_t button_a_press_time = 0;
static uint32_t button_b_press_time = 0;
static bool button_a_held = false;
static bool button_b_held = false;
#define LONG_PRESS_MS   1000

// 7-segment display structure
static Seg7Display seg_display = {
    .digit = {0, 0, 0, 0},
    .colon_on = true
};

// Volume control (potentiometer)
uint8_t current_volume = 50;             // 0-100, default mid volume
#define VOLUME_UPDATE_INTERVAL_MS   50    // read pot every 50 ms
Event volume_update_event;

// Events
Event button_check_event;
Event player_update_event;

// ---------------------------------------------------------------------------
// Hardware helpers
// ---------------------------------------------------------------------------
static inline int ReadSW1(void) {
    return !(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4) & GPIO_PIN_4);
}
static inline int ReadSW2(void) {
    return !(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0) & GPIO_PIN_0);
}

// Debounced button reads (returns -1 if not stable)
static int DebounceSW1(void) {
    static int last = 0;
    static uint32_t last_change = 0;
    int now_state = ReadSW1();
    uint32_t now = EventGetCurrentTime();
    if (now_state != last) {
        last_change = now;
        last = now_state;
    }
    if (now - last_change >= 50)
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

// ---------------------------------------------------------------------------
// 7-segment display update function
// ---------------------------------------------------------------------------
void UpdateSegDisplay(void) {
    uint8_t cat_num = current_category + 1;     // user sees 1..4
    seg_display.digit[3] = (cat_num / 10) % 10;
    seg_display.digit[2] = cat_num % 10;
    seg_display.digit[1] = (current_song_index + 1) / 10;
    seg_display.digit[0] = (current_song_index + 1) % 10;
    Seg7Update(&seg_display);
}

// ---------------------------------------------------------------------------
// LED color helper
// ---------------------------------------------------------------------------
void ShowCategoryColor(uint8_t cat) {
    uint32_t color = g_categories[cat].led_color;
    uint8_t r = (color >> 16) & 0xFF;
    uint8_t g = (color >> 8)  & 0xFF;
    uint8_t b = color & 0xFF;
    int pw_r = (r * 100) / 255;
    int pw_g = (g * 100) / 255;
    int pw_b = (b * 100) / 255;
    PwmLedSet(5000, pw_r, pw_b, pw_g);
}

// ---------------------------------------------------------------------------
// Volume update task (NEW)
// ---------------------------------------------------------------------------
void VolumeUpdateTask(Event *event) {
    uint8_t new_volume = VolumeControlGetLevel();
    // Simple hysteresis to avoid jitter
    if (abs((int)new_volume - (int)current_volume) > 2) {
        current_volume = new_volume;
    }
    EventSchedule(&volume_update_event, event->time + VOLUME_UPDATE_INTERVAL_MS);
}

// ---------------------------------------------------------------------------
// Button check task
// ---------------------------------------------------------------------------
void ButtonCheckTask(Event *event) {
    uint32_t now = EventGetCurrentTime();
    int sw1 = DebounceSW1();
    int sw2 = DebounceSW2();

    // --- SW1 (Button A) ---
    if (sw1 == 1) {
        if (!button_a_held) {
            button_a_press_time = now;
            button_a_held = true;
        }
    } else if (sw1 == 0) {
        if (button_a_held) {
            uint32_t duration = now - button_a_press_time;
            if (duration >= LONG_PRESS_MS) {
                // Long press: Play/Pause when playing
                if (ui_state == STATE_PLAYING) {
                    PlayerState ps = MusicPlayerGetState();
                    if (ps == PLAYER_PLAYING)
                        MusicPlayerPause();
                    else if (ps == PLAYER_PAUSED)
                        MusicPlayerResume();
                }
            } else {
                // Short press: next category or next song
                if (ui_state == STATE_CATEGORY) {
                    current_category = (current_category + 1) % NUM_CATEGORIES;
                    ShowCategoryColor(current_category);
                    UpdateSegDisplay();
                } else if (ui_state == STATE_SONG) {
                    const CategoryInfo *cat = &g_categories[current_category];
                    if (cat->count > 0) {
                        current_song_index = (current_song_index + 1) % cat->count;
                        UpdateSegDisplay();
                    }
                }
            }
            button_a_held = false;
        }
    }

    // --- SW2 (Button B) ---
    if (sw2 == 1) {
        if (!button_b_held) {
            button_b_press_time = now;
            button_b_held = true;
        }
    } else if (sw2 == 0) {
        if (button_b_held) {
            uint32_t duration = now - button_b_press_time;
            if (duration >= LONG_PRESS_MS) {
                // Long press: Stop and return to category selection
                MusicPlayerStop();
                ui_state = STATE_CATEGORY;
                ShowCategoryColor(current_category);
                UpdateSegDisplay();
            } else {
                // Short press: enter song selection or play selected song
                if (ui_state == STATE_CATEGORY) {
                    const CategoryInfo *cat = &g_categories[current_category];
                    if (cat->count > 0) {
                        ui_state = STATE_SONG;
                        current_song_index = 0;
                        ShowCategoryColor(current_category);
                        UpdateSegDisplay();
                    }
                } else if (ui_state == STATE_SONG) {
                    const CategoryInfo *cat = &g_categories[current_category];
                    const Song *song = cat->songs[current_song_index];
                    MusicPlayerPlaySong(song);
                    ui_state = STATE_PLAYING;
                    UpdateSegDisplay();   // keep showing track during playback
                }
            }
            button_b_held = false;
        }
    }

    EventSchedule(&button_check_event, now + 20);
}

void PlayerUpdateTask(Event *event) {
    MusicPlayerUpdate();

    if (ui_state == STATE_PLAYING && MusicPlayerGetState() == PLAYER_STOPPED) {
        ui_state = STATE_SONG;
        ShowCategoryColor(current_category);
        UpdateSegDisplay();
    }

    EventSchedule(&player_update_event, event->time + 10);
}

// ---------------------------------------------------------------------------
// Main
// ---------------------------------------------------------------------------
int main(void) {
    LaunchPadInit();
    Seg7Init();                     // 7-segment display (PD0, PD1 on J6)
    VolumeControlInit();            // Potentiometer (PE3 on J8)  <-- NEW
    IntMasterEnable();

    // Configure buttons (SW1 on PF4, SW2 on PF0)
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= 0x01;
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0;
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4 | GPIO_PIN_0);
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4 | GPIO_PIN_0,
                     GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    MusicInitLed();
    MusicInitBuzzer();
    MusicPlayerInit();

    // Prepare events
    EventInit(&button_check_event, ButtonCheckTask);
    EventInit(&player_update_event, PlayerUpdateTask);
    EventInit(&volume_update_event, VolumeUpdateTask);   // <-- NEW

    EventSchedule(&button_check_event, 20);
    EventSchedule(&player_update_event, 10);
    EventSchedule(&volume_update_event, VOLUME_UPDATE_INTERVAL_MS);   // <-- NEW

    // Initial UI state
    ui_state = STATE_CATEGORY;
    current_category = 0;
    current_song_index = 0;
    ShowCategoryColor(current_category);
    UpdateSegDisplay();

    while (1) {
        asm(" wfi");
        EventExecute();
    }
}

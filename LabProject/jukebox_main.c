/*
 * Jukebox Main – Instrumented for Debugging
 */

#include "launchpad.h"
#include "music.h"
#include "music_player.h"
#include "song_library.h"
#include "pwm_led.h"
#include "driverlib/gpio.h"

// UI States
typedef enum {
    STATE_CATEGORY,
    STATE_SONG,
    STATE_PLAYING
} UIState;

static UIState ui_state = STATE_CATEGORY;
static uint8_t current_category = 0;
static uint8_t current_song_index = 0;

// Button tracking
static uint32_t button_a_press_time = 0;
static uint32_t button_b_press_time = 0;
static bool button_a_held = false;
static bool button_b_held = false;
#define LONG_PRESS_MS   1000

Event check_push_button_event;
Event player_update_event;

void CheckPushButton(Event *event);
void PlayerUpdateTask(Event *event);

// ---------- Debug GPIO Helpers ----------
static void DebugInit(void) {
    // Unlock PF0 and configure PF0, PF1, PF2 as outputs
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= 0x01;
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2);
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2, 0);
}

static void TogglePF0(void) {
    static int toggle = 0;
    toggle = !toggle;
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, toggle ? GPIO_PIN_0 : 0);
}

static void TogglePF1(void) {
    static int toggle = 0;
    toggle = !toggle;
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, toggle ? GPIO_PIN_1 : 0);
}

static void TogglePF2(void) {
    static int toggle = 0;
    toggle = !toggle;
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, toggle ? GPIO_PIN_2 : 0);
}
// -----------------------------------------

// Read buttons directly (active low)
static uint8_t ReadButtonsRaw(void) {
    uint8_t state = 0;
    uint32_t pins = GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4 | GPIO_PIN_0);
    if (!(pins & GPIO_PIN_4))  state |= 1;   // SW1
    if (!(pins & GPIO_PIN_0))  state |= 2;   // SW2
    return state;
}

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

void CheckPushButton(Event *event) {
    TogglePF0();   // Blink green LED at 25 Hz if called every 20 ms

    uint32_t now = event->time;
    uint8_t buttons = ReadButtonsRaw();

    // SW1 (Button A)
    if (buttons & 1) {
        if (!button_a_held) {
            button_a_press_time = now;
            button_a_held = true;
        }
    } else {
        if (button_a_held) {
            uint32_t duration = now - button_a_press_time;
            if (duration >= LONG_PRESS_MS) {
                if (ui_state == STATE_PLAYING) {
                    PlayerState ps = MusicPlayerGetState();
                    if (ps == PLAYER_PLAYING)
                        MusicPlayerPause();
                    else if (ps == PLAYER_PAUSED)
                        MusicPlayerResume();
                }
            } else {
                if (ui_state == STATE_CATEGORY) {
                    current_category = (current_category + 1) % NUM_CATEGORIES;
                    ShowCategoryColor(current_category);
                } else if (ui_state == STATE_SONG) {
                    const CategoryInfo *cat = &g_categories[current_category];
                    if (cat->count > 0)
                        current_song_index = (current_song_index + 1) % cat->count;
                }
            }
            button_a_held = false;
        }
    }

    // SW2 (Button B)
    if (buttons & 2) {
        if (!button_b_held) {
            button_b_press_time = now;
            button_b_held = true;
        }
    } else {
        if (button_b_held) {
            uint32_t duration = now - button_b_press_time;
            if (duration >= LONG_PRESS_MS) {
                MusicPlayerStop();
                ui_state = STATE_CATEGORY;
                ShowCategoryColor(current_category);
            } else {
                if (ui_state == STATE_CATEGORY) {
                    const CategoryInfo *cat = &g_categories[current_category];
                    if (cat->count > 0) {
                        ui_state = STATE_SONG;
                        current_song_index = 0;
                        ShowCategoryColor(current_category);
                    }
                } else if (ui_state == STATE_SONG) {
                    const CategoryInfo *cat = &g_categories[current_category];
                    const Song *song = cat->songs[current_song_index];
                    MusicPlayerPlaySong(song);
                    ui_state = STATE_PLAYING;
                }
            }
            button_b_held = false;
        }
    }

    EventSchedule(&check_push_button_event, now + 20);
}

void PlayerUpdateTask(Event *event) {
    TogglePF1();   // Blink red LED at 50 Hz if called every 10 ms

    MusicPlayerUpdate();

    if (ui_state == STATE_PLAYING && MusicPlayerGetState() == PLAYER_STOPPED) {
        ui_state = STATE_SONG;
        ShowCategoryColor(current_category);
    }

    EventSchedule(&player_update_event, event->time + 10);
}

int main(void) {
    LaunchPadInit();
    IntMasterEnable();              // Essential!

    DebugInit();                    // Set up PF0, PF1, PF2 for debugging

    MusicInitLed();
    MusicInitBuzzer();
    MusicPlayerInit();

    EventInit(&check_push_button_event, CheckPushButton);
    EventInit(&player_update_event, PlayerUpdateTask);
    EventSchedule(&check_push_button_event, 20);
    EventSchedule(&player_update_event, 10);

    ui_state = STATE_CATEGORY;
    current_category = 0;
    ShowCategoryColor(current_category);

    while (1) {
        TogglePF2();   // Blink blue LED at loop rate (should be very fast, may appear dim)
        asm(" wfi");
        EventExecute();
    }
}

/*
 * Integrated Test: Auto Play First Song with Debug Toggles
 */

#include "launchpad.h"
#include "music.h"
#include "music_player.h"
#include "song_library.h"
#include "pwm_led.h"
#include "driverlib/gpio.h"

// ---------- Debug GPIO Helpers ----------
static void DebugInit(void) {
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

// Event for player update
Event player_update_event;

void PlayerUpdateTask(Event *event) {
    TogglePF1();   // Blink red LED at ~50 Hz (if called every 10 ms)

    MusicPlayerUpdate();

    // Check if song finished
    if (MusicPlayerGetState() == PLAYER_STOPPED) {
        // Restart the same song for continuous testing
        const Song *testSong = g_categories[0].songs[0];
        MusicPlayerPlaySong(testSong);
    }

    EventSchedule(&player_update_event, event->time + 10);
}

int main(void) {
    LaunchPadInit();
    IntMasterEnable();

    DebugInit();
    MusicInitLed();
    MusicInitBuzzer();
    MusicPlayerInit();

    // Schedule player update event
    EventInit(&player_update_event, PlayerUpdateTask);
    EventSchedule(&player_update_event, 10);

    // Start playing the first song immediately
    const Song *testSong = g_categories[0].songs[0];
    MusicPlayerPlaySong(testSong);

    while (1) {
        TogglePF2();   // Blink blue LED at loop rate (should be fast/dim)
        asm(" wfi");
        EventExecute();
    }
}

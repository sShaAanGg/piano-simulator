// Piano Simulator: play one octave with the computer keyboard.
// MVP scope: see "MVP" in PLAN.md.

#include <stdbool.h>
#include <stddef.h>

#include "raylib.h"

#define ARRAY_LEN(a) (sizeof(a) / sizeof((a)[0]))

#define WINDOW_WIDTH  960
#define WINDOW_HEIGHT 320

typedef struct {
    int key;            // raylib key code, e.g. KEY_A
    int midi;           // MIDI note number, middle C = 60
    bool is_black;      // black key on the piano?
    const char *sample; // path to the WAV file for this note
    bool down;          // is the key held right now?
    // TODO(milestone 2): what else does a note need to remember to play and fade out?
} PianoKey;

// TODO(milestone 3): fill in the rest of the octave (12 keys in total).
// Which computer keys feel natural for the white keys, and which for the black ones?
static PianoKey keys[] = {
    { KEY_A, 60, false, "samples/C4.wav", false },
};

static void note_on(PianoKey *k)
{
    k->down = true;
    // TODO(milestone 2): start this note's sound from the beginning.
}

static void note_off(PianoKey *k)
{
    k->down = false;
    // TODO(milestone 2): don't stop abruptly; start fading out instead.
}

static void update_audio(float dt)
{
    (void)dt;
    // TODO(milestone 2): advance every fading note's volume by dt; stop it once it reaches 0.
}

static void draw_keyboard(void)
{
    const int count = (int)ARRAY_LEN(keys);
    const int width = WINDOW_WIDTH / count;

    for (int i = 0; i < count; i++) {
        const PianoKey *k = &keys[i];
        Color fill = k->is_black ? BLACK : RAYWHITE;
        if (k->down) fill = SKYBLUE;

        DrawRectangle(i * width, 0, width - 2, WINDOW_HEIGHT, fill);
    }
}

int main(void)
{
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Piano Simulator");
    InitAudioDevice();
    SetTargetFPS(60);

    // TODO(milestone 2): load each key's sample here.

    while (!WindowShouldClose()) {
        for (size_t i = 0; i < ARRAY_LEN(keys); i++) {
            if (IsKeyPressed(keys[i].key))  note_on(&keys[i]);
            if (IsKeyReleased(keys[i].key)) note_off(&keys[i]);
        }

        update_audio(GetFrameTime());

        BeginDrawing();
        ClearBackground(DARKGRAY);
        draw_keyboard();
        EndDrawing();
    }

    // TODO(milestone 2): unload the samples here.

    CloseAudioDevice();
    CloseWindow();
    return 0;
}

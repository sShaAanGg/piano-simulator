# Piano Simulator — Development Plan

English | [繁體中文](PLAN.zh-TW.md)

> The English version is the source of truth. Update it first, then sync the Chinese translation.

## Goal

A small program for playing the piano with your computer keyboard, written in **C + raylib**, developed and run **natively on Windows 11**.

- The aim is something fun that you want to keep using, and that can be used to practice piano.
- **Latency from key press to sound** (the feel) is the most important quality metric.
- **Fun comes first; engineering practice is a bonus.** Verification is added where it helps, but it must never block development.

## Decisions made

| Decision | Reason |
|---|---|
| Use **C** (not C++) | No effect on latency; C is closer to the metal and has fewer dependencies, which suits writing from scratch |
| Use **sample playback** (recordings of a real piano), not synthesis | Skip the frequency math for now and focus on simulating a piano; samples sound like a real piano from day one |
| Develop **natively on Windows**, not in WSL | WSLg audio takes a detour to reach Windows, adding latency that hurts the feel |
| Toolchain: the official raylib Windows installer (includes the w64devkit compiler) | Compiles C out of the box, no MSVC setup needed |
| Draw the UI **directly with raylib**, no separate GUI framework | Piano keys are just rectangles; the screen is redrawn every frame, like a three.js render loop |
| Use **raygui** when buttons or menus are needed | A single header file that pairs with raylib, in line with keeping dependencies few |
| Import songs by **dragging a file into the window** | raylib has built-in `IsFileDropped` / `LoadDroppedFiles`, so no open-file dialog is needed |
| Use **MIDI** for songs, with a hand-written parser | MIDI is a list of events (time, pitch, velocity); the format is simple and a good exercise |
| **Commit the sample WAV files to the repo** | One octave is only a few MB; versioning them with the code means the source can't disappear or be tampered with. If the files get too big later (all 88 keys, multiple velocity layers), consider Git LFS or a download script (with SHA-256 checks) |
| License: **MIT** | Lets others use the code freely; raylib's zlib license is also permissive and compatible |
| Credit third-party assets and libraries in **one `CREDITS.md`** at the repo root | One easy-to-find place for every source and license (samples, raylib, raygui), instead of scattering it across the README and subfolders |
| Docs are bilingual, with **English as the source of truth** | Open-source readers mostly read English; one canonical version keeps the two from drifting apart |
| Chinese docs use **full-width punctuation** (`，` `。` `：` `（）`) | Reads naturally in Chinese |

## Architecture principle

**Separate input from sound.** Both keyboard presses and MIDI songs turn into the same set of events:

```
Keyboard input ──┐
                 ├──► note_on(pitch) / note_off(pitch) ──► sound + visuals
MIDI song ───────┘
```

- Pitches are identified internally by **MIDI note number** (middle C = 60).
- This way, autoplay, key hints and scoring can be added later without rewriting the sound code.

## Engineering practice (non-blocking)

Verification helps, but never gets in the way of building the fun parts:

- **Pushing straight to `main` is fine.** No required PRs or approvals.
- **CI only reports, it doesn't block.** Once there is code, CI builds it on a Windows runner and runs the unit tests; a red check is a hint to fix soon, not a wall.
- **Test only the pure logic**, where the same input always gives the same output: the note state machine (press, hold, fade-out, sustain pedal), key mapping, and the MIDI parser. Feel and latency are judged by playing, not by unit tests.
- **Write properties first only when it's worth it**, for example before the fade-out logic or the MIDI parser, not for every change.
- **Bonus exercises**, whenever they sound fun:
  - Fuzz the MIDI parser with libFuzzer + AddressSanitizer
  - Measure key-to-sound latency (for example by recording the key click and the note with a microphone)
  - Pass notes from the main thread to the audio thread safely without blocking locks (real concurrency practice)

## How the piano sound behaves

| Event | What the program does |
|---|---|
| Key pressed | Play this key's recording from the start |
| Key held | Keep playing and let the natural decay in the recording happen (a held piano note also fades) |
| Key released | **Don't stop abruptly**: ramp the volume down to 0 over a fraction of a second, then stop (like the damper returning to the string). Stopping abruptly causes a click |
| Sustain pedal (space bar) | While held, released keys don't fade out; they all fade out together when the pedal is released |

## Milestones

1. **[Next] Environment**: install raylib for Windows and compile an empty window
2. **First sound**: press A to play a real piano C; release to fade out. Use the `note_on` / `note_off` architecture from this step on
3. **One octave**: 12 keys, each mapped to a WAV file committed to the repo; several keys can sound at once (polyphony)
4. **Sustain pedal**: space bar
5. **Visuals**: draw a piano keyboard with raylib, with animations or particle effects on key press
6. **Practice mode**:
   1. Drag a MIDI file into the window and read its notes with a hand-written parser
   2. Note blocks fall from the top; when one reaches the keyboard, it's time to press that key
   3. (Optional) Scoring: compare actual key-press times to note times and rate accuracy, like a rhythm game

## MVP (minimum viable product)

**Scope**: play one octave with the computer keyboard; notes fade out on release; 12 rectangles on screen light up when pressed. Roughly milestones 1–3 plus a bare-bones slice of milestone 5.

**Out of scope for the MVP**: sustain pedal, nice visuals and particles, MIDI and practice mode, writing samples to the audio stream yourself.

**What to learn** (raylib's `Sound` already handles mixing and the audio thread, so none of PCM, ring buffers or threads is needed yet):

| # | Topic | Concretely |
|---|---|---|
| 1 | raylib game loop | `InitWindow` → `while (!WindowShouldClose())` → `BeginDrawing` / `EndDrawing` |
| 2 | Keyboard input | `IsKeyPressed` (just pressed), `IsKeyReleased` (just released) |
| 3 | Playing sounds | `InitAudioDevice`, `LoadSound`, `PlaySound`, `SetSoundVolume`, `StopSound` |
| 4 | Fade-out | After release, lower the volume a bit every frame, using `GetFrameTime()` to know how much; stop at 0 |
| 5 | The 12 keys of an octave | Where the black keys go; find a CC0 sample set with a recording for every key |

**Known MVP trade-off**: the volume changes once per frame (about every 16 ms at 60 FPS), so the fade is a staircase rather than smooth. Accept it for the MVP; revisit if it clicks.

## Things to watch out for

- **Choosing a sample library**: many free piano sample libraries record only every few keys, and the notes in between have to be pitch-shifted, which brings the frequency math back. Pick a library that records every key, or start with one octave and one WAV per key. **Because the project is open source and the files are committed to the repo, the sample license must allow redistribution** (for example CC0 or CC-BY), and the source and license must be credited in the repo.
- **Keyboard rollover limits** (ghosting / key rollover): many keyboards can't detect many keys pressed at once, so some notes in a chord may not sound. This is a hardware limit of the keyboard, not a bug in the program.
- **The parser must handle broken files**: MIDI files are dragged in by the user and may be malformed or even deliberately crafted. A parser written in C must check every length field against the file's bounds before reading, or it may read outside memory, crashing the program or creating a security hole.

## Open questions

- What layers does sound pass through from the program to the speakers? Why does each extra layer add a bit of latency?
- For polyphony, the program must track which notes are sounding and where each is in its fade-out. If stored in a fixed-size C array, how many slots are enough?
- How should a MIDI note number map to a key on the computer keyboard and to the WAV file to play? Should these two mappings live in one table or two?
- What does checking downloaded files with SHA-256 protect against, and what doesn't it protect against?
- (Optional, skipped for now) By what factor does the frequency change for each semitone up?

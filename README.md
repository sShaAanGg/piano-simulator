# Piano Simulator

English | [繁體中文](README.zh-TW.md)

Play the piano with your computer keyboard. Written from scratch in C with [raylib](https://www.raylib.com/), with a focus on **feel**: the delay between pressing a key and hearing the note should be as low as possible.

> 🚧 This project has just started and there is nothing to run yet. See [PLAN.md](PLAN.md) for progress and design notes.

## Planned features

- 🎹 Play with your computer keyboard, using recordings of a real piano
- 🎶 Press several keys at once to play chords
- 🦶 Space bar as the sustain pedal
- ✨ A drawn piano keyboard with animations and effects when keys are pressed
- 📖 Practice mode: drag a MIDI file into the window, and notes fall from the top to show which key to press

## Building

Requirements:

- Windows 11
- [raylib Windows installer](https://raysan5.itch.io/raylib) (includes the w64devkit compiler), installed at `C:\raylib`. If it's somewhere else, set `RAYLIB_DIR`.

From the repo root:

```
build.bat
build\piano.exe
```

Run it from the repo root, because samples are loaded from `samples/`.

## License

[MIT](LICENSE)

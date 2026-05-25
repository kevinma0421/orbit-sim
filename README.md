# Satellite Visualizer

A real-time 3D satellite constellation visualizer built with C++ and OpenGL. Load any set of satellites from TLE data and watch them orbit Earth in real time.

## Features

- 3D Earth with up to 10K texture, cloud layer, axial tilt, and Phong lighting
- Thousands of satellites rendered in real time using live TLE orbital data
- Physically accurate orbits computed from Keplerian elements (Kepler's equation via Newton-Raphson)
- Controllable simulation speed — pause, play, or fast-forward time
- Orbital camera with mouse drag and scroll-to-zoom
- Star skybox background
- GUI controls and FPS counter via ImGui

## How It Works

TLE (Two-Line Element) data is fetched from an external API (rate limited to once per 2 hours, cached locally) and parsed into orbital elements. A Python preprocessing script solves Kepler's equation for each satellite and precomputes 100-point orbit paths, writing them to a binary file. The C++ application loads this at startup and interpolates satellite positions each frame based on mean motion and elapsed simulation time.

The simulation speed is tied to Earth's actual sidereal rotation rate (86,164 seconds), so satellites and Earth rotate in sync at any speed multiplier.

## Dependencies

- [GLFW](https://www.glfw.org/) — window and input
- [GLAD](https://glad.dav1d.de/) — OpenGL function loader
- [GLM](https://github.com/g-truc/glm) — math library
- [ImGui](https://github.com/ocornut/imgui) — GUI
- [stb_image](https://github.com/nothings/stb) — texture loading
- Python 3 + NumPy — orbit preprocessing

## Build

Update the library paths in `CMakeLists.txt` to match your local setup, then:

```bash
cmake -S . -B build
cmake --build build
```

Run the preprocessing script before first launch:

```bash
cd scripts
python orbits.py
```

## Controls

| Input | Action |
|---|---|
| Left mouse drag | Rotate camera |
| Scroll wheel | Zoom in / out |
| Play / Pause / Forward | Control simulation speed |

## Upcoming

- [ ] Docker setup to remove manual dependency configuration
- [ ] Satellite filtering by inclination, altitude, constellation, etc.
- [ ] Move TLE parsing and orbit computation into C++ — eliminate Python dependency
- [ ] Auto-refresh TLE data from API when cache is stale (2hr limit)
- [ ] Support any satellite constellation, not just Starlink
- [ ] Click a satellite to see its info (name, altitude, velocity, orbital period)
- [ ] Ground track projection onto Earth's surface
- [ ] More camera controls and keyboard shortcuts
- [ ] Night side Earth texture

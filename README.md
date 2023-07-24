# Game Collection

![Windows Build Status](https://img.shields.io/github/actions/workflow/status/hendrik-git/GameCollection/Windows.yml?logo=Windows&style=for-the-badge)
![Linux Build Status](https://img.shields.io/github/actions/workflow/status/hendrik-git/GameCollection/Linux.yml?logo=Linux&style=for-the-badge)
![License](https://img.shields.io/github/license/hendrik-git/GameCollection?style=for-the-badge)
![Language count](https://img.shields.io/github/languages/count/hendrik-git/GameCollection?style=for-the-badge)
![File count](https://img.shields.io/github/directory-file-count/hendrik-git/GameCollection?style=for-the-badge)
![Code size in bytes](https://img.shields.io/github/languages/code-size/hendrik-git/GameCollection?style=for-the-badge)

A bunch of games and tech demos collected in a single application for learning C++ and game design patterns. Uses SFML as graphics backend.

## Games

### Asteroids

A simple Asteroids implementation using an entity-component-system. The game includes game state tracking (and displaying in a HUD), sound effects and particle effects.

![AsteroidsGame](docs/images/Asteroids.gif)

### Splines

Implements a path consisting of Catmull–Rom splines. This path is used to move the ship along a trajectory defined by points. The rotation of the ship is derived from the gradient of the spline.

![AsteroidsGame](docs/images/Splines.gif)

## How to build

Tested with MSVC and Clang, uses C++20

```
git clone https://github.com/hendrik-git/GameCollection
cd GameCollection
cmake -S . -B build
cmake --build build --config Release --target GameCollectionApp
```

### Linux

The GameCollection can also be used with Linux. Though SFML requires the following dependencies:
```
libsfml-dev
libudev-dev
libopenal-dev
libvorbis-dev
libflac-dev
libxcursor-dev
libfreetype6-dev 
libx11-dev 
libxrandr-dev
```
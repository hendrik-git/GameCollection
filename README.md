# Game Collection

![Windows Build Status](https://img.shields.io/github/actions/workflow/status/hendrik-git/GameCollection/Windows.yml?logo=Windows&style=for-the-badge)
![Linux Build Status](https://img.shields.io/github/actions/workflow/status/hendrik-git/GameCollection/Linux.yml?logo=Linux&style=for-the-badge)
![License](https://img.shields.io/github/license/hendrik-git/GameCollection?style=for-the-badge)
![Language count](https://img.shields.io/github/languages/count/hendrik-git/GameCollection?style=for-the-badge)
![File count](https://img.shields.io/github/directory-file-count/hendrik-git/GameCollection?style=for-the-badge)
![Code size in bytes](https://img.shields.io/github/languages/code-size/hendrik-git/GameCollection?style=for-the-badge)

A bunch of games collected in a single application for learning C++ and game design patterns. Uses SFML as graphics backend.


![AsteroidsGame](docs/images/Asteroids01.png)

## How to build

Tested with MSVC and Clang, uses C++20

```
git clone https://github.com/hendrik-git/GameCollection

cd GameCollection
mkdir build
cd build

cmake -S .. -B .
cmake --build . --config Release --target GameCollectionApp
```
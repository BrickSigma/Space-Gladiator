# Space Galdiator
A retro-style, pixel-art themed space shooter game inspired by the NES game "Gradius" by Konami from 1986, built using C++ and Raylib.

> [!NOTE]  
> If you are a lecturer reviewing or marking this work, kindly note that this project is still in development. Once it is complete, this README will reflect screenshots of the completed system. You can view the YouTube video submission for the project here: [https://www.youtube.com/watch?v=TfqxgaHlM-4](https://www.youtube.com/watch?v=TfqxgaHlM-4)

## Table of contents
- [Background](#background)
- [Controls](#controls)
- [list of enemies](#list-of-enemies)
- [Power-ups](#power-ups)
- [Remaining items](#remaining-items)
- [Credits](#credits)

## Building and running
This project uses CMake as its build system and fetches Raylib automatically, so there is no need to download any extra libraries. Just make sure to install CMake and have a C++ compiler setup on your system.

To build a release version of the game, simply use the command in the root of the project in your favorite terminal emulator:
```bash
cmake --preset release
cmake --build release
```

The final game should be saved in the `release/SpaceGladiator/SpaceGladiator.exe` folder.

## Background
This game was done as part of my Computer Graphics unit in university. The idea of the game takes inspriation from the NES classic "Gradius", a side-scroller space shooting game. It contains 3 levels (**Note:** I'm still working on the levels), each with a variety of enemies and bosses to battle, as well as multiple power-ups to help make it through the levels

## Controls
| Key        | Action                    |
| ---------- | ------------------------- |
| Arrow keys | Move the spaceship around |
| S/Space    | Shoot projectiles         |
| P          | Pause the game            |
| C          | Toggle CRT filter         |

## List of enemies
The game contains 8 different types of enemies:
1. Asteroids - these break apart into smaller chunks as you destroy them
2. Sine-wave flyers - flying ships that move like a wave
3. Spinning laser - similar to the spinning fire-balls in Mario
4. Canons - fire multiple shots across the screen
5. Flyer spawner - spawns multiple flyers heading straight towards the player
6. Diagonal flyers - move across the screen and dash towards you when you get too close
7. Moving laser beam - fires a giant laser once its locked on to you
8. Ground shooters - Stay close to the ground and target the player

There are also 3 bosses, one for each level:
1. Big Spaceship - identical to the one in Gradius
2. Snake heads - 3 snake heads that try to shoot you
3. Laser eye - a giant eye that creates a bullet hell for you to avoid

## Power-ups
To help the player survive, several power-ups can be collected when destroying certain enemies:
1. Speed up - make the player move a little faster
2. Multishot - allow the player to shoot in multiple directions
3. Multiplier - allow the player to shoot more bullets at a time
4. Laser - allow the player to shoot an indestructible laser beam
5. Shield - protect the player from enemies

## Remaining items
The following items still need to be added to the game, kindly keep them in mind if you are assessing this work:
- [ ] Snake boss
- [ ] Laser eye boss
- [ ] Final level design
- [ ] Complete title screen and credits
- [ ] SFX and music

## Credits
This game was made using assets created by Nintendo and Konami. All art work, sprites, backgrounds, sound tracks, and themes belong to them. 

**This project is in submission for a class project in university, and is not designed or developed with the intention of being sold or publically played**
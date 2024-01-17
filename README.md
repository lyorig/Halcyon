# HalodaQuest

This is nothing more than a hobby project of mine, on which I intend to practice various C++ idioms and good coding practices in general, along with CMake and Git.

## Made with Halcyon

Included are several independent libraries.
- **Halcyon**  
A C++ SDL wrapper, focused on speed and ease of use.
- **lyoSTL**  
Various classes and types that enhance and simplify coding.
- **lyoECS**  
An Entity-Component System, features both static and dynamic variants.

## The rendering system
There are a few steps to rendering the game.
1. **Scene Canvas**  
Everything is rendered onto a target texture. This allows for camera scale independency. Camera information is passed in, enabling culling for unseen objects.  
Distance from the camera is taken into account.
2. **Final Render**  
The camera scale is used to render the scene canvas onto the screen.
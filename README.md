# Wolf-3D
Reading "Masters of Doom". Got inspired to remake Wolfenstein3D

This is stalled as I either switch to 2D rendering, increase performance
of the 3D renderer, or find a clever datastructure that would reduce the
reliance on renderer iteration performance.

## Assets

The PNG files inside `assets/` are sprite sheets. `walls.png` contains
64x64 tiles arranged in a grid. Currently the first tile of this sheet is
used as the wall texture by the raycaster.

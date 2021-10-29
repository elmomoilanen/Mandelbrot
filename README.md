# Mandelbrot #

[![main](https://github.com/elmomoilanen/Mandelbrot/actions/workflows/main.yml/badge.svg)](https://github.com/elmomoilanen/Mandelbrot/actions/workflows/main.yml)

A command line program to plot different kind of images of the Mandelbrot set which is consisted of points *c* in the complex plane for which the sequence <img src="https://render.githubusercontent.com/render/math?math=z_{n%2B1} = z_{n}^2 %2B c"> remains bounded when iterated from z=0. To plot the set, an optimized escape time algorithm is utilized, in which a color is chosen for every pixel based on the iteration count until the algorithm reached the escape condition or user-defined maximal iteration count. Reaching of this latter threshold is taken as evidence that the particular point belongs to the Mandelbrot set and the pixel corresponding to the point is then usually colored with black.

This program can be used with its configured default settings or by providing command line arguments, these settings can be modified. Read the **Use** section below for more information.

## Build ##

Expected to build successfully in most common Linux distros (gcc installed) and macOS. C11 standard is used.

Build the program with the following command in a shell

```bash
make && make clean
```

Notice that for macOS one might need to override the compiler setting from the Makefile which is gcc by default. In this case the following should work
```bash
make CC=clang && make clean
```

Target of the Makefile is a *mandelbrot* executable which after successful build will be placed directly to the root folder of this program.

## Use ##

Usage with default settings is simple
```bash
./mandelbrot
```

and as a result it would produce a bitmap file with content similar to the following figure

![](docs/fractal_example.png)

Different kind of results can be achieved by providing command line arguments. There is for example an option which specifies whether the coloring algorithm is continuous (as in the previous figure), histogram or simple which uses just two different colors. There is a convenient way to see all options the program can take.

To inspect all allowed options and their arguments, invoke the following command
```bash
./mandelbrot -s
```

Following command illustrates a case where multiple arguments are passed to the program
```bash
./mandelbrot -a histogram -m 250000 -e 2 -p green
```

It would plot the Mandelbrot set with the histogram coloring algorithm using 250000 as the maximal iteration count and two as a value for the escape bound. In addition, a coloring palette "green" would be used.

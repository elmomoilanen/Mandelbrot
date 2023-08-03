# Mandelbrot #

[![main](https://github.com/elmomoilanen/Mandelbrot/actions/workflows/main.yml/badge.svg)](https://github.com/elmomoilanen/Mandelbrot/actions/workflows/main.yml)

This is a command-line program designed to draw the Mandelbrot set with various parameter options. The Mandelbrot set consists of points *c* in the complex plane for which the sequence $$z_{n+1} = z_{n}^2 + c$$ remains bounded when iterated from z=0. To plot the set, an optimized escape time algorithm is utilized. It chooses a color for every pixel based on the iteration count until the algorithm reaches the escape condition or user-defined maximal iteration count. Reaching this latter threshold is taken as evidence that the particular point belongs to the Mandelbrot set, and the corresponding pixel is usually colored black.

## Build ##

It is expected to work on most common Linux distros and macOS. The program uses the C11 standard and links with math and posix thread libraries.

To build the program and clean up unneeded object files afterward, run the following command

```bash
make && make clean
```

If the build is successful, the `mandelbrot` executable will be created in the current directory.

## Usage ##

Usage with default parameters is simple from command line

```bash
./mandelbrot
```

and this command produces a bitmap file with content similar to the figure below (default size 800 x 600 pixels)

![](docs/fractal_cont.png)

Several parameter options can be given to override the defaults. For example, there is an option that specifies whether the coloring algorithm is continuous (as in the previous figure), histogram or simple, which uses just two different colors. There is a convenient way to see all the options the program can take.

To see all allowed options and short descriptions of their arguments, run the following command

```bash
./mandelbrot -s
```

There are few restrictions on parameter combinations. The histogram coloring algorithm can only be used with the color options red, green and blue. The simple algorithm (-a simple) does not use the color option (-p) at all.

The next example command illustrates a case where multiple arguments are passed to the executable. It plots the Mandelbrot set on a 1920 x 1080 pixel canvas using the histogram coloring algorithm with a maximum iteration count of 250,000 and a red color palette.

```bash
./mandelbrot -a histogram -m 250000 -p red -w 1980 -h 1080
```

The figure below (not to scale) represents a possible result from the previous command

![](docs/fractal_hist.png)

#pragma once

#include <string>
#include <vector>
#include <Magick++.h>
#include "maze.h"

using namespace Magick;

using std::string;

/* Read an image from a file, and lightly convert it.
 *
 * Can read anything that ImageMagick knows how to handle,
 * which includes a bunch of different formats and it'll even
 * handle URLs from the internet correctly.
 *
 * Modifications from the read image include thresholding and
 * eliminating empty space around the edge of the image.
 */
Image read_image(string filename);

/* Convert an image to a simpler format for further processing
 *
 * The result is just a 2D vector of bools, with 'true' for any
 * wall pixels.
 *
 * This function takes care to block off the corners, so that
 * any mazes with extra space around the edge don't just end up
 * with solutions that go around the edge of the maze uselessly.
 */
maze to_maze(const Image & img);

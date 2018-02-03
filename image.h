#pragma once

#include <string>
#include <vector>
#include <Magick++.h>

using namespace Magick;

using std::string;
using std::vector;

typedef vector<vector<bool>> maze;

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

maze to_maze(Image & img);

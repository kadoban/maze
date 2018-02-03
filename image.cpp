
#include <Magick++.h>
#include "image.h"

using namespace Magick;

void read_image(string filename) {
  Image img(filename);
  img.display();
}

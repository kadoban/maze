
#include <boost/program_options.hpp>
#include <iostream>
#include <string>
#include <vector>
#include "main.h"
#include "image.h"

namespace po = boost::program_options;

using std::cin;
using std::cout;
using std::endl;
using std::cerr;
using std::vector;
using std::string;


void do_usage(std::ostream & out, const po::options_description & opts) {
  out << "Usage: " << endl;
  out << "maze [options] [input-file] [output-file]" << endl;
  out << opts << endl;
}

int main(int argc, char * argv[]) {
  string infile, outfile;

  po::options_description generic_opts("Generic options");
  generic_opts.add_options()
    ("help", "produce this helpful message");

  po::options_description inout_opts("File specification options");
  inout_opts.add_options()
    ("input-file", po::value(&infile), "input image file")
    ("output-file", po::value(&outfile), "output image file");

  po::positional_options_description pos_opts_desc;
  pos_opts_desc.add("input-file", 1);
  pos_opts_desc.add("output-file", 1);

  po::options_description all_opts, visible_opts;

  all_opts.add(generic_opts).add(inout_opts);
  visible_opts.add(generic_opts);

  try {
    po::variables_map cmd_opts;
    po::store(po::command_line_parser(argc, argv).
              options(all_opts).positional(pos_opts_desc).run(), cmd_opts);
    po::notify(cmd_opts);

    if (cmd_opts.count("help")) {
      do_usage(cout, visible_opts);
      return 0;
    }

    auto img = read_image(infile);
    to_maze(img);
  } catch(std::exception & e) {
    cerr << e.what() << endl;
    cerr << endl;
    do_usage(cerr, visible_opts);
    return -1;
  }
  return 0;
}

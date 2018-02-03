
#include "main.h"
#include <boost/program_options.hpp>
#include <iostream>

namespace po = boost::program_options;
using std::cin;
using std::cout;
using std::endl;
using std::cerr;

int main(int argc, char * argv[]) {
  try {
    po::options_description opts_desc("Options");
    opts_desc.add_options()
      ("help", "produce this helpful message");
    po::variables_map cmd_opts;
    po::store(po::parse_command_line(argc, argv, opts_desc), cmd_opts);
    po::notify(cmd_opts);

    if (cmd_opts.count("help")) {
      cout << opts_desc << endl;
      return 0;
    }

  } catch (...) {
    cerr << "Error of unknown type!" << endl;
    return -1;
  }
  return 0;
}

#pragma once

#include <boost/program_options.hpp>
#include <iostream>

namespace po = boost::program_options;

void do_usage(std::ostream & out, const po::options_description & opts);

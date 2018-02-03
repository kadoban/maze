#pragma once

#include <vector>
#include <utility>
#include <cstddef>

typedef std::vector<std::vector<bool>> maze;
typedef std::pair<size_t, size_t> point;

std::vector<std::pair<size_t, size_t>> solve_maze(const maze &);
std::vector<std::pair<size_t, size_t>> solve_maze(const maze &, size_t &);

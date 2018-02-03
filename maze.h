#pragma once

#include <vector>
#include <utility>
#include <cstddef>

typedef std::vector<std::vector<bool>> maze;
typedef std::pair<size_t, size_t> point;

/* Solve a maze.
 *
 * Expects a rectangular 2D vector of states, with 'true' meaning that there
 * is a wall there. 'false' are passable. Will only go N, S, E, W to solve the
 * maze (no diagonals).
 *
 * It guesses where the start/end are by just trying to find an opening on any
 * two sides of the maze (N, S, E, W) and then trying to find a way through from
 * any one side to any other. Once it finds one, it returns that and stops.
 *
 * The size_t& argument is used to return a guess as to the "radius" one could
 * use to draw a line through the maze so it will look good with the maze.
 *
 * It heuristically tries to determine that based on the distance from each
 * point on the path to the nearest 'wall'.
 */
std::vector<std::pair<size_t, size_t>> solve_maze(const maze &, size_t &);
std::vector<std::pair<size_t, size_t>> solve_maze(const maze &);

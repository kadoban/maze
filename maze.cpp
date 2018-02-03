
#include <iostream>
#include <vector>
#include <cstddef>
#include <utility>
#include <stdexcept>
#include <deque>
#include <queue>
#include <algorithm>
#include <limits>
#include "maze.h"

using std::vector;
using std::pair;
using std::deque;
using std::make_pair;

typedef pair<vector<vector<point>>, vector<vector<size_t>>> djik_res;

vector<vector<size_t>> distance_from_wall(const maze &);
vector<point> neighbors(size_t i, size_t j);
djik_res djikstra(const maze &, const vector<vector<size_t>> &, size_t x, size_t y);

vector<point> solve_maze(const maze & m) {
  size_t bogus = 0;
  return solve_maze(m, bogus);
}

vector<point> solve_maze(const maze & m, size_t & suggested_radius) {
  size_t w, h;
  w = m.size();
  if (!w || !(h = m[0].size())) { // <--- h set here
    throw std::invalid_argument("maze is nil sized");
  }

  auto dists = distance_from_wall(m);
  auto costs(dists);
  // We want the "cost" of using a node to be opposite in magnitude
  // compared to the distance from the wall, we want to stay in the
  // center of paths through the maze. So subtract all the values
  // in that 2D array from the maximum one found.
  size_t max_dist = 0;
  for (const auto & v : costs) { // take the max of the max of each column
    max_dist = std::max(max_dist, *std::max_element(v.begin(), v.end()));
  }
  for (auto & v : costs) { // "negate" the values
    for (auto & e : v) {
      e = max_dist - e + 1;
      e = e * 4;
    }
  }

  // Figure out what point to use as the 'start' from each part of
  // the maze (top/bottom/left/right). We just find the minimum-cost
  // point on that part.
  vector<point> starts;
  for (size_t x = 0; x < w; x += w-1) { // go along the left and the right
    size_t min_so_far = max_dist;
    size_t best_y = h;
    for (size_t y = 0; y < h; ++y) {
      if (costs[x][y] < min_so_far) {
        min_so_far = costs[x][y];
        best_y = y;
      }
    }
    if (min_so_far < max_dist) {
      starts.push_back(make_pair(x, best_y));
    }
  }
  for (size_t y = 0; y < h; y += h-1) { // go along the top and the bottom
    size_t min_so_far = std::numeric_limits<size_t>::max();
    size_t best_x = w;
    for (size_t x = 0; x < w; ++x) {
      if (costs[x][y] < min_so_far) {
        min_so_far = costs[x][y];
        best_x = x;
      }
    }
    if (min_so_far < std::numeric_limits<size_t>::max()) {
      starts.push_back(make_pair(best_x, y));
    }
  }
  if (starts.size() < 2) {
    throw std::invalid_argument("Maze must have an entrance and exit on the edge!");
  }
  for (auto s : starts) {
    auto res = djikstra(m, costs, s.first, s.second);
    for (auto e : starts) {
      if (s != e &&
          res.second[e.first][e.second] < std::numeric_limits<size_t>::max()) {
        vector<point> out;
        size_t x = e.first, y = e.second;
        size_t min_dist = dists[x][y];
        do {
          min_dist = std::min(min_dist, dists[x][y]);
          auto p = res.first[x][y];
          x = p.first;
          y = p.second;
          out.push_back(make_pair(x, y));
        } while (make_pair(x, y) != s);
        out.push_back(make_pair(s.first, s.second));
        suggested_radius = std::max(min_dist / 3, (size_t) 2);
        return out;
      }
    }
  }
  throw std::invalid_argument("Maze has no solution!");
}

/*
 * Get the neighbors of a point, using cardinal directions only
 *
 * Avoids going off the boundary to the "negative" side, but
 * can't avoid the other boundary, if there is one
 */
vector<point> neighbors(size_t i, size_t j) {
  vector<point> res;
  for (int x = i == 0 ? 1 : -1; x <= 1; x += 2) {
      res.push_back(make_pair(i + x, j));
  }
  for (int y = j == 0 ? 1 : -1; y <= 1; y += 2) {
      res.push_back(make_pair(i, j + y));
  }
  return res;
}

/* Get the minimum distance from a wall for each point in
 * the maze.
 *
 * Distance is calculated in manhattan fashion.
 */
vector<vector<size_t>> distance_from_wall(const maze & m) {
  size_t w = m.size(), h = m[0].size();
  vector<vector<bool>> seen(w, vector<bool>(h, false));
  vector<vector<size_t>> res(w, vector<size_t>(h, 0));
  // todo queue of points to process, used in a BFS manner
  deque<pair<point, size_t>> todo;
  // add all the actual walls to the todo
  for (size_t x = 0; x < w; ++x) {
    for (size_t y = 0; y < h; ++y) {
      if (m[x][y]) {
        seen[x][y] = true;
        todo.push_back(make_pair(make_pair(x, y), 0));
      }
    }
  }
  // do a BFS from the walls, setting the manhattan distance
  // for each thing we encounter
  while (!todo.empty()) {
    auto next = todo.front();
    auto x = next.first.first;
    auto y = next.first.second;
    auto d = next.second;
    todo.pop_front();
    res[x][y] = d;
    for (auto c : neighbors(x, y)) {
      if (c.first < w && c.second < h && !seen[c.first][c.second]) {
        seen[c.first][c.second] = true;
        todo.push_back(make_pair(c, d+1));
      }
    }
  }
  return res;
}

djik_res djikstra(const maze & m, const vector<vector<size_t>> & costs,
                  size_t x, size_t y) {
  typedef pair<size_t, point> dist_point;
  size_t w = m.size(), h = m[0].size();
  vector<vector<point>> preds(w, vector<point>(h));
  vector<vector<size_t>> dists(w, vector<size_t>(h, std::numeric_limits<size_t>::max()));
  std::priority_queue<dist_point, vector<dist_point>, std::greater<dist_point>> q;
  dists[x][y] = 0;
  q.push(make_pair(0, make_pair(x, y)));

  while (!q.empty()) {
    auto p = q.top().second; // the point
    auto d = q.top().first; // distance to here
    q.pop();
    auto x = p.first;
    auto y = p.second;
    if (dists[x][y] < d) { continue; } // already have better, redundant
    for (auto e : neighbors(x, y)) {
      auto i = e.first;
      auto j = e.second;
      if (i < w && j < h && !m[i][j]) {
        if (dists[i][j] > dists[x][y] + costs[i][j]) {
            dists[i][j] = dists[x][y] + costs[i][j];
            preds[i][j] = make_pair(x, y);
            q.push(make_pair(dists[i][j], e));
        }
      }
    }
  }
  return make_pair(preds, dists);
}

/**
 * This file contains class that deals with shortest path between two points in a 2D-Grid
 */

#ifndef ALGO_SHORTEST_PATH_H
#define ALGO_SHORTEST_PATH_H

#include <array>
#include <limits>
#include <stdexcept>
#include <vector>

#include "grid_components.h"

namespace algo {

namespace problems {

/**
 * @brief This class encapsulates a 2D grid as well provides APIs to traverse through the grid.
 *
 * Assumptions: The GRID starts with (0,0) - the top left corner
 */
class Grid {
 public:
  Grid(const int rows,
       const int columns,
       AllowedDirections allowed_dirs = AllowedDirections::MAX_ALLOWED) :
      ROWS(rows), COLUMNS(columns), dir_max_allowed_(allowed_dirs) {}

  static AllowedDirections set_allowed_directions(AllowedDirections directions, AllowedDirections dir_to_set) {
    return (directions | dir_to_set);
  }

  static AllowedDirections unset_allowed_directions(AllowedDirections directions, AllowedDirections dir_to_unset) {
    return (directions & ~dir_to_unset);
  }

  AllowedDirections set_dir_for_traversal(const Coordinates& src, const Coordinates& dest) {
    // allowed directions
    auto x_diff = dest.x_ - src.x_;
    auto y_diff = dest.y_ - src.y_;

    AllowedDirections  directions{AllowedDirections::NONE};
    if(x_diff > 0)
      directions = set_allowed_directions(directions, AllowedDirections::RIGHT);
    else if(x_diff < 0)
      directions = set_allowed_directions(directions, AllowedDirections::LEFT);

    if(y_diff > 0)
      directions = set_allowed_directions(directions, AllowedDirections::UP);
    else if(y_diff < 0)
      directions = set_allowed_directions(directions, AllowedDirections::DOWN);

    // enable diagonal only if both x and y axis can be traversed
    if((bool)(dir_max_allowed_ & AllowedDirections::DIAGONAL)) {
      if(x_diff && y_diff)
        directions = set_allowed_directions(directions, AllowedDirections::DIAGONAL);
    }

    // unset any disallowed moves
    directions = directions & dir_max_allowed_;

    return directions;
  }

  /**
   * @brief     Get the number of moves required to move from coordinate a to coordinate b
   * @param src : source
   * @param dest: destinateion
   * @return    : number of moves required, INT_MAX if not allowed
   */
  int get_num_moves(const Coordinates& src, const Coordinates& dest) {
    AllowedDirections directions = set_dir_for_traversal(src, dest);

    return get_num_moves(src, dest, directions);
  }

  /**
   * @brief     Get the number of moves (shortest path) to connect all the points supplied
   * @param src         : source
   * @param coordinates : list of coordinates that has to be connected
   * @return            : number of steps to connect all associated coordinates
   */
  int get_num_moves(const Coordinates& src, const std::vector<Coordinates>& coordinates) {
    if(coordinates.empty()) {
      throw std::runtime_error("No destination given!");
    }

    auto citr = coordinates.cbegin();
    Coordinates dest = *citr;
    int moves = get_num_moves(src, dest);

    while(++citr != coordinates.end()) {
      moves += get_num_moves(dest, *citr);
      dest = *citr;
    }

    return moves;
  }

  /**
   * @brief         Gets the minimum number of moves to reach from src to dest.
   * @param src     Coordinates of source
   * @param dest    Coordinates of destination
   * @return        Number of moves
   */
  int get_min_moves_norestrict(const Coordinates &src, const Coordinates &dest) {
    if(!sanity_check(src) || !sanity_check(dest))
      throw std::runtime_error("Out of bounds!");

    int x_diff = std::abs(dest.x_ - src.x_);
    int y_diff = std::abs(dest.y_ - src.y_);

    int min_moves = std::min(x_diff, y_diff) // the diagonal movement
        + std::abs(x_diff - y_diff);         // the axis movement

    return min_moves;
  }


  /**
   * @brief     Get the number of min moves to connect all the points supplied
   * @param src         : source
   * @param begin       : start of list of coordinates that needs to be traversed to
   * @param end         : end of list of coordinates that needs to be traversed to
   * @return            : number of steps to connect all associated coordinates
   *                        In case of any of the coordinates are invalid, it returns ERR_MOVENUM
   */
  template<typename ForwardIterator>
  int get_min_moves_norestrict(const Coordinates &src, ForwardIterator begin, ForwardIterator end) {
    if(std::distance(begin, end) == 0) {
      throw std::runtime_error("No destination given!");
    }

    int moves{0};
    Coordinates from = src;

    for(auto itr = std::next(begin); itr != end; ++itr)  {
      moves += get_min_moves_norestrict(from, *itr);
      from = *itr;
    }

    return moves;
  }


 private:
  bool sanity_check(const Coordinates& coordinates) {
    if((coordinates.x_ < 0 || coordinates.x_ == ROWS) ||
        (coordinates.y_ < 0 || coordinates.y_ == COLUMNS))
      return false;
    return true;
  }

  int get_num_moves(const Coordinates& src, const Coordinates& dest, AllowedDirections dir) {
    // stop propagation when attempt is made to move out of bounds
    if((dest.x_ < 0 || dest.x_ == ROWS) ||
        (dest.y_ < 0 || dest.y_ == COLUMNS))
      return ERR_MOVENUM;

    // fine tuning direction movement based on current coordinates
    if(src.x_ == dest.x_) // movement along Y axis only
      dir = unset_allowed_directions(dir, AllowedDirections::LEFT | AllowedDirections::RIGHT | AllowedDirections::DIAGONAL);
    if(src.y_ == dest.y_) // movement along X axis only
      dir = unset_allowed_directions(dir, AllowedDirections::DOWN | AllowedDirections::UP | AllowedDirections::DIAGONAL);

    // reached destination ... no additional increments
    if(dir == AllowedDirections::NONE)
      return 0;


    // move in horizontal direction
    int move_horz{0};
    if((bool)(dir & AllowedDirections::LEFT)) {
      move_horz = -1;
    } else if((bool)(dir & AllowedDirections::RIGHT)) {
      move_horz = +1;
    }

    int horz_moves = ERR_MOVENUM;
    if(move_horz)
      horz_moves = 1 + get_num_moves(Coordinates{src.x_ + move_horz, src.y_}, dest, dir);

    // movement in vert direction
    int move_vert{0};
    if((bool)(dir & AllowedDirections::UP)) {
      move_vert = 1;
    } else if((bool)(dir & AllowedDirections::DOWN)) {
      move_vert = -1;
    }

    int vert_moves = ERR_MOVENUM;
    if(move_vert) {
      vert_moves = 1 + get_num_moves(Coordinates{src.x_, src.y_ + move_vert}, dest, dir);
    }

    int diag_moves = ERR_MOVENUM;
    if((bool)(dir & AllowedDirections::DIAGONAL)) {
      diag_moves = 1 + get_num_moves(Coordinates{src.x_ + move_horz, src.y_ + move_vert}, dest, dir);
    }

    return std::min(diag_moves, std::min(horz_moves, vert_moves));
  }


 public:
  static constexpr const int ERR_MOVENUM = std::numeric_limits<int>::max();

 private:

  AllowedDirections dir_max_allowed_;   // max allowed directions; in reality only a subset can be used
  const int         ROWS;
  const int         COLUMNS;
};


} // namespace algo::problems

} // namespace algo

#endif //ALGO_SHORTEST_PATH_H

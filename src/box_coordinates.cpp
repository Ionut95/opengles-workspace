#include "box_coordinates.hpp"

#include <iostream>

namespace opengles_workspace
{
std::vector<GLfloat> BoxCoordinates::CalculateBoxCoordinates(std::size_t row, std::size_t column, std::size_t nr_total_rows, std::size_t nr_total_columns) const
{
    GLfloat side_length = kNormalizedWindowSize / static_cast<GLfloat>(nr_total_columns);
    GLfloat side_width = kNormalizedWindowSize / static_cast<GLfloat>(nr_total_rows);

    GLfloat x_coordinate = kTopLeftCornerX + static_cast<GLfloat>(column) * side_length;
    GLfloat y_coordinate = kTopLeftCornerY - static_cast<GLfloat>(row) * side_width;

    return {x_coordinate, y_coordinate,
            x_coordinate + side_length, y_coordinate,
            x_coordinate, y_coordinate - side_width,
            x_coordinate + side_length, y_coordinate,
            x_coordinate, y_coordinate - side_width,
            x_coordinate + side_length, y_coordinate - side_width};
}
} // opengles_workspace
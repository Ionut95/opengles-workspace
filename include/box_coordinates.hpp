#pragma once

#include <cstddef>
#include <vector>

#include <glad/gl.h>

namespace opengles_workspace
{
class BoxCoordinates
{
    public:
        BoxCoordinates() = default;
        ~BoxCoordinates() = default;

        std::vector<GLfloat> CalculateBoxCoordinates(std::size_t row, std::size_t column, std::size_t nr_total_rows, std::size_t nr_total_columns) const;

    private:
        const GLfloat kNormalizedWindowSize = 2.0f;
        const GLfloat kTopLeftCornerX = -1.0f;
        const GLfloat kTopLeftCornerY =  1.0f;
};
} // opengles_workspace
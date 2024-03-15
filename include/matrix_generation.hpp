#pragma once

#include <vector>

#include <glad/gl.h>
#include <cstddef>
#include <iostream>

namespace opengles_workspace
{
class MatrixGeneration
{
    public:
        MatrixGeneration();
        ~MatrixGeneration() = default;

    
    private:
        void CreateMatrix();

        const float kOffset = 0.00625f;
        const float kSideLength = 0.125f;
        const float kSideWidth = 0.125f;
        const float kWindowLength = 2.0f;
        std::size_t nr_rows = (kWindowLength - kSideWidth) / kOffset;
        std::size_t nr_columns = (kWindowLength - kSideLength) / kOffset;
    public:
        std::vector<std::vector<std::vector<GLfloat>>> window_matrix_{};
};
} // opengles_workspace
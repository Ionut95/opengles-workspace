#include "matrix_generation.hpp"

namespace opengles_workspace
{
MatrixGeneration::MatrixGeneration()
{
    std::cout << "Ctor\n" << " nr_rows: " << nr_rows << " nr_columns: " << nr_columns << "\n";
    CreateMatrix();
}

void MatrixGeneration::CreateMatrix()
{
    float x = -1.0f;
    float y =  1.0f;

    for(std::size_t i = 0; i < 22; ++i)
    {
        //std::cout << "i: " << i << "\n";
        window_matrix_.emplace_back(std::vector<std::vector<GLfloat>>());
        for(std::size_t j = 0; j < 22; ++j)
        {
            //upper triangle
            GLfloat upper_top_left_x = x;
            GLfloat upper_top_left_y = y;
            GLfloat upper_top_right_x = x + kSideLength;
            GLfloat upper_top_right_y = y;
            GLfloat upper_bottom_left_x = x;
            GLfloat upper_bottom_left_y = y - kSideWidth;
            //lower triangle
            GLfloat lower_top_right_x = x + kSideLength;
            GLfloat lower_top_right_y = y;
            GLfloat lower_bottom_left_x = x;
            GLfloat lower_bottom_left_y = y - kSideWidth;
            GLfloat lower_bottom_right_x = x + kSideLength;
            GLfloat lower_bottom_right_y = y - kSideWidth;

            if(i == 21 && j <= 21)
            {
                

                std::cout << "j: " << j << "\n\n";

                std::cout << "x: " << x << " y: " << y << "\n\n";

                std::cout << "x: " << upper_top_left_x << " y: " << upper_top_left_y << "\n"; 
                std::cout << "x: " << upper_top_right_x << " y: " << upper_top_right_y << "\n";
                std::cout << "x: " << upper_bottom_left_x << " y: " << upper_bottom_left_y << "\n";
                std::cout << "x: " << lower_top_right_x << " y: " << lower_top_right_y << "\n";
                std::cout << "x: " << lower_bottom_left_x << " y: " << lower_bottom_left_y << "\n";
                std::cout << "x: " << lower_bottom_right_x << " y: " << lower_bottom_right_y << "\n";
                std::cout << "\n";
            }

            window_matrix_[i].emplace_back(std::vector<float>());
            window_matrix_[i][j].insert(window_matrix_[i][j].end(), {upper_top_left_x, upper_top_left_y, 
                                    upper_top_right_x, upper_top_right_y,
                                    upper_bottom_left_x, upper_bottom_left_y,
                                    lower_top_right_x, lower_top_right_y,
                                    lower_bottom_left_x, lower_bottom_left_y,
                                    lower_bottom_right_x, lower_bottom_right_y});

            if(i == 21 && j <= 21)
                std::cout << " x before offset" << x << " offset " << kOffset << "\n\n";
            x += kOffset;
            if(i == 21 && j <= 21)
                std::cout << " x after offset: " << x << " offset " << kOffset <<"\n\n";
        }
        
        x  =  -1.0f;
        y += kOffset;
    }

    // std::cout << "\nsize matrix: " << window_matrix_[288][140][0];
    // std::cout << "\nsize matrix: " << window_matrix_[289][140][1];
    // std::cout << "\nsize matrix: " << window_matrix_[290][140][2];
    // std::cout << "\nsize matrix: " << window_matrix_[291][140][3];
    // std::cout << "\nsize matrix: " << window_matrix_[292][140][4];
    // std::cout << "\nsize matrix: " << window_matrix_[293][140][5];
    // std::cout << "\nsize matrix: " << window_matrix_[294][140][6];
    // std::cout << "\nsize matrix: " << window_matrix_[295][140][7];
    // std::cout << "\nsize matrix: " << window_matrix_[296][140][8];
    // std::cout << "\nsize matrix: " << window_matrix_[297][140][9];
    // std::cout << "\nsize matrix: " << window_matrix_[298][140][10];
    // std::cout << "\nsize matrix: " << window_matrix_[299][140][11] << "\n";
}

} // opengles_workspace
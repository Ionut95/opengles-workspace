#include "bouncing_box_state.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

namespace opengles_workspace
{
BouncingBoxState::BouncingBoxState()
{
    std::tuple<std::size_t, std::size_t, std::size_t, std::size_t> data(ReadData());
	nr_rows_ = std::get<0>(data);
	nr_columns_ = std::get<1>(data);
    current_row_ = std::get<2>(data);
    current_column_ = std::get<3>(data);
}

std::size_t BouncingBoxState::GetNrRows() const
{
    return nr_rows_;
}

std::size_t BouncingBoxState::GetNrColumns() const
{
    return nr_columns_;
}

std::size_t BouncingBoxState::GetObjectRow() const
{
    return current_row_;
}

std::size_t BouncingBoxState::GetObjectColumn() const
{
    return current_column_;
}

bool BouncingBoxState::DetermineDirection()
{
    if(current_row_ == 0)
        is_going_up_ = true;
    
    if(current_row_ == nr_rows_ - 1)
        is_going_up_ = false;
    
    return is_going_up_;
}

void BouncingBoxState::ChangeState()
{
    if(nr_rows_ == 1) //|| nr_columns_ == 1)
        return;

    if(DetermineDirection())
        current_row_++;
    else
        current_row_--;
}

std::tuple<std::size_t, std::size_t, std::size_t, std::size_t> BouncingBoxState::ReadData() const
{
    std::ifstream file("../data_files/data.txt");

    if(!file.is_open())
    {
        std::cout << "Error: file not open" << std::endl;
        return {0, 0, 0, 0};
    }

    std::string line;

    getline(file, line);
    size_t nr_rows = GetValueFromLine(line);

    getline(file, line);
    size_t nr_columns = GetValueFromLine(line);

    getline(file, line);
    size_t current_row = GetValueFromLine(line);

    getline(file, line);
    size_t current_column = GetValueFromLine(line);

    return {nr_rows, nr_columns, current_row, current_column};
}

std::size_t BouncingBoxState::GetValueFromLine(std::string line) const
{
    std::stringstream stream(line);
    std::string str;

    getline(stream, str, ':');
    getline(stream, str, ':');

    std::stringstream to_size_t(str);
    std::size_t value;
    to_size_t >> value;
    return value;
}

} // opengles_workspace
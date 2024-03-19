#pragma once

#include <cstddef>
#include <tuple>
#include <string>

namespace opengles_workspace
{
class BouncingBoxState
{
    public:
        BouncingBoxState();
        ~BouncingBoxState() = default;

        std::size_t GetNrRows() const;
        std::size_t GetNrColumns() const;
        std::size_t GetObjectRow() const;
        std::size_t GetObjectColumn() const;

        void ChangeState();

    private:
        std::size_t nr_rows_;
        std::size_t nr_columns_;

        bool is_going_up_ = true;
        bool DetermineDirection();

        std::tuple<std::size_t, std::size_t, std::size_t, std::size_t> ReadData() const;
		std::size_t GetValueFromLine(std::string line) const;

        std::size_t current_row_;
        std::size_t current_column_;
};
} // opengles_workspace
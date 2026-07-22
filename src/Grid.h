#pragma once

#include <vector>


class Grid{
    private:
        int n;
        std::vector<float> grid;
    public:
        Grid(int n): n(n), grid((n+2)*(n+2), 0.0f) {}
        float& operator()(int i, int j){
            return grid[i*(n+2) + j];
        }
        void swap(Grid& other){
            grid.swap(other.grid);
        }
        void reset(){
            for (int i = 0; i < (n+2)*(n+2); i++){
                grid[i] = 0.0f;
            }
        }
};
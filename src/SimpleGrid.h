#pragma once

#include <vector>


class SimpleGrid{
    private:
        int n; // number of rows
        int m; // number of columns
        std::vector<float> grid;
    public:
        SimpleGrid(int n, int m): n(n), m(m), grid((n+2)*(m+2), 0.0f) {}
        
        
        float& operator()(int i, int j){
            return grid[i*(m+2) + j];
        }
        void swap(SimpleGrid& other){
            int tempN = n;
            int tempM = m;
            n = other.n;
            m = other.m;
            other.n = tempN;
            other.m = tempM;

            grid.swap(other.grid);
        }
        void reset(){
            for (int i = 0; i < (n+2)*(m+2); i++){
                grid[i] = 0.0f;
            }
        }

        int getN() const { return n; }
        int getM() const { return m; }
};
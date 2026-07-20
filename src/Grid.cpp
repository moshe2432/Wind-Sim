#include <vector>


class Grid{
    private:
        int N;
        std::vector<float> grid;
    public:
        Grid(int n): N(n), grid((n+2)*(n+2), 0.0f) {}
        float& operator()(int i, int j){
            return grid[i*(N+2) + j];
        }
};
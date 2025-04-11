#include "emp/web/Animate.hpp"
#include "emp/web/web.hpp"

emp::web::Document doc{"target"};

class GradientAnimator : public emp::web::Animate {
    emp::web::Canvas canvas{800, 800, "canvas"};
    int generation;
    std::vector<std::vector<float>> cells;
    int num_w_boxes = 20;
    int num_h_boxes = 20;
    std::vector<std::vector<float>> next_generation;

    public:
    GradientAnimator() {
        doc << canvas;
        doc << GetToggleButton("Toggle");
        doc << GetStepButton("Step");
        generation = 0;
        cells.resize(num_w_boxes, std::vector<float>(num_h_boxes, 0));
        cells[10][10] = 100;
        next_generation.resize(num_w_boxes, std::vector<float>(num_h_boxes, 0));
    }

    public:
    void DoFrame() override {
        canvas.Clear();
        generation++;
        DrawGrid();
        FindNextGeneration();
        UpdateGrid();
    }

    // get the value of each cell in the next generation and store it in next_generation
    public:
    void FindNextGeneration() {
        for (int x=0; x<num_w_boxes; x++) {
            for (int y=0; y<num_h_boxes; y++) {
                next_generation[x][y] = UpdateFunction(x, y);
            }
        }
    }

    // copy the values from next_generation into cells
    public:
    void UpdateGrid() {
        for (int x=0; x<num_w_boxes; x++) {
            for (int y=0; y<num_h_boxes; y++) {
                cells[x][y] = next_generation[x][y];
            }
        }
    }

    // draw a grid of rectangles, each with brightness according to that cell's value
    public:
    void DrawGrid() {
        for (int x=0; x<num_w_boxes; x++) {
            for (int y=0; y<num_h_boxes; y++) {
                float value = cells[x][y];
                // clamp value to [0, 1]
                if (value > 1) value = 1;
                if (value < 0) value = 0;
                canvas.Rect(40 * x, 40 * y, 40, 40, emp::ColorHSV(0, 0, value), "black");
            }
        }
    }

    // the bright point should move to the right, slightly spreading out over time
    public:
    float UpdateFunction(int x, int y) {
        return 0.96 * GetCell(x - 1, y) + 0.01 * GetCell(x, y - 1) + 0.01 * GetCell(x, y + 1) + 0.02 * GetCell(x, y);
    }

    public:
    float GetCell(int x, int y) {
        return cells[emp::Mod(x, num_w_boxes)][emp::Mod(y, num_h_boxes)];
    }
};

GradientAnimator animator;

int main() {
    animator.Step();
}
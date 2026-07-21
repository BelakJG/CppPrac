#include <iostream>
#include <cstdint>
#include <vector>
#include <cmath>

using namespace std;

struct Board {
    Board(const auto& lights_container) {
        num_lights = lights_container.size();
        for (int i = 0; i < num_lights; ++i) {
            lights |= lights_container[i] << i;
        }

        initialize_masks();
    }

    void print() {
        int length = sqrt(num_lights);
        for (int row = 0; row < length; ++row) {
            for (int col = 0; col < length; ++col) {
                int offset = (row * length) + col;
                cout << ((lights & 1u << offset) ? "X " : "O ");
            }
            cout << endl;
        }
    }

    void solve() {
        for (int col = 0; col < num_lights; ++col) {
            int pivot = -1;
            for (int row = col; row < num_lights; ++row) {
                if (toggle_masks[row] & 1u << col) {
                    pivot = row;
                    break;
                }
            }
            if (pivot == -1) continue;
            swap(toggle_masks[col], toggle_masks[pivot]);

            for (int row = 0; row < num_lights; ++row) {
                if (row == col) continue;
                if (toggle_masks[row] & 1u << col) {
                    toggle_masks[row] ^= toggle_masks[col];
                }
            }
        }

        solution = 0;
        for (int i = 0; i < num_lights; ++i) {
            if (toggle_masks[i] & 1u << num_lights) {
                solution |= 1u << i;
            };
        }
    }

    void print_solution() {
        int length = sqrt(num_lights);
        for (int row = 0; row < length; ++row) {
            for (int col = 0; col < length; ++col) {
                int offset = (row * length) + col;
                cout << ((solution & 1u << offset) ? "X " : "O ");
            }
            cout << endl;
        }
    }
private:
    int num_lights;
    uint32_t lights = 0;
    vector<uint32_t> toggle_masks;
    uint32_t solution;

    void initialize_masks() {
        int length = sqrt(num_lights);
        for (int row = 0; row < length; ++row) {
            for (int col = 0; col < length; ++col) {
                uint32_t mask = 1u << ((row * length) + col);
                auto toggle = [&mask, length](int rr, int cc) {
                    if ((rr >= 0 and rr < length) and (cc >= 0 and cc < length)) {
                        mask |= 1u << ((rr * length) + cc);
                    }
                };

                toggle(row - 1, col);
                toggle(row + 1, col);
                toggle(row, col - 1);
                toggle(row, col + 1);
                
                uint32_t aug = (lights & 1u << ((row * length) + col) ? 1 : 0);
                mask |= aug << num_lights;

                toggle_masks.push_back(mask);
            }
        }
    }
};

int main() {
    vector<bool> lights = {1,0,0,1,0,
                            0,1,0,1,0,
                            0,0,0,1,1,
                            0,1,1,1,0,
                            0,0,1,1,1};
    Board b(lights);
    b.print();
    b.solve();
    cout << endl;
    b.print_solution();
}

#include <iostream>
#include <cstdint>
#include <vector>

using namespace std;

struct Board {
    void initilize(const vector<int>& lights) {
        switches = 0;
        for (int i = 24; i >= 0; --i) {
            switches <<= 1u;
            switches |= lights[i];
        }
        switch_solution = switches;

        for (int r = 0; r < 5; r++) {
            for (int c = 0; c < 5; c++) {

                uint32_t m = 0;

                auto set = [&](int rr, int cc) {
                    if (rr >= 0 && rr < 5 && cc >= 0 && cc < 5) {
                        m |= 1u << (rr * 5 + cc);
                    }
                };

                set(r,c);
                set(r-1,c);
                set(r+1,c);
                set(r,c-1);
                set(r,c+1);

                switch_masks[r*5+c] = m;
            }
        }
    }

    void toggle_switch(int pos) {
        switches ^= switch_masks[pos];
    }

    void print() {
        for (int row = 0; row < 5; ++row) {
            for (int col = 0; col < 5; ++col) {
                int offset = (row * 5) + col;
                cout << ((switches & (1u << offset)) ? "X " : "O ");
            }
            cout << endl;
        }
    }

    void solve() {
        for (int col = 0; col < 25; ++col) {
            int pivot = -1;
            for (int row = col; row < 25; ++row) {
                if (switch_masks[row] & 1u << col) {
                    pivot = row;
                    break;
                }
            }

            if (pivot == -1) continue;

            swap(switch_masks[col], switch_masks[pivot]);
            bool a = (switch_solution >> col) & 1;
            bool b = (switch_solution >> pivot) & 1;
            if (a != b) {
                switch_solution ^= 1u << col;
                switch_solution ^= 1u << pivot;
            }

            for (int row = 0; row < 25; ++row) {
                if (row == col) continue;

                if (switch_masks[row] & 1u << col) {
                    switch_masks[row] ^= switch_masks[col];
                    if (switch_solution & 1u << col) {
                        switch_solution ^= 1u << row;
                    }
                }
            }
        }
        
        for (int row = 0; row < 5; ++row) {
            for (int col = 0; col < 5; ++col) {
                int offset = (row * 5) + col;
                cout << ((switch_solution & (1u << offset)) ? "X " : "O ");
            }
            cout << endl;
        }
    }
private:
    uint32_t switch_masks[25];
    uint32_t switches;
    uint32_t switch_solution;
};

int main() {
    vector<int> lights = {1,0,0,0,0,
                            0,1,0,0,1,
                            1,0,1,1,0,
                            1,0,0,0,1,
                            0,0,1,0,1};
    
    Board b;
    b.initilize(lights);
    b.print();
    cout << endl;
    b.solve();
}

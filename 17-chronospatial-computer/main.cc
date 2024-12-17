#include <cmath>
#include <iostream>
#include <print>
#include <vector>

using namespace std;

struct pcode {
    int         a = 729, b = 0, c = 0;
    vector<int> ops{ 0, 1, 5, 4, 3, 0 };
    int         pc = 0;

    int combo(int in) {
        if (in < 4) {
            return in;
        } else if (in == 4) {
            return a;
        } else if (in == 5) {
            return b;
        } else if (in == 6) {
            return c;
        }
        cerr << "no operand found.";
        exit(0);
    }

    void run() {
        while (pc < ops.size()) {
            int opcode = ops[pc], operand = ops[pc + 1];
            pc += 2;
            switch (opcode) {
                case 0: {
                    int numerator = a, denominator = pow(2, combo(operand));
                    if (denominator == 0) {
                        cerr << "Division by 0\n";
                        exit(0);
                    }
                    a = numerator / denominator;
                    break;
                }
                case 1: {
                    int tmp = (b ^ operand) & 0x7;
                    b       = tmp;
                    break;
                }
                case 2: {
                    int tmp = (combo(operand) % 8);
                    b       = tmp;
                    break;
                }
                case 3: {
                    if (a) {
                        pc = operand;
                        continue;
                    }
                    break;
                }
                case 4: {
                    int tmp = (b ^ c) & 0x7;
                    b       = tmp;
                    break;
                }
                case 5: {
                    cout << ((combo(operand) % 8)) << ",";
                    break;
                }
                case 6: {
                    int numerator = a, denominator = pow(2, combo(operand));
                    if (denominator == 0) {
                        cerr << "Division by 0\n";
                        exit(0);
                    }
                    b = numerator / denominator;
                    break;
                }
                case 7: {
                    int numerator = a, denominator = pow(2, combo(operand));
                    if (denominator == 0) {
                        cerr << "Division by 0\n";
                        exit(0);
                    }
                    c = numerator / denominator;
                    break;
                }
                default:
                    cerr << "This should not happen\n";
                    exit(0);
                    break;
            }
        }
    }
};

int main(int argc, char *argv[]) {
    pcode p;
    p.run();
    cout << "\n";
    return 0;
}

// AquaQ Challenge Hub
// Challenge 31: Brandless Combination Cubes
// https://challenges.aquaq.co.uk/challenge/31

#include <array>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include <gsl/util>

static bool readFile(const std::string& fileName, std::vector<std::string>& lines)
{
    std::ifstream in{fileName};
    if (!in) {
        std::cerr << "Cannot open file " << fileName << std::endl;
        return false;
    }
    auto closeStream = gsl::finally([&in] { in.close(); });
    std::string str;
    while (std::getline(in, str)) {
        lines.push_back(str);
    }
    return true;
}

using Row = std::array<uint8_t, 3>;
using Face = std::array<Row, 3>;
using RotateFunction = std::function<void(void)>;

Face F{{{1, 1, 1}, {1, 1, 1}, {1, 1, 1}}};
Face U{{{2, 2, 2}, {2, 2, 2}, {2, 2, 2}}};
Face L{{{3, 3, 3}, {3, 3, 3}, {3, 3, 3}}};
Face R{{{4, 4, 4}, {4, 4, 4}, {4, 4, 4}}};
Face D{{{5, 5, 5}, {5, 5, 5}, {5, 5, 5}}};
Face B{{{6, 6, 6}, {6, 6, 6}, {6, 6, 6}}};

inline void rotateFace(Face& f)
{
    for (size_t i = 0; i < 2; ++i) {
        const auto tmp{f[0][i]};
        f[0][i] = f[2 - i][0];
        f[2 - i][0] = f[2][2 - i];
        f[2][2 - i] = f[i][2];
        f[i][2] = tmp;
    }
}

void rotateF()
{
    rotateFace(F);
    const auto tmp{D[0]};
    for (size_t i = 0; i < 3; ++i) {
        D[0][i] = R[2 - i][0];
        R[2 - i][0] = U[2][2 - i];
        U[2][2 - i] = L[i][2];
        L[i][2] = tmp[i];
    }
}

void rotateU()
{
    rotateFace(U);
    const auto tmp{F[0]};
    for (size_t i = 0; i < 3; ++i) {
        F[0][i] = R[0][i];
        R[0][i] = B[2][2 - i];
        B[2][2 - i] = L[0][i];
        L[0][i] = tmp[i];
    }
}

void rotateL()
{
    rotateFace(L);
    const Row tmp{D[2][0], D[1][0], D[0][0]};
    for (size_t i = 0; i < 3; ++i) {
        D[2 - i][0] = F[2 - i][0];
        F[2 - i][0] = U[2 - i][0];
        U[2 - i][0] = B[2 - i][0];
        B[2 - i][0] = tmp[i];
    }
}

void rotateR()
{
    rotateFace(R);
    const Row tmp{D[0][2], D[1][2], D[2][2]};
    for (size_t i = 0; i < 3; ++i) {
        D[i][2] = B[i][2];
        B[i][2] = U[i][2];
        U[i][2] = F[i][2];
        F[i][2] = tmp[i];
    }
}

void rotateD()
{
    rotateFace(D);
    const auto tmp{B[0]};
    for (size_t i = 0; i < 3; ++i) {
        B[0][i] = R[2][2 - i];
        R[2][2 - i] = F[2][2 - i];
        F[2][2 - i] = L[2][2 - i];
        L[2][2 - i] = tmp[i];
    }
}

void rotateB()
{
    rotateFace(B);
    const auto tmp{U[0]};
    for (size_t i = 0; i < 3; ++i) {
        U[0][i] = R[i][2];
        R[i][2] = D[2][2 - i];
        D[2][2 - i] = L[2 - i][0];
        L[2 - i][0] = tmp[i];
    }
}

uint64_t product(const Face& f)
{
    uint64_t prod{1};
    for (const auto& row : f) {
        for (const auto v : row) {
            prod *= v;
        }
    }
    return prod;
}

int main(int argc, char* argv[])
{
    std::vector<std::string> lines{};
    if (argc == 2) {
        if (!readFile(argv[1], lines)) {
            return EXIT_FAILURE;
        }
    }

    const std::map<char, RotateFunction> rotateFunc{
        {{'F', rotateF}, {'U', rotateU}, {'L', rotateL}, {'R', rotateR}, {'D', rotateD}, {'B', rotateB}}};

    RotateFunction rot = []() {};
    for (const auto r : lines[0]) {
        if ('\'' == r) {
            // Lazy implementation: Treat anticlockwise rotation as triple clockwise rotation
            rot();
        } else {
            rot = rotateFunc.at(r);
        }
        rot();
    }
    std::cout << product(F) << std::endl;

    return EXIT_SUCCESS;
}

#include "SecureBox.hpp"
#include <thread>
#include <functional>
#include <mutex>
#include <cmath>

bool OpenBox(SecureBox& box)
{
    int iterations = 3;
    int x_size = box.getSize().first;
    int y_size = box.getSize().second;
    if (x_size % 2 != 0 && y_size % 2 != 0) {
        iterations = 1;
    }
    auto state = box.getState();
    std::vector<std::pair<int, int>> remainder;

    for (int k = 0; k < iterations; k++) {
        remainder.clear();
        state = box.getState();
        if (y_size % 2 == 0) {
            for (int j = 0; j < y_size; j++) {
                for (int i = 0; i < x_size; i++) {
                    if (state[i][j]) {
                        remainder.push_back({ i,j });
                    }
                }
            }
        }
        else {
            for (int i = 0; i < x_size; i++) {
                for (int j = 0; j < y_size; j++) {
                    if (state[i][j]) {
                        remainder.push_back({ i,j });
                    }
                }
            }
        }
        for (auto point : remainder) {
            if (!box.isLocked()) {    
                return true;
            }
            box.flip(point.first, point.second);
        }
    } 
    return box.isLocked();
}

//checking configurations of different sizes for a random matrix
bool testOpenBox() {
    int N = 100; //max matrix size
    int step = 20;
    std::vector<std::pair<std::pair<int, int>, bool>> matrixSizes;
    for (int i = 1; i <= N; i+=step) {
        for (int j = 1; j <= N; j+=step) {

            matrixSizes.push_back({ {i,j},0 });

        }
    }

    std::mutex mtx;
    int progress = 0;
    bool fail = false;

    std::function<void()> handlePoint = [&mtx, &matrixSizes, &progress, &fail, &N]() {

        for (auto& item : matrixSizes) {
            if (fail)return;
            std::lock_guard<std::mutex> lock(mtx);
            if (!item.second) {
                SecureBox box(item.first.first, item.first.second);
                OpenBox(box);
                if (box.isLocked()) {
                    std::cout << "fail\n";
                    fail = true;
                    return;
                }
                item.second = true;
                progress++;
                if (progress % (int)pow(matrixSizes.size(), 0.5) == 0)
                    std::cout << (double)progress * 100 / matrixSizes.size() << "%\n";
            }
        }
    };

    std::vector<std::thread> threads;
    int threadsCount = std::thread::hardware_concurrency();
    std::cout << "threads count " << threadsCount << '\n';
    for (int i = 0; i < threadsCount; i++) {
        std::thread thr(handlePoint);
        threads.emplace_back(std::move(thr));
    }

    for (auto& thr : threads) {
        thr.join();
    }

    return !fail;
}

int main() {
    if(testOpenBox())
        std::cout << "successfully";
    else
        std::cout << "fail";
}
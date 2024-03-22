**Compile for main** : `g++ -std=c++17 -o main -I ./src src/main.cpp src/symbols.cpp src/trader.cpp src/training.cpp src/indicators/*.cpp src/neat/*.cpp src/trading/*.cpp src/utils/*.cpp`
**Run executable** : `./main`
**Compile for testing** : `g++ -std=c++17 -o run-tests -I ./src src/symbols.cpp src/tests.cpp src/tests/*.cpp src/trader.cpp src/training.cpp src/trading/*.cpp src/trading/tests/*.cpp src/utils/*.cpp src/utils/tests/*.cpp src/indicators/*.cpp src/indicators/tests/*.cpp src/neat/*.cpp src/neat/tests/*.cpp -lgtest -lgmock -lgtest_main -pthread`
**Run tests** : `./run-tests`

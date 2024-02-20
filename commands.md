**Compile for main** : `g++ -std=c++17 -o main -I ./src main.cpp src/*.cpp`
**Run executable** : `./main`
**Compile for testing neat** : `g++ -std=c++17 -o src/neat/run-tests -I ./src/neat src/neat/*.cpp src/neat/tests/*.cpp -lgtest -lgtest_main -pthread`
**Run tests for neat** : `./src/neat/run-tests`
**Compile for testing** : `g++ -std=c++17 -o run-tests -I ./src src/*.cpp src/tests/*.cpp src/utils/*.cpp src/utils/tests/*.cpp src/indicators/*.cpp src/indicators/tests/*.cpp -lgtest -lgtest_main -pthread`
**Run tests** : `./run-tests`

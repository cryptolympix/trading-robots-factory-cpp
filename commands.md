**Compile for main** : `g++ -std=c++17 -o main -I ./src src/main.cpp src/symbols.cpp src/trader.cpp src/trading_schedule.cpp src/trading_tools.cpp src/training.cpp src/indicators/*.cpp src/neat/activation_functions.cpp src/neat/config.cpp src/neat/connection_gene.cpp src/neat/connection_history.cpp src/neat/genome.cpp src/neat/math_utils.cpp src/neat/node.cpp src/neat/population.cpp src/neat/species.cpp src/utils/*.cpp`
**Run executable** : `./main`
**Compile for testing neat** : `g++ -std=c++17 -o src/neat/run-tests -I ./src/neat src/neat/*.cpp src/neat/tests/*.cpp -lgtest -lgtest_main -pthread`
**Run tests for neat** : `./src/neat/run-tests`
**Compile for testing** : `g++ -std=c++17 -o run-tests -I ./src src/symbols.cpp src/tests.cpp src/trader.cpp src/trading_schedule.cpp src/trading_tools.cpp src/training.cpp src/tests/*.cpp src/utils/*.cpp src/utils/tests/*.cpp src/indicators/*.cpp src/neat/*.cpp src/indicators/tests/*.cpp -lgtest -lgmock -lgtest_main -pthread`
**Run tests** : `./run-tests`

**Compile for main** : `g++ -std=c++17 -g -o run_training -I/opt/homebrew/Cellar/boost/1.85.0/include src/run_training.cpp src/symbols.cpp src/trader.cpp src/training.cpp src/configs/*.cpp src/indicators/*.cpp src/neat/*.cpp src/trading/*.cpp src/utils/*.cpp -L/opt/homebrew/lib -lboost_iostreams`
**Run executable** : `./run_training`
**Run executable with debug** : `lldb ./run_training`
**Set breakpoint** : `breakpoint set -n malloc_error_break`
**Compile for testing** : `g++ -std=c++17 -g -o run_tests -I /opt/homebrew/Cellar/googletest/1.14.0/include -I /opt/homebrew/Cellar/boost/1.85.0/include src/symbols.cpp src/run_tests.cpp src/tests/*.cpp src/trader.cpp src/training.cpp src/configs/*.cpp src/trading/*.cpp src/trading/tests/*.cpp src/utils/*.cpp src/utils/tests/*.cpp src/indicators/*.cpp src/indicators/tests/*.cpp src/neat/*.cpp src/neat/tests/*.cpp -L/opt/homebrew/lib -lboost_iostreams -lgtest -lgmock -lgtest_main -pthread`
**Run tests** : `./run_tests`
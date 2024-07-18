# Commands

**Compile for run_training** : `g++ -std=c++17 -g -o run_training -I/opt/homebrew/Cellar/boost/1.85.0/include src/run_training.cpp src/symbols.cpp src/trader.cpp src/training.cpp src/configs/*.cpp src/indicators/*.cpp src/neat/*.cpp src/trading/*.cpp src/utils/*.cpp -L/opt/homebrew/lib -lboost_iostreams`
**Run executable for run_training** : `./run_training`
**Run executable with debug for run_training** : `lldb ./run_training`
**Set lldb breakpoint** : `breakpoint set -n malloc_error_break`

**Compile for run_tests** : `g++ -std=c++17 -g -o run_tests -I /opt/homebrew/Cellar/googletest/1.14.0/include -I /opt/homebrew/Cellar/boost/1.85.0/include src/symbols.cpp src/run_tests.cpp src/tests/*.cpp src/trader.cpp src/training.cpp src/configs/*.cpp src/configs/tests/*.cpp src/trading/*.cpp src/trading/tests/*.cpp src/utils/*.cpp src/utils/tests/*.cpp src/indicators/*.cpp src/indicators/tests/*.cpp src/neat/*.cpp src/neat/tests/*.cpp -L/opt/homebrew/lib -lboost_iostreams -lgtest -lgmock -lgtest_main -pthread`
**Run executable run_tests** : `./run_tests`

**Compile for generate_json_configs** : `g++ -std=c++17 -o generate_json_configs -I/opt/homebrew/Cellar/boost/1.85.0/include src/generate_json_configs.cpp src/symbols.cpp src/configs/*.cpp src/indicators/*.cpp src/neat/*.cpp src/trading/*.cpp src/utils/*.cpp -L/opt/homebrew/lib -lboost_iostreams`
**Run executable for run_training** : `./generate_json_configs`

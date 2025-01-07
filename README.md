# 🤖 Trading Robots Factory

**Trading Robots Factory** is a powerful C++ project that enables you to create, train, and optimize trading robots using the neuroevolution algorithm ([NEAT](https://fr.wikipedia.org/wiki/Algorithme_NEAT)). Design your own trading strategies, leverage advanced indicators, and simulate trading in a flexible and performance-optimized environment.

---

## Features

- 🔨 **Custom Strategy Creation**: Build your own trading strategies tailored to your needs.
- ✅ **Multi-Timeframe Training**: Supports training across multiple timeframes for better adaptability.
- 🎛 **Diverse Indicators**: Utilize indicators like moving averages, Bollinger Bands, RSI, MACD, ADX, and more.
- 🕰 **Trading Session Management**: Configure precise trading sessions for optimal results.
- ⏱ **Trade Duration Control**: Limit the duration of trades as part of your strategy.
- ✅ **Money Management**: Integrate risk management into your strategies.
- 🩺 **Detailed Reports**: Automatically generate comprehensive HTML reports with strategy statistics.
- 🚀 **Cache Optimization**: Speed up training with optimized caching.
- ⏸️ **Pause and Resume**: Interrupt training anytime and resume from where you left off.
- 📁 **Save Best Strategies**: Store the top-performing strategies for future use.

---

## Getting Started

### 1. Download the Data

- Obtain historical data for a symbol as a CSV file from [MetaTrader 5](https://www.metatrader5.com/fr).
- Save the file to:  
  `./data/<symbol_name>/<symbol_name>_<timeframe>.csv`
  - `<symbol_name>`: Name of the symbol in uppercase.
  - `<timeframe>`: Timeframe of the data (e.g., M1, M5, M15, M30, H1, H4, D1).
- Ensure the CSV file contains the following columns:  
  `<DATE>,<OPEN>,<HIGH>,<LOW>,<CLOSE>,<TICKVOL>`.

---

### 2. Write Your Strategy

1. **Create the Configuration File**  
   - Write your strategy configuration file and save it in the `./src/configs/` folder.

2. **Update the Configuration List**  
   - Modify `./src/configs/config_list.hpp` to include your new configuration file and add its `Config` variable to the `configs` list.

3. **Rebuild the Executable**  
   - Compile the configuration generator using the following command:
  
     ```bash
     g++ -std=c++17 -o generate_json_configs -I/opt/homebrew/Cellar/boost/1.85.0/include src/generate_json_configs.cpp src/symbols.cpp src/configs/*.cpp src/indicators/*.cpp src/neat/*.cpp src/trading/*.cpp src/utils/*.cpp -L/opt/homebrew/lib -lboost_iostreams
     ```

4. **Generate the JSON Configuration Files**  
   - Run the command:
  
     ```bash
     ./generate_json_config
     ```

   - The JSON file will be saved in the `./configs` folder.

---

### 3. Generate Indicators Data for Your Strategy

1. **Update Required Indicators**  
   - Edit the file `./src/generate_csv_indicators_data.cpp` to list the indicators needed for your strategy.

2. **Rebuild the Executable**  
   - Compile the indicators generator using the following command:
  
     ```bash
     g++ -std=c++17 -o generate_csv_indicators_data -I/opt/homebrew/Cellar/boost/1.85.0/include src/generate_csv_indicators_data.cpp src/symbols.cpp src/configs/*.cpp src/indicators/*.cpp src/neat/*.cpp src/trading/*.cpp src/utils/*.cpp -L/opt/homebrew/lib
     ```

3. **Generate Indicators Data**  
   - Execute the following command:
  
     ```bash
     ./generate_csv_indicators_data <symbol> <timeframe>
     ```

   - Repeat this step for all timeframes used in your strategy configuration.

---

### 4. Run the Training

- Start the training process by running:
  
  ```bash
  ./run_training <id> ./configs/<strategy_configuration_file>
  ````

## Configuring Your Strategy

Refer to the example configuration provided in `./src/configs/custom/Example.hpp`.
The Config struct type is defined in the file `./src/type.hpp`.

## Strategy report

At each generation, a detailed HTML report is created for the best-performing strategy. This report includes comprehensive statistics, helping you analyze and refine your strategy.

See an example of report [here](./report_example.html)

## MetaTrader 5 Integration (Work in Progress)

This feature is under development and not functional yet.

### Key Improvements

1. **Clarity and Professional Tone**: Improved phrasing for better readability and a more professional tone.
2. **Formatting**: Enhanced use of sections and subsections for a clean layout.
3. **Consistency**: Standardized terminology and file path usage throughout the document.
4. **MetaTrader Integration**: Reworded the status of the feature to reflect its development stage while maintaining an optimistic tone.
  
## License

[MIT.](./LICENSE)
Stock Analysis Tool

Overview:
This Stock Analysis Tool is a C++ application that helps you analyze stock market data. It provides various features to examine historical stock prices, calculate technical indicators, and identify potential trading opportunities.

Features:
- Load and Display Stock Data: Load stock data from CSV files and display the latest stock information.
- Moving Averages: Calculate and display 3-day moving averages to identify trends.
- Price Analysis: Find highest and lowest stock prices in the dataset.
- Profit Calculations: Calculate potential profits using different strategies.
- Technical Indicators:
  - RSI (Relative Strength Index): Identify overbought and oversold conditions.
  - Next Greater Element: Find when a stock price will next exceed the current price.
- Sorting Functions: Sort stock data by close price or trading volume.
- Search Capabilities: Search for stock information by date.
- Trading Analysis: Find the best single trade (buy-sell combination) for maximum profit.
- Price Similarity Analysis: Calculate a minimum spanning tree weight of price similarities.

Prerequisites:
- C++ compiler (supporting C++11 or later)
- CSV file containing stock data in the following format:
  ```
  Date,Open,High,Low,Close,Adj Close,Volume
  2023-04-14,165.05,166.45,164.49,165.21,164.83,52402400
  ```

Compilation:
Compile the program using your C++ compiler:
```
g++ stockanalysis.cpp -o stockanalysis
```

Running the Program:
1. Run the executable:
   ```
   ./stockanalysis
   ```
2. Enter the path to your CSV file when prompted.
3. Use the menu to access different analysis features.

Menu Options:
1. Show latest stock info: Display the most recent stock data.
2. Show 3-day moving average: Calculate and display 3-day moving averages.
3. Show highest and lowest prices: Find the highest and lowest prices in the dataset.
4. Show total greedy profit: Calculate potential profit using a greedy approach.
5. Show next greater element days: Find when a stock price will exceed the current price.
6. Sort by close price: Sort stock data by closing price.
7. Sort by volume: Sort stock data by trading volume.
8. Search stock by date: Find stock information for a specific date.
9. Show total MST similarity weight: Calculate price similarity using a minimum spanning tree.
10. Show best single trade: Find the optimal buy and sell dates for maximum profit.
11. Show RSI (14-day): Calculate and display the Relative Strength Index.

Technical Indicators Explained:

1. RSI (Relative Strength Index)
- A momentum indicator that measures the speed and change of price movements.
- Values range from 0 to 100.
- Above 70: Potentially overbought condition (price may fall).
- Below 30: Potentially oversold condition (price may rise).

2. Moving Average
- Smooths out price data to identify trends.
- The program uses a 3-day moving average to identify short-term trends.

Data Structures and Algorithms:
This tool uses several key algorithms and data structures:
- Merge Sort: For efficient sorting of stock data
- Binary Search: For quick lookup of stocks by date
- Stack: For finding next greater elements
- Binary Search Tree: For organizing price data
- Minimum Spanning Tree: For analyzing price similarities

Note:
- The program handles only CSV files in the specified format.
- For large datasets, some calculations may take longer to process.

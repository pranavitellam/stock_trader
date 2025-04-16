#include <vector>
#include <string>
#include <algorithm>
#include <limits>
#include <stack>
#include <fstream>
#include <sstream>
#include <cmath>
#include <numeric>
#include <iostream>
#include <iomanip>
#include <map>
#include <cctype>
#include <cstdlib>
#include <set>

using namespace std;

struct Stock {
    string date;
    double open;
    double high;
    double low;
    double close;
    double adjClose;
    long volume;
};

vector<Stock> loadCSV(const string& filename) {
    vector<Stock> stocks;
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;
        return stocks;
    }

    string line;
    bool isHeader = true;

    while (getline(file, line)) {
        if (isHeader) {
            isHeader = false;
            continue;
        }

        stringstream ss(line);
        Stock stock;
        string token;

        getline(ss, token, ','); stock.date = token;
        getline(ss, token, ','); stock.open = stod(token);
        getline(ss, token, ','); stock.high = stod(token);
        getline(ss, token, ','); stock.low = stod(token);
        getline(ss, token, ','); stock.close = stod(token);
        getline(ss, token, ','); stock.adjClose = stod(token);
        getline(ss, token, ','); stock.volume = stol(token);

        stocks.push_back(stock);
    }

    file.close();
    return stocks;
}

vector<double> calculateMovingAverage(const vector<Stock>& data, int window) {
    vector<double> result;
    for (size_t i = 0; i < data.size(); ++i) {
        if (i < window - 1) result.push_back(NAN);
        else {
            double sum = 0;
            for (size_t j = i - window + 1; j <= i; ++j) {
                sum += data[j].close;
            }
            result.push_back(sum / window);
        }
    }
    return result;
}

pair<Stock, Stock> findHighLow(const vector<Stock>& data) {
    if (data.empty()) {
        cout << "Error: No stock data available!" << endl;
        return {Stock(), Stock()};
    }

    Stock highest = data[0];
    Stock lowest = data[0];

    for (const auto& stock : data) {
        if (stock.high > highest.high) highest = stock;
        if (stock.low < lowest.low) lowest = stock;
    }

    return {highest, lowest};
}

double greedyProfit(const vector<Stock>& data) {
    double profit = 0.0;
    for (size_t i = 1; i < data.size(); ++i) {
        if (data[i].close > data[i - 1].close) {
            profit += data[i].close - data[i - 1].close;
        }
    }
    return profit;
}

vector<int> nextGreaterElement(const vector<Stock>& data) {
    int n = data.size();
    vector<int> result(n, -1);
    stack<int> st;

    for (int i = 0; i < n; ++i) {
        while (!st.empty() && data[i].close > data[st.top()].close) {
            result[st.top()] = i;
            st.pop();
        }
        st.push(i);
    }

    return result;
}

// Added missing RSI calculation function
vector<double> calculateRSI(const vector<Stock>& data, int period = 14) {
    vector<double> rsi;
    vector<double> gains, losses;

    for (size_t i = 1; i < data.size(); ++i) {
        double change = data[i].close - data[i-1].close;
        gains.push_back(change > 0 ? change : 0);
        losses.push_back(change < 0 ? -change : 0);
    }

    for (size_t i = period; i < gains.size(); ++i) {
        double avg_gain = accumulate(gains.begin() + i - period, gains.begin() + i, 0.0) / period;
        double avg_loss = accumulate(losses.begin() + i - period, losses.begin() + i, 0.0) / period;
        
        if (avg_loss == 0) {
            rsi.push_back(100.0);
        } else {
            double rs = avg_gain / avg_loss;
            rsi.push_back(100.0 - (100.0 / (1.0 + rs)));
        }
    }

    // Pad with NANs to match original data size
    vector<double> padded(data.size(), NAN);
    copy(rsi.begin(), rsi.end(), padded.begin() + period);
    return padded;
}

// Fixed printBestTrade function
void printBestTrade(const vector<Stock>& data) {
    if (data.empty()) {
        cout << "No data available for trade calculation." << endl;
        return;
    }

    int buy_day = 0;
    int sell_day = 0;
    double max_profit = 0.0;

    for (size_t i = 0; i < data.size(); ++i) {
        for (size_t j = i + 1; j < data.size(); ++j) {
            double profit = data[j].close - data[i].close;
            if (profit > max_profit) {
                max_profit = profit;
                buy_day = i;
                sell_day = j;
            }
        }
    }

    cout << fixed << setprecision(2);
    cout << "\nBest Single Trade:\n";
    cout << "Buy on:  " << data[buy_day].date << " at " << data[buy_day].close << endl;
    cout << "Sell on: " << data[sell_day].date << " at " << data[sell_day].close << endl;
    cout << "Profit:  " << max_profit << endl;
}

class PriceBST {
    struct Node {
        double price;
        string date;
        Node* left;
        Node* right;

        Node(double p, string d) : price(p), date(d), left(nullptr), right(nullptr) {}
    };

    Node* root = nullptr;

    void insert(Node*& node, double price, const string& date) {
        if (!node) node = new Node(price, date);
        else if (price < node->price) insert(node->left, price, date);
        else insert(node->right, price, date);
    }

    void inOrder(Node* node, vector<pair<double, string>>& result) {
        if (!node) return;
        inOrder(node->left, result);
        result.push_back({node->price, node->date});
        inOrder(node->right, result);
    }

    void cleanup(Node* node) {
        if (!node) return;
        cleanup(node->left);
        cleanup(node->right);
        delete node;
    }

public:
    void insert(double price, const string& date) {
        insert(root, price, date);
    }

    vector<pair<double, string>> getSortedPrices() {
        vector<pair<double, string>> result;
        inOrder(root, result);
        return result;
    }

    ~PriceBST() {
        cleanup(root);
    }
};

void merge(vector<Stock>& data, int left, int mid, int right) {
    vector<Stock> leftVec(data.begin() + left, data.begin() + mid + 1);
    vector<Stock> rightVec(data.begin() + mid + 1, data.begin() + right + 1);

    size_t i = 0, j = 0, k = left;
    while (i < leftVec.size() && j < rightVec.size()) {
        if (leftVec[i].close <= rightVec[j].close) data[k++] = leftVec[i++];
        else data[k++] = rightVec[j++];
    }
    while (i < leftVec.size()) data[k++] = leftVec[i++];
    while (j < rightVec.size()) data[k++] = rightVec[j++];
}

void mergeSort(vector<Stock>& data, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(data, left, mid);
        mergeSort(data, mid + 1, right);
        merge(data, left, mid, right);
    }
}

void mergeByKey(vector<Stock>& data, int left, int mid, int right, const string& key) {
    vector<Stock> leftVec(data.begin() + left, data.begin() + mid + 1);
    vector<Stock> rightVec(data.begin() + mid + 1, data.begin() + right + 1);

    size_t i = 0, j = 0, k = left;
    while (i < leftVec.size() && j < rightVec.size()) {
        bool condition = false;
        if (key == "close") condition = leftVec[i].close <= rightVec[j].close;
        else if (key == "volume") condition = leftVec[i].volume <= rightVec[j].volume;
        else condition = leftVec[i].date <= rightVec[j].date;

        if (condition) data[k++] = leftVec[i++];
        else data[k++] = rightVec[j++];
    }
    while (i < leftVec.size()) data[k++] = leftVec[i++];
    while (j < rightVec.size()) data[k++] = rightVec[j++];
}

void mergeSort(vector<Stock>& data, int left, int right, const string& key) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(data, left, mid, key);
        mergeSort(data, mid + 1, right, key);
        mergeByKey(data, left, mid, right, key);
    }
}

int binarySearchByDate(const vector<Stock>& data, const string& date) {
    int low = 0, high = data.size() - 1;
    while (low <= high) {
        int mid = (low + high) / 2;
        if (data[mid].date == date) return mid;
        else if (data[mid].date < date) low = mid + 1;
        else high = mid - 1;
    }
    return -1;
}

double minimumSpanningTree(const vector<Stock>& data) {
    int n = data.size();
    if (n == 0) return 0;

    vector<bool> inMST(n, false);
    vector<double> key(n, numeric_limits<double>::max());
    key[0] = 0;
    double total = 0;

    for (int count = 0; count < n; ++count) {
        double minKey = numeric_limits<double>::max();
        int u = -1;
        for (int v = 0; v < n; ++v) {
            if (!inMST[v] && key[v] < minKey) {
                minKey = key[v];
                u = v;
            }
        }

        if (u == -1) break;
        inMST[u] = true;
        total += key[u];

        for (int v = 0; v < n; ++v) {
            double weight = fabs(data[u].close - data[v].close);
            if (!inMST[v] && weight < key[v]) {
                key[v] = weight;
            }
        }
    }

    return total;
}

void printLatestStockInfo(const Stock& stock) {
    cout << "Latest Stock Info:\n";
    cout << "Date: " << stock.date << ", Open: " << stock.open << ", High: " << stock.high
         << ", Low: " << stock.low << ", Close: " << stock.close << ", AdjClose: " << stock.adjClose
         << ", Volume: " << stock.volume << endl;
}

void printMovingAverage(const vector<Stock>& data, const vector<double>& ma) {
    cout << "\nMoving Average:\n";
    for (size_t i = 0; i < ma.size(); ++i) {
        cout << data[i].date << ": ";
        if (std::isnan(ma[i])) cout << "N/A" << endl;
        else cout << ma[i] << endl;
    }
}

void printHighLow(const Stock& high, const Stock& low) {
    cout << "\nHigh/Low Info:\n";
    cout << "Highest: " << high.high << " on " << high.date << endl;
    cout << "Lowest: " << low.low << " on " << low.date << endl;
}

void printNextGreater(const vector<Stock>& data, const vector<int>& next) {
    cout << "\nNext Greater Close Prices:\n";
    for (size_t i = 0; i < next.size(); ++i) {
        cout << data[i].date << " -> ";
        if (next[i] == -1) cout << "None" << endl;
        else cout << data[next[i]].date << " (" << data[next[i]].close << ")" << endl;
    }
}

void printRSI(const vector<Stock>& data, const vector<double>& rsi) {
    cout << fixed << setprecision(2);
    cout << "\nRelative Strength Index (RSI):\n";
    for (size_t i = 0; i < rsi.size(); ++i) {
        if (!isnan(rsi[i])) {
            cout << data[i].date << ": " << rsi[i];
            if (rsi[i] >= 70) cout << " (Overbought)";
            else if (rsi[i] <= 30) cout << " (Oversold)";
            cout << endl;
        }
    }
}

void printStockByDate(const vector<Stock>& data, const string& date) {
    int idx = binarySearchByDate(data, date);
    if (idx == -1) cout << "Stock on " << date << " not found." << endl;
    else printLatestStockInfo(data[idx]);
}

void runMenu(vector<Stock> stocks) {
    int choice;
    do {
        cout << "\n=== Stock Analysis Menu ===" << endl;
        cout << "1. Show latest stock info" << endl;
        cout << "2. Show 3-day moving average" << endl;
        cout << "3. Show highest and lowest prices" << endl;
        cout << "4. Show total greedy profit" << endl;
        cout << "5. Show next greater element days" << endl;
        cout << "6. Sort by close price (Merge Sort)" << endl;
        cout << "7. Sort by volume (Merge Sort)" << endl;
        cout << "8. Search stock by date" << endl;
        cout << "9. Show total MST similarity weight" << endl;
        cout << "10. Show best single trade" << endl;
        cout << "11. Show RSI (14-day)" << endl;
        cout << "0. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                if (!stocks.empty()) printLatestStockInfo(stocks.back());
                break;
            case 2:
                printMovingAverage(stocks, calculateMovingAverage(stocks, 3));
                break;
            case 3:
                if (!stocks.empty()) {
                    pair<Stock, Stock> result = findHighLow(stocks);
                    printHighLow(result.first, result.second);
                }
                break;
            case 4:
                cout << "Total Greedy Profit: " << greedyProfit(stocks) << endl;
                break;
            case 5:
                printNextGreater(stocks, nextGreaterElement(stocks));
                break;
            case 6:
                mergeSort(stocks, 0, stocks.size() - 1, "close");
                for (const auto& s : stocks) cout << s.date << ": " << s.close << endl;
                break;
            case 7:
                mergeSort(stocks, 0, stocks.size() - 1, "volume");
                for (const auto& s : stocks) cout << s.date << ": " << s.volume << endl;
                break;
            case 8: {
                cout << "Enter date (YYYY-MM-DD): ";
                string d;
                cin >> d;
                printStockByDate(stocks, d);
                break;
            }
            case 9:
                cout << "Total MST (price similarity weight): " << minimumSpanningTree(stocks) << endl;
                break;
            case 10:
                printBestTrade(stocks);
                break;
            case 11: {
                vector<double> rsi = calculateRSI(stocks, 14);
                printRSI(stocks, rsi);
                break;
            }
            case 0:
                cout << "Exiting..." << endl;
                break;
            default:
                cout << "Invalid choice. Try again." << endl;
        }
    } while (choice != 0);
}

int main() {
    string filename;
    cout << "Enter CSV filename (with path if needed): ";
    cin >> filename;

    vector<Stock> stocks = loadCSV(filename);
    if (!stocks.empty()) {
        runMenu(stocks);
    } else {
        cout << "No data loaded. Exiting program." << endl;
    }

    return 0;
}

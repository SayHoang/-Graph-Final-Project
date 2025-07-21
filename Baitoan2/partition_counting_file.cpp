#include <iostream>
#include <vector>
#include <fstream>
#include <string>
using namespace std;

// Hàm đếm số phân hoạch của n với phần tử lớn nhất là k (p_max(n,k))
int count_partitions_max_k(int n, int k) {
    if (k > n) return 0;
    if (k == n) return 1;
    if (k == 0) return 0;
    
    // dp[i][j] = số phân hoạch của i với phần tử lớn nhất là j
    vector<vector<int>> dp(n + 1, vector<int>(k + 1, 0));
    
    // Khởi tạo: phân hoạch của 0
    dp[0][0] = 1;
    
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= k && j <= i; j++) {
            // Có thể sử dụng j làm phần tử lớn nhất
            if (j <= i) {
                // Thêm j vào phân hoạch của i-j với phần tử lớn nhất <= j
                for (int prev = 0; prev <= j; prev++) {
                    if (i - j >= 0) {
                        dp[i][j] += dp[i - j][prev];
                    }
                }
            }
        }
    }
    
    return dp[n][k];
}

// Hàm đếm số phân hoạch của n thành đúng k phần (p_k(n)) - SỬA LỖI
int count_partitions_k_parts(int n, int k) {
    if (k > n) return 0;
    if (k == 1) return 1;
    if (k == n) return 1;
    
    // dp[i][j] = số phân hoạch của i thành đúng j phần
    vector<vector<int>> dp(n + 1, vector<int>(k + 1, 0));
    
    // Khởi tạo các trường hợp cơ sở
    dp[0][0] = 1;  // Có 1 cách phân hoạch 0 thành 0 phần (phân hoạch rỗng)
    
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= k && j <= i; j++) {
            // Công thức truy hồi đúng: p_k(n) = p_{k-1}(n-1) + p_k(n-k)
            // dp[i][j] = dp[i-1][j-1] + dp[i-j][j]
            
            // Thêm 1 vào mỗi phần của phân hoạch i-1 thành j-1 phần
            dp[i][j] += dp[i - 1][j - 1];
            
            // Thêm 1 vào mỗi phần của phân hoạch i-j thành j phần
            if (i - j >= 0) {
                dp[i][j] += dp[i - j][j];
            }
        }
    }
    
    return dp[n][k];
}

// Hàm in tất cả phân hoạch của n với phần tử lớn nhất là k
void print_partitions_max_k(int n, int k, vector<int>& current, int sum, int max_used) {
    if (sum == n) {
        if (max_used == k) {
            cout << "  ";
            for (int i = 0; i < current.size(); i++) {
                cout << current[i];
                if (i < current.size() - 1) cout << " + ";
            }
            cout << endl;
        }
        return;
    }
    
    for (int i = 1; i <= k && sum + i <= n; i++) {
        current.push_back(i);
        print_partitions_max_k(n, k, current, sum + i, max(max_used, i));
        current.pop_back();
    }
}

// Hàm in tất cả phân hoạch của n thành đúng k phần
void print_partitions_k_parts(int n, int k, vector<int>& current, int sum, int parts) {
    if (parts == k) {
        if (sum == n) {
            cout << "  ";
            for (int i = 0; i < current.size(); i++) {
                cout << current[i];
                if (i < current.size() - 1) cout << " + ";
            }
            cout << endl;
        }
        return;
    }
    
    int start = (current.empty()) ? 1 : current.back();
    for (int i = start; i <= n - sum && parts + 1 <= k; i++) {
        if (sum + i <= n) {
            current.push_back(i);
            print_partitions_k_parts(n, k, current, sum + i, parts + 1);
            current.pop_back();
        }
    }
}

void process_test_case(int n, int k) {
    cout << "\n=== TEST CASE: n = " << n << ", k = " << k << " ===" << endl;
    
    // Tính p_max(n,k)
    int p_max_nk = count_partitions_max_k(n, k);
    cout << "p_max(" << n << "," << k << ") = " << p_max_nk << endl;
    
    // Tính p_k(n)
    int p_k_n = count_partitions_k_parts(n, k);
    cout << "p_" << k << "(" << n << ") = " << p_k_n << endl;
    
    // So sánh kết quả
    cout << "\n=== SO SANH ===" << endl;
    if (p_max_nk == p_k_n) {
        cout << "p_max(" << n << "," << k << ") = p_" << k << "(" << n << ")" << endl;
        cout << "p_max(" << n << "," << k << ") = p_" << k << "(" << n << ") = " << p_max_nk << endl;
    } else {
        cout << "p_max(" << n << "," << k << ") != p_" << k << "(" << n << ")" << endl;
        cout << "p_max(" << n << "," << k << ") = " << p_max_nk << endl;
        cout << "p_" << k << "(" << n << ") = " << p_k_n << endl;
    }
    
    // In chi tiết các phân hoạch (nếu n và k không quá lớn)
    if (n <= 10 && k <= 5) {
        cout << "\n=== CHI TIET CAC PHAN HOACH ===" << endl;
        
        cout << "\nCac phan hoach cua " << n << " voi phan tu lon nhat la " << k << ":" << endl;
        vector<int> temp;
        print_partitions_max_k(n, k, temp, 0, 0);
        
        cout << "\nCac phan hoach cua " << n << " thanh dung " << k << " phan:" << endl;
        temp.clear();
        print_partitions_k_parts(n, k, temp, 0, 0);
    }
    
    cout << "\n" << string(50, '=') << endl;
}

int main() {
    cout << "=== BAI TOAN 2: DEM PHAN HOACH SO NGUYEN ===" << endl;
    cout << "Doc input tu file 'input.txt'..." << endl;
    
    ifstream input_file("input.txt");
    if (!input_file.is_open()) {
        cout << "Loi: Khong the mo file input.txt!" << endl;
        cout << "Hay nhap n va k thu cong: ";
        int n, k;
        cin >> n >> k;
        process_test_case(n, k);
        return 1;
    }
    
    int n, k;
    int test_count = 0;
    
    while (input_file >> n >> k) {
        test_count++;
        process_test_case(n, k);
    }
    
    input_file.close();
    
    cout << "\n=== TONG KET ===" << endl;
    cout << "Da xu ly " << test_count << " test case." << endl;
    
    return 0;
} 
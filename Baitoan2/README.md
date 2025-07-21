# Bài toán 2: Đếm phân hoạch số nguyên

## Mô tả bài toán
Cho hai số nguyên dương n và k. Viết chương trình để:
1. Đếm số phân hoạch p_max(n,k) của n sao cho phần tử lớn nhất là k
2. Đếm số phân hoạch p_k(n) của n thành đúng k phần
3. So sánh p_max(n,k) và p_k(n)

## Lý thuyết
- **p_max(n,k)**: Số phân hoạch của n sao cho phần tử lớn nhất là k
- **p_k(n)**: Số phân hoạch của n thành đúng k phần

Theo lý thuyết phân hoạch số nguyên, ta có: **p_max(n,k) = p_k(n)**

## Files trong thư mục

### 1. `partition_counting.cpp`
- Chương trình chính để nhập n, k từ bàn phím
- Tính và so sánh p_max(n,k) và p_k(n)
- In chi tiết các phân hoạch (nếu n ≤ 10 và k ≤ 5)

### 2. `partition_counting_file.cpp`
- Phiên bản đọc input từ file `input.txt`
- Xử lý nhiều test case cùng lúc
- Phù hợp để test với nhiều bộ dữ liệu

### 3. `input.txt`
- File chứa các test case
- Mỗi dòng chứa hai số n và k
- Các test case mẫu:
  ```
  5 3
  6 2
  7 4
  8 3
  10 5
  4 2
  6 3
  5 2
  ```

## Cách biên dịch và chạy

### Phiên bản nhập từ bàn phím:
```bash
g++ -o partition_counting partition_counting.cpp
./partition_counting
```

### Phiên bản đọc từ file:
```bash
g++ -o partition_counting_file partition_counting_file.cpp
./partition_counting_file
```

## Ví dụ kết quả

### Test case: n = 5, k = 3

**p_max(5,3)**: Số phân hoạch của 5 với phần tử lớn nhất là 3
- 3 + 2
- 3 + 1 + 1
→ p_max(5,3) = 2

**p_3(5)**: Số phân hoạch của 5 thành đúng 3 phần
- 3 + 1 + 1
- 2 + 2 + 1
→ p_3(5) = 2

**Kết luận**: p_max(5,3) = p_3(5) = 2 ✓

## Thuật toán

### Tính p_max(n,k):
- Sử dụng quy hoạch động
- dp[i][j] = số phân hoạch của i với phần tử lớn nhất là j
- Công thức: dp[i][j] = Σ(dp[i-j][prev]) với prev ≤ j

### Tính p_k(n):
- Sử dụng quy hoạch động
- dp[i][j] = số phân hoạch của i thành đúng j phần
- Công thức: dp[i][j] = dp[i-j][j-1] + dp[i-j][j]

## Lưu ý
- Chương trình có thể in chi tiết các phân hoạch nếu n ≤ 10 và k ≤ 5
- Với n và k lớn, chỉ hiển thị kết quả số lượng
- Đảm bảo file `input.txt` tồn tại khi chạy phiên bản file 
q :: Int -> Int
q 0 = 0
q n = 1 + next_q n 0 n - 1

next_q :: Int -> Int -> Int -> Int
next_q n amount next = if amount == n
                       then 1
                       else if amount > n || next < 1
                       then 0
                       else (next_q n (amount + next) (next - 1)) + (next_q n amount (next - 1))
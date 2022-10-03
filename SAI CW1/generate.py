# (a ∧ b ∧ c ∧ d) ∨ e ∨ f
for i in range(1 << 6):
    a, b, c, d, e, f = map(int, format(i, "06b"))
    print(a, b, c, d, e, f, " ",(a & b & c & d | e | f))

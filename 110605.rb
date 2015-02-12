def f(n)
  return 1 if n == 0 || n == 1
  n*f(n-1)
end

def n(k, d)
  return 1 if d == 0
  n(k,d-1) + k**d
end

def a(k, d)
  return 1 if d == 0
  return f(k) if d == 1
  f((n(k, d)-1))/(f(n(k, d-1))**k)*a(k, d-1)**k
end

puts a(3,7)

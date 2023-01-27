int fibonacci(int n) {
  if (n == 0) {
    return 0;
  } else { if (n == 1) {
    return 1;
  } else {
    return fibonacci(n - 1) + fibonacci(n - 2);
  }
  }
  return 0;
}

int main(void) {
  int i = 0;
  int j;
  while (i < 10) {
    j = fibonacci(i);
    i += 1;
  }
  return j;
} 
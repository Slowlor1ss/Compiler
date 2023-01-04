//a*(b+3);

//int test()
//{
//    int b = 2;
//    int c = 3;
//    int d = c+d;
//    return d;
//}

// Test basic arithmetic operations

int test_arithmetic() {
  int x = 5 + 3;      // Should return 8
  int y = x - 2;      // Should return 6
  int z = y * 4;      // Should return 24
  return z;
}

// Test if statements

int test_if(int x, int y) {
  int a;
  if (x > 4) {
    a = 1;
  } else {
    a = 0;
  }

  // Should set 'a' to 1

  int b;
  if (y < 10) {
    b = 1;
  } else {
    b = 0;
  }

  // Should set 'b' to 1
  return a + b;
}

// Test while loops

int test_while() {
  int i = 0;
  while (i < 5) {
    i = i + 1;
  }

  // Should set 'i' to 5

  int j = 0;
  while (j < 10) {
    j = j + 1;
    if (j == 5) {
      return i + j;//break;
    }
  }

  // Should set 'j' to 5
  return i + j;
}

// Test function calls

int test_function_calls() {
  int k = test_arithmetic();
  int x = 8;
  int l = test_if(x, 6);
  int m = test_while();
  return k + l + m;
}

int main() {
  int result = test_function_calls();

  // Should return 43
  return result;
}

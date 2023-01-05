
int main() {
    int x;
    int y;

  int a;
  if (x > 4) {
    a = 0;
  } else {
    a = 1;
  }

  a = 2;

  // Should set 'a' to 1
  int b = 0;
  if (y < 10) {
    a = 3;
  } else {
    b = 1;
  }

  // Should set 'b' to 1
  return a + b;
}

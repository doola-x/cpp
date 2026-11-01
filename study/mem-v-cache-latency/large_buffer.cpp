#include <chrono>
#include <iostream>
#include <vector>

using namespace std::chrono;

int main() {
  std::cout << "==================================" << '\n';
  std::cout << "==================================" << '\n';
  std::cout << "Benching L1 and Mem Stuff on Macos" << '\n';
  std::cout << "==================================" << '\n';
  std::cout << "==================================" << '\n' << '\n' << '\n';
  const int BLOCK = 1'000'000;

  // ------------------ init a buffer and flood the size of L1
  std::vector<std::byte> buffer{0};
  buffer.reserve(BLOCK);
  // ------------------ touch buffer so we actually load it
  buffer[0] = std::byte{0b00000001};
  volatile auto sink = 0;

  auto clock = steady_clock();

  auto start = clock.now();
  // ------------------ write to a volatile sink, just touch buffer
  for (int i = 0; i < BLOCK; i += 1) {
    sink = (int)buffer[0];
  }
  auto end = clock.now();

  auto duration_read = duration_cast<microseconds>(end - start);
  std::cout << "duration(ms) reading buffer 1m times: " << duration_read.count()
            << '\n';

  start = clock.now();
  // ------------------ update volatile sink, how does this affect data cache?
  for (int i = 0; i < BLOCK; i += 1) {
    buffer[i] = std::byte{0b01010101};
    sink += (int)buffer[i];
  }
  end = clock.now();

  auto duration = duration_cast<microseconds>(end - start);
  std::cout << "duration(ms) writing to buffer 1m times: " << duration.count()
            << '\n';

  return 0;
}

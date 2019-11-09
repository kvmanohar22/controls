#include "controls/global.hpp"
#include "controls/siso.hpp"

namespace {

using namespace std;

void test() {
  vector<float> alphas{1, 2, 3}; 
  vector<float> roots{-1,-1,-2};
  controls::SISO<float> siso(alphas, roots);
  siso.summary();
}

}

int main() {
  ::test();
}


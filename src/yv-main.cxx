#include "tools.hh"
#include <cstdio>
#include <cstdlib>
#include "yvonne.hh"
int main(int argc, char* argv[]) {
  int in = 0;
  if (argc > 1) in = atoi(argv[1]);
  plotVariables();
  return 0;
}

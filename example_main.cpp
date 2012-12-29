#include	<cstdlib>
#include	<algorithm>
#include	"getopt.hh"

void
print_args(char *str) {
  std::cout << str << std::endl;
}

int
main(int argc, char **argv) {
  Getopt	opt(argc, argv, "a:bc*");

  opt.dump();
  return (EXIT_SUCCESS);
}

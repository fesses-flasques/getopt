#include	<cstdlib>
#include	<algorithm>
#include	"getopt.hh"

void
print_args(char *str) {
  std::cout << str << std::endl;
}

int
main(int argc, char **argv) {
  const char		*l_opt[] = {
    "color",
    NULL
  };
  const char		*mc_opt[] = {
    "name:<3>",
    "group:",
    "false",
    NULL
  };
  Getopt	*opt;
  std::string	l("lol");
  try {
    opt = new Getopt(argc, argv, "a:bc*d:<3>ef:gh:", l_opt, mc_opt);
  } catch (...) {
    std::cout << "Should appear but never does" << std::endl;
  }
  opt->dump();
  delete opt;

  return (EXIT_SUCCESS);
}

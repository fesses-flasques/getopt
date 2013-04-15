#include	<cstdlib>
#include	<algorithm>
#include	"getopt.hh"

void
print_args(char *str) {
  std::cout << str << std::endl;
}

int
main(int argc, char **argv) {
  const char		*sg_opt = "a:bc*d:<3>ef:gh:";
  const char		*l_opt[] = {
    "color",
    NULL
  };
  const char		*mc_opt[] = {
    "-group:",
    "-or",
    "-name:",
    "-false",
    "{",
    NULL
  };
  Getopt	*opt;
  std::string	l("lol");
  try {
    opt = new Getopt(argc, argv, sg_opt, l_opt, mc_opt);
  } catch (...) {
    std::cout << "Should appear but never does" << std::endl;
  }
  opt->dump();
  delete opt;

  return (EXIT_SUCCESS);
}

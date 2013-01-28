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
    ":name",
    ":group",
    "false",
    NULL
  };
  Getopt	*opt;
  std::string	l("lol");
  try {
    opt = new Getopt(argc, argv, "a:bc*d:<3>e", l_opt, mc_opt);
  } catch (...) {
    std::cout << "NU T'ES LA" << std::endl;
  }
  opt->dump();
  delete opt;

  return (EXIT_SUCCESS);
}

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
  try {
    Getopt	opt(argc, argv, "a:bc*d:<3>e", l_opt, mc_opt);
    opt.dump();
  } catch (std::exception &e) {
    std::cout << "NU T'ES LA" << std::endl;
    std::cout << e.what() << std::endl;
  }

  return (EXIT_SUCCESS);
}

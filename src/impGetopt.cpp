#include	<cstdio>
#include	"getopt.hh"

inline bool	Getopt::
HAS_ARGS(char c) const { return (c == SINGLE_HANDLE_CHAR || c == MULT_HANDLE_CHAR); }

inline bool	Getopt::
ISLOWER(char c) const { return (c >= 'a' && c <= 'z'); }

inline bool	Getopt::
ISUPPER(char c) const { return (c >= 'A' && c <= 'Z'); }

inline int	Getopt::
SRCCASE(char c) const { return (ISLOWER(c) ? 'a' : 'A'); }

inline int	Getopt::
TABOPT(char c) const { return (1 << (c - SRCCASE(c))); }

inline int	Getopt::
MAPOPT(int x, char c) const { return (x & TABOPT(c)); }

inline bool	Getopt::
_still_args() const {
  return (_ind < (_argc - _swp));
}

bool		Getopt::
isSet(char c) const {
  if (ISLOWER(c))                                                              
    return ((MAPOPT(_low, c)) ? true : false);
  if (ISUPPER(c))
    return ((MAPOPT(_up, c)) ? true : false);
  return (false);
}

char		*Getopt::
getLastArg(char c) const {
  std::map<char, std::list<char *> >::const_iterator	it;

  if (!c)
    return (_rem.back());
  if ((it = this->_args.find(c)) == this->_args.end())
    return (NULL);
  return (it->second.back());
}

const std::list<char *>		*Getopt::
getArgs(char c) const {
  std::map<char, std::list<char *> >::const_iterator	it;

  if (!c)
    return (&_rem);
  if ((it = this->_args.find(c)) == this->_args.end())
    return (NULL);
  return (&it->second);
}

void		Getopt::
_reinit_vars() {
  this->_ind = 1;
  this->_opt = 0;
  this->_swp = 0;
  this->_low = 0;
  this->_up = 0;
  this->_ign.clear();
}

void		Getopt::
_getswap() {
  char		*tmp = _argv[_ind];
  int		i = _ind + 1;

  while (i < _argc) {
    _argv[i - 1] = _argv[i];
    ++i;
  }
  _argv[_argc - 1] = tmp;
  ++this->_swp;
}

bool	Getopt::
_get_mc_option() {
  int		i = 0, args_ndx;
  std::string	cmp(_argv[_ind]);

  if (_mc_opt == NULL)
    return (false);
  while (_mc_opt[i]) {
    args_ndx = 0;
    if (cmp == _mc_opt[i] + args_ndx) {
      return (true);
    }
  }
  return (false);
}

bool	Getopt::
_get_l_option() {
  if (_argv[_ind][0] == OPT_CHAR && _argv[_ind][1] == OPT_CHAR) {
    std::cout << "long opt:\t" << _argv[_ind] << std::endl;
    ++_ind;
    return (true);
  }
  return (false);
}

char		Getopt::
_gn_opt() {
  char		ret;
  size_t	pos;

  _optarg = NULL;
  if (!_opt) {
    while (_still_args() && _argv[_ind][0] != OPT_CHAR)
      this->_getswap();

    if (_ind >= (_argc - _swp))
      return (0);

    // Multi-characters options
    if (this->_get_mc_option())
      return (0);
    // Long options
    if (this->_get_l_option())
      return (0);
  }
  if (!(ret = _argv[_ind][++_opt])) {
    ++_ind;
    _opt = 0;
    return (this->_gn_opt());
  }

  if ((pos = _fmt.find_first_of(ret)) == std::string::npos) {
    _ign += ret;
    return (0);
  }
  _setopt(ret);
  return (_resolve_arg(pos, ret));
}

void		Getopt::
_setopt(char c) {
  if (ISLOWER(c))
  {
    if (!MAPOPT(_low, c))
      _low += MAPOPT(0xFFFFFFFF, c);
  }
  else if (ISUPPER(c))
  {
    if (!MAPOPT(_up, c))
      _up += MAPOPT(0xFFFFFFFF, c);
  }
}

int		Getopt::
_nb_args(size_t pos) const {
  (void)pos;
  // Define a format string and parse it! :D
  return (9);
}

void		Getopt::
_setarg(char hasarg, char c, int nb) {
  (void)nb;
  if (hasarg == SINGLE_HANDLE_CHAR && _optarg && nb == 1)
    _args[c].push_back(_optarg);
  else if ((hasarg == MULT_HANDLE_CHAR || nb > 1) && _optarg) {
    _args[c].push_back(_optarg);
    while (_still_args() && _argv[_ind] &&
	((_argv[_ind][0] != '-' && hasarg == MULT_HANDLE_CHAR) || (--nb > 0))) {
      std::cout << nb << std::endl;
      _args[c].push_back(_argv[_ind]);
      ++_ind;
    }
    return ;
  }
}

bool		Getopt::
_resolve_arg(size_t pos, char c) {
  char		hasarg;

  if (HAS_ARGS((hasarg = _fmt[pos + 1]))) {
    if (!_argv[_ind][_opt + 1]) {
      if (++_ind >= (_argc - _swp))
	return (false);
      _optarg = _argv[_ind];
    }
    else
      _optarg = _argv[_ind] + _opt + 1;
    ++_ind;
    _opt = 0;
    this->_setarg(hasarg, c, _nb_args(pos));
    return (true);
  }
  return (false);
}

void		Getopt::
_build_opts() {
  this->_reinit_vars();
  while (_still_args())
    _gn_opt();
  while (_ind < (_argc)) {
    _rem.push_back(_argv[_ind]);
    ++_ind;
  }
}

Getopt::
Getopt(int argc, char **argv, std::string &fmt, char **l_opt, char **mc_opt)
  : _argc(argc), _argv(argv), _fmt(fmt), _l_opt(l_opt), _mc_opt(mc_opt) {
    this->_build_opts();
  }

Getopt::
Getopt(int argc, char **argv, const char *fmt, char **l_opt, char **mc_opt)
  : _argc(argc), _argv(argv), _fmt(std::string(fmt)), _l_opt(l_opt), _mc_opt(mc_opt) {
    this->_build_opts();
  }

Getopt::
~Getopt() {
}

Getopt::
Getopt(const Getopt &oth) {
  (void)oth;
  std::cout << "Hello World ! (copy reference)" << std::endl;
}

Getopt		&Getopt::
operator=(const Getopt &oth) {
  (void)oth;
  std::cout << "Hello World ! (Operator =)" << std::endl;
  return (*this);
}

void	Getopt::
dump() const {
  const std::list<char *>	*args;
  std::list<char *>::const_iterator	it;
  char				c;

  if (_low || _up) {
    std::cout << "Options:\t" << "-";
    for (c = 'a'; c <= 'z'; ++c)
      if (this->isSet(c))
	std::cout << c;
    for (c = 'A'; c <= 'Z'; ++c)
      if (this->isSet(c))
	std::cout << c;
    std::cout << std::endl;
    for (c = 'a'; c <= 'z'; ++c)
      if ((args = getArgs(c)))
      {
	std::cout << "\t" << c << ":";
	int i = 0;
	for (it = args->begin(); it != args->end(); ++it,++i)
	  std::cout << (i ? "\t" : "") << "\t["
	    << i << "]: "
	    << *it << std::endl;
      }
    for (c = 'A'; c <= 'Z'; ++c)
      if ((args = getArgs(c)))
      {
	std::cout << "\t" << c << ":";
	int i = 0;
	for (it = args->begin(); it != args->end(); ++it,++i)
	  std::cout << (i ? "\t" : "") << "\t["
	    << i << "]: "
	    << *it << std::endl;
      }
  }
  if (_ign.size())
    std::cout << "Ignored:\t" << "-" << _ign << std::endl;
  if ((args = getArgs(0))->size()) {
    std::cout << "Remain\t:";
    int i = 0;
    for (it = args->begin(); it != args->end(); ++it,++i)
      std::cout << (i ? "\t" : "") << "\t["
	<< i << "]: "
	<< *it << std::endl;
  }
}

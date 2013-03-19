#include	<cstdio>
#include	"getopt.hh"

void		Getopt::
_thrower(
    char token,
    const char *err,
    const char *which,
    const char *fmt,
    unsigned i
    ) const throw() {
  std::string	error(err);

  error += " [";
  error += token;
  error += "] in {";
  error += which;
  error += "} --\n";
  error += fmt;
  error += '\n';
  error.append(i, '~');
  error += '^';
  // OUAIS J'AI PAS PU FAIRE PLUS CRADE! :D
  syntaxError *t = new syntaxError(error);
  throw *t;
}

inline bool	Getopt::
HAS_ARGS(char c) const { return (c == SINGLE_HANDLE_CHAR || c == MULT_HANDLE_CHAR); }

inline bool	Getopt::
ISDIGIT(char c) const { return (c >= '0' && c <= '9'); }

inline bool	Getopt::
ISLOWER(char c) const { return (c >= 'a' && c <= 'z'); }

inline bool	Getopt::
ISUPPER(char c) const { return (c >= 'A' && c <= 'Z'); }

inline bool	Getopt::
ISOPT(char c) const { return (ISLOWER(c) || ISUPPER(c)); }

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

inline bool	Getopt::
_no_more_args() const {
  return (!_still_args());
}

bool		Getopt::
isSet(char c) const {
  if (ISLOWER(c))                                                              
    return ((MAPOPT(_low, c)) ? true : false);
  if (ISUPPER(c))
    return ((MAPOPT(_up, c)) ? true : false);
  return (false);
}

char		**Getopt::
getRemain() const {
  return (this->_argv + _ind);
}

char		*Getopt::
getLastArg(char c) const {
  std::map<char, args_data>::const_iterator	it;

  if (!c)
    return (_rem.back());
  if ((it = this->_sg_args.find(c)) == this->_sg_args.end())
    return (NULL);
  return (it->second.args->back());
}

const std::list<char *>		*Getopt::
getArgs(char c) const {
  std::map<char, args_data>::const_iterator	it;

  if (!c)
    return (&_rem);
  if ((it = this->_sg_args.find(c)) == this->_sg_args.end())
    return (NULL);
  return ((it->second.args && it->second.args->empty())
      ? NULL
      : it->second.args
      );
}

unsigned int	Getopt::
_bracket_token(std::string &fmt, unsigned int i, char treat) {
  if (fmt[i] != '<')
    return (i);
  if (ISDIGIT(fmt[++i])) {// Simple number
    _sg_args[treat].nb = charstrait_extract<int>(fmt.c_str() + i); 
    while (ISDIGIT(fmt[++i]));
  }
  if (fmt[i] != '>')
    this->_thrower(fmt[i], "Expected '>' token", "UNDEFINED", _fmt.c_str(), i);
  return (i + 1);
}

bool		Getopt::
_ptoken_caller(
    unsigned int (Getopt::*ptr)(std::string &, unsigned int, char),
    std::string &fmt,
    unsigned int &i,
    char treat
    ) {
  unsigned int	s;

  if ((s = (this->*ptr)(fmt, i, treat)) == i)
    return (false);
  i = s;
  return (true);
}

void		Getopt::
_parse_hasarg(std::string &fmt, unsigned int &i, char treat) {
  unsigned int	ndx = 0;
  unsigned int	(Getopt::* const tok[])(std::string &, unsigned int, char) = {
    &Getopt::_bracket_token
  };
  this->_sg_args[treat].nb = 1;
  ++i;
  if (!_fmt[i] || ISOPT(_fmt[i])) {
    return ;
  }
  while (ndx < (sizeof(tok) / sizeof(*tok))) {
    if (_ptoken_caller(tok[ndx], fmt, i, treat) == true)
      return ;
    ++ndx;
  }
  _thrower(_fmt[i], "Unexpected token", "_fmt", _fmt.c_str(), i);
}

void		Getopt::
_init_fmt() {
  unsigned int	i = 0;
  std::map<char, args_data>::const_iterator	it;
  const unsigned int	len = _fmt.length();
  char		treat;
  std::string	no_handle_char;

  this->_sg_args.clear();
  while (i < len) {
    treat = _fmt[i];

    // Errors
    if (!ISOPT(treat)) {
      _thrower(treat, "Expected opt. Invalid token", "_fmt", _fmt.c_str(), i);
    }
    if ((it = this->_sg_args.find(treat)) != this->_sg_args.end()) {
      _thrower(treat, "Multiple definition of token", "_fmt", _fmt.c_str(), i);
    }
    if (no_handle_char.find(treat) != std::string::npos) {
      _thrower(treat, "Multiple definition of mini-token", "_fmt", _fmt.c_str(), i);
    } //

    if (HAS_ARGS(_fmt[++i])) {
      this->_sg_args[treat].args = NULL;
      if (_fmt[i] == MULT_HANDLE_CHAR) {
	this->_sg_args[treat].nb = NB_MULT_HANDLE_CHAR;
	++i;
      }
      else
	_parse_hasarg(_fmt, i, treat);
    }
    else
      no_handle_char += treat;
  }
}

void		Getopt::
_init_l_opt() {
  //this->_l_args.clear();
  std::cout << __FUNCTION__ << std::endl;
}

std::string	*Getopt::
_extract_optname(const char *treat) const {
  size_t	pos;
  std::string	*ext = new std::string(treat);

  if ((pos = ext->find_first_not_of(CHAR_STRING)) != std::string::npos)
    ext->erase(pos);
  return (ext);
}

void		Getopt::
_init_mc_opt() {
  std::cout << __FUNCTION__ << std::endl;
  unsigned int	i = 0;
  std::string	*extract;

  this->_mc_args.clear();
  this->_mc_args_push_order.clear();
  while (_mc_opt[i]) {
    std::cout << "_mc_opt[" << i << "] == {" << _mc_opt[i] << "}" << std::endl;
    extract = this->_extract_optname(_mc_opt[i]);

    std::cout << extract << std::endl;
    ++i;
  }
}

void		Getopt::
_reinit_vars() {
  this->_ind = 1;
  this->_opt = 0;
  this->_swp = 0;
  this->_low = 0;
  this->_up = 0;

  // Clearing existing datas
  this->_ign.clear();
  this->_rem.clear();

  // Initialisation of args maps
  this->_init_fmt();
  this->_init_l_opt();
  this->_init_mc_opt();
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

  //std::cout << "STD: " << cmp.erase(0, 1) << std::endl;
  if (_mc_opt == NULL)
    return (false);
  while (_mc_opt[i]) {
    args_ndx = 0;
    if (cmp == _mc_opt[i] + args_ndx) {
      std::cout << _mc_opt[i] << ": Catched" << std::endl;
      return (true);
    }
    ++args_ndx;
    ++i;
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
    if (_no_more_args())
      return (0);
#if 1
    if (this->_get_mc_option()) // Multi-characters options
      return (0);
#endif
#if 0
    if (this->_get_l_option()) // Long options
      return (0);
#endif
  }
  if (!(ret = _argv[_ind][++_opt])) {
    ++_ind;
    _opt = 0;
    return (this->_gn_opt());
  }

  if ((pos = _fmt.find_first_of(ret)) == std::string::npos) {
    if (_ign.find_first_of(ret) == std::string::npos)
      _ign += ret;
    return (0);
  }
  _setopt(ret);
  return (_resolve_arg(ret));
}

void		Getopt::
_setopt(char c) {
  if (ISLOWER(c)) {
    if (!MAPOPT(_low, c))
      _low += MAPOPT(0xFFFFFFFF, c);
  }
  else if (ISUPPER(c)) {
    if (!MAPOPT(_up, c))
      _up += MAPOPT(0xFFFFFFFF, c);
  }
}

int		Getopt::
_nb_args(char c) const {
  std::map<char, args_data>::const_iterator	it;

  if ((it = this->_sg_args.find(c)) == this->_sg_args.end())
    return (0);
  return (it->second.nb);
}

void		Getopt::
_setarg(char c, int nb) {
  if (!(this->_sg_args[c].args))
    this->_sg_args[c].args = new std::list<char *>;
  if (_optarg && nb == 1)
    _sg_args[c].args->push_back(_optarg);
  else if ((nb == NB_MULT_HANDLE_CHAR || nb > 1) && _optarg) {
    if ((nb == NB_MULT_HANDLE_CHAR) && (_optarg[0] == '-')) {
      return ;
    }
    _sg_args[c].args->push_back(_optarg);
    while (_still_args() && _argv[_ind] &&
	((_argv[_ind][0] != '-' && nb == NB_MULT_HANDLE_CHAR) || (--nb > 0))) {
      _sg_args[c].args->push_back(_argv[_ind]);
      ++_ind;
    }
  }
}

bool		Getopt::
_resolve_arg(char c) {
  int           nb_args;

  nb_args = _nb_args(c);
  if ((nb_args >= 1) || nb_args == NB_MULT_HANDLE_CHAR) {
    if (!_argv[_ind][_opt + 1]) {
      ++_ind;
      if (_no_more_args())
	return (false);
      _optarg = _argv[_ind];
    }
    else
      _optarg = _argv[_ind] + _opt + 1;
    ++_ind;
    _opt = 0;
    this->_setarg(c, nb_args);
    return (true);
  }
  return (false);
}

void		Getopt::
_build_opts() {
  // If someone knows how to init static char * into in-class static data member initialization
  this->CHAR_STRING =
    "abcdefghijklmnopqrstuvwxyz"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "0123456789"
    "_+-";

  this->_reinit_vars();
  while (_still_args())
    _gn_opt();
#ifdef	REMAIN_IN_LIST
  { // Only if you need to get a std::list of remain args
    int	remain = _ind;
    while (remain < (_argc)) {
      _rem.push_back(_argv[_ind]);
      ++remain;
    }
  }
#endif
}

Getopt::
Getopt(int argc, char **argv, std::string &fmt, const char **l_opt, const char **mc_opt)
  : _argc(argc), _argv(argv), _fmt(fmt), _l_opt(l_opt), _mc_opt(mc_opt) {
    this->_build_opts();
  }

Getopt::
Getopt(int argc, char **argv, const char *fmt, const char **l_opt, const char **mc_opt)
  : _argc(argc), _argv(argv), _fmt(std::string(fmt)), _l_opt(l_opt), _mc_opt(mc_opt) {
    this->_build_opts();
  }

Getopt::
~Getopt() {
  std::map<char, args_data>::iterator	sg_it;
  for (sg_it = _sg_args.begin(); sg_it != _sg_args.end(); ++sg_it) {
    if (sg_it->second.args)
      delete sg_it->second.args;
  }
  // Later :)
  //std::map<char *, args_data>::iterator	mc_it;
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
  /* Just for correct indenting
#ifdef REMAIN_IN_LIST
if ((args = getArgs(0))->size()) {
std::cout << "Remain\t:";
int i = 0;
for (it = args->begin(); it != args->end(); ++it,++i)
std::cout << (i ? "\t" : "") << "\t["
<< i << "]: "
<< *it << std::endl;
#else
*/
  char **rem;
  if ((rem = getRemain()) && rem[0]) {
    std::cout << "Remain\t:";
    int	i = 0;
    while (i < _argc - _ind) {
      std::cout << (i ? "\t" : "") << "\t["
	<< i << "]: "
	<< rem[i] << std::endl;
      ++i;
    }
    //#endif
  }
}

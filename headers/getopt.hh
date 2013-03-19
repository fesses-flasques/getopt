#ifndef		_GETOPT_HH
# define	_GETOPT_HH

# include	<iostream>
# include	<map>
# include	<list>
# include	<exception>
# include	<sstream>

# undef		REMAIN_IN_LIST

template	<typename C>
static C	charstrait_extract(const char *t) {
  C		val;
  std::istringstream	iss(std::string(t), std::istringstream::in);
  iss >> val;
  return (val);
}

class			Getopt
{
  static const char	OPT_CHAR = '-';
  static const char	SINGLE_HANDLE_CHAR = ':';
  static const char	MULT_HANDLE_CHAR = '*';

  static const int      NB_ERR = -2;
  static const int      NB_MULT_HANDLE_CHAR = -1;
  static const int      NB_NO_HANDLE_CHAR = 0;

  const char		*CHAR_STRING;

  // Exceptions
  class			syntaxError : public std::exception
  {
    std::string		_str;

    public:
    syntaxError(std::string &str) : _str("Syntax Error: " + str) {}
    virtual ~syntaxError() throw() {}
    virtual const char	*what() const throw() {
      return (this->_str.c_str());
    }
  };

  void			_thrower(
      char,
      const char *,
      const char *,
      const char *,
      unsigned
      ) const throw();
  // manipulating functions
  inline bool		ISLOWER(char) const;
  inline bool		ISUPPER(char) const;
  inline bool		ISDIGIT(char) const;
  inline bool		ISOPT(char) const;
  inline int		SRCCASE(char) const;
  inline int		TABOPT(char) const;
  inline int		MAPOPT(int, char) const;
  inline bool		HAS_ARGS(char) const;

  inline bool		_still_args() const;
  inline bool		_no_more_args() const;

  // Manip Datas
  int			_argc;
  char			**_argv;
  std::string		_fmt;
  const char		**_l_opt;
  const char		**_mc_opt;

  // Data indexes
  int			_ind;
  unsigned int		_opt;
  int			_swp;
  char			*_optarg;

  // Results
  std::string		_ign;
  int			_low;
  int			_up;
  struct		args_data
  {
    int			nb;
    std::list<char *>	*args;
  };
  struct		args_mc_data
  {
    int			nb;
    unsigned int	ndx;
  };
  std::map<char, args_data>		_sg_args;
  std::map<std::string *, args_mc_data>	_mc_args; //Conf only
  std::list<
    std::pair<unsigned int, char *>
    >					_mc_args_push_order;
  std::list<char *>			_rem;

  // Parse functions

  // Utilities for mc and l options.
  std::string		*_extract_optname(const char *) const;
  // -> Differents token taker
  int			_bracket_token(const char *, unsigned int &);

  // -> Mechanisms for tokens
  int			_ptoken_caller(
      int		(Getopt::*)(const char *, unsigned int &),
      const char	*,
      unsigned int	&
      );
  int			_parse_hasarg(const char *, unsigned int &);

  // -> different options types caller
  void			_init_fmt();
  void			_init_l_opt();
  void			_init_mc_opt();

  // Operations functions
  Getopt();
  void			_build_opts();
  void			_reinit_vars();
  void			_getswap();
  char			_gn_opt();
  bool			_resolve_arg(char);
  void			_setarg(char, int nb = 1);
  int			_nb_args(char) const;
  void			_setopt(char);
  bool			_get_l_option();
  bool			_get_mc_option();

  public:
  Getopt(
      int,
      char **,
      std::string &,
      const char **l_opt = NULL,
      const char **mc_opt = NULL
      );
  Getopt(
      int,
      char **,
      const char *,
      const char **l_opt = NULL,
      const char **mc_opt = NULL
      );
  Getopt(const Getopt &);
  ~Getopt();
  Getopt			&operator=(const Getopt &);
  void				dump() const;

  // Used Accessors
  bool				isSet(char) const ;
  char				*getLastArg(char) const;
  const std::list<char *>	*getArgs(char) const;
  char				**getRemain() const;
};

#endif

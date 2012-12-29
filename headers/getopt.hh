#ifndef		_GETOPT_HH
# define	_GETOPT_HH

# include	<iostream>
# include	<map>
# include	<list>

static const char	OPT_CHAR = '-';
static const char	SINGLE_HANDLE_CHAR = ':';
static const char	MULT_HANDLE_CHAR = '*';

class			Getopt
{
  // manipulating functions
  inline bool		ISLOWER(char) const;
  inline bool		ISUPPER(char) const;
  inline int		SRCCASE(char) const;
  inline int		TABOPT(char) const;
  inline int		MAPOPT(int, char) const;
  inline bool		HAS_ARGS(char) const;

  // Manip Datas
  int			_argc;
  char			**_argv;
  std::string		_fmt;
  char			**_l_opt;

  // Data indexes
  int			_ind;
  unsigned int		_opt;
  unsigned int		_swp;
  char			*_optarg;
  char			_hasarg;

  // Results
  std::string		_ign;
  int			_low;
  int			_up;
  std::map<char, std::list<char *> >	_args;

  // Operations functions
  Getopt();
  void			_build_opts();
  void			_reinit_vars();
  void			_getswap();
  char			_gn_opt();
  void			_setarg(char);
  void			_setopt(char);

  public:
  Getopt(int, char **, std::string &, char **l_opt = NULL);
  Getopt(int, char **, const char *, char **l_opt = NULL);
  Getopt(const Getopt &);
  ~Getopt();
  Getopt			&operator=(const Getopt &);
  void				dump() const;

  // Used Accessors
  bool				isSet(char) const ;
  char				*getLastArg(char) const;
  const std::list<char *>	*getArgs(char) const;
};

#endif

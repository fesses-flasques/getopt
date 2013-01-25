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

  inline bool		_still_args() const;
  // Manip Datas
  int			_argc;
  char			**_argv;
  std::string		_fmt;
  char			**_l_opt;
  char			**_mc_opt;

  // Data indexes
  int			_ind;
  unsigned int		_opt;
  unsigned int		_swp;
  char			*_optarg;

  // Results
  std::string		_ign;
  int			_low;
  int			_up;
  std::map<char, std::list<char *> >	_args;
  std::list<char *>	_rem;

  // Operations functions
  Getopt();
  void			_build_opts();
  void			_reinit_vars();
  void			_getswap();
  char			_gn_opt();
  bool			_resolve_arg(size_t, char);
  void			_setarg(char, char, int nb = 1);
  int			_nb_args(size_t pos) const;
  void			_setopt(char);
  bool			_get_l_option();
  bool			_get_mc_option();

  public:
  Getopt(int, char **, std::string &, char **l_opt = NULL, char **mc_opt = NULL);
  Getopt(int, char **, const char *, char **l_opt = NULL, char **mc_opt = NULL);
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

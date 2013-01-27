#ifndef		_GETOPT_HH
# define	_GETOPT_HH

# include	<iostream>
# include	<map>
# include	<list>
# include	<exception>

static const char	OPT_CHAR = '-';
static const char	SINGLE_HANDLE_CHAR = ':';
static const char	MULT_HANDLE_CHAR = '*';

class			Getopt
{
  // Exceptions
  class			syntaxError : public std::exception {
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
      unsigned)
    const throw();
  // manipulating functions
  inline bool		ISLOWER(char) const;
  inline bool		ISUPPER(char) const;
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
  struct		data {
    int			nb;
    std::list<char *>	*args;//DELETE
  };
  std::map<char, data>	_args;
  std::list<char *>	_rem;

  // Parse functions
  bool			_ptoken_caller(unsigned (Getopt::*)(unsigned int),
      unsigned int &);
  unsigned		_bracket_token(unsigned int);
  void			_parse_hasarg(char, unsigned int &);
  void			_init_fmt();
  void			_init_l_opt();
  void			_init_mc_opt();

  // Operations functions
  Getopt();
  void			_build_opts();
  void			_reinit_vars();
  void			_getswap();
  char			_gn_opt();
  bool			_resolve_arg(size_t, char);
  void			_setarg(char, char, int nb = 1);
  int			_nb_args(char) const;
  void			_setopt(char);
  bool			_get_l_option();
  bool			_get_mc_option();

  public:
  Getopt(int, char **, std::string &,
      const char **l_opt = NULL, const char **mc_opt = NULL);
  Getopt(int, char **, const char *,
      const char **l_opt = NULL, const char **mc_opt = NULL);
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

##		PROJECT_NAME
ifeq ($(OSTYPE),cygwin)
NAME		=	libgetopt.dll
else
NAME		=	libgetopt.so
endif

##		COLORS			#######################################
BLUE		=	"\033[33;34m"
PURPLE		=	"\033[33;35m"
WHITE		=	"\033[33;39m"
##
COL_RM		=	$(PURPLE)
COL_ORG		=	$(WHITE)
COL_NAME	=	$(BLUE)
##
CCOL_ORG	=	\x1b[0m
CCOL_OK		=	\x1b[32;01m
CCOL_ERR	=	\x1b[31;01m
CCOL_WRN	=	\x1b[33;01m
##
OK_STRING	=	$(CCOL_OK)[OK]\t$(CCOL_ORG)
ERROR_STRING	=	$(CCOL_ERR)[ERR]\t$(CCOL_ORG)
WARN_STRING	=	$(CCOL_WRN)[WARN]$(CCOL_ORG) 

###############################################################################
##		COMMANDS		#######################################
###############################################################################
CXX		=	g++
RM		=	rm -rfv
ECHO		=	echo -en
SIL_RM		=	@$(RM)
SIL_ECHO	=	@$(ECHO)

###############################################################################
##		SOURCE MANAGER		#######################################
###############################################################################
SRC_DIR		=	src/
##	must contain all sub-dir listed above... I dunnow how to do it properly
SRC_SS		=
###############################################################################

INCLUDE_DIRS	=	'headers/'
INCLUDE_DIRS	+=	$(foreach s, $(SRC_SS), '$s/headers')

SRC		=	$(SRC_DIR)impGetopt.cpp

OBJ		=	$(SRC:.cpp=.o)

CXXFLAGS	+=	-Wall -Wextra
CXXFLAGS	+=	-g3
CXXFLAGS	+=	-ansi -pedantic
CXXFLAGS	+=	$(foreach i, $(INCLUDE_DIRS), -I$i)

LDFLAGS		=
## IF LIB
CXXFLAGS	+=	-fPIC
LDFLAGS		+=	-shared -Wl,-soname,$(NAME)

EXAMPLE_NAME	=	example
EXAMPLE_SRC	=	example_main.cpp

$(NAME):	$(OBJ)
	@$(CXX) -o $(NAME) $(OBJ) $(LDFLAGS) && $(ECHO) "$(OK_STRING)" || \
	  ( $(ECHO) "$(ERROR_STRING)" && test 1 -eq 2 )
	@echo -e - Created binary $(COL_NAME)$(NAME)$(COL_ORG)

%.o:		%.cpp
	@$(CXX) -c $< $(CXXFLAGS) -o $@ && $(ECHO) "$(OK_STRING)" || \
	  ( $(ECHO) "$(ERROR_STRING)" && test 1 -eq 2 )
	@echo -e - Created object from $(COL_NAME)'$<'$(COL_ORG)

all:		$(NAME)

clean:
	$(SIL_ECHO) $(COL_RM)
	$(SIL_RM) $(OBJ)
	$(SIL_RM) $(EXAMPLE_NAME)
	$(SIL_ECHO) $(COL_ORG)

fclean:		clean
	$(SIL_ECHO) $(COL_RM)
	$(RM) $(NAME)
	$(SIL_ECHO) $(COL_ORG)

re:		fclean all

$(EXAMPLE_NAME): re
	$(CXX) $(EXAMPLE_SRC) -Iheaders/ -L. -lgetopt -o $(EXAMPLE_NAME)
.PHONY:		all clean fclean re

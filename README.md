getopt
======

Options getter for c++ programs

How to use:

	Getopt	obj(argc, argv, "abc:def*g");
							^
							fmt string.
	
The fmt string tells Getopt which options are setable and which options take one or several argument. As the example above, the fmt string tells that the program can receive "-a" , "-b" , and "-c something".

':' stands for one argument.
'\*' stands for several arguments.

Note that if an argument is expected for an option but none is filled in, the option is still counted as set; but the string returned by the arguments accessors will just be 'NULL'.

The '\*' token will catch up every argument after the option until the next option. To stop an array, you can simple enter arguments like this:

	(shell)$> ./prog "-f" "arg\_for\_f" "another" "--" "arg\_for\_program"

For any questions, ask. Simply.

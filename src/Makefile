GCC:=g++-4.8 -std=c++11
FLAGS_LIB:=-fPIC -shared -I.
all:libcssh.so main
main: test.cpp
	$(GCC) $(FLAG) -Wl,-rpath,. -I. -L. $< -o $@ -lcssh -lssh2
libcssh.so:Cssh.cpp
	$(GCC) $(FLAG) $(FLAGS_LIB) $< -o $@ 


.PHONY:clean
clean:
	-rm libcssh.so main

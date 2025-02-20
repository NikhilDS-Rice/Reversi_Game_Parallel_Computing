NOWARN=-wd3947
OBJ =  \
	cilk-reducers-demo \
	reducer-list-append-demo \
	reducer-list-prepend-demo \
	reducer-max-demo \
	reducer-max-index-demo \
	reducer-min-demo \
	reducer-min-index-demo \
	reducer-opadd-demo \
	reducer-opand-demo \
	reducer-opor-demo \
	reducer-opxor-demo \
	reducer-ostream-demo \
	reducer-string-demo \
	reducer-wstring-demo

OPT=-O2

all: $(OBJ) 

run:
	for f in  $(OBJ) ; do  ./$$f; done

%: %.cpp
	icpc $(OPT) $(NOWARN) -o $@ $< 

clean:
	/bin/rm -f $(OBJ) 

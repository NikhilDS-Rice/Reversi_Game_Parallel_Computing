NOWARN=-wd3950 # suppress warnings about deprecated array notation

OBJ =  \
	add-arrays \
	call-scalar-function \
	check-for-5 \
	copy-and-add \
	copy-array \
	letter-type-if \
	set-2d-array \
	set-2d-row \
	set-2d-stride \
	set-array \
	set-first-n \
	set-sequence \
	set-stride

OPT=-O2

all: $(OBJ) 

run:
	for f in  $(OBJ) ; do  ./$$f; done

%: %.cpp
	icpc $(OPT) $(NOWARN) -o $@ $< 

clean:
	/bin/rm -f $(OBJ) 

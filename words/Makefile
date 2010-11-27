
FILES=100words.h 200words.h 300words.h 400words.h 500words.h

all: $(FILES)

%.h: %.txt convert.pl
	perl convert.pl $< > $@

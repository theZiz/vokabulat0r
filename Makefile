all: vokabulat0r

vokabulat0r: vokabulat0r.c
	gcc -std=c99 $(CFLAGS) $(LDFLAGS) $(CPPFLAGS) vokabulat0r.c -o vokabulat0r

debug: vokabulat0r.c
	gcc -std=c99 -g $(CFLAGS) $(LDFLAGS) $(CPPFLAGS) vokabulat0r.c -o vokabulat0r

clean:
	rm vokabulat0r

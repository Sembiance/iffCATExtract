CFLAGS	= -D_DEFAULT_SOURCE -std=c11 -Wall -Wextra -flto -O3 -s
LDFLAGS	= -flto
LIBS	= -liff

iffCATExtract: iffCATExtract.c
	gcc $(CFLAGS) $(LDFLAGS) -o $@ $< $(LIBS)

clean:
	rm -f ilbm2frames

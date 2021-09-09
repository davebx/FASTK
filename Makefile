DEST_DIR = $(PREFIX)/bin
INSTALL = install -p

PROGRAMS = FastK \
		   Fastrm \
		   Fastmv \
		   Fastcp \
		   Fastmerge \
		   Histex \
		   Tabex \
		   Profex \
		   Logex \
		   Vennex \
		   Symmex \
		   Haplex \
		   Homex

all: deflate.lib libhts.a $(PROGRAMS)

include HTSLIB/htslib_static.mk

deflate.lib: LIBDEFLATE
	cd LIBDEFLATE; make; cd ..

libhts.a: HTSLIB
	cd HTSLIB; make lib-static; cd ..

HTSLIB/htslib_static.mk:
	cd HTSLIB; make htslib_static.mk; cd ..

libfastk.c : gene_core.c
libfastk.h : gene_core.h

FastK: FastK.c FastK.h io.c split.c count.c table.c merge.c io.c gene_core.c gene_core.h MSDsort.c LSDsort.c libfastk.c libfastk.h
	$(CC) $(CFLAGS) -o FastK -I./HTSLIB $(HTSLIB_static_LDFLAGS) FastK.c io.c split.c count.c table.c merge.c MSDsort.c LSDsort.c libfastk.c LIBDEFLATE/libdeflate.a HTSLIB/libhts.a -lpthread -lrt $(HTSLIB_static_LIBS)

Fastrm: Fastrm.c gene_core.c gene_core.h
	$(CC) $(CFLAGS) -o Fastrm Fastrm.c gene_core.c -lpthread -lm -lrt

Fastmv: Fastxfer.c gene_core.c gene_core.h
	$(CC) $(CFLAGS) -DMOVE -o Fastmv Fastxfer.c gene_core.c -lpthread -lm -lrt

Fastcp: Fastxfer.c gene_core.c gene_core.h
	$(CC) $(CFLAGS) -UMOVE -o Fastcp Fastxfer.c gene_core.c -lpthread -lm -lrt

Fastmerge: Fastmerge.c libfastk.c libfastk.h
	$(CC) $(CFLAGS) -o Fastmerge Fastmerge.c libfastk.c -lpthread -lm -lrt

Histex: Histex.c libfastk.c libfastk.h
	$(CC) $(CFLAGS) -o Histex Histex.c libfastk.c -lpthread -lm -lrt

Tabex: Tabex.c libfastk.c libfastk.h
	$(CC) $(CFLAGS) -o Tabex Tabex.c libfastk.c -lpthread -lm -lrt

Profex: Profex.c libfastk.c libfastk.h
	$(CC) $(CFLAGS) -o Profex Profex.c libfastk.c -lpthread -lm -lrt

Logex: Logex.c libfastk.c libfastk.h
	$(CC) $(CFLAGS) -o Logex Logex.c libfastk.c -lpthread -lm -lrt

Vennex: Vennex.c libfastk.c libfastk.h
	$(CC) $(CFLAGS) -o Vennex Vennex.c libfastk.c -lpthread -lm -lrt

Symmex: Symmex.c libfastk.c libfastk.h LSDsort.c
	$(CC) $(CFLAGS) -o Symmex Symmex.c libfastk.c LSDsort.c -lpthread -lm -lrt

Haplex: Haplex.c libfastk.c libfastk.h
	$(CC) $(CFLAGS) -o Haplex Haplex.c libfastk.c -lpthread -lm -lrt

Homex: Homex.c libfastk.c libfastk.h
	$(CC) $(CFLAGS) -o Homex Homex.c libfastk.c -lpthread -lm -lrt


tidyup:
	rm -f $(ALL)
	rm -fr *.dSYM
	rm -f FastK.tar.gz

clean:
	cd LIBDEFLATE; make clean; cd ..
	cd HTSLIB; make clean; cd ..
	rm -f $(ALL)
	rm -fr *.dSYM
	rm -f FastK.tar.gz

install:
	$(INSTALL) $(PROGRAMS) $(DEST_DIR)

package:
	make clean
	tar -zcf FastK.tar.gz LICENSE README.md Makefile *.h *.c

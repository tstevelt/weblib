PROG = weblib
LIB = /usr/local/lib64/weblib.a
DEF = /usr/local/include

FILES = \
	$(LIB)(webContentFuncs.o)	\
	$(LIB)(webCookie.o)		\
	$(LIB)(webEntities.o)	\
	$(LIB)(webFixHex.o)		\
	$(LIB)(webFreeParms.o)	\
	$(LIB)(webGetBrowser.o)	\
	$(LIB)(webGetCommandLine.o)	\
	$(LIB)(webGetEnvironment.o)	\
	$(LIB)(webGetInput.o)	\
	$(LIB)(webGetOneEnv.o)	\
	$(LIB)(webGetQuery.o)	\
	$(LIB)(webGetStdin.o)	\
	$(LIB)(webGetStdin.o)	\
	$(LIB)(webInitParms.o)	\
	$(LIB)(webTrim.o)

.SILENT:

$(LIB): $(FILES)
	make include

$(FILES): webprivate.h weblib.h webCookie.h

include: $(DEF)/weblib.h $(DEF)/webCookie.h

$(DEF)/weblib.h: weblib.h 
	cp -pv weblib.h $(DEF)/weblib.h

$(DEF)/webCookie.h: webCookie.h 
	cp -pv webCookie.h $(DEF)/webCookie.h

strip:
	ls -l $(LIB)
	strip --strip-unneeded $(LIB)
	ls -l $(LIB)

clean:
	rm -f $(LIB)
	rm -f $(DEF)/weblib.h
	rm -f $(DEF)/webCookie.h

all:
	make clean
	make
	make include

.PRECIOUS: $(LIB)


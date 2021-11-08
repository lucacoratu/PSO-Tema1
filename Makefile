CC=cl
CFLAGS=/nologo /Wall -D _CRT_SECURE_NO_WARNINGS /l
L = link

build: so-cpp.exe

so-cpp.exe: HashMap.obj ParameterParser.obj FileParser.obj main.obj
	$(L) /nologo /out:$@ $?

main.obj: main.c
#	$(CC) $(CFLAGS) /c $@ $?

HashMap.obj: HashMap.c HashMap.h Core.h
#	$(CC) $(CFLAGS) /c HashMap.c HashMap.h Core.h

ParameterParser.obj: ParameterParser.c ParameterParser.h
#	$(CC) $(CFLAGS) /c $@ ParameterParser.c

FileParser.obj: FileParser.c FileParser.h HashMap.h ParameterParser.h
#	$(CC) $(CFLAGS) /c $@ FileParser.c

.Phony: clean

clean:
	del *.obj so-cpp.exe
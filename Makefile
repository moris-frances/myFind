CC = gcc
CFLAGS = -D_DEFAULT_SOURCE -Wall -Wextra -pedantic -Wno-unused-parameter
CFILES = $(wildcard *.c)
HFILES = $(wildcard *.h)
EXENAME = myFind
FILENAME1 = main
FILENAME2 = func

${EXENAME}: ${CFILES} ${HFILES}
	${CC} ${CFLAGS} ${CFILES} ${HFILES} -o ${EXENAME}
clean: ${EXENAME}
	rm -f ${EXENAME}.o ${EXENAME}
	echo Clean done

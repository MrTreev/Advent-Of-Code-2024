.EXPORT_ALL_VARIABLES:
.DELETE_ON_ERROR:

PROJECT		=	.
PATH_SRC	=	${PROJECT}/src
PATH_TXT	=	${PROJECT}/txt
PATH_OUT	=	${PROJECT}/out
PATH_BIN	=	${PATH_OUT}/bin
PATH_LIB	=	${PATH_OUT}/lib
PATH_DAT	=	${PATH_OUT}/test

CXX			=	clang++

CXXFLAGS	+=	-std=c++26
CXXFLAGS	+=	-stdlib=libc++
CXXFLAGS	+=	-fexperimental-library
CXXFLAGS	+=	-iquote${PATH_SRC}

CXXFLAGS	+=	-Weverything
CXXFLAGS	+=	-pedantic
CXXFLAGS	+=	-Wno-padded
CXXFLAGS	+=	-Wno-c99-extensions
CXXFLAGS	+=	-Wno-c99-compat
CXXFLAGS	+=	-Wno-c++-compat
CXXFLAGS	+=	-Wno-c++20-compat
CXXFLAGS	+=	-Wno-c++98-compat
CXXFLAGS	+=	-Wno-c++98-compat-pedantic
CXXFLAGS	+=	-Wno-pre-c++20-compat-pedantic
CXXFLAGS	+=	-Wno-pre-c++17-compat-pedantic
CXXFLAGS	+=	-Wno-switch-default

LDFLAGS		+=	-L${PATH_LIB}
LDFLAGS		+=	-lc
LDFLAGS		+=	-lc++
LDFLAGS		+=	-ltbb

TSTFLAGS	+=	-g
TSTFLAGS	+=	-ggdb
TSTFLAGS    +=  -UNDEBUG

OPTFLAGS    +=  -O3
OPTFLAGS    +=  -DNDEBUG

DAYS	=	01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25
.PHONY: all
all: $(foreach day,${DAYS},${PATH_BIN}/day${day}) $(foreach day,${DAYS},${PATH_BIN}/test${day})

${PATH_LIB}/lib%.so: ${PATH_SRC}/answers.cpp
	@mkdir -p $(dir $@)
	${CXX} ${CXXFLAGS} -o $@ -shared $^

${PATH_BIN}/day%: ${PATH_SRC}/day%.cpp ${PATH_SRC}/util.cpp | ${PATH_LIB}/libanswers.so
	@mkdir -p $(dir $@)
	${CXX} ${CXXFLAGS} ${LDFLAGS} ${OPTFLAGS} -lanswers -o $@ $^

${PATH_BIN}/test%: ${PATH_SRC}/day%.cpp ${PATH_SRC}/util.cpp | ${PATH_LIB}/libanswers.so
	@mkdir -p $(dir $@)
	${CXX} ${CXXFLAGS} ${LDFLAGS} ${TSTFLAGS} -lanswers -o $@ $^

${PATH_DAT}/%: ${PATH_TXT}/%
	@mkdir -p $(dir $@)
	@cp $< $@

.PHONY: clean
clean:
	@rm -rf ${PATH_OUT}

.PHONY: $(BIN_FILES:${PATH_BIN}/%=run%)
run%: ${PATH_BIN}/day% ${PATH_DAT}/day%.txt
	LD_LIBRARY_PATH=${PATH_LIB} $<

.PHONY: $(BIN_FILES:${PATH_BIN}/%=test%)
test%: ${PATH_BIN}/test% ${PATH_DAT}/test%.txt
	LD_LIBRARY_PATH=${PATH_LIB} $<

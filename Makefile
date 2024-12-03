include Makefile.defs

SRC_FILES	=	$(shell find ${PATH_SRC} -type f -name '*.cpp')
BIN_FILES	=	$(patsubst ${PATH_SRC}%.cpp,${PATH_BIN}%,${SRC_FILES})
TST_FILES	=	$(patsubst ${PATH_SRC}%.cpp,${PATH_BIN}%,${SRC_FILES})

${BIN_FILES}: ${SRC_FILES}
	${CPP} ${CXX_FLAGS} $< -o $@

.PHONY: all
all: ${BIN_FILES} echo

.PHONY: echo
echo:
	@echo SRC_FILES: "${SRC_FILES}"
	@echo BIN_FILES: "${BIN_FILES}"
	@echo TST_FILES: "${TST_FILES}"


# .PHONY: test
# test: ${TST_FILES}

.PHONY: clean
clean:
	@rm -rf ${OUT_PATH}

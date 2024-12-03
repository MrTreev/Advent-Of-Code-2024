.EXPORT_ALL_VARIABLES:
include Makefile.defs

.PHONY: all
all: test ${BIN_FILES}

${BLD_FILES}: ${PATH_BLD}/%.o: ${PATH_SRC}/%.cpp
	@mkdir -p $(dir $@)
	@${CXX} ${CXXFLAGS} -o $@ -c $<
	@chmod +x $@

${PATH_BIN}/day%: ${PATH_BLD}/day%.o ${PATH_BLD}/util.o
	@mkdir -p $(dir $@)
	${CXX} -o $@ $^

.PHONY: test
test: ${TST_FILES}
${TST_FILES}: ${PATH_TST}/%: ${PATH_TXT}/%
	@mkdir -p $(dir $@)
	@cp $< $@

.PHONY: clean
clean:
	@rm -rf ${PATH_OUT}

.PHONY: echo
echo:
	echo BIN_FILES: "${BIN_FILES}"
	echo BLD_FILES: "${BLD_FILES}"

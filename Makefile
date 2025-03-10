.EXPORT_ALL_VARIABLES:
include Makefile.defs

.PHONY: all
all: text ${BIN_FILES}

${BLD_FILES}: ${PATH_BLD}/%.o: ${PATH_SRC}/%.cpp
	@mkdir -p $(dir $@)
	${CXX} ${CXXFLAGS} -o $@ -c $<

${PATH_BIN}/day%: ${PATH_BLD}/day%.o ${PATH_BLD}/util.o ${PATH_BLD}/answers.o
	@mkdir -p $(dir $@)
	${LD} ${CXXFLAGS} ${LDFLAGS} -o $@ $^

.PHONY: test
text: ${DAT_FILES}
${DAT_FILES}: ${PATH_DAT}/%: ${PATH_TXT}/%
	@mkdir -p $(dir $@)
	@cp $< $@

.PHONY: clean
clean:
	@rm -rf ${PATH_OUT}

.PHONY: $(BIN_FILES:${PATH_BIN}/%=run%)
run%: ${PATH_BIN}/day% ${PATH_DAT}/day%.txt
	$<

.PHONY: $(BIN_FILES:${PATH_BIN}/%=test%)
test%: ${PATH_BIN}/day% ${PATH_DAT}/day%.txt
	$< test

.PHONY: echo
echo:
	echo BIN_FILES: "${BIN_FILES}"
	echo BLD_FILES: "${BLD_FILES}"

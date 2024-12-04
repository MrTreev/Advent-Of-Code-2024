.EXPORT_ALL_VARIABLES:
include Makefile.defs

CXXFLAGS += -g -ggdb

.PHONY: all
all: text ${BIN_FILES}

${BLD_FILES}: ${PATH_BLD}/%.o: ${PATH_SRC}/%.cpp
	@mkdir -p $(dir $@)
	@${CXX} ${CXXFLAGS} -o $@ -c $<

${PATH_BIN}/day%: ${PATH_BLD}/day%.o ${PATH_BLD}/util.o
	@mkdir -p $(dir $@)
	@${CXX} -o $@ $^

.PHONY: test
text: ${TST_FILES}
${TST_FILES}: ${PATH_TST}/%: ${PATH_TXT}/%
	@mkdir -p $(dir $@)
	@cp $< $@

.PHONY: clean
clean:
	@rm -rf ${PATH_OUT}

.PHONY: $(BIN_FILES:${PATH_BIN}/%=run%)
run%: ${PATH_BIN}/day% ${PATH_TST}/day%.txt
	$<

.PHONY: $(BIN_FILES:${PATH_BIN}/%=test%)
test%: ${PATH_BIN}/day% ${PATH_TST}/day%.txt
	$< test

.PHONY: echo
echo:
	echo BIN_FILES: "${BIN_FILES}"
	echo BLD_FILES: "${BLD_FILES}"

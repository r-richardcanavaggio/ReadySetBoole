################################################################################
#                                   VARIABLES                                  #
################################################################################

CXX			:= c++
CXXFLAGS	:= -Werror -Wall -Wextra -std=c++23 -MMD -MP

BIN_DIR		:= bin
OBJ_DIR		:= .obj

EX_DIRS		:=	ex00 \
				ex01 \
				ex02 \
				ex03 \
				ex04 \
				ex05 \
				ex06 \
				ex07 \
				ex08 \
				ex09 \
				ex10 \
				ex11 \
				ex12 \
				ex13 \
				ex14 \

################################################################################
#                                 SOURCE FILES                                 #
################################################################################

FILES_ex05	:= AST.cpp ASTHelper.cpp ASTPrint.cpp ex05.cpp RPN.cpp
FILES_ex06	:= AST.cpp ASTHelper.cpp ASTPrint.cpp CNF.cpp ex06.cpp RPN.cpp
FILES_ex09	:= ex09.cpp set_operations.cpp

################################################################################
#                                    COlORS                                    #
################################################################################

COM_COLOR   = \033[0;34m
OBJ_COLOR   = \033[0;36m
OK_COLOR    = \033[0;32m
ERROR_COLOR = \033[0;31m
WARN_COLOR  = \033[0;33m
NO_COLOR    = \033[m

define run_and_test
printf "%b" "$(COM_COLOR)$(COM_STRING) $(OBJ_COLOR)$(@F)$(NO_COLOR)\r"; \
	$(1) 2> $@.log; \
	RESULT=$$?; \
	if [ $$RESULT -ne 0 ] || [ -s $@.log ]; then \
		printf "%-60b%b" "$(COM_COLOR)$(COM_STRING)$(OBJ_COLOR) $@" "$(ERROR_COLOR)$(ERROR_STRING)$(NO_COLOR)\n"   ; \
	elif [ -s $@.log ]; then \
		printf "%-60b%b" "$(COM_COLOR)$(COM_STRING)$(OBJ_COLOR) $@" "$(WARN_COLOR)$(WARN_STRING)$(NO_COLOR)\n"   ; \
	else  \
		printf "%-60b%b" "$(COM_COLOR)$(COM_STRING)$(OBJ_COLOR) $(@F)" "$(OK_COLOR)$(OK_STRING)$(NO_COLOR)\n"   ; \
	fi; \
	cat $@.log; \
	rm -f $@.log 2> /dev/null; \
	exit $$RESULT
endef

OK_STRING    = "[OK]"
ERROR_STRING = "[ERROR]"
WARN_STRING  = "[WARNING]"
COM_STRING   = "Compiling"

################################################################################
#                                   COMMANDS                                   #
################################################################################

all: $(EX_DIRS)

define GEN_RULES
RAW_FILES_$(1) := $$(if $$(FILES_$(1)),$$(FILES_$(1)),$(1).cpp)
SRCS_$(1) := $$(addprefix $(1)/,$$(RAW_FILES_$(1)))
OBJS_$(1) := $$(addprefix $(OBJ_DIR)/$(1)/,$$(RAW_FILES_$(1):.cpp=.o))
DEPS_$(1) := $$(OBJS_$(1):.o=.d)

-include $$(DEPS_$(1))

$(1): $(BIN_DIR)/$(1)

$(BIN_DIR)/$(1): COM_STRING = Linking
$(BIN_DIR)/$(1): $$(OBJS_$(1)) | $(BIN_DIR)
	@$$(call run_and_test,$$(CXX) $$(CXXFLAGS) -o $$@ $$^)

$(OBJ_DIR)/$(1)/%.o: COM_STRING = Compiling
$(OBJ_DIR)/$(1)/%.o: $(1)/%.cpp | $(OBJ_DIR)/$(1)
	@$$(call run_and_test,$$(CXX) $$(CXXFLAGS) -c $$< -o $$@)

$(OBJ_DIR)/$(1):
	@mkdir -p $$@

run-$(1): $(1)
	@./$(BIN_DIR)/$(1)

clean-$(1): COM_STRING = Cleaning
clean-$(1):
	@$$(call run_and_test,rm -rf $(OBJ_DIR)/$(1))

fclean-$(1): COM_STRING = Cleaning
fclean-$(1): clean-$(1)
	@$$(call run_and_test,rm -f $(BIN_DIR)/$(1))

re-$(1): fclean-$(1) $(1)
endef

$(foreach d,$(EX_DIRS),$(eval $(call GEN_RULES,$(d))))

$(BIN_DIR):
	@mkdir -p $@

clean: COM_STRING = Cleaning
clean:
	@$(call run_and_test,rm -rf $(OBJ_DIR))


fclean: COM_STRING = Cleaning
fclean: clean
	@$(call run_and_test,rm -rf $(BIN_DIR))

re: fclean all

.PHONY: all clean fclean re help \
		$(EX_DIRS) \
		$(addprefix run-,$(EX_DIRS)) \
		$(addprefix clean-,$(EX_DIRS)) \
		$(addprefix fclean-,$(EX_DIRS)) \
		$(addprefix re-,$(EX_DIRS))

help:
	@echo "Usage:"
	@echo "  make ex00       - build ex00 -> bin/ex00"
	@echo "  make run-ex00   - build and run ex00"
	@echo "  make all        - build all ex*/"
	@echo "  make clean|fclean|re"
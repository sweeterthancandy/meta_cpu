CXXFLAGS+=-std=c++14

CPPFLAGS+=-DBOOST_MPL_CFG_NO_PREPROCESSED_HEADERS 
CPPFLAGS+=-DBOOST_MPL_LIMIT_VECTOR_SIZE=50
CPPFLAGS+=-DCONFIG_MAX_PRIME=10

.PHONY: default-target
default-target: mcpu-proxy
mcpu-proxy: mcpu


mcpu.cpp: assign_reg__.h contex.h int.h label.h masm.h op_cond_jmp.h op_jmp.h op_arth.h ops.h op_traits.h primatives.h register.h test_prime.h test_rt_prime.h

.PHONY: clean
clean:
	$(RM) mcpu

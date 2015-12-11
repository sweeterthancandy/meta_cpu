CXXFLAGS+=-std=c++14
CXXFLAGS+=-Wno-deprecated-declarations

CPPFLAGS+=-DBOOST_MPL_CFG_NO_PREPROCESSED_HEADERS 
CPPFLAGS+=-DBOOST_MPL_LIMIT_VECTOR_SIZE=50
ifdef MAX_PRIME
 CPPFLAGS+=-DCONFIG_MAX_PRIME=$(MAX_PRIME)
else
 CPPFLAGS+=-DCONFIG_MAX_PRIME=10
endif


# the template depth is there to stop infinate recursion 
# we dont need this arbitary limit
CXXFLAGS+=-ftemplate-depth=1000000

CPPFLAGS+=-I/home/dest/boost_1_59_0

.PHONY: default-target
default-target: mcpu-proxy
mcpu-proxy:
	$(RM) mcpu
	$(MAKE) mcpu


mcpu.cpp: assign_reg__.h contex.h int.h label.h masm.h op_arth.h op_cond_jmp.h op_jmp.h ops.h op_traits.h primatives.h register.h test_rt_prime.h


.PHONY: clean
clean:
	$(RM) mcpu

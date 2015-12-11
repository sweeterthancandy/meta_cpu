#include "masm.h"

#include <boost/mpl/reverse.hpp>
#include "test_rt_prime.h"

DEF_LABEL(start_outer_)
DEF_LABEL(start_inner_)
DEF_LABEL(end_inner_ )
DEF_LABEL(end_outer_ )
DEF_LABEL(not_prime_ )
DEF_LABEL(fun_ )
DEF_LABEL(fun2_ )
DEF_LABEL(start_ )
DEF_LABEL(test_ )
DEF_LABEL(end_ )


using prog_ = mpl::vector<
        mov< int_<3>, reg<0> >, 
start_outer_,
        sub< reg<0>, int_<CONFIG_MAX_PRIME>, reg<2> >,
        jgt< reg<2>, end_outer_> , 
        mov< int_<3>, reg<1> >,
start_inner_,
        mul< reg<1>, reg<1>, reg<2> >,
        sub< reg<2>, reg<0>, reg<2> >,
        jgt< reg<2>, end_inner_ >,
        mod< reg<0>, reg<1>, reg<2> >,
        jeq< reg<2>, not_prime_ >,
        add< reg<1>, int_<2>, reg<1> >,
        jmp<start_inner_>,
end_inner_,
        opush< reg<0> >,
not_prime_,
        add< reg<0>, int_<2>, reg<0> >,
        jmp<start_outer_>,
end_outer_
>;

using prog2_ = mpl::vector<
        nop,
        jmp<start_>,
        // using reg<0> for in/out
test_,
        push< reg<1> >,
        push< reg<2> >,

        mov< int_<3>, reg<1> >,
start_outer_,
        mul< reg<1>, reg<1>, reg<2> >,
        sub< reg<2>, reg<0>, reg<2> >,
        jgt< reg<2>, end_ >,
        opush < reg<0> >,
        add< reg<0>, int_<2>, reg<0> >,
        jmp <start_outer_>,
end_,
        pop< reg<2> >,
        pop< reg<1> >,
        ret,
start_,
        mov< int_<7>, reg<0> >,       
        call<test_>,
        mov< int_<10>, reg<0> >,       
        call<test_>,
        nop
>;

using prog  = program<4,prog2_>;

int main(int argc, char** argv){
        try{

                std::cout << "------- asm code\n";
                prog::print_asm();
                std::cout << "------- symbols resolved\n";
                prog::print_assembed();
                std::cout << "------- debug run\n";
                prog::debugger();
                
                // stack is in reverse
                using rev_result = prog::output_stack;
                using result = mpl::reverse<rev_result>::type;
                
                std::cout << "------- output stack\n";
                mpl::for_each<result>([](auto&& value){
                        std::cout << value << ",";
                });
                std::cout << "\n";
                std::cout << "------- real result\n";
                print_primes_5();

        } catch( const std::exception& e){
                std::cerr 
                        << "Caught exception: "
                        << boost::diagnostic_information(e)
                        ;
                return EXIT_FAILURE;
        }
}

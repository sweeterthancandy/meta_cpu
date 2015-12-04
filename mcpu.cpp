#include "masm.h"

#include <boost/mpl/reverse.hpp>
#include "test_rt_prime.h"
#include "test_prime.h"

int main(int argc, char** argv){
        try{

                std::cout << "------- asm code\n";
                prog::print_asm();
                std::cout << "------- symbols resolved\n";
                prog::print_assembed();
                std::cout << "------- debug run\n";
                prog::debugger();
                
                // stack is in reverse
                using rev_result = prog::stack;
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

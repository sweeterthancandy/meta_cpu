#pragma once

void print_primes(){
        struct local{
                static bool test(int i){
                        for(int j=3;j*j<=i;j+=2){
                                if( i % j == 0 )
                                        return false;
                        }
                        return true;
                }
        };
        for(int i=3;i<=CONFIG_MAX_PRIME;i+=2){
                if( local::test(i)){
                        std::cout << i << ",";
                }
        }
        std::cout << std::endl;
}
void print_primes_2(){
        for(int i=3;i<=CONFIG_MAX_PRIME;i+=2){
                for(int j=3;j*j<=i;j+=2){
                        if( i % j == 0 )
                                goto not_prime;
                }
                std::cout << i << ",";
not_prime:;
        }
        std::cout << std::endl;
}
void print_primes_3(){
        int i;
        int j;

        i = 3;
        for( ;;){
start_outer:
                if( i>CONFIG_MAX_PRIME )
                        goto end_outer;
                for( j=3;j*j<=i;j+=2){
                        if( i % j == 0 )
                                goto not_prime;
                }
                std::cout << i << ",";
not_prime:;
                i+=2;
                goto start_outer;

        }
end_outer:;
        std::cout << std::endl;
}
void print_primes_4(){
        int i;
        int j;

        i = 3;
start_outer:
        if( i>CONFIG_MAX_PRIME )
                goto end_outer;
        j=3;
start_inner:
        if( j*j > i )
                goto end_inner;
        if( i % j == 0 )
                goto not_prime;
        j += 2;
        goto start_inner;
end_inner:;
          std::cout << i << ",";
not_prime:;
          i+=2;
          goto start_outer;

end_outer:;
          std::cout << std::endl;
}
void print_primes_5(){
        std::array<int,4> reg;

        reg[0] = 3;
start_outer:
        reg[2] = reg[0] - CONFIG_MAX_PRIME;
        if( reg[2] > 0 )
                goto end_outer;
        reg[1]=3;
start_inner:
        reg[2] = reg[1] * reg[1];
        reg[2] = reg[2] - reg[0];
        if( reg[2] > 0 )
                goto end_inner;
        reg[2] = reg[0] % reg[1];
        if( reg[2] == 0 )
                goto not_prime;
        reg[1] += 2;
        goto start_inner;
end_inner:;
        std::cout << reg[0];
        std::cout << ",";
not_prime:;
        reg[0]+=2;
        goto start_outer;
end_outer:;
        std::cout << std::endl;
}

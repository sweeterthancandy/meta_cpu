#!/bin/bash

for i in {3..100} ; do
        echo "Doing $i"
        time make MAX_PRIME=$(echo "2^$i" | bc) > /dev/null 2>&1 | grep 'real'
done





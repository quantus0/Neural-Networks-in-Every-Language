#!/bin/bash

A=3
B=4
C=2

for ((i = 0; i < A * B; i++)); do
    D[$i]=$(awk -v s=$RANDOM 'BEGIN { srand(s); printf("%.6f\n", rand() - 0.5)}')
done

for ((i = 0; i < B * C; i++)); do
    E[$i]=$(awk -v s=$RANDOM 'BEGIN { srand(s); printf("%.6f\n", rand() - 0.5)}')
done

for ((i = 0; i < B; i++)); do
    F[$i]=$(awk -v s=$RANDOM 'BEGIN { srand(s); printf("%.6f\n", rand() - 0.5)}')
done

for ((i = 0; i < C; i++)); do
    G[$i]=$(awk -v s=$RANDOM 'BEGIN { srand(s); printf("%.6f\n", rand() - 0.5)}')
done

s() {
    x=$1
    echo "scale=10; 1 / (1 + e(-($x)))" | bc -l
}

r() {
    x=$1
    awk -v x="$x" 'BEGIN {print (x < 0) ? 0 : x}'
}

ds() {
    x=$1
    echo "scale=10; $x * (1 - $x)" | bc -l
}

dr() {
    x=$1
    awk -v x="$x" 'BEGIN {print (x < 0) ? 0 : 1}'
}

f() {
    I=("$@")
    H=()
    O=()

    for ((i = 0; i < B; i++)); do
        Z=0
        for ((j = 0; j < A; j++)); do
            Z=$(echo "scale=10; $Z + ${I[$j]} * ${D[$((i*A+j))]}" | bc -l)
        done
        Z=$(echo "scale=10; $Z + ${F[$i]}" | bc -l)
        H[$i]=$(r $Z)
    done

    for ((i = 0; i < C; i++)); do
        Z=0
        for ((j = 0; j < B; j++)); do
            Z=$(echo "scale=10; $Z + ${H[$j]} * ${E[$((i*B+j))]}" | bc -l)
        done
        Z=$(echo "scale=10; $Z + ${G[$i]}" | bc -l)
        O[$i]=$(s $Z)
    done

    echo "${O[@]}"
}

b() {
    I=("$@")
    T=("${@: -2}")
    H=()
    O=()
    E1=()
    E2=()

    for ((i = 0; i < B; i++)); do
        Z=0
        for ((j = 0; j < A; j++)); do
            Z=$(echo "scale=10; $Z + ${I[$j]} * ${D[$((i*A+j))]}" | bc -l)
        done
        Z=$(echo "scale=10; $Z + ${F[$i]}" | bc -l)
        H[$i]=$(r $Z)
    done

    for ((i = 0; i < C; i++)); do
        Z=0
        for ((j = 0; j < B; j++)); do
            Z=$(echo "scale=10; $Z + ${H[$j]} * ${E[$((i*B+j))]}" | bc -l)
        done
        Z=$(echo "scale=10; $Z + ${G[$i]}" | bc -l)
        O[$i]=$(s $Z)
    done

    for ((i = 0; i < C; i++)); do
        E=$(echo "scale=10; ${T[$i]} - ${O[$i]}" | bc -l)
        E1[$i]=$(echo "scale=10; $E * $(ds ${O[$i]})" | bc -l)
    done

    for ((i = 0; i < B; i++)); do
        Z=0
        for ((j = 0; j < C; j++)); do
            Z=$(echo "scale=10; $Z + ${E1[$j]} * ${E[$((j*B+i))]}" | bc -l)
        done
        E2[$i]=$(echo "scale=10; $Z * $(dr ${H[$i]})" | bc -l)
    done

    for ((i = 0; i < A*B; i++)); do
        W=${D[$i]}
        IN=${I[$(($i%A))]}
        GR=${E2[$(($i/A))]}
        D[$i]=$(echo "scale=10; $W + 0.1 * $IN * $GR" | bc -l)
    done

    for ((i = 0; i < B*C; i++)); do
        W=${E[$i]}
        HN=${H[$(($i%B))]}
        GR=${E1[$(($i/B))]}
        E[$i]=$(echo "scale=10; $W + 0.1 * $HN * $GR" | bc -l)
    done

    for ((i = 0; i < B; i++)); do
        F[$i]=$(echo "scale=10; ${F[$i]} + 0.1 * ${E2[$i]}" | bc -l)
    done

    for ((i = 0; i < C; i++)); do
        G[$i]=$(echo "scale=10; ${G[$i]} + 0.1 * ${E1[$i]}" | bc -l)
    done
}

for ((e = 0; e < 100; e++)); do
    I=(1 2 3)
    T=(0.5 0.8)
    b "${I[@]}" "${T[@]}"
done

I=(1 2 3)
O=($(f "${I[@]}"))
echo "Output: ${O[@]}"

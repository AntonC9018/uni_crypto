# Lucrarea de laborator la RSA
# Curmanschii Anton, IA1901
# 2021
import math

def main():
    # Rezultatele de afisare sunt copiate si in functiile respective
    # insa puteti decomenta liniile ce apeleaza functiile acestea pentru
    # a verifica raspunsurile.

    # n1 
    # Programe pentru GCD si numere prime
    #    Asa programe am realizat de mai multe ori.
    #    Evident, nu le programam in cea mai optima metoda, 
    #    de exemplu ca cea descrisa in prezentarea dvs.
    #    Codul la Algoritmul Euclidean Extins gasiti mai jos (functia egcd)


    # n2_var1()
    # n2_var2()
    # n3_var2()
    # n4()
    # n5()

    pass

def n2_var1():
    n = 3937
    e = 17
    [p, q] = factor(n)
    ϕ = (p - 1) * (q - 1)
    d = inverse_modulo(e, ϕ)

    print(f'n = {n} = pq = {p} * {q}')
    print(f'ϕ(n) = ϕ(pq) = (p - 1) * (q - 1) = {ϕ}')
    print(f'd = e^-1 mod ϕ = {e}^-1 mod {ϕ} = {d}')

    # La executare se afiseaza:
    # n = 3937 = pq = 31 * 127
    # ϕ(n) = ϕ(pq) = (p - 1) * (q - 1) = 3780
    # d = e^-1 mod ϕ = 17^-1 mod 3780 = 3113

def n2_var2():
    n = 319
    d = 101
    [p, q] = factor(n)
    ϕ = (p - 1) * (q - 1)
    e = inverse_modulo(d, ϕ)

    print(f'n = {n} = pq = {p} * {q}')
    print(f'ϕ(n) = ϕ(pq) = (p - 1) * (q - 1) = {ϕ}')
    print(f'e = d^-1 mod ϕ = {d}^-1 mod {ϕ} = {e}')

    # La executare se afiseaza:
    # n = 319 = pq = 11 * 29
    # ϕ(n) = ϕ(pq) = (p - 1) * (q - 1) = 280
    # e = d^-1 mod ϕ = 101^-1 mod 280 = 61

def n3_var2():
    T = "THIS IS THOUGH"
    n, e = 12091, 13
    encoded_chars = string_to_nums(T)
    
    print(encoded_chars)

    # avem nevoie sa lipim codificarea numerica a caracterelor
    # aici avem mai multe posibilitati:
    #
    #   1. sa lipim direct numerele in reprezentarea zecimala
    #      de exemplu, [11, 23] -> [1123]
    #      astfel incat numarul primit sa fie mereu mai mic ca n 
    #      (pentru 12091, am avea cate 2 numere pentru un singur bloc) 
    #
    #   2. sa criptam direct caracterele codificate, fara nici o lipire
    #
    #   3. sa le lipim pe biti. 32 = 2^5, putem reprezenta fiecare caracter pe doar 5 biti
    #      voi selecta aceasta optiune

    num_bits_n = num_bits(n)
    num_chars_per_message = num_bits_n // 5 # <-- literele se reprezinta pe 5 caractere
    merged_message = bit_merge_by(encoded_chars, num_chars_per_message, 5)

    print(merged_message)

    # criptarea se face prin calcularea lui c = m^e mod n
    merged_encrypted = [modular_pow(m, e, n) for m in merged_message]

    print(merged_encrypted)

    # decriptarea se face prin calcularea lui m = c^d mod n
    [p, q] = factor(n)
    ϕ = (p - 1) * (q - 1)
    d = inverse_modulo(e, ϕ)
    merged_decrypted = [modular_pow(c, d, n) for c in merged_encrypted]

    print(merged_decrypted)

    # convertam inapoi in litere
    unmerged_decrypted = bit_unmerge_by(merged_decrypted, num_chars_per_message, 5)

    print(unmerged_decrypted)

    # concatenam sirul
    message_decrypted = nums_to_string(unmerged_decrypted)

    print(message_decrypted)

    # La executare se afiseaza:
    # [20, 8, 9, 19, 0, 9, 19, 0, 20, 8, 15, 21, 7, 8]
    # [648, 307, 9, 608, 648, 501, 232]
    # [4697, 6042, 11336, 4161, 4697, 11330, 11410]
    # [648, 307, 9, 608, 648, 501, 232]
    # [20, 8, 9, 19, 0, 9, 19, 0, 20, 8, 15, 21, 7, 8]
    # THIS IS THOUGH

def n4():
    n = 1050589
    ϕ = 1048540

    # n = pq
    # ϕ = (p - 1)(q - 1) = pq - p - q + 1 = n - p - q + 1
    # -p - q = -n + ϕ - 1
    # 
    # p + q = n - ϕ + 1 
    # pq = n
    #
    # p = n / q 
    # n / q + q = n - ϕ + 1
    # n + q^2 = q(n - ϕ + 1)
    # q^2 - q(n - ϕ + 1) + n = 0
    
    def solve_quadratic(a, b, c):
        delta = math.sqrt(b*b - 4 * a * c)
        return (-b - delta) // (2 * a), (-b + delta) // (2 * a)

    q1, q2 = solve_quadratic(1, -(n - ϕ + 1), n)
    q = int(q2)
    p = n // q
    print(f'q = {q}, p = {p}')

    # verificarea
    assert p * q == n
    assert (p - 1) * (q - 1) == ϕ

    # Afiseaza:
    # q = 1031, p = 1019

def n5():
    # probabilitatea ca un numar prim `p` este egala cu 1 / ln(cel_mai_mare_bit(p))
    # adica pentru numarul de biti ai lui p = 8, cel_mai_mare_bit(p) = 2^8
    # 
    # Vrem o expresie pentru 1 / ln(2^n)
    # 2^n = 10^m
    # ln( 2^n ) = ln( 10^m )
    # ln(2^n) = b = ln(10) * m
    # 1 / b = 1 / ln(2^n) = 1 / (ln(10) * m)
    m = 200
    b = math.log(10) * m

    # Estimarea ar fi p imultita cu numarul de numere pe 200 de pozitii zecimale
    # Exista cate 0.9 * 10^200  
    print(f'Numarul de numere pe 200 pozitii estimata este {0.9 / b} e200')

    b100 = (math.log(10) * 100)
    print(f'Numarul de numere pe 100 pozitii estimata este {0.9 / b100} e100')
    
    # Aceste doua valori nici nu sunt comparabile, deoarece cantitatea numerelor prime
    # creste impreuna cu cantitatea totala a numerelor, insa raritatile numerelor sunt comparabile.
    # Avand in vedere ca raritatea numerelor este o functie logaritmica inversa (1 / ln)
    # raritatea numerelor prime tot creste logaritmic.
    # In acest exemplu, numarul de pozitii a crescut de 2 ori, impreuna cu raritatea
    # (probabilitatea a scazut de doua ori)

    # Codul afiseaza:
    # Numarul de numere pe 200 pozitii estimata este 0.001954325168564633 e200
    # Numarul de numere pe 100 pozitii estimata este 0.003908650337129266 e100


def factor(n):
    result = []
    for i in range(2,n):
        if n % i == 0:
            result.append(i)
            n = n // i
    return result


def egcd(a, b):
    r0, r1 = a, b
    s0, s1 = 1, 0
    t0, t1 = 0, 1

    while r1 != 0:
        q = r0 // r1 

        r_new = r0 - q * r1
        r0 = r1
        r1 = r_new

        t_new = t0 - q * t1
        t0 = t1
        t1 = t_new

        s_new = s0 - q * s1
        s0 = s1
        s1 = s_new
    
    return r0, t1, s1


def inverse_modulo(a, n):
    t0, t1 = 0, 1
    r0, r1 = n, a

    while r1 != 0:
        q = r0 // r1 
        
        r_new = r0 - q * r1
        r0 = r1
        r1 = r_new

        t_new = t0 - q * t1
        t0 = t1
        t1 = t_new
    
    if r0 != 1:
        return 0

    if t0 < 0:
        return t0 + n

    return t0

def char_to_num(ch):
    if ch == ' ': 
        return 0
    else:
        return ord(ch) - ord('A') + 1

def num_to_char(num):
    if num == 0: 
        return ' '
    else:
        return chr(num + ord('A') - 1)

def string_to_nums(s):
    return [char_to_num(ch) for ch in s]

def nums_to_string(nums):
    return ''.join([num_to_char(num) for num in nums])

def num_bits(n):
    num_bits = 0
    while n != 0:
        num_bits += 1
        n = n >> 1
    return num_bits

def bit_merge_by(arr, n, bits_per_input):
    result = []
    for i in range(0, len(arr), n):
        t = 0
        for j in range(n):
            t = (t << bits_per_input) | arr[i + j]
        result.append(t)
    return result

def bit_unmerge_by(arr, n, bits_per_input):
    result = []
    mask = (1 << bits_per_input) - 1
    for t in arr:
        for j in range(n):
            shift_amount = bits_per_input * (n - j - 1)
            result.append((t >> shift_amount) & mask)
    return result

def modular_pow(base, exponent, modulus):
    result = 1
    base = base % modulus
    while exponent > 0:
        if exponent % 2 == 1:
            result = (result * base) % modulus
        exponent = exponent >> 1
        base = (base * base) % modulus
    return result

main()
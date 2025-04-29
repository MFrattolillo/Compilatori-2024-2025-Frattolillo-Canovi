int computeSum(int a, int b, int n) {
    int sum = 0;
    for (int i = 0; i < n; ++i) {
        // Questa è la nostra candidata “loop-invariant”:
        // - dipende solo da a e b (parametri, non definiti nel loop)
        // - è definita una sola volta per iterazione (unique def)
        // - il blocco del loop la domina e domina tutti gli use (sum += ...)
        // - il blocco del loop domina tutte le uscite (i < n false, i>=n)
        int c = a + b;

        // “c” viene poi usata qui, in un blocco dominato da quello che la definisce
        sum += c * i;
    }
    return sum;
}

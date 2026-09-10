extern unsigned long _estack;

void Reset_Handler(void);
int main(void);

__attribute__((section(".isr_vector"), used))
const void *vector_table[] = {
    &_estack,
    (void *)Reset_Handler
};

void Reset_Handler(void)
{
    main();

    for (;;) {
    }
}
.text
.global  _start, _vector_table

/* Entry point and vector table definition */
_start:
_vector_table:
    ldr     pc, =Reset_Handler            /* Reset handler */
    ldr     pc, =Undefined_Handler        /* Undefined instruction handler */
    ldr     pc, =SVC_Handler              /* Supervisor Call handler */
    b       halt                         /* Placeholder for Prefetch Abort */
    b       halt                         /* Placeholder for Data Abort */
    .word   0                            /* Reserved */
    b       halt                         /* Placeholder for IRQ handler */
    b       halt                         /* Placeholder for FIQ handler */

.align 2

/* Undefined instruction exception handler */
Undefined_Handler:
    stmdb   sp!, {r0-r12, lr}             /* Save registers and LR on stack */
    mrs     r0, cpsr                      /* Move current program status register to r0 */
    bl      printException                /* Call function to print exception information */
    ldmia   sp!, {r0-r12, pc}^            /* Restore registers and return from exception */

/* Supervisor Call exception handler (Software interrupt) */
SVC_Handler:
    stmdb   sp!, {r0-r12, lr}             /* Save registers and LR on stack */
    mrs     r0, cpsr                      /* Move current program status register to r0 */
    bl      printException                /* Call function to print exception information */
    ldmia   sp!, {r0-r12, pc}^            /* Restore registers and return from exception */

/* Reset handler initializes the system */
Reset_Handler:
    mrc     p15, 0, r0, c1, c0, 0        /* Read CP15 System Control register */
    bic     r0, r0, #(0x1 << 13)         /* Clear V bit for normal exception vectors */
    bic     r0, r0, #(0x1 << 12)         /* Disable I Cache */
    bic     r0, r0, #(0x1 << 2)          /* Disable D Cache */
    bic     r0, r0, #(0x1 << 1)          /* Disable strict alignment */
    bic     r0, r0, #(0x1 << 11)         /* Disable branch prediction */
    bic     r0, r0, #0x1                 /* Disable MMU */
    mcr     p15, 0, r0, c1, c0, 0        /* Write back to CP15 System Control register */

    cps     #0x13                        /* Change to Supervisor mode */
    ldr     sp, =0x80200000              /* Set stack pointer for Supervisor mode */

    ldr     r0, =_vector_table           /* Load address of vector table */
    mcr     p15, 0, r0, c12, c0, 0       /* Set Vector Base Address Register */

    bl      clean_bss                    /* Clear uninitialized data */
    bl      system_init                  /* Call system initialization */

halt:
    b       halt                         /* Infinite loop to halt CPU */

/* Function to clear the BSS segment */
clean_bss:
    ldr     r1, =__bss_start             /* Load start address of BSS section */
    ldr     r2, =__bss_end               /* Load end address of BSS section */
    mov     r3, #0                       /* Zero value to fill */
clean_loop:
    cmp     r1, r2                       /* Compare pointers */
    strlt   r3, [r1], #4                 /* Clear and increment pointer */
    blt     clean_loop                   /* Loop until whole BSS is cleared */

    mov     pc, lr                       /* Return from function */

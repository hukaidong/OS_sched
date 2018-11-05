#ifndef MY_CONST_H
#define MY_CONST_H value

#define UNIT_MB 0x100000
#define UNIT_KB 0x000400

#define PAGE_MSK (8*UNIT_MB-4*UNIT_KB)
#define PAGE_OFFSET 12
#define PAGE_SIZE 4*UNIT_KB
#define PAGE_LIM_PER_THREAD 1*UNIT_KB

#define PCB_SIZE ((6*UNIT_MB) / (4*UNIT_KB))
#define P_RW (PROT_READ | PROT_WRITE)
#define P_N  (PROT_NONE)

#define VM_SIZE (8*UNIT_MB)
#define sys_vm_size ((2*UNIT_MB) - (16*UNIT_KB))

#define SEG_AVIL_FMSK 0x01
#define SEG_TERM_FMSK 0x02

#define MAX_Q_SIZE 20

#define FIB_RUNNING 0
#define FIB_TERMINATED 1

#endif /* ifndef MY_CONST_H */

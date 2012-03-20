
#if WINDOWS

extern "C" int dbg_init(void)
{
    return 0;
}

#else

#include <sys/prctl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>
#include <unistd.h>
#include <execinfo.h>

#include "shared.h"
#include "dbg.h"


extern void dump_stack(void);


static struct sigaction old_int, old_ill, old_term, old_segv, old_fpe, old_abrt;

void dump_stack(void)
{
    void *trace[32];
    char **messages;
    int i, trace_size;


    trace_size = backtrace(trace, 32);
    messages = backtrace_symbols(trace, trace_size);
    if (messages != NULL)
    {
        printf("backtrace:\n");
        for (i = 1; i < trace_size; i++)
            printf("%s\n", messages[i]);
        FREE(messages);
    }
}

static void show_regs(struct sigcontext *regs)
{
    char comm[20] = { '\0' };
    prctl(PR_GET_NAME, comm);
    printf("\n");
    printf("Pid: %d, comm: %20s\n", getpid(), comm);
    printf("pc : [<%08lx>]    lr : [<%08lx>]    psr: %08lx\n"
           "sp : %08lx  ip : %08lx  fp : %08lx\n",
        regs->arm_pc, regs->arm_lr, regs->arm_cpsr,
        regs->arm_sp, regs->arm_ip, regs->arm_fp);
    printf("r10: %08lx  r9 : %08lx  r8 : %08lx\n",
        regs->arm_r10, regs->arm_r9,
        regs->arm_r8);
    printf("r7 : %08lx  r6 : %08lx  r5 : %08lx  r4 : %08lx\n",
        regs->arm_r7, regs->arm_r6,
        regs->arm_r5, regs->arm_r4);
    printf("r3 : %08lx  r2 : %08lx  r1 : %08lx  r0 : %08lx\n",
        regs->arm_r3, regs->arm_r2,
        regs->arm_r1, regs->arm_r0);
}


static void sighandler(int signo, siginfo_t *info, void *context)
{
    ucontext_t *uc = (ucontext_t *)context;
#if CONFIG_OSK
    a_cfg_set_testmode_error_code(CFG_CUSTOM_SERVMODE_ASSERT);
#endif
    switch (signo)
    {
    case SIGINT:
        printf("\n%s\n\n", "Interrupt from keyboard");
        printf("Using default signal handler.\n");
        sigaction(SIGINT, &old_int, NULL);
        break;

    case SIGILL:
        printf("\n%s\n\n", "Illegal Instruction");
        printf("\tpc: %p, inst: 0x%08lx\n", (void *)(uc->uc_mcontext.arm_pc), *(unsigned long *)(uc->uc_mcontext.arm_pc));
        show_regs(&uc->uc_mcontext);
        dump_stack();
        printf("Using default signal handler.\n");
        while (1) pause();
        sigaction(SIGILL, &old_ill, NULL);
        break;

    case SIGFPE:
        printf("\n%s\n\n", "Floating point exception");
        printf("\tpc: %p, inst: 0x%08lx\n", (void *)(uc->uc_mcontext.arm_pc), *(unsigned long *)(uc->uc_mcontext.arm_pc));
        show_regs(&uc->uc_mcontext);
        dump_stack();
        printf("Using default signal handler.\n");
        while (1) pause();
        sigaction(SIGFPE, &old_fpe, NULL);
        break;

    case SIGSEGV:
        printf("\n%s\n\n", "Invalid memory reference");
        printf("\tpc: %p, addr: %p\n", (void *)(uc->uc_mcontext.arm_pc), info->si_addr);
        show_regs(&uc->uc_mcontext);
        dump_stack();
        printf("Using default signal handler.\n");
        while (1) pause();
        sigaction(SIGSEGV, &old_segv, NULL);
        break;

    case SIGTERM:
        printf("\n%s\n\n", "Termination signal");
        printf("Using default signal handler.\n");
        sigaction(SIGTERM, &old_term, NULL);
        break;

    case SIGABRT:
        printf("\n%s\n\n", "Abort signal");
        show_regs(&uc->uc_mcontext);
        dump_stack();
        while (1) pause();
        sigaction(SIGABRT, &old_abrt, NULL);
        break;

    default:
        printf("\n%s: %d\n\n", "Unexpected signal", signo);
        while (1) pause();
        break;
    }
}


int dbg_init(void)
{
    struct sigaction sa;
    int ret;

    sa.sa_sigaction = &sighandler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_SIGINFO;
    if ((ret = sigaction(SIGINT, &sa, &old_int)) != 0) return ret;
    if ((ret = sigaction(SIGILL, &sa, &old_ill)) != 0) return ret;
    if ((ret = sigaction(SIGTERM, &sa, &old_term)) != 0) return ret;
    if ((ret = sigaction(SIGFPE, &sa, &old_fpe)) != 0) return ret;
    if ((ret = sigaction(SIGABRT, &sa, &old_abrt)) != 0) return ret;
    sa.sa_flags |= SA_ONSTACK;
    if ((ret = sigaction(SIGSEGV, &sa, &old_segv)) != 0) return ret;

    return 0;
}

#endif



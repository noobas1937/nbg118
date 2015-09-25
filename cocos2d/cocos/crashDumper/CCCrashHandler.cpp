//
//  CCCrashHandler.cpp
//  GameCore
//
//  Created by ChenFei on 13-10-29.
//
//

#include "CCCrashHandler.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <pthread.h>
#include <stdarg.h>
#include <fcntl.h>
#include <sys/types.h>
#include <dirent.h>
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <sys/ptrace.h>
#endif
#include <sys/wait.h>
#include <sys/stat.h>
#include <dlfcn.h>
#include "CCArmSingalCallStackWalker.h"
#include "CCRegionMap.h"
#include "CCFileDumpper.h"
//#include "../../../Classes/Ext/CCDevice.h"

#define LOG(format, ...)      CCLOG(format, ##__VA_ARGS__)

#ifndef TRACE_ARM_EXIDX
#define TRACE_ARM_EXIDX    0x70000001      /* .ARM.exidx segment */
#endif

#define EI_NIDENT       16

typedef uint16_t Elf32_Half;
typedef uint32_t Elf32_Word;
typedef int32_t  Elf32_Sword;
typedef uint32_t Elf32_Addr;
typedef uint32_t Elf32_Off;


typedef struct _Elf32_Ehdr
{
    unsigned char    e_ident[EI_NIDENT];    /* Magic number and other info */
    Elf32_Half    e_type;            /* Object file type */
    Elf32_Half    e_machine;        /* Architecture */
    Elf32_Word    e_version;        /* Object file version */
    Elf32_Addr    e_entry;        /* Entry point virtual address */
    Elf32_Off    e_phoff;        /* Program header table file offset */
    Elf32_Off    e_shoff;        /* Section header table file offset */
    Elf32_Word    e_flags;        /* Processor-specific flags */
    Elf32_Half    e_ehsize;        /* ELF header size in bytes */
    Elf32_Half    e_phentsize;        /* Program header table entry size */
    Elf32_Half    e_phnum;        /* Program header table entry count */
    Elf32_Half    e_shentsize;        /* Section header table entry size */
    Elf32_Half    e_shnum;        /* Section header table entry count */
    Elf32_Half    e_shstrndx;        /* Section header string table index */
} Elf32_Ehdr;

/*
 * Program Header
 */
typedef struct {
    Elf32_Word      p_type;         /* entry type */
    Elf32_Off       p_offset;       /* offset */
    Elf32_Addr      p_vaddr;        /* virtual address */
    Elf32_Addr      p_paddr;        /* physical address */
    Elf32_Word      p_filesz;       /* file size */
    Elf32_Word      p_memsz;        /* memory size */
    Elf32_Word      p_flags;        /* flags */
    Elf32_Word      p_align;        /* memory & file alignment */
} Elf32_Phdr;

/* e_ident offsets */
#define EI_MAG0         0       /* '\177' */
#define EI_MAG1         1       /* 'E'    */
#define EI_MAG2         2       /* 'L'    */
#define EI_MAG3         3       /* 'F'    */
#define EI_CLASS        4       /* File class */
#define EI_DATA         5       /* Data encoding */
#define EI_VERSION      6       /* File version */
#define EI_OSABI        7       /* Operating system/ABI identification */
#define EI_ABIVERSION   8       /* ABI version */
#define EI_PAD          9       /* Start of padding bytes up to EI_NIDENT*/
#define EI_NIDENT       16      /* First non-ident header byte */

/* e_ident[EI_MAG0,EI_MAG3] */
#define ELFMAG0         0x7f
#define ELFMAG1         'E'
#define ELFMAG2         'L'
#define ELFMAG3         'F'
#define ELFMAG          "\177ELF"
#define SELFMAG         4

#define IS_ELF(ehdr)    ((ehdr).e_ident[EI_MAG0] == ELFMAG0 && \
(ehdr).e_ident[EI_MAG1] == ELFMAG1 && \
(ehdr).e_ident[EI_MAG2] == ELFMAG2 && \
(ehdr).e_ident[EI_MAG3] == ELFMAG3)

const char* SIGNAL_DESC[] = 
{
    "NULL:Undefined",
    "SIGHUP:hangup",
    "SIGINT:interrupt",
    "SIGQUIT:quit",
    "SIGILL:illegal instruction (not reset when caught)",
    "SIGTRAP:trace trap (not reset when caught)",
    "SIGABRT:abort()",
#if  (defined(_POSIX_C_SOURCE) && !defined(_DARWIN_C_SOURCE))
    "SIGPOLL:pollable event ([XSR] generated, not supported)",
#else	/* (!_POSIX_C_SOURCE || _DARWIN_C_SOURCE) */
    "SIGIOT	SIGABRT SIGEMT compatibility EMT instruction",
#endif	/* (!_POSIX_C_SOURCE || _DARWIN_C_SOURCE) */
    "SIGFPE:floating point exception",
	"SIGKILL:kill (cannot be caught or ignored)",
	"SIGBUS:bus error",
	"SIGSEGV:segmentation violation",
	"SIGSYS:bad argument to system call",
	"SIGPIPE:write on a pipe with no one to read it",
	"SIGALRM:alarm clock",
	"SIGTERM:software termination signal from kill",
	"SIGURG:urgent condition on IO channel",
	"SIGSTOP:sendable stop signal not from tty",
	"SIGTSTP:stop signal from tty",
	"SIGCONT:continue a stopped process",
	"SIGCHLD:to parent on child stop or exit",
	"SIGTTIN:to readers pgrp upon background tty read",
	"SIGTTOU:like TTIN for output if (tp->t_local&LTOSTOP)",
#if  (!defined(_POSIX_C_SOURCE) || defined(_DARWIN_C_SOURCE))
    "SIGIO:input/output possible signal",
#endif
    "SIGXCPU:exceeded CPU time limit",
    "SIGXFSZ:exceeded file size limit",
    "SIGVTALRM:virtual time alarm",
    "SIGPROF:profiling time alarm",
#if  (!defined(_POSIX_C_SOURCE) || defined(_DARWIN_C_SOURCE))
    "SIGWINCH:window size changes",
    "SIGINFO:information request",
#endif
    "SIGUSR1:user defined signal 1",
    "SIGUSR2:user defined signal 2",
};

#define SIGNAL_MAX 31

NS_CC_BEGIN

CCCrashHandler* CCCrashHandler::sCrashHandler = NULL;


void CCCrashHandler::cbCrashHandler(int sig, siginfo_t* sigInfo,void * uc)
{
    struct sigaction cur_handler;
    if (sigaction(sig, NULL, &cur_handler) == 0 &&
        (cur_handler.sa_flags & SA_SIGINFO) == 0) {
        // Reset signal handler with the right flags.
        sigemptyset(&cur_handler.sa_mask);
        sigaddset(&cur_handler.sa_mask, sig);
        
        cur_handler.sa_sigaction = cbCrashHandler;
        cur_handler.sa_flags = SA_SIGINFO;
        
        if (sigaction(sig, &cur_handler, NULL) == -1) {
            // When resetting the handler fails, try to reset the
            // default one to avoid an infinite loop here.
            signal(sig, SIG_DFL);
        }
        return;
    }
    
    CCRegionMap::sharedRegionMap()->refresh();
//    pthread_attr_getstack
    char crashID[256] = {0};
    CCArmSingalCallStackWalker walker((uintptr_t)uc);
    
    int count = sprintf(crashID, "%08lX%08lX",
                        (unsigned long)CCRegionMap::sharedRegionMap()->getOffsetInRegionMappingByAddr(walker.getPC()),
                        (unsigned long)CCRegionMap::sharedRegionMap()->getOffsetInRegionMappingByAddr(walker.getLR()));
    for(int i = 0; i < count; ++i)
    {
        // '0' -> 'A' 'F' -> 'W'
        crashID[i] += 0x11;
    }
    
    // only get libgame.so message
    auto pcInfo = CCRegionMap::sharedRegionMap()->getRegionInfoByAddr(walker.getPC());
    auto lrInfo = CCRegionMap::sharedRegionMap()->getRegionInfoByAddr(walker.getLR());
    if ((pcInfo && strstr((char*)pcInfo->name, "libgame") != NULL)
        || (lrInfo || strstr((char*)lrInfo->name, "libgame") != NULL)
        ) {
        // There is an analyse rule in umeng SDK to group the error message, they will filter out the HEX value and decimal value, in out logic, the crash ID
        // is designed to identify the error group, so based on their advise we need to put the in the tail of first dump line.
        DUMP("[PC:%s LR:%s]Process crash with signal %s, fault address 0x%08lX, CrashID:%s",
             CCRegionMap::sharedRegionMap()->getAddressDesc(walker.getPC()).c_str(),
             CCRegionMap::sharedRegionMap()->getAddressDesc(walker.getLR()).c_str(),
             getSignalDesc(sig), sigInfo->si_addr, crashID);
        
        DUMP("    si_signo %d", sigInfo->si_signo);		/* signal number */
        DUMP("    si_errno %d", sigInfo->si_errno);		/* errno association */
        DUMP("    si_code %d", sigInfo->si_code);		/* signal code */
        DUMP("    si_pid %d", sigInfo->si_pid);			/* sending process */
        DUMP("    si_uid %d", sigInfo->si_uid);			/* sender's ruid */
        DUMP("    si_status %d", sigInfo->si_status);		/* exit value */
        DUMP("    si_addr 0x%08lX", sigInfo->si_addr);		/* faulting instruction */
        DUMP("    sigval si_value 0x%08lX", sigInfo->si_value.sival_int);		/* signal value */
        DUMP("    si_band %ld", sigInfo->si_band);		/* band event for SIGPOLL */
        
        DUMP("    uc 0x%08lX", uc);		/* band event for SIGPOLL */
        
        // Fork current process so that the separated process will not be killed by triggering the singal.
        
        sCrashHandler->dumpProcInfo();
        walker.dump();
        
        // save dump info to file
        CCFileDumpper dumper("if_crash.log");
        if (dumper.isFileExist()) {
            dumper.deleteFile();
        }
        dumper.write(CCCrashDump::sharedDump()->getDumpContent());
    }
    
    CCCrashDump::sharedDump()->clear();
    
    if(sCrashHandler->mOldSigaAction.sa_sigaction)
    {
        CCLOG("CCCrashHandler call old sigaction, perhapse the debuggerd");
        sCrashHandler->mOldSigaAction.sa_sigaction(sig, sigInfo, uc);
    }
    signal(sig, SIG_DFL);//SIG_DFL);
}

const char* CCCrashHandler::getSignalDesc(int signal)
{
    if( signal > SIGNAL_MAX || signal < 0 )
    {
        signal = 0;
    }
    return SIGNAL_DESC[signal];
}

void CCCrashHandler::dumpProcInfo()
{
    char data[1024] = {0};
    char * procName = NULL;
    FILE *fp;
    
    sprintf(data, "/proc/%d/cmdline", this->mPID);
    fp = fopen(data, "r");
    if(fp)
    {
        procName = fgets(data, 1024, fp);
        fclose(fp);
    }
    DUMP("*** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** ");

    
    DUMP("Crashed process: %s(%d)", procName, this->mPID);
    DUMP("Crashed thread: %d", this->mTID);
    
}

CCCrashHandler* CCCrashHandler::sharedHandler()
{
    if(!sCrashHandler)
    {
        sCrashHandler = new CCCrashHandler();
    }
    return sCrashHandler;
}

CCCrashHandler::CCCrashHandler():mPID(-1),mTID(-1),mLibBase(0)
{
    memset(&mSigaAction, 0, sizeof(struct	sigaction));
    memset(&mOldSigaAction, 0, sizeof(struct	sigaction));
    memset(&mSignalStack, 0, sizeof(stack_t));
}

CCCrashHandler::~CCCrashHandler()
{
    CC_SAFE_FREE(mSignalStack.ss_sp);
}

void CCCrashHandler::init()
{
    this->mPID = getpid();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
    this->mTID = gettid();
#endif
    int ret = 0;
    mSignalStack.ss_sp = malloc(SIGSTKSZ);
    mSignalStack.ss_size = SIGSTKSZ;
    
    mSigaAction.sa_sigaction = CCCrashHandler::cbCrashHandler;
    mSigaAction.sa_flags = SA_SIGINFO | SA_ONSTACK;
    ret |= sigaltstack(&mSignalStack, NULL);
    CCLOG("sigaltstack with ret=%d", ret);
//    sigaction(SIGILL, &mSigaAction, NULL);
//    sigaction(SIGABRT, &mSigaAction, NULL);
//    ret |= sigaction(SIGBUS, &mSigaAction, NULL);
//    ret |= sigaction(SIGFPE, &mSigaAction, NULL);
    ret |= sigaction(SIGSEGV, &mSigaAction, &mOldSigaAction);
//    ret |= sigaction(SIGPIPE, &mSigaAction, NULL);
    CCLOG("sigaction with ret=%d", ret);
}

void CCCrashHandler::raiseSignal()
{
    raise(SIGSEGV);
    char* buffer = (char *)0x1234;
    buffer[0] = 'a';
}

NS_CC_END

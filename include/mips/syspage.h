/*-
 * Copyright (c) 2011, Peter Tworek 
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the author nor the names of any co-contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#ifndef _MIPS_SYSPAGE_H
#define _MIPS_SYSPAGE_H

#include <conf/config.h>

/**
 * syspage layout:
 *
 * +------------------+ CONFIG_SYSPAGE_BASE
 * | Exception page   |
 * +------------------+ +0x0400
 * | Boot information |
 * +------------------+ +0x0800
 * | Boot stack       |
 * +------------------+ +0x1000
 * |                  |
 * | Exc mode stack   |
 * |                  |
 * +------------------+ +0x3000
 * |                  |
 * | Int mode stack   |
 * |                  |
 * +------------------+ +0x5000
 * | PGD for boot     |
 * | (MMU only)       |
 * |                  |
 * +------------------+ +0x8000
 * | PTE0 for boot    |
 * | (MMU only)       |
 * +------------------+ +0x9000
 * | PTE1 for UART I/O|
 * | (MMU only)       |
 * +------------------+ +0xA000
 */

#define SYSPAGE		CONFIG_SYSPAGE_BASE
#define BOOTINFO	(SYSPAGE + 0x0400)
#define BOOTSTK		(SYSPAGE + 0x0800)
#define EXCSTK		(SYSPAGE + 0x1000)
#define INTSTK		(SYSPAGE + 0x3000)
#define BOOT_PGD	(SYSPAGE + 0x5000)
#define BOOT_PTE0	(SYSPAGE + 0xB000)
#define BOOT_PTE1	(SYSPAGE + 0xC000)

#if 0
#define BOOT_PGD_PHYS	0x4000
#define BOOT_PTE0_PHYS	0x8000
#define BOOT_PTE1_PHYS	0x9000
#endif

#define EXCSTKSZ	0x2000
#define INTSTKSZ	0x2000
#define BOOTSTKSZ	0x0800

#define EXCSTKTOP	(EXCSTK  + EXCSTKSZ)
#define INTSTKTOP	(INTSTK  + INTSTKSZ)
#define BOOTSTKTOP	(BOOTSTK + BOOTSTKSZ)

#ifdef CONFIG_MMU
#define SYSPAGESZ	0xA000
#else
#define SYSPAGESZ	0x5000
#endif


#endif /* !_MIPS_SYSPAGE_H */

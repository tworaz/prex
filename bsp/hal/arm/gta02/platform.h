/*-
 * Copyright (c) 2009, Sean Chiang<n2i911@gmail.com>
 * Copyright (c) 2010, Peter Tworek<tworaz666@gmail.com>
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

#ifndef _GTA02_PLATFORM_H
#define _GTA02_PLATFORM_H

/* vector relocation target */
#define ARM_VECTORS	ARM_VECTORS_LOW

/* base address of user stack */
#define USTACK_BASE	(0 + PAGE_SIZE)

/* number of interrupt vectors */
#define NIRQS		32

#ifdef CONFIG_MMU
#define CM_IOMEM_BASE	0xc0000000
#else
#define CM_IOMEM_BASE	0
#endif

#define WT_BASE		(CM_IOMEM_BASE + 0x53000000)
#define TIMER_BASE	(CM_IOMEM_BASE + 0x51000000)
#define ICU_BASE	(CM_IOMEM_BASE + 0x4A000000)
#define UART_BASE	(CM_IOMEM_BASE + 0x50008000)

#endif /* !_GTA02_PLATFORM_H */

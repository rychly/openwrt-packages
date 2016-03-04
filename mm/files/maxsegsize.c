/* ====================================================================
 * Copyright (c) 1999-2006 Ralf S. Engelschall <rse@engelschall.com>
 * Copyright (c) 1999-2006 The OSSP Project <http://www.ossp.org/>
 * Copyright (c) 2016 Marek Rychly <marek.rychly@gmail.com>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. All advertising materials mentioning features or use of this
 *    software must display the following acknowledgment:
 *    "This product includes software developed by
 *     Ralf S. Engelschall <rse@engelschall.com>."
 *
 * 4. Redistributions of any form whatsoever must retain the following
 *    acknowledgment:
 *    "This product includes software developed by
 *     Ralf S. Engelschall <rse@engelschall.com>."
 *
 * THIS SOFTWARE IS PROVIDED BY RALF S. ENGELSCHALL ``AS IS'' AND ANY
 * EXPRESSED OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL RALF S. ENGELSCHALL OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 * ====================================================================
 */

/*
**
**  maxsegsize.c -- Find maximum possible allocation size
**  (extracted from aclocal.m4 in ftp://ftp.ossp.org/pkg/lib/mm/mm-1.4.2.tar.gz)
**
*/

/*
 * There are different shared memory allocation methods:
 * MM_SHMT_BEOS: BeOS areas
 * MM_SHMT_MMFILE: Classical mmap() on temporary file
 *  Works on Linux versions.
 * MM_SHMT_MMPOSX: mmap() via POSIX.1 shm_open() on temporary file
 * MM_SHMT_MMZERO: SVR4-style mmap() on /dev/zero
 *  Not allowed up to and included to Linux 2.2.
 *  Should be supported in Linux 2.3, but still hangs there.
 *  Will be supported from Linux 2.4 on the first time really.
 * MM_SHMT_IPCSHM: SysV IPC shmget()
 *  Supported under Linux 2.0 and upwards. Works out of the
 *  box on Linux 2.2, but requires an explicitly mounted shm
 *  filesystem under 2.3 only. The requirement for the shm
 *  filesystem should be gone with Linux 2.4.
 * MM_SHMT_MMANON: 4.4BSD-style mmap() via MAP_ANON
 *  Not allowed up to and included to Linux 2.2.
 *  Should be supported in Linux 2.3, but still hangs there.
 *  Will be supported from Linux 2.4 on the first time really.
 *
 * For MM_SHMT_MM* compile with flag -DTEST_MMAP.
 * For MM_SHMT_IPCSHM compile with flag -DTEST_SHMGET.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/file.h>
#include <sys/types.h>
#include <sys/stat.h>
#ifdef TEST_MMAP
#include <sys/mman.h>
#endif
#ifdef TEST_SHMGET
#ifdef MM_OS_SUNOS
#define KERNEL 1
#endif
#ifdef MM_OS_BS2000
#define _KMEMUSER
#endif
#include <sys/ipc.h>
#include <sys/shm.h>
#ifdef MM_OS_SUNOS
#undef KERNEL
#endif
#ifdef MM_OS_BS2000
#undef _KMEMUSER
#endif
#if !defined(SHM_R)
#define SHM_R 0400
#endif
#if !defined(SHM_W)
#define SHM_W 0200
#endif
#endif
#if !defined(MAP_FAILED)
#define MAP_FAILED ((void *)(-1))
#endif
#ifdef MM_OS_BEOS
#include <kernel/OS.h>
#endif

int testit(int size)
{
	int fd;
	void *segment;
#ifdef TEST_MMAP
	char file[] = "./ac_test.tmp";
	unlink(file);
	if ((fd = open(file, O_RDWR|O_CREAT, S_IRUSR|S_IWUSR)) == -1)
		return 0;
	if (ftruncate(fd, size) == -1)
		return 0;
	if ((segment = (void *)mmap(NULL, size, PROT_READ|PROT_WRITE,
		                        MAP_SHARED, fd, 0)) == (void *)MAP_FAILED) {
		close(fd);
		return 0;
	}
	munmap((caddr_t)segment, size);
	close(fd);
	unlink(file);
#endif
#ifdef TEST_SHMGET
	if ((fd = shmget(IPC_PRIVATE, size, SHM_R|SHM_W|IPC_CREAT)) == -1)
		return 0;
	if ((segment = (void *)shmat(fd, NULL, 0)) == ((void *)-1)) {
		shmctl(fd, IPC_RMID, NULL);
		return 0;
	}
	shmdt(segment);
	shmctl(fd, IPC_RMID, NULL);
#endif
#ifdef TEST_BEOS
	area_id id;
	id = create_area("mm_test", (void*)&segment, B_ANY_ADDRESS, size,
		B_LAZY_LOCK, B_READ_AREA|B_WRITE_AREA);
	if (id < 0)
		return 0;
	delete_area(id);
#endif
	return 1;
}

#define ABS(n) ((n) >= 0 ? (n) : (-(n)))

int main(int argc, char *argv[])
{
	int t, m, b;
	int d;
	int rc;
	FILE *f;

	/*
	 * Find maximum possible allocation size by performing a
	 * binary search starting with a search space between 0 and
	 * 64MB of memory.
	 */
	t = 1024*1024*64 /* = 67108864 */;
	if (testit(t))
		m = t;
	else {
		m = 1024*1024*32;
		b = 0;
		for (;;) {
			/* fprintf(stderr, "t=%d, m=%d, b=%d\n", t, m, b); */
			rc = testit(m);
			if (rc) {
				d = ((t-m)/2);
				b = m;
			}
			else {
				d = -((m-b)/2);
				t = m;
			}
			if (ABS(d) < 1024*1) {
				if (!rc)
					m = b;
				break;
			}
			if (m < 1024*8)
				break;
			m += d;
		}
		if (m < 1024*8)
			m = 0;
	}
	fprintf(stdout, "%d\n", m);
	exit(0);
}

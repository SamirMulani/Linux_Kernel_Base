/*
 * ******************************************************************************
 * This program is part of the source code provided with "Linux Kernel Programming"
 * (C) 2022  Samir Mulani
 *
 * Git repository:
 * https://github.com/SamirMulani/Linux_Kernel_Base
 * ******************************************************************************
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 *
 */

#include <sys/klog.h>
#include <stdio.h>
#include <stdlib.h>


int main(void)
{
	int len, level;
	char *buf;
	int last = '\n';
	int in = 0;

	len = klogctl(10, NULL, 0);	/* read ring buffer size */
	if (len < 16 * 1024)
		len = 16 * 1024;
	if (len > 16 * 1024 * 1024)
		len = 16 * 1024 * 1024;

	buf = malloc(len);
	if (!buf) {
		printf("\nCannot allocate memory\n");
		return 0;
	}

	len = klogctl(3, buf, len);	/* read ring buffer */
	if (len < 0) {
		printf("\nCannot Read the buffer\n");
		free(buf);
		return 0;
	}
	if (len == 0)
		return 0;



	/* Skip <#> at the start of lines */
	while (1) {
		if (last == '\n' && buf[in] == '<') {
			in += 3;
			if (in >= len)
				break;
		}
		last = buf[in];
		putchar(last);
		in++;
		if (in >= len)
			break;
	}
	/* Make sure we end with a newline */
	if (last != '\n')
		putchar('\n');

	free(buf);

	return 0;
}

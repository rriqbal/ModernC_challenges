#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

#define TYPE unsigned
#define SOE sizeof(TYPE)
#define NUM(a) ((a) * SOE)
#include "array.h"

#define MERGE merge_swap_buffer

void
merge_2n(unsigned len1, unsigned arr1[static 1],
		unsigned len2, unsigned arr2[static 1])
{
	unsigned* new = malloc(NUM(len1 + len2));
	unsigned i = 0;
	unsigned j = 0;
	unsigned k = 0;
	while (i != len1 &&
			j != len2)
	{
		if (arr2[j] < arr1[i])
		{
			new[k] = arr2[j];
			++j;
		}
		else
		{
			new[k] = arr1[i];
			++i;
		}
		++k;
	}

	while (i != len1)
	{
		new[k] = arr1[i];
		++k;
	}

	while (j != len2)
	{
		new[k] = arr2[j];
		++k;
	}

	memcpy(arr1, new, len1 + len2);
}

/* all algorithms below are O(len1 * len2) time complexity */

typedef struct map map;
struct map
{
	unsigned* i;
	unsigned* j;
	unsigned len;
};
void
merge_map(unsigned len1, unsigned arr1[static 1],
		unsigned len2, unsigned arr2[static 1])
{
	unsigned* i = &arr1[0];
	unsigned* j = &arr2[0];
	map* m = malloc((len1 + len2) * sizeof(map));
	unsigned k = 0;
	unsigned* s = 0;

	while (i != &arr1[len1] &&
			j != &arr2[len2])
	{
		if (*j < *i)
		{
			if (!s)
			{
				m[k].i = i;
				m[k].j = j;
				s = j;
			}
			++j;
		}
		else
		{
			if (s)
			{
				m[k].len = j - s;
				++k;
				s = 0;
			}
			++i;
		}
	}

	if (s)
	{
		m[k].len = j - s;
		++k;
	}

	for (unsigned n = 0;
			n < k;
			++n)
	{
		unsigned* i = m[n].i;
		unsigned* j = m[n].j;
		unsigned len = m[n].len;
		unsigned* save = malloc(NUM(len));
		memcpy(save, j, NUM(len));
		memmove(i + len, i, NUM(len));
		memcpy(i, save, NUM(len));
	}
}


void
merge_swap_buffer(unsigned len1, unsigned* arr1,
		unsigned len2, unsigned* arr2)
{
	unsigned* i = &arr1[0];
	unsigned* j = &arr2[0];
	unsigned* sw = 0;

	while ((j - i) &&
			j != &arr2[len2])
	{
		if (*i > *j)
		{
			if (!sw)
				sw = j;
			++j;
		}
		else if (sw)
		{
			unsigned swl = j - sw;
			unsigned* save = malloc(NUM(swl));
			memcpy(save, sw, NUM(swl));
			memmove(i + swl, i, NUM(sw - i));
			memcpy(i, save, NUM(swl));

			i += swl;
			sw = 0;
			free(save);
		}
		else
			++i;
	}

	if (sw)
	{
		unsigned swl = j - sw;
		unsigned* save = malloc(NUM(swl));
		memcpy(save, sw, NUM(swl));
		memmove(i + swl, i, NUM(sw - i));
		memcpy(i, save, NUM(swl));

		free(save);
	}
}

void
merge_inplace(unsigned len1, unsigned* arr1,
		unsigned len2, unsigned* arr2)
{
	unsigned* i = &arr1[0];
	unsigned* j = &arr2[0];

	while ((j - i) &&
			j != &arr2[len2])
	{
		if (*i > *j)
		{
			unsigned save = *j;
			memmove(i + 1, i, NUM(j - i));
			*i = save;
			++j;
		}
		++i;
	}
}

void
msortu(unsigned len, unsigned* arr)
{
	for (unsigned i = 2;
			i <= pow(2, 1 + log2(len));
			i *= 2)
	{
		for (unsigned j = 0;
				j < len;
				j += i)
		{
			unsigned a = j;
			unsigned al = i / 2;
			unsigned b = j + (i / 2);
			unsigned bl = al;
			
			if (a + al == len)
				break;

			if (b + bl >= len)
				bl = len - b;

			MERGE(al, &arr[a],
					bl, &arr[b]);
		}
	}
}

typedef struct lifo lifo;
struct lifo
{
	unsigned i;
	unsigned sz;
	unsigned m;
};

void
msorts(unsigned len, unsigned* arr)
{
	lifo* s = malloc(((log(len) + 1) - 1)*sizeof(lifo));
	
	/* seed call */
	s[0].i = 0;
	s[0].sz = len;
	s[0].m = 0;
	/**/

	unsigned t = 0;
	while (1)
	{
		if (s[t].sz <= 2)
		{
			if (s[t].sz == 2)
			{
				unsigned i = s[t].i;
				MERGE(1, &arr[i], 
						1, &arr[i + 1]);
			}
			if (!t)
				break;
			--t;
			++s[t].m;
		}
		else
		{
			if (s[t].m == 0)
			{
				lifo* p = &s[t];
				++t;
				s[t].i = p->i;
				s[t].sz = p->sz / 2;
				s[t].m = 0;
			}
			else if (s[t].m == 1)
			{
				lifo* p = &s[t];
				++t;
				s[t].i = p->i + (p->sz / 2);
				s[t].sz = p->sz - (p->sz / 2);
				s[t].m = 0;
			}
			else /* if (s[t].m == 2) TODO optimized? */
			{
				lifo* p = &s[t];
				MERGE(p->sz / 2, 
						&arr[s[t].i],
						p->sz - (p->sz / 2), 
						&arr[s[t].i + (p->sz / 2)]);
				if (!t)
					break;
				--t;
				++s[t].m;
			}
		}
	}
}

void
msortr(unsigned len, unsigned* arr)
{
	if (len > 1)
	{
		unsigned len1 = len/2;
		unsigned len2 = len - len1;

		msortr(len1, &arr[0]);
		msortr(len2, &arr[len1]);

		MERGE(len1, &arr[0],
				len2, &arr[len1]);
	}
}


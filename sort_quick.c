#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <assert.h>

#include "array.h"

#define TYPE unsigned
#define SOE sizeof(TYPE)
#define NUM(a) ((a) * SOE)

typedef struct lifo lifo;
struct lifo
{
	unsigned i;
	unsigned len; 
};
void
not_qsort_inplace(unsigned len, unsigned* arr)
{
	lifo* s = malloc((len - 1) * 
			sizeof(lifo));

	/* seed call */
	s[0].i = 0;
	s[0].len = len;
	/**/

	unsigned t = 0;
	while (1)
	{
		if (s[t].len < 3)
		{
			if (s[t].len == 2)
			{
				if (arr[s[t].i] > arr[s[t].i + 1])
					SWAP(arr[s[t].i], arr[s[t].i + 1]);
			}

			if (!t) /* we split in two, so this works */
				break;
			else
			{
				--t;
				continue;
			}
		}

		unsigned* ac = &arr[s[t].i];

		unsigned i = 0;
		unsigned mid = s[t].len / 2;
		unsigned p = ac[mid]; /* pivot */
		unsigned m = mid + 1;
		bool of = 0; /* overflow of hi */
		unsigned om; /* old m before overflow */
		unsigned j = s[t].len - 1;
		while (i <= j)
		{
			if (!of &&
					j < m)
			{
				om = m;
				m = j = mid - 1;
				of = 1;
			} 
			else if (i == mid)
			{
				i = m;
			}

			if (ac[i] > p)
			{
				if (!j) break;
				SWAP(ac[i], ac[j]);
				--j; 
			}
			else if (ac[i] == p)
			{
				if (i < m)
				{
					if (of)
					{
						SWAP3(ac[i], ac[m], ac[j]);
						--m;
						--j;
						if (j == UINT_MAX) break;
					}
					else
					{
						SWAP(ac[i], ac[m]);
						++m;
					}
				}
				else
				{
					SWAP(ac[i], ac[m]);
					++i;
					++m;
				}
			}
			else
				++i;
		}

		/* TODO compare to memmove memset */
		if (!of)
		{
			/* began with mid > 0, so 
			 * i > mid because !of, so i > 0 */

			for (unsigned k = i - 1;
					k >= m &&
					(i - 1) - k < m - mid;
					--k)
			{
				ac[mid + ((i - 1) - k)] = ac[k];
				ac[k] = p; /* TODO optimized out? */
			}

			unsigned len = s[t].len;
			s[t].len = i - (m - mid);
			++t;
			s[t].i = i;
			s[t].len = len - s[t].i;
		}
		else
		{
			for (unsigned k = j + 1; /* TODO: add counter */
					m != UINT_MAX && /* all pivots edge case TODO: move out */
					k <= m &&
					k - (j + 1) < om - (m + 1);
					++k)
			{
				ac[(om - 1) - (k - (j + 1))] = ac[k];
				ac[k] = p;
			}

			unsigned len = s[t].len;
			s[t].len = i;
			++t;
			s[t].i = i + (om - (m + 1));
			s[t].len = len - s[t].i;
		}
	}
}

void
not_qsort_inplaceu(unsigned len, unsigned* arr)
{
	lifo* s = malloc((len - 1) * 
			sizeof(lifo));

	/* seed call */
	s[0].i = 0; /* TODO */
	s[0].len = len;
	/**/

	unsigned t = 0;
	while (1)
	{
		if (s[t].len < 2)
		{
			if (!t) /* we split in two, so this works */
				break;
			else
			{
				--t;
				continue;
			}
		}

		unsigned i = 0;
		unsigned p = arr[s[t].len - 1];
		unsigned j = s[t].len - 2;
		while (i <= j)
		{
			if (arr[i] > p)
			{
				if (!j) break;
				SWAP(arr[i], arr[j]);
				--j;
			}
			else
				++i;
		}
		arr[s[t].len - 1] = arr[i];
		arr[i] = p;

		unsigned save = s[t].len;
		s[t].len = i;
		++t;
		s[t].i += i + 1;
		s[t].len = save - (i + 1);
	}
}

void
not_qsort(unsigned len, unsigned* arr)
{
	lifo* s = malloc((len - 1) * sizeof(lifo));

	/* seed call */
	s[0].i = &arr[0];
	s[0].len = len;
	/**/

	unsigned t = 0;
	unsigned* l = 0;
	unsigned* r = 0;
	while (1)
	{
		if (s[t].len < 2)
		{
			if (!t)
				break;
			else
			{
				--t;
				continue;
			}
		}

		unsigned i = 0;
		l = realloc(l, NUM(s[t].len - 1));
		unsigned j = 0;
		r = realloc(r, NUM(s[t].len - 1));
		unsigned p = arr[s[t].len - 1];
		unsigned k = 1;
		while (i < s[t].len - 1)
		{
			if (arr[i] < p)
			{
				l[j] = arr[i];
				++j;
			}
			else if (arr[i] == p)
			{
				++k;
			}
			else
			{
				/* TODO does x get optimized to this? */
				r[i - (j + k)] = arr[i];
			}
			++i;
		}

		unsigned n = 0;
		memcpy(&arr[n], l, NUM(j));
		n += j;
		memset(&arr[n], p, NUM(k));
		n += k;
		memcpy(&arr[n], r, NUM(i - n));

		unsigned* save = s[t].i;
		s[t].i += j + k;
		s[t].len -= j + k;
		++t;
		s[t].i = save;
		s[t].len = j;
	}


	free(l);
	free(r);
}

void
not_qsortu(unsigned len, unsigned* arr)
{
	lifo* s = malloc((len - 1) * sizeof(lifo));

	/* seed call */
	s[0].i = &arr[0];
	s[0].len = len;
	/**/

	unsigned t = 0;
	unsigned* l = 0;
	unsigned* r = 0;
	while (1)
	{
		if (s[t].len < 2)
		{
			if (!t)
				break;
			else
			{
				--t;
				continue;
			}
		}

		unsigned p = arr[s[t].len - 1];
		unsigned i = 0;
		l = realloc(l, NUM(s[t].len - 1));
		unsigned j = 0;
		r = realloc(r, NUM(s[t].len - 1));
		while (i < s[t].len - 1)
		{
			if (arr[i] < p)
			{
				l[j] = s[t].i[i];
				++j;
			}
			else
			{
				/* TODO does x get optimized to this? */
				r[i - j] = s[t].i[i];
			}
			++i;
		}

		memcpy(&arr[0], l, NUM(j));
		arr[j] = p;
		memcpy(&arr[j + 1], r, NUM(i - j));

		unsigned save = s[t].len;
		s[t].len = j;
		++t;
		s[t].i = &s[t - 1].i[j + 1];
		s[t].len = save - (j + 1);
	}

	free(l);
	free(r);
}

void
not_qsortr(unsigned len, unsigned* arr)
{
	if (len < 2)
		return;

	unsigned* piv = &arr[len - 1];
	unsigned* i = &arr[0];
	unsigned* hi = &arr[len - 2];

	while (i <= hi)
	{
		if (*i > *piv)
		{
			SWAP(i, hi);

			if (hi == arr) /* prevent UB */
				break;
			--hi;
		}
		else
			++i;
	}
	SWAP(i, piv);

	not_qsortr(i - arr, 
			&arr[0]);
	not_qsortr(len - (i - arr + 1), 
			&arr[i + 1]);
}

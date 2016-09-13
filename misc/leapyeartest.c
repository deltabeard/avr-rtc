#include <stdio.h>

/**
 * Full implementation for finding leap years.
 *
 * \return	0 if leap year.
 */
char isleap(int year)
{
	if(!(year%100))
	{
		return (char)(year%400);
	}
	else
	{
		return (char)(year%4);
	}
}

/**
 * Partial implementation for finding leap leap years.
 *
 * \return	0 if leap year.
 */
char isleappartial(int year)
{
	return (char)(year%4);
}

/**
 * Print out the differences between a full and partial leap year checker.
 *
 * \return	0 always.
 */
int main(void)
{
	for(int year = 0; year < 2500; year++)
	{
		if(isleap(year) != isleappartial(year))
		{
			printf("Issue with year %d\n", year);
		}
	//	printf("Year: %d\tLeap: %s\tPartial: %s\n", year,
	//			isleap(year) == 0 ? "Yes" : "No",
	//			isleappartial(year) == 0 ? "Yes" : "No");
	}

	return 0;
}

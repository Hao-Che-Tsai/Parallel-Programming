#define _USE_MATH_DEFINES
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <stdio.h>

int	NowYear;		// 2023 - 2028
int	NowMonth;		// 0 - 11
float	NowPrecip;		// inches of rain per month
float	NowTemp;		// temperature this month
float	NowHeight;		// rye grass height in inches
int	NowNumRabbits;		// number of rabbits in the current population
int NowNumRabbitKiller;       //number of rabbit killer
int NowNumVillager;       //number of villager

const float RYEGRASS_GROWS_PER_MONTH = 20.0;
const float ONE_RABBITS_EATS_PER_MONTH = 1.0;

const float AVG_PRECIP_PER_MONTH = 12.0;	// average
const float AMP_PRECIP_PER_MONTH = 4.0;	// plus or minus
const float RANDOM_PRECIP = 2.0;	// plus or minus noise

const float AVG_TEMP = 60.0;	// average
const float AMP_TEMP = 20.0;	// plus or minus
const float RANDOM_TEMP = 10.0;	// plus or minus noise

const float MIDTEMP = 60.0;
const float MIDPRECIP = 14.0;


float Ranf(float low, float high)
{
	float r = (float)rand();              // 0 - RAND_MAX

	return(low + r * (high - low) / (float)RAND_MAX);
}

float Sqr(float x)
{
	return x * x;
}


void Rabbits() 
{
	while (NowYear < 2029)
	{
		int nextNumRabbits = NowNumRabbits;
		int carryingCapacity = (int)(NowHeight);

		// compute a temporary next-value for this quantity
		// based on the current state of the simulation:
		if (nextNumRabbits < carryingCapacity)
			nextNumRabbits++;
		else
			if (nextNumRabbits > carryingCapacity)
				nextNumRabbits--;

		if (nextNumRabbits < 0)
			nextNumRabbits = 0;
		// DoneComputing barrier:
	#pragma omp barrier

		NowNumRabbits = nextNumRabbits;

		// DoneAssigning barrier:
	#pragma omp barrier


// DonePrinting barrier:
	#pragma omp barrier

	}
}

void RyeGrass() 
{
	while (NowYear < 2029)
	{
		float tempFactor = exp(-Sqr((NowTemp - MIDTEMP) / 10.));
		float precipFactor = exp(-Sqr((NowPrecip - MIDPRECIP) / 10.));

		// compute a temporary next-value for this quantity
		// based on the current state of the simulation:
		float nextHeight = NowHeight;
		nextHeight += tempFactor * precipFactor * RYEGRASS_GROWS_PER_MONTH;
		nextHeight -= (float)NowNumRabbits * ONE_RABBITS_EATS_PER_MONTH;
		if (nextHeight < 0.) nextHeight = 0.;

		// DoneComputing barrier:
	#pragma omp barrier
		NowHeight = nextHeight;

		// DoneAssigning barrier:
	#pragma omp barrier

// DonePrinting barrier:
	#pragma omp barrier

	}
}


void Watcher()
{
	while (NowYear < 2029)
	{
		// compute a temporary next-value for this quantity
		// based on the current state of the simulation:

		// DoneComputing barrier:
#pragma omp barrier


// DoneAssigning barrier:
#pragma omp barrier
		float C = (5. / 9.) * (NowTemp - 32);
		float CM = NowHeight * 2.54;

		printf("%6.2f,%6.2f,%6.2f,%d,%d,%d\n", C, NowPrecip, CM, NowNumRabbits, NowNumRabbitKiller, NowNumVillager);

		float ang = (30. * (float)NowMonth + 15.) * (M_PI / 180.);

		float temp = AVG_TEMP - AMP_TEMP * cos(ang);
		NowTemp = temp + Ranf(-RANDOM_TEMP, RANDOM_TEMP);

		float precip = AVG_PRECIP_PER_MONTH + AMP_PRECIP_PER_MONTH * sin(ang);
		NowPrecip = precip + Ranf(-RANDOM_PRECIP, RANDOM_PRECIP);

		if (NowPrecip < 0.)
			NowPrecip = 0.;

		if (++NowMonth == 12)
		{
			NowYear++;
			NowMonth = 0;
		}
		// DonePrinting barrier:
#pragma omp barrier

	}
}


void MyAgent() 
{
	while (NowYear < 2029)
	{
		// compute a temporary next-value for this quantity
		// based on the current state of the simulation:
		int nextNumRabbitKiller = NowNumRabbitKiller;
		int nextNumVillager = NowNumVillager;
		int carringNumVillager = NowNumRabbits;
		int i = floor(NowNumVillager / 6);

		if (i < 1)
			nextNumRabbitKiller = 1;
		else
			nextNumRabbitKiller = i;

		if (NowNumRabbits >= NowNumRabbitKiller)
			nextNumVillager = nextNumVillager + 3;
		else
			nextNumVillager = nextNumVillager - 1;

		if (nextNumVillager < 0)
		{
			nextNumRabbitKiller = 0;
			nextNumVillager = 0;
		}

			// DoneComputing barrier:
			#pragma omp barrier
			NowNumVillager = nextNumVillager;
			NowNumRabbitKiller = nextNumRabbitKiller;

			// DoneAssigning barrier:
			#pragma omp barrier


			// DonePrinting barrier:
			#pragma omp barrier

	}
}


int main() {
	float x = Ranf(-1.f, 1.f);
	// starting date and time:
	NowMonth = 0;
	NowYear = 2023;

	// starting state (feel free to change this if you want):
	NowNumRabbits = 6;
	NowHeight = 5.;
	NowNumVillager = 20; 


	omp_set_num_threads(4);	// same as # of sections
#pragma omp parallel sections
	{
	#pragma omp section
		{
			Rabbits();
		}
	
	#pragma omp section
		{
			RyeGrass();
		}

	#pragma omp section
		{
			Watcher();
		}

	#pragma omp section
		{
			MyAgent();	// your own
		}
	}       // implied barrier -- all functions must return in order
	// to allow any of them to get past here

}
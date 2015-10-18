#include <cstdio>
#include <random>
#include <time.h>
#include <unistd.h>
#include <functional>
#include <thread>
#include "randUtils.h"
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>

using namespace std;

class RandStreamToken
{
	static int tokenCount;
	static const gsl_rng_type *type;
	
	const int tokenID;
	
	RandStreamToken() : tokenID(tokenCount++), rng(gsl_rng_alloc(type))
	{
		hash<thread::id> h;
		int seed = time(NULL) + getpid() + h(this_thread::get_id()) + tokenID;
		gsl_rng_set(rng, seed);
		gsl_rng_uniform(rng); // throw out first value to avoid clustering
	}
	
	friend const RandStreamToken *getNewRandStreamToken();

	public:
	
		gsl_rng *rng;
	
		~RandStreamToken()
		{
			gsl_rng_free(rng);
		}
	
};

int RandStreamToken::tokenCount = 1;
const gsl_rng_type * RandStreamToken::type = gsl_rng_ranlxs0;

const RandStreamToken *RST_DEFAULT = getNewRandStreamToken();


const RandStreamToken *getNewRandStreamToken()
{
	return new RandStreamToken();
}

void releaseToken(const RandStreamToken *token)
{
	delete token;
}



void setSeed(double d, const RandStreamToken *token)
{
	fprintf(stderr, "ERROR: setSeed(double) not supported in this implementation. %s: %d\n", __FILE__, __LINE__);
	exit(1);
}
	

void setSeed(int n, const RandStreamToken *token)
{
	gsl_rng_set(token->rng, (unsigned long)n);
}

// returns a uniformly distributed random value on [0,1)
double urand(const RandStreamToken *token)
{
	return gsl_rng_uniform(token->rng);
}

// returns a uniformly distributed random value on [min,max)
double urand(double min, double max, const RandStreamToken *token)
{
	return urand() * (max-min) + min;
}

// returns a uniformly distributed random integer on [min,max]
int urand(int min, int max, const RandStreamToken *token)
{
	return min + gsl_rng_uniform_int(token->rng, max-min + 1);
}

// returns a random variable from a unit normal distribution
double nrand(const RandStreamToken *token)
{
	return gsl_ran_gaussian(token->rng, 1.0);
}

// returns a normally distributed random value with the given mean and variance
double nrand(double mean, double var, const RandStreamToken *token)
{
	return gsl_ran_gaussian(token->rng, sqrt(var)) + mean;
}


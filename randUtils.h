#ifndef RAND_UTILS_H
#define RAND_UTILS_H


class RandStreamToken;


const RandStreamToken *getNewRandStreamToken();

extern const RandStreamToken *RST_DEFAULT;

void releaseToken(const RandStreamToken *token);

void setSeed(double d, const RandStreamToken *token = RST_DEFAULT);

void setSeed(int n, const RandStreamToken *token = RST_DEFAULT);

// returns a uniformly distributed random value on [0,1)
double urand(const RandStreamToken *token = RST_DEFAULT);

// returns a uniformly distributed random value on [min,max)
double urand(double min, double max, const RandStreamToken *token = RST_DEFAULT);

// returns a uniformly distributed random integer on [min,max]
int urand(int min, int max, const RandStreamToken *token = RST_DEFAULT);

// returns a random variable from a unit normal distribution
double nrand(const RandStreamToken *token = RST_DEFAULT);

// returns a normally distributed random value with the given mean and variance
double nrand(double mean, double var, const RandStreamToken *token = RST_DEFAULT);


#endif
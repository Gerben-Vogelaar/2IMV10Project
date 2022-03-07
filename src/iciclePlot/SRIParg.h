#pragma once
/*
* 	W		= width of the screen [0,2];
	h		= height of each layer;
	gamma	= horizontal gap between consecutive children in a layer;
	rho		= space-reclaiming parameter [0,1] (0 no reclaiming 1 max reclaiming) ;
*/

struct SRIP1_arg {
	float W;
	float h;
	float gamma;
	float rho;

	void setW(float W_new) {
		W = W_new;
	}

	void seth(float h_new) {
		h = h_new;
	}

	void setGamma(float gamma_new) {
		gamma = gamma_new;
	}

	void setRho(float rho_new) {
		rho = rho_new;
	}
};

/* 	W		= width of the screen [0,2];
	h		= height of each layer;
	gamma	= horizontal gap between consecutive children in a layer;
	rho		= space-reclaiming parameter [0,1] (0 no reclaiming 1 max reclaiming);
	epsilon = maximum width of a layer [0, W]
	sigma	= sticky node shrinking factor [0,1] (shrinking each node by a factor sigma for lambda layers)
	lambda	= depth span of a sticky node
*/
struct SRIP2_arg {
	float W;
	float h;
	float gamma;
	float rho;

	float epsilon;
	float sigma;
	float lambda;

	void setW(float W_new) {
		W = W_new;
	}

	void seth(float h_new) {
		h = h_new;
	}

	void setGamma(float gamma_new) {
		gamma = gamma_new;
	}

	void setRho(float rho_new) {
		rho = rho_new;
	}

	void setEpsilon(float epsilon_new) {
		epsilon = epsilon_new;
	}

	void setSigma(float sigma_new) {
		sigma = sigma_new;
	}

	void setLambda(float lambda_new) {
		lambda = lambda_new;
	}
};
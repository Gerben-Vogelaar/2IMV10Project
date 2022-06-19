#pragma once
#include <stdexcept>
#include <iostream>
#include <string>


enum class AlgorithmType {SRIP1, SRIP2};

constexpr const char* AlgorithmTypeToString(AlgorithmType e) throw()
{
	switch (e)
	{
	case AlgorithmType::SRIP1: return "SRIP1";
	case AlgorithmType::SRIP2: return "SRIP2";

	default: throw std::invalid_argument("Unimplemented item");
	}
};

const AlgorithmType StringToAlgorithmType(std::string e);

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

struct SRIP_args {
	
	AlgorithmType algo = AlgorithmType::SRIP1;

	float W;
	float h;
	float gamma;
	float rho;

	float epsilon;
	float sigma;
	float lambda;

	SRIP1_arg getSRIP1arg() {
		SRIP1_arg args1;
		args1.setGamma(gamma);
		args1.seth(h);
		args1.setRho(rho);
		args1.setW(W);

		return args1;
	}
	
	SRIP2_arg getSRIP2arg() {
		SRIP2_arg args2;
		args2.setGamma(gamma);
		args2.seth(h);
		args2.setRho(rho);
		args2.setW(W);
		args2.setEpsilon(epsilon);
		args2.setSigma(sigma);
		args2.setLambda(lambda);

		return args2;
	}

	void setAlgoType(AlgorithmType type) {
		algo = type;
	}

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

	void printArgs() {
		std::cout << "gamma: " << gamma << ", h: " << h << ", rho: " << rho << ", W: " << W << ", epsilon: " << epsilon << ", sigma: " << sigma << ", lambda: " << lambda << std::endl;
	}
};
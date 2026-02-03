// Tigran Tumanyan, Arshia Zakeri Rad

#include <stdio.h>
#include <time.h> 
#include <stdlib.h> 
#include <math.h>

//I got a nan error once, gotta see what's up with that 	

int optimal_index_finder(float* probabilities){
	float max = 0; 
	int max_index;
	for (int i = 0; i < 10; i++){
		if (probabilities[i] >= max){
			max = probabilities[i]; 
			max_index = i; 
		}
	}

	return max_index; 
}

int bernoulli(float p){
	float random = (float)rand() / RAND_MAX; 
	//printf("this is the random num - %.1f, this is the p being compared to - %.1f\n", random, p); 
	return random < p; 
}

void individual_round(float* probabilities){
	int c = 2; //coefficient, might change value later depending on the demands of the question
	int* n_t = (int*) calloc(10, 1); //number of times each action has been chosen 
	float* q_t = (float*) calloc(10,1); 
	float* a_t = (float*) calloc(10,1); 

	for (int i = 0; i < 10; i++){
		if (bernoulli(probabilities[i]) == 0){
			q_t[i] = 0; 
		}
		else if (bernoulli(probabilities[i]) == 1){
			q_t[i] = 1; 
		}
		n_t[i]++;  
	}

	for (int i = 0; i < 4990; i++){
		a_t[i%10] = q_t[i%10] + c*(sqrt(log((double) i)/n_t[i%10])); 
		int action = optimal_index_finder(a_t); 
		n_t[action]++; 
		int reward = bernoulli(probabilities[action]); 
		q_t[action] += 1/i*(reward - q_t[action]); 
	}

	for (int i = 0; i < 10; i++){
		printf("Action %d was tried %d times\n", i, n_t[i]); 
	}
}


void array_maker(float* probablities){
	srand((unsigned int) time(NULL));	

	for (int i = 0; i < 10; i++){
		probablities[i] = (float) (rand() % 10) / 10.0f; 
	}
}

void question1(){
	float* probabilities; 
	probabilities = (float*) calloc(10, 1); 
	array_maker(probabilities); 
	individual_round(probabilities); 


	free(probabilities);
}

int main(){
	question1(); 
	return 0;
}

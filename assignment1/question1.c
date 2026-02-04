// Tigran Tumanyan, Arshia Zakeri Rad

#include <stdio.h>
#include <time.h> 
#include <stdlib.h> 
#include <math.h>
#include <unistd.h>

int optimal_index_finder(float* array){
	float max = 0; 
	int max_index = 0;
	for (int i = 0; i < 10; i++){
		if (array[i] > max){
			max = array[i]; 
			max_index = i; 
		}
	}

	return max_index; 
}

int bernoulli(float p){
	float random = (float)rand() / RAND_MAX; //make a random number. 
	return random < p; //with probability p, this equality will either be true or false. 
}

void individual_round(float* probabilities){
	int optimal_action_index = optimal_index_finder(probabilities); 
	int optimality_count = 0; //tracking number of times that optimal action was taken
	int c = 2; //coefficient, might change value later depending on the demands of the question
	int* n_t = (int*) calloc(10, sizeof(int)); //number of times each action has been chosen 
	float* q_t = (float*) calloc(10, sizeof(float));  
	float* a_t = (float*) calloc(10, sizeof(float)); 
	/*
	each of these are size 10 arrays, and they each represent q (value), 
	n (number of times action has been taken), and a (the ucb score for that arm). 
	*\


	/*
	In the first ten iterations, each arm is assumed to be maximizing, therefore, we try all the 10 arms 
	in our initial exploration. 
	*/
	for (int i = 0; i < 10; i++){
		int reward = bernoulli(probabilities[i]); 
		if (reward == 0){
			q_t[i] = 0; 
		}
		else if (reward == 1){
			q_t[i] = 1; 
		}
		n_t[i]++;  
	}

	for (int i = 0; i < 4990; i++){
		a_t[i%10] = q_t[i%10] + c*(sqrt(log((double) i)/n_t[i%10])); 
		int action = optimal_index_finder(a_t); 
		n_t[action]++; 
		int reward = bernoulli(probabilities[action]); 
		q_t[action] += (1/i)*(reward - q_t[action]); 

		if (action == optimal_action_index){
			optimality_count++; 
		}
	}

	printf("The optimality count is - %d\n", optimality_count); 
	free(n_t); free(a_t); free(q_t); 
}


void array_maker(float* probablities){
	srand((unsigned int) time(NULL));	

	for (int i = 0; i < 10; i++){
		probablities[i] = (float) (rand() % 10) / 10.0f; 
	}
}

void question1(){
	for (int i = 0; i < 99; i++){
		usleep(100000);
		float* probabilities; 
		probabilities = (float*) calloc(10, sizeof(float)); 
		array_maker(probabilities); 
		individual_round(probabilities); 
		free(probabilities);
	}
}

int main(){
	question1(); 
	return 0;
}

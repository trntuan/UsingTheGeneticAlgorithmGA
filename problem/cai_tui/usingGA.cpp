#include <iostream>
#include <random>
#include <algorithm>
#include <vector>

struct Item
{
    int value;
    int weight;
};

struct Solution
{
    std::vector<int> genes;
    int fitness;
};

int calcFitness(const Solution &s, std::vector<Item> &items, int num_items, int max_weight)
{
    int totalValue = 0;
    int totalWeight = 0;
    for (int i = 0; i < num_items; i++)
    {
        if (s.genes[i] == 1)
        {
            totalValue += items.at(i).value;
            totalWeight += items.at(i).weight;
        }
    }
    if (totalWeight > max_weight)
    {
        return 0;
    }
    return totalValue;
}

Solution selectParent(std::vector<Solution> &population, int population_size)
{
    int maxFitness = 0;
    //choose children that can maxFitness
    for (int i = 0; i < population_size; i++)
    {
        maxFitness = std::max(maxFitness, population[i].fitness);
    }
    //random from 0 to maxFitness and choose any number greater than that ramdom
    int fitnessThreshold = std::rand() % maxFitness;
    for (int i = 0; i < population_size; i++)
    {
        if (population[i].fitness >= fitnessThreshold)
        {
            return population[i];
        }
    }// if not, then select the first element
    return population[0];
}

Solution crossover(const Solution &parent1, const Solution &parent2, int num_items)
{
    Solution offspring;
    int crossoverPoint = std::rand() % num_items;
    for (int i = 0; i < crossoverPoint; i++)
    {
        offspring.genes.push_back(parent1.genes[i]);
    }
    for (int i = crossoverPoint; i < num_items; i++)
    {
        offspring.genes.push_back(parent2.genes[i]);
    }
    return offspring;
}

void mutate(Solution &s, int num_items)
{
    int mutationPoint = std::rand() % num_items;
    s.genes[mutationPoint] = !s.genes[mutationPoint];
}

bool ShouldMutate(float probability)
{
    // Generate a random number between 0 and 1.
    float randomNumber = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);

    // Return true if the random number is less than the probability value, false otherwise.
    return randomNumber < probability;
}

int main()
{
    int generation;
    int population_size;
    int num_items;
    int max_weight;
	float mutationRate;
    std::vector<Solution> population;
    std::vector<Item> items;

    std::cout << "Enter the maximum weight: ";
    std::cin >> max_weight;

    std::cout << "Enter the number of items: ";
    std::cin >> num_items;

    // Initialize the items and the population
    for (int i = 0; i < num_items; i++)
    {
        Item item;
        std::cout << "Enter the value and weight of each item: " << i << std::endl;
        std::cin >> item.value;
        std::cin >> item.weight;
        items.push_back(item);
    }
    std::cout << "number of generations: ";
    std::cin >> generation;
    std::cout << "nnumber of chromosomes in each population: ";
    std::cin >> population_size;
	std::cout << "\nmutation rate (vd: 0.1 = 10%): ";
 	std::cin >> mutationRate;
 	
    population.resize(population_size);
    for (int i = 0; i < population_size; i++)
    {
        for (int j = 0; j < num_items; j++)
        {
            population[i].genes.push_back(std::rand() % 2);
        }
        population[i].fitness = calcFitness(population[i], items, num_items, max_weight);
    }

    int numGenerations = 0;
    while (numGenerations <generation)
    {
        // Select the parents
        Solution parent1 = selectParent(population, population_size);
        Solution parent2 = selectParent(population, population_size);

        // Perform crossover to create the offspring
        Solution offspring = crossover(parent1, parent2, num_items);
		
		 // Perform mutation on the offspring
		if (ShouldMutate(mutationRate))
			{
		    mutate(offspring, num_items);
			}
        // Evaluate the fitness of the offspring
        offspring.fitness = calcFitness(offspring, items, num_items, max_weight);

        // Replace the least fit member of the population with the offspring
        int leastFitIndex = 0;
        for (int i = 1; i < population_size; i++)
        {
            if (population[i].fitness < population[leastFitIndex].fitness)
            {
                leastFitIndex = i;
            }
        }
        population[leastFitIndex] = offspring;

		 numGenerations++;
    }

    // Find the fittest solution in the final population
    int fittestIndex = 0;
    for (int i = 1; i < population_size; i++)
    {
        if (population[i].fitness > population[fittestIndex].fitness)
        {
            fittestIndex = i;
        }
    }

    std::cout << "Fittest solution: ";
    for (int i = 0; i < num_items; i++)
    {
        std::cout << population[fittestIndex].genes[i] << " ";
    }
    std::cout << std::endl;
    std::cout << "Fitness: " << population[fittestIndex].fitness << std::endl;

    return 0;
}


//
//	 std::cout << "population: " << numGenerations<< std::endl;
//    for (int i = 0; i < num_items; i++)
//    {
//       for(int j=0;j<num_items;j++)
//        {
//        	std::cout << population[i].genes[j] << "  ";
//		}
//        std::cout << "Fitness: " << population[i].fitness << std::endl;
//    }

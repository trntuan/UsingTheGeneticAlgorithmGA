#include <algorithm>
#include <cstdlib>
#include <ctime> // for time()
#include <iostream>
#include <vector>
#include <sstream> // for stringstream
#include <fstream> // for ifstream
#include <cmath>

struct Point
{
    float x;
    float y;
};

// Calculates the distance between two points
float Distance(const Point &p1, const Point &p2)
{
    int dx = p1.x - p2.x;
    int dy = p1.y - p2.y;
    return std::sqrt(dx * dx + dy * dy);
}

// A chromosome represents a possible tour through the cities.
// It is encoded as a permutation of the city indices.
struct Chromosome
{
    std::vector<int> cities;
    float fitness;
};

// Initialize a population of chromosomes with random tours.
void InitPopulation(std::vector<Chromosome> &population, int populationSize, int n)
{
    for (int i = 0; i < populationSize; i++)
    {
        Chromosome c;
        // The first city in the tour should always be city 0.
        c.cities.push_back(0);
        // Add the remaining cities in a random order.
        for (int j = 1; j < n; j++)
        {
            c.cities.push_back(j);
        }
        std::random_shuffle(c.cities.begin() + 1, c.cities.end());
        population.push_back(c);
    }
}

// Calculate the fitness of a chromosome by summing the distances between each pair of cities in the tour.
float CalcFitness(const Chromosome &c, std::vector<std::vector<float>> distances, int n)
{
    float fitness = 0;
    for (int i = 0; i < n - 1; i++)
    {
        fitness += distances[c.cities[i]][c.cities[i + 1]];
    }
    // Add the distance from the last city back to the first city.
    fitness += distances[c.cities[n - 1]][c.cities[0]];
    // Return the negative of the total distance to make the fittest chromosome have the lowest distance.
    return -fitness;
}
// Select a pair of chromosomes from the population using roulette wheel selection. produce offspring
void Select(const std::vector<Chromosome> &population, Chromosome &c1, Chromosome &c2, int tournamentSize)
{

    std::vector<Chromosome> tournament;
    for (int i = 0; i < tournamentSize; i++)
    {
        int r = rand() % population.size();
        tournament.push_back(population[r]);
    }

    // Select the fittest chromosome from the tournament.
    Chromosome fittest = tournament[0];
    for (int i = 1; i < tournamentSize; i++)
    {
        if (tournament[i].fitness > fittest.fitness)
        {
            fittest = tournament[i];
        }
    }
    c1 = fittest;

    // Repeat the process to select the second chromosome.
    tournament.clear();
    for (int i = 0; i < tournamentSize; i++)
    {
        int r = rand() % population.size();
        tournament.push_back(population[r]);
    }
    fittest = tournament[0];
    for (int i = 1; i < tournamentSize; i++)
    {
        if (tournament[i].fitness > fittest.fitness)
        {
            fittest = tournament[i];
        }
    }
    c2 = fittest;
}

// Perform crossover on the two chromosomes to produce offspring.
void Crossover(const Chromosome &c1, const Chromosome &c2, Chromosome &offspring1, Chromosome &offspring2, int n)
{
    // Pick a random crossover point.
    int crossoverPoint = rand() % n;

    // Create the offspring by combining the two parent chromosomes at the crossover point.
    offspring1.cities.assign(c1.cities.begin(), c1.cities.begin() + crossoverPoint);
    offspring2.cities.assign(c2.cities.begin(), c2.cities.begin() + crossoverPoint);
    for (int i = 0; i < n; i++)
    {
        int city = c2.cities[i];
        if (std::find(offspring1.cities.begin(), offspring1.cities.end(), city) == offspring1.cities.end())
        {
            offspring1.cities.push_back(city);
        }
    }
    for (int i = 0; i < n; i++)
    {
        int city = c1.cities[i];
        if (std::find(offspring2.cities.begin(), offspring2.cities.end(), city) == offspring2.cities.end())
        {
            offspring2.cities.push_back(city);
        }
    }
}

// Perform mutation on a chromosome by swapping two random cities.
void Mutate(Chromosome &c)
{
    // Choose two random indices to swap.
    int i = rand() % c.cities.size();
    int j = rand() % c.cities.size();
    // Only swap the cities if neither of the chosen indices is 0.
    while (i == 0 || j == 0)
    {
        i = rand() % c.cities.size();
        j = rand() % c.cities.size();
    }
    std::swap(c.cities[i], c.cities[j]);
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

    int choose;
    std::string input_path;
    std::string output_path;

    int numGenerations = 1000;
    int populationSize = 100; // number of chromosomes in each population
    float mutationRate = 1;
    int show_process;
    int adjacency_matrix;
    do
    {
        std::cout << "\n\t========== Menu solve TSP problem using GA algorithm ========== \n";
        std::cout << "Choose the number to solve the problem\n";
        std::cout << "1: test city - 6 Cities\n";
        std::cout << "2: Western Sahara - 29 Cities \n";
        std::cout << "3: Djibouti - 38 Cities \n";
        std::cout << "4: Qatar - 194 Cities \n";
        std::cout << "0: exit \n";
        std::cin >> choose;
        switch (choose)
        {
        case 1:
            input_path = "data1\\input.tsp";
            output_path = "data1\\kq.dat";
            break;
        case 2:
            input_path = "data2\\input.tsp";
            output_path = "data2\\kq.dat";
            break;
        case 3:
            input_path = "data3\\input.tsp";
            output_path = "data3\\kq.dat";
            break;
        case 4:
            input_path = "data4\\input.tsp";
            output_path = "data4\\kq.dat";
            break;
        default:
            choose = 0;
        }
       
        if (choose != 0)
        {
        	 std::cout << "number of generations: ";
        std::cin >> numGenerations;
        std::cout << "\nnumber of chromosomes in each population: ";
        std::cin >> populationSize;
        std::cout << "\nmutation rate (vd: 0.1 = 10%): ";
        std::cin >> mutationRate;
        std::cout << "\nshow the process crossover and mutation:\n";
        std::cout << "0: not displayed \n";
        std::cout << "1: show only the best chromosome\n";
        std::cout << "2: all chromosomes of each generation\n";
        std::cin >> show_process;
        std::cout << "show adjacency matrix (no = 0 | yes = 1): \n";
        std::cin >> adjacency_matrix;
        

            // Open the file for reading
            std::ifstream input_file(input_path);

            // Check if the file was successfully opened
            if (!input_file.is_open())
            {
                std::cout << "Error opening file" << std::endl;
                return 1;
            }

            // Read the data from the file line by line
            std::string line;
            std::vector<Point> points;
            while (std::getline(input_file, line))
            {
                // Split the line into tokens separated by whitespace
                std::stringstream ss(line);
                std::string token;
                std::vector<std::string> tokens;
                while (ss >> token)
                {
                    tokens.push_back(token);
                }

                // Process the tokens
                if (tokens[0] == "NODE_COORD_SECTION")
                {
                    // Read the coordinates of the points
                    while (std::getline(input_file, line))
                    {
                        if (line == "EOF")
                        {
                            break;
                        }
                        std::stringstream ss(line);
                        int id;
                        float x;
                        float y;
                        ss >> id >> x >> y;
                        Point p{x, y};
                        points.push_back(p);
                    }
                }
            }
            input_file.close();
            int n = points.size();
            int eliteSize = n / 2 + 1;      // the number of chromosomes kept in the old population
            int tournamentSize = n / 2 + 1; // the number of chromosomes chosen to select the parents
			
            std::vector<std::vector<float>> distances(n, std::vector<float>(n)); // adjacency matrix storage
            for (int i = 0; i < n; i++)
            {
                for (int j = 0; j < n; j++)
                {
                    distances[i][j] = Distance(points[i], points[j]);
                }
            }

            //	print adjacency matrix
            if(adjacency_matrix==1)
            for (int i = 0; i < n; i++)
            {
                for (int j = 0; j < n; j++)
                {
                    std::cout << distances[i][j] << "  ";
                }
                std::cout << "\n";
            }
            std::cout << "number city: " << n << "\n";
            srand(time(nullptr));
            std::vector<Chromosome> population;
            InitPopulation(population, populationSize, n);

            int i = 0;

            // Calculate the fitness of each chromosome.
            // fitness recalculation for chromosomes
            for (auto &c : population)
            {
                c.fitness = CalcFitness(c, distances, n);
            }
            while (i < numGenerations)
            {
            	
                // Sort the population by fitness.
                std::sort(population.begin(), population.end(), [](const Chromosome &c1, const Chromosome &c2)
                          { return c1.fitness > c2.fitness; });

                // Stop when you find the right fitness | Pull down botttom 2
                // print chromosomes of the generation
                if (show_process != 0)
                {
                	std::cout << "---Generation: " << i << " ---\n";
                    if (show_process == 1)
                    {
                        std::cout << "Best fitness:" << population[0].fitness << "\n";
                    }
                    else
                    {
                        for (int i = 0; i < populationSize; i++)
                        {
                        	std::cout<< "chromosome "<< i<<": ";
                            for (int j = 0; j < n; j++)
                                std::cout<< population[i].cities[j] << " ";
                            std::cout << ":" << population[i].fitness << "\n";
                        }
                    }
                }

                // Keep the elite chromosomes.
                std::vector<Chromosome> newPopulation;
                for (int j = 0; j < eliteSize; j++)
                {
                    newPopulation.push_back(population[j]);
                } // Perform crossover and mutation to produce the rest of the new population.
                while (newPopulation.size() < populationSize)
                {
                    // Select the parents using tournament selection.
                    Chromosome c1, c2;
                    Select(population, c1, c2, tournamentSize);

                    // Perform crossover to produce offspring.
                    Chromosome offspring1, offspring2;
                    Crossover(c1, c2, offspring1, offspring2, n);

                    // Mutate the offspring.
                    if (ShouldMutate(mutationRate))
                    {
                        Mutate(offspring1);
                        Mutate(offspring2);
                    }

                    // Add the offspring to the new population.
                    newPopulation.push_back(offspring1);
                    if (newPopulation.size() < populationSize)
                    {
                        newPopulation.push_back(offspring2);
                    }
                }

                for (auto &c : newPopulation)
                {
                    c.fitness = CalcFitness(c, distances, n);
                }

                population = newPopulation;
                i++;
            }

            // Sort the population by fitness.
            std::sort(population.begin(), population.end(), [](const Chromosome &c1, const Chromosome &c2)
                      { return c1.fitness > c2.fitness; });

            
            // Find the chromosome with the highest fitness
            Chromosome fittest = population[0];

            std::cout << "Fittest chromosome: ";
            for (int i = 0; i < n; i++)
            {
                std::cout << fittest.cities[i] << " ";
            }
            std::cout << std::endl;
            std::cout << "Fitness: " << -fittest.fitness << std::endl;

            // Write the shortest tour to a file.
            std::ofstream output(output_path);
            output << "COMMENT: Tour length " << -fittest.fitness << "\n";
            output << "DIMENSION: " << n << "\n";
            output << "TOUR_SECTION"
                   << "\n";
            for (int i = 0; i < fittest.cities.size(); i++)
            {
                output << fittest.cities[i] + 1 << "\n";
            }
            output.close();
        }
    } while (choose != 0);

    // Close the file

    return 0;
}

/*  bottom 1
   sample parameters
      float result = 86891;
      int populationSize = 100; // number of chromosomes in each population
      int numGenerations = 10000;
      float mutationRate = 1;

             int populationSize = 100; // number of chromosomes in each population
        int numGenerations = 1000;
        float mutationRate = 1;
*/

/* bottom 2
// Stop when you find the right fitness
        if(population[0].fitness <= -result) break;
        std::cout<<population[0].fitness<<"\n";

// print chromosomes of the generation
        for (int i = 1; i < populationSize; i++)
        {
        for (int j = 0; j < n; j++)
            std::cout << population[i].cities[j] << " ";
        std::cout << ":" << population[i].fitness << "\n";
    }
    
    std::cout << "--the most optimal chromosomes--\n";
            for (int i = 1; i < populationSize; i++)
            {
                for (int j = 0; j < n; j++)
                    std::cout << population[i].cities[j] << " ";
                std::cout << ":" << population[i].fitness << "\n";
            }
*/
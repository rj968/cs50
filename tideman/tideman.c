#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
} pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    // TODO
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i]) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // TODO
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 1; j < candidate_count - i; j++)
        {
            preferences[ranks[i]][ranks[candidate_count - (j)]]++;
        }
    }
    return;
}
// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // TODO
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = candidate_count; j > i; j--)
        {
            if (preferences[i][j] != preferences[j][i])
            {
                if (preferences[i][j] > preferences[j][i])
                {
                    pairs[pair_count].winner = i;
                    pairs[pair_count].loser = j;
                }
                else
                {
                    pairs[pair_count].winner = j;
                    pairs[pair_count].loser = i;
                }
                pair_count++;
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // TODO
    int a = 0, b = 0, count = 1;
    while (count > 0)
    {
        count = 0;
        for (int i = 0; i < pair_count - 1; i++)
        {
            if (preferences[pairs[i].winner][pairs[i].loser] <
                preferences[pairs[i + 1].winner][pairs[i + 1].loser])
            {
                a = pairs[i].winner;
                b = pairs[i].loser;
                pairs[i].winner = pairs[i + 1].winner;
                pairs[i].loser = pairs[i + 1].loser;
                pairs[i + 1].winner = a;
                pairs[i + 1].loser = b;
                count++;
            }
        }
    }
    return;
}
// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // TODO
    int loop = 0;
    for (int i = 0; i < pair_count; i++)
    {
        loop = pairs[i].winner;
        for (int j = 0; j < pair_count; j++)
        {
            if (locked[pairs[j].winner][loop])
            {
                loop = pairs[j].winner;
                j = 0;
            }
        }
        if (loop != pairs[i].loser)
            locked[pairs[i].winner][pairs[i].loser] = true;
    }
    return;
}

// Print the winner of the election
void print_winner(void)
{
    // TODO
    int loop = 0;
    for (int i = 0; i < candidate_count; i++)
    {
        loop = 0;
        for (int j = 0; j < candidate_count; j++)
        {
            if (!locked[j][i] && j == candidate_count - 1)
            {
                printf("%s\n", candidates[i]);
            }
            else if (locked[j][i])
                break;
        }
    }
    return;
}

/*write a code that checks for i in pair_count if the winner of the i th locked pair is a
loser in some locked pair j. then check for j until there is no further j
it will take in a an input i and return j in the end
problems-
1. there can be more than 1 j pairs.
2. idk what the tied thing means*/

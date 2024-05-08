#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

struct ThreadData
{
    int *data;
    int size;
    long long int sum;
    int min;
    int max;
};

void single(int *data, int size)
{
    long long int sum = 0;
    int min;
    int max;
    int num;
    for (int i = 0; i < size; i++)
    {
        num = data[i];
        if (i == 0){
            min = num;
            max = num;
        }
        sum += num;
        if (num < min)
        {
            min = num;
        }
        if (num > max)
        {
            max = num;
        }
    }
    printf("Sum is: %lld\nminimum is: %d\nmaximum is: %d\n", sum, min, max);
}

void *threadFunction(void *arg)
{
    struct ThreadData *threadData = (struct ThreadData *)arg;

    int min = 100;
    int max = 0;
    int long long sum = 0;
    int num;

    for (int i = 0; i < threadData->size; i++)
    {
        num = threadData->data[i];
        sum += num;
        if (num < min)
        {
            min = num;
        }
        if (num > max)
        {
            max = num;
        }
    }

    threadData->sum = sum;
    threadData->min = min;
    threadData->max = max;

    pthread_exit(NULL);
}

void multi(int *data, int size, int numThreads)
{
    pthread_t *threads = malloc(numThreads * sizeof(pthread_t));
    struct ThreadData *threadData = malloc(numThreads * sizeof(struct ThreadData));

    if (threads == NULL || threadData == NULL)
    {
        printf("Memory allocation failed\n");
        return;
    }

    int segmentSize = size / numThreads;
    int remainder = size % numThreads;

    int currentIndex = 0;

    for (int i = 0; i < numThreads; i++)
    {
        threadData[i].data = &data[currentIndex];
        threadData[i].size = (i < remainder) ? (segmentSize + 1) : segmentSize;

        pthread_create(&threads[i], NULL, threadFunction, (void *)&threadData[i]);

        currentIndex += threadData[i].size;
    }

    for (int i = 0; i < numThreads; i++)
    {
        pthread_join(threads[i], NULL);
    }

    long long int totalSum = 0;
    int totalMin = 100;
    int totalMax = 0;

    for (int i = 0; i < numThreads; i++)
    {
        totalSum += threadData[i].sum;

        if (threadData[i].min < totalMin)
        {
            totalMin = threadData[i].min;
        }

        if (threadData[i].max > totalMax)
        {
            totalMax = threadData[i].max;
        }
    }

    printf("Sum is: %lld\nMinimum is: %d\nMaximum is: %d\n", totalSum, totalMin, totalMax);
    free(threads);
    free(threadData);
}

int main() {
    int *numbers = NULL;
    int size = 0;
    int value;
    char buffer[100];
    char fileName[100];
    char functionName[10];
    int threads = 4;
    clock_t start, end;
    double total;

    printf("Enter the filename: ");
    scanf("%s", fileName);

    printf("Enter the function name (single or multi): ");
    scanf("%s", functionName);
    while (getchar() != '\n');
    FILE *filePointer = fopen(fileName, "r");

    if (filePointer == NULL) {
        perror("Error opening the file");
        return 1;
    }

    while (fgets(buffer, sizeof(buffer), filePointer) != NULL) {
        size++;
    }

    fseek(filePointer, 0, SEEK_SET);

    numbers = (int *)malloc(size * sizeof(int));

    for (int i = 0; i < size; i++) {
        fgets(buffer, sizeof(buffer), filePointer);
        value = atoi(buffer);
        numbers[i] = value;
    }

    if (strcmp(functionName, "single") == 0) {
        start = clock();
        single(numbers, size);
        end = clock();
    } else if (strcmp(functionName, "multi") == 0) {
        printf("Enter number of threads (default is 4): ");
        char userInput[100];
        fgets(userInput, sizeof(userInput), stdin);

        if (strlen(userInput) > 1) { // Check if there is an input
            int readValue;
            sscanf(userInput, "%d", &readValue);
            threads = readValue;
        }

        start = clock();
        multi(numbers, size, threads);
        end = clock();
    } else {
        printf("Invalid function name provided\n");
    }

    total = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Time taken: %f\n", total);

    free(numbers);
    fclose(filePointer);
    return 0;
}
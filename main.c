// Manages input, computation, and presentation of a convolution operation

// Eduardo Souza Rocha, Gustavo Vieira Ferreira, Isadora Carolina Siebert,
// Murilo Valentim Zabott, Rogério Lopes Lube 2nd Semester 2024 University of
// São Paulo, Institute of Mathematical and Computer Science
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int max(int x, int y) {
    return (x > y) ? x : y;
}
int min(int x, int y) {
    return (x < y) ? x : y;
}

int clamp(int x, int a, int b) {
    return min(max(x, a), b);
}

void input_and_validation(int *N, int *M, int *S, int *filter_radius) {
    scanf("%d %d %d", N, M, S);

    if ((*M & 1) == 0) {
        printf("M must be an odd number.\n");
        exit(1);
    }

    *filter_radius = *M >> 1;

    srand(*S);
}

void print_image(int N, int *image) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%3d ", image[i*N + j]);
        }
        printf("\n");
    }
}

void print_filter(int M, float *filter) {
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < M; j++) {
            printf("%1.1f ", filter[i*M + j]);
        }
        printf("\n");
    }
}

void create_image(int N, int *image) {
    for (int y = 0; y < N; y++) {
        for (int x = 0; x < N; x++) {
            image[y*N + x] = rand() % 256;
        }
    }
}

void create_filter(int M, float *filter) {
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < M; j++) {
            filter[i*M + j] = (rand() % 10) / 10.0;
        }
    }
}

int main() {
    int N, M, S, filter_radius;

    input_and_validation(&N, &M, &S, &filter_radius);

    int *image = (int *) malloc(N * N * sizeof(int));
    float *filter = (float *) malloc(M * M * sizeof(float));
    int *output_image = (int *) malloc(N * N * sizeof(int));

    create_image(N, image);
    create_filter(M, filter);

    int menor = 255;
    int maior = 0;
    #pragma omp parallel for collapse(2) reduction(max : maior) reduction(min : menor)
    for (int y = 0; y < N; y++) {
        for (int x = 0; x < N; x++) {
            float acum = 0;  // Inicialize a variável de acumulação
            
            int max_i = min(filter_radius, N-1-y);
            int max_j = min(filter_radius, N-1-x);

            #pragma omp simd collapse(2) reduction(+ : acum)
            for (int i = -min(filter_radius, y); i <= max_i; i++) {
                for (int j = -min(filter_radius, x); j <= max_j; j++) {
                    acum += image[(y+i)*N + x + j] * filter[(i + filter_radius) * M + j + filter_radius];
                }
            }

            output_image[y*N + x] = clamp((int)acum, 0, 255);  // Aplique o clamp antes de atualizar o menor e maior
            menor = min(menor, output_image[y*N + x]);
            maior = max(maior, output_image[y*N + x]);
        }
    }

    printf("%d %d", maior, menor);

    free(image);
    free(filter);
    free(output_image);

    return 0;
}

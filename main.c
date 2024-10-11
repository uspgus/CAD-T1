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

void print_image(int N, int image[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%3d ", image[i][j]);
        }
        printf("\n");
    }
}

void print_filter(int M, float filter[M][M]) {
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < M; j++) {
            printf("%1.1f ", filter[i][j]);
        }
        printf("\n");
    }
}

void create_image(int N, int image[N][N]) {
    for (int y = 0; y < N; y++) {
        for (int x = 0; x < N; x++) {
            image[y][x] = rand() % 256;
        }
    }
}

void create_filter(int M, float filter[M][M]) {
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < M; j++) {
            filter[i][j] = (rand() % 10) / 10.0;
        }
    }
}

int image_access(int y, int x, int N, int image[N][N]) {
    return (y < 0 || y >= N || x < 0 || x >= N) ? 0 : image[y][x];
}

int main() {
    int N, M, S, filter_radius;

    input_and_validation(&N, &M, &S, &filter_radius);

    int image[N][N];
    float filter[M][M];
    int output_image[N][N];

    create_image(N, image);
    create_filter(M, filter);

    int menor = 255;
    int maior = 0;
    #pragma omp parallel for collapse(2) reduction(max : maior) reduction(min : menor)
    for (int y = 0; y < N; y++) {
        for (int x = 0; x < N; x++) {
            float pixel = 0;
            #pragma omp simd collapse(2) reduction(+ : pixel)
            for (int i = -filter_radius; i <= filter_radius; i++) {
                for (int j = -filter_radius; j <= filter_radius; j++) {
                    acum += image_access(y + i, x + j, N, image) * filter[i + filter_radius][j + filter_radius];
                }
            }
            menor = min(menor, output_image[y][x]);
            maior = max(maior, output_image[y][x]);
            output_image[y][x] = clamp(acum, 0, 255);
        }
    }

    printf("\n");
    print_image(N, image);
    printf("\n");
    print_filter(M, filter);
    printf("\n");
    print_image(N, output_image);
    printf("maior: %d\n", maior);
    printf("menor: %d\n", menor);

    return 0;
}

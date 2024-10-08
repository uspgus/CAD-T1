// Manages input, computation, and presentation of a convolution operation

// Eduardo Souza Rocha, Gustavo Vieira Ferreira, Isadora Carolina Siebert, Murilo Valentim Zabott, Rogério Lopes Lube
// 2nd Semester 2024
// University of São Paulo, Institute of Mathematical and Computer Science

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void input_and_validation(int *N, int *M, int *S, int *filter_radius) {
    scanf("%d %d %d", N, M, S);
    
    if ((*M & 1) == 0) {
        printf("M must be an odd number.\n");
        exit(1);
    }

    *filter_radius = M >> 1;

    srand(*S);
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
            filter[i][j] = (rand() % 100) / 100.0;
        }
    }
}

int image_access(int y, int x, int N, int M) {
    return (y < 0 || y >= N || x < 0 || x >= N) ? 0 : image[y][x];
}

int convolution(int x, int y, int N, int M, int image[N][N], float filter[M][M], int filter_radius){
    int acum = 0;
    for (int i = -filter_radius; i <= filter_radius; i++){
        for (int j = -filter_radius; j <= filter_radius; j++) {
            acum += image_access(y+i, x+j, N, M) * filter[i + filter_radius][j + filter_radius];
        }
    }
    return acum;
}

int main() {
    int N, M, S, filter_radius;

    input_and_validation(&N, &M, &S, &filter_radius);

    int image[N][N];
    float filter[M][M];
    int output_image[N][N];

    create_image(N, image);
    create_filter(M, filter);

    # pragma omp parallel for
    for (int y = 0; y < N; y++){
        for (int x = 0; x < N; x++) {
            output_image[y][x] = convolution(x, y, N, M, image, filter, filter_radius);
        }
    }

    return 0;
}

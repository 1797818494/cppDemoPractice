#include <benchmark/benchmark.h>
#include <immintrin.h>

constexpr int N = 1000000;

static void normal(benchmark::State &state) {
  float *a = static_cast<float *>(_mm_malloc(sizeof(float) * N, 16));
  float *b = static_cast<float *>(_mm_malloc(sizeof(float) * N, 16));
  float *c = static_cast<float *>(_mm_malloc(sizeof(float) * N, 16));
  for (int i = 0; i < N; ++i) {
    a[i] = i;
    b[i] = 2 * i;
  }

  for (auto _ : state) {
    for (int i = 0; i < N; ++i) {
      c[i] = a[i] + b[i];
    }
  }

  _mm_free(a);
  _mm_free(b);
  _mm_free(c);
}

static void avx128(benchmark::State &state) {
  float *a = static_cast<float *>(_mm_malloc(sizeof(float) * N, 64));
  float *b = static_cast<float *>(_mm_malloc(sizeof(float) * N, 64));
  float *c = static_cast<float *>(_mm_malloc(sizeof(float) * N, 64));
  for (int i = 0; i < N; ++i) {
    a[i] = i;
    b[i] = 2 * i;
  }

  for (auto _ : state) {
    for (int i = 0; i < N; i += 4) {
      __m128 v1 = _mm_load_ps(a + i);
      __m128 v2 = _mm_load_ps(b + i);
      __m128 v3 = _mm_add_ps(v1, v2);
      _mm_store_ps(c + i, v3);
    }
  }

  _mm_free(a);
  _mm_free(b);
  _mm_free(c);
}

static void avx256(benchmark::State &state) {
  float *a = static_cast<float *>(_mm_malloc(sizeof(float) * N, 64));
  float *b = static_cast<float *>(_mm_malloc(sizeof(float) * N, 64));
  float *c = static_cast<float *>(_mm_malloc(sizeof(float) * N, 64));
  for (int i = 0; i < N; ++i) {
    a[i] = i;
    b[i] = 2 * i;
  }

  for (auto _ : state) {
    for (int i = 0; i < N; i += 8) {
      __m256 v1 = _mm256_load_ps(a + i);
      __m256 v2 = _mm256_load_ps(b + i);
      __m256 v3 = _mm256_add_ps(v1, v2);
      _mm256_store_ps(c + i, v3);
    }
  }

  _mm_free(a);
  _mm_free(b);
  _mm_free(c);
}

static void avx512(benchmark::State &state) {
  float *a = static_cast<float *>(_mm_malloc(sizeof(float) * N, 64));
  float *b = static_cast<float *>(_mm_malloc(sizeof(float) * N, 64));
  float *c = static_cast<float *>(_mm_malloc(sizeof(float) * N, 64));
  for (int i = 0; i < N; ++i) {
    a[i] = i;
    b[i] = 2 * i;
  }

  for (auto _ : state) {
    for (int i = 0; i < N; i += 16) {
      __m512 v1 = _mm512_load_ps(a + i);
      __m512 v2 = _mm512_load_ps(b + i);
      __m512 v3 = _mm512_add_ps(v1, v2);
      _mm512_store_ps(c + i, v3);
    }
  }

  _mm_free(a);
  _mm_free(b);
  _mm_free(c);
}

BENCHMARK(normal);
BENCHMARK(avx128);
BENCHMARK(avx256);
BENCHMARK(avx512);

BENCHMARK_MAIN();
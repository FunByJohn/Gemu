#include <array>
#include <initializer_list>

template<std::size_t N>
class Matrix {
private:
	std::array<std::array<float, N>, N> m;

public:
	Matrix(std::initializer_list<std::array<float, N>> rows) {
		assert(rows.size() == N);

		int i = 0;
		for(auto& row : rows) m[i++] = row;
	}

	std::array<float, N> apply(const std::array<float, N>& a) {
		std::array<float, N> b;
		b.fill(0);

		for(int y = 0; y < N; y++) {
			for(int x = 0; x < N; x++) {
				b[y] += a[x] * m[y][x];
			}
		}

		return b;
	}

};

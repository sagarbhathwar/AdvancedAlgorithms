#include <iostream>
#include <ctime>
#include <cmath>
#include <chrono>
#include "matrix.hpp"

int main()
{
	std::ios::sync_with_stdio(false);
    /*
	srand(129892);

	for (int i = 2; i <= int(pow(2, 16)); i *= 2)
	{
		Matrix<int> A(i, i);
		Matrix<int> B(i, i);
		Matrix<int> C1(i, i);
		Matrix<int> C2(i, i);

		for (int j = 0; j < i; ++j)
		{
			for (int k = 0; k < i; ++k)
			{
				A[j][k] = rand() % 512;
				B[j][k] = rand() % 512;
			}
		}

		std::cout << "Size: " << i << std::endl;
		auto t1 = std::chrono::high_resolution_clock::now();
		C1 = A * B;
		auto t2 = std::chrono::high_resolution_clock::now();

		std::cout << "Naive -" << " Time taken: " << (t2 - t1).count() << std::endl;

		t1 = std::chrono::high_resolution_clock::now();
		C2 = Matrix<int>::recursive_matrix_multiplication(A, B);
		t2 = std::chrono::high_resolution_clock::now();

		std::cout << "Recursive - " << " Time taken: " << (t2 - t1).count() << std::endl << std::endl;
	}
    */
    Matrix<int> A(4, 4);
    std::cin >> A;
    Matrix<int> C2(Matrix<int>::recursive_matrix_multiplication(A, A));
    std::cout << C2 << std::endl;
}
